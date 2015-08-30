/**
 * Sketch to implement a RS-485 bus controller.  This sketch requires an arduino with two UART/Serial Ports.
 * It has been tested on Arduino MEGA and Arudino Leonardos.
 */
#include <DcsBios.h>

// UART Serial that is connected to your PC.
#define PC_SERIAL Serial

// UART Serial that is connected to RS-485 transeiver
#define BUS_SERIAL Serial1

// Pin connected to RS-485 transeiver transmit / receive pins.
#define BUS_TX_PIN 2

DcsBiosRs485Controller dcsBiosController;

void setup() {
  // Initialize the PC serial port to 250000 baud
  PC_SERIAL.begin(250000);
  // Initialize the RS-485 bus serial port to 250000 baud
  BUS_SERIAL.begin(250000);
  // Tell the controller which serial ports to communicate on and which pin to
  // use for transmitting on the RS-485 pus.
  dcsBiosController.begin(&BUS_SERIAL, BUS_TX_PIN, &PC_SERIAL);

  while (!PC_SERIAL || !BUS_SERIAL) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

void loop() {
  // Loop and continously process data.
  dcsBiosController.process();
}