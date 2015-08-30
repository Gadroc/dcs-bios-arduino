#include <DcsBios.h>

/* Instantiate a device object to parse the DCS-BIOS export stream */
DcsBiosSerialDevice dcsBiosDevice;
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