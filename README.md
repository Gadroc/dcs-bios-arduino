#DCS-BIOS Arduino Library

This is an alternative to the official DCS-BIOS arduino library.  The primary feature of this library is that of a half-duplix polling system, which allows more modularity in cockpit building.  It allows easily switching back and forth with direct serial protocol and an RS-485 protocol by only changing one line in a sketch.

## Diferences from official library
* No namespacing
* Additional control objects like DimmableLed
* Stepper motor support (including acceleration)
* IO abstract layer to support IO Expanders
* Support half-duplex polling protocol (RS-485)
* Wraps more IO leaving cleaners sketches

## Example Sketch
```c++
#include <DcsBios.h>

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
}
```

## Library Overview
This library is broken down into four layers - PC Interface, Control Interface, DCS-BIOS Interface and IO Abstraction.  Most sketches will only use PC Interface and Control Interface objects.

#### PC Interface
The PC Interface layer handles interaction with the PC.  Every sketch will interact with this layer.  This layer consistes of three objects.

Usage of this layer consists of three parts to your sketches. First is defining your PC Interface object, typicall right after the includes at the top of the sketch.

```c++
DcsBiosSerialDevice dcsBiosDevice(&Serial);
```

Second you must call the process() method during each loop in your primary loop.

```c++
void loop() {
	dcsBiosDevice.process();
}
```

Use the interface object to send messages back to DCS.

```c++
void sendDcsBiosMessage(const char* msg, const char* arg) {
  dcsBiosDevice.sendDcsBiosMessage(msg, arg);
}
```

#### Control Interface
The control interface layer mappes the I/O pins of your arduino to DCS-BIOS.   This is the primary layer you will use when creating your sketches.

#### DCS-BIOS Interface
The DCS-BIOS layer handles extracting and sending information into DCS-BIOS.

#### IO Abstraction
The IO Abstraction layer is a set of interfaces that can be used to implement IO Expanders.
