#DCS-BIOS Arduino Library

This is an alternative to the official DCS-BIOS arduino library.  The primary feature of this library is that of a half-duplix polling system, which allows more modularity in cockpit building.  It allows easily switching back and forth with direct serial protocol and an RS-485 protocol by only changing one line in a sketch.

## Diferences from official library
* No namespacing
* Additional control objects like DimmableLed
* Stepper motor support (including acceleration)
* IO abstract layer to support IO Expanders
* Support half-duplex polling protocol (RS-485)
* Wraps more IO leaving cleaners sketches

## Sketch Overview
```#include <DcsBios.h>

/* Instantiate a device object to parse the DCS-BIOS export stream */
DcsBiosSerialDevice dcsBiosDevice(&Serial);

/* Declare a Master Caution Reset button on pin 10 */
Switch2Pos masterCautionButton("UFC_MASTER_CAUTION", 10);

/* Make the LED connected to pin 13 into a Master Caution Light */
Led masterCautionLed(0x1012, 0x0800, 11, 13);

/*
 * Your setup routine needs to intialize your communications
 * and all of your input devices.
 */
void setup() {
  // Initialize the serial port to 250000 baud.
  Serial.begin(250000);

  // Tell DCS-BIOS which serial port to communicate on.
  dcsBiosDevice.begin(&Serial);

  // Initialize all of your polling inputs.
  PollingInput::initInputs();
}

/**
 * Your main loop needs to process bus data and poll your inputs.
 */
void loop() {
  // Process any incoming data.
  dcsBiosDevice.process();
  // Check our inputs and record messages for DCS-BIOS
  PollingInput::pollInputs();
  // Run stepper motors (uncomment if you are using steppers in this sketch)
  //StepperOutput::runSteppers();
}

/*
 * This method will get called at the end of every data frame.  You
 * can update any outputs here if necessary.  Built in output devices
 * automatcially update, and don't need code here.
 */
void onDcsBiosFrameSync() {
}

/*
 * You need to define 
 * void sendDcsBiosMessage(const char* msg, const char* arg)
 * so that the string msg, followed by a space, the string arg
 * and a newline gets sent to the DCS-BIOS import stream.
 * Typically we will use the built in function of the serial 
 * DcsBiosSerialDevice or DcsBiosRs485Device to send data back.
 * But you can use this method to manually use other communication
 * techniques.
 */
void sendDcsBiosMessage(const char* msg, const char* arg) {
  dcsBiosDevice.sendDcsBiosMessage(msg, arg);
}```

## Library Overview
This library is broken down into four layers - PC Interface, Control Interface, DCS-BIOS Interface and IO Abstraction.  Most sketches will only use PC Interface and Control Interface objects.

### PC Interface
The PC Interface layer handles interaction with the PC.  Every sketch will interact with this layer.  This layer consistes of three objects.

Usage of this layer consists of three parts to your sketches. First is defining your PC Interface object, typicall right after the includes at the top of the sketch.
```DcsBiosSerialDevice dcsBiosDevice(&Serial);```

Second you must call the process() method during each loop in your primary loop.
```void loop() {
	dcsBiosDevice.process();
}```

Use the interface object to send messages back to DCS.
```void sendDcsBiosMessage(const char* msg, const char* arg) {
  dcsBiosDevice.sendDcsBiosMessage(msg, arg);
}```

#### DcsBiosSerialDevice
This object implements the direct serial protocol.  This protocol is a direct retransmission of the data contained with in the UDP packets from DCS-BIOS.  This is the default protcol used by the original DCS-BIOS arduino library.  Sketches using DcsBiosSerialDevice interface directly with the PC via an RS-232 serial port.
##### Initialization
```DcsBiosSerialDevice dcsBiosDevice(&Serial);```
Define an object of DcsBiosSerialDevice and pass it the address of the serial port stream it should use to talke to the PC.  Be sure to initialize the Serial port to the right baud rate in your setup() method.

#### DcsBiosRs485Device
This object implements the RS-485 half-duplex protocol for control boards.  Sketches using DcsBiosRs485Device will talk to a DcsBiosRs485Controller, which will in trun communicate with the PC.
##### Initialization
```DcsBiosRs485Device dcsBiosDevice(&Serial, 8, 0);```
Define an object of DcsBiosRs485Device passing in the address of the serial port to talk to the controller through, the IO pin connnected to the transmit pin on your RS-485 controller and the address of this device on the bus (must be unique on the bus, but two different buses can use the same ids).  Be sure to initialize the Serial port to the right baud rate in your setup() method.

#### DcsBiosRS485Controller
This object manages communication with the PC and communicating with up to 32 DcsBiosRs485Device based sketches.  See the example sketch for an implementation of the controller.

#### DcsBiosDevice
This is a base object/interface that all new protocols for talking with the PC should implement.  All DcsBiosDevices must implement two methods process and sendDcsBiosMessage.  This will allow existing sketches to be converted easily.

### Control Interface
The control interface layer mappes the I/O pins of your arduino to DCS-BIOS.   This is the primary layer you will use when creating your sketches.

### DCS-BIOS Interface
The DCS-BIOS layer handles extracting and sending information into DCS-BIOS.

### 

## RS-485 Protocol
The DCS-BIOS bus is setup as a single master / many slave configuration.  Slave devices are only allowed to reply to packets specifically addressed to them.

### Computer Interface
DCS-BIOS bus is interfaced to the computer by a Bus Controller.  This controller speaks through a half duplex serial protocol at 250kbps and acts as the master for the bus.  This bus controller takes care of polling across all devices on the bus and interfacing with the PC.  The controller hardware must have at least two high speed UARTS capable of 250kbps and one of those UARTs must be able to be interfaced with the PC. (Ex: ArduinoMega or Leonardo).

### Electrical Signaling
Signalling is up to the cockpit designer, but the protocol is designed around a multi-drop master/slave bus.  Half-Duplex RS-485 at 250kbps is the reference signal specification.

### Addressing
Each bus can handle 32 (Addresses 0-31) polled devices which can supply input to the PC.  Each address will be polled roughly 3 times per second.  In addition as many listen only devices can be added up to the limits of your signaling chipset selection.  Listen only devices would read the data inside the polling update packets and display the information in their appropriate format. (Example many cockpit flight instruments do not have any input.  They can be implemented as listen only devices.)  Master device does not have an address as all response packet are directed at it.

### Packet Categories
Request - These are packets from the master directed at an addressed slave which require a response.  The host controller will wait for 2ms for a response before moving on.  Requests can only be transmitted by the master device.
Response - Response to a request.  These can only be transmitted by a slave device and only when they have received a request addressed to them.

### Packet Types
ID - Category - Name - Description
0 - Request - Polling Request - A polling request for a device.
1 - Response - Polling Response - Returns any messages this devices has queued up since it's last poll.
2 - Request - Config Request - Sends config request to slave device.
3 - Response - Config Response - Response packet to config request.
4-7 Reserved for future use

All data in Polling Requests are broadcast. Polling Requests should be inspected by all devices on the bus for any applicable data for display, but should only respond with a Polling Response if it was addressed to them.

Note: When the Bus Controller has no new update data it will send polling requests of type 0 with a data size of 0.

### Packet Structure
Byte 0 = Address / Command Bits
 Bit 7-5 = Packet Type
 Bit 4-0 = Request Target Address or Response Source Address
Byte 1 = Data Size
Byte 2-X = Packet Data (Export stream in Polling Request, or Config data structure in Config Request)

### PC to Bus Controller Protocol