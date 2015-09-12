/**
 * Sketch to implement a RS-485 bus controller.  This sketch requires an arduino with two UART/Serial Ports.
 * It has been tested on Arduino MEGA and Arudino Leonardos.
 */
#include "DcsBios.h"

// UART Serial that is connected to your PC.
#define PC_SERIAL Serial

// UART Serial that is connected to RS-485 transeiver
#define BUS_SERIAL Serial1

// Pin connected to RS-485 transeiver transmit / receive pins.
#define BUS_TX_PIN 2

DcsBiosRs485Controller controller(BUS_SERIAL, BUS_TX_PIN, PC_SERIAL);

void setup() {
  PC_SERIAL.begin(250000);
  BUS_SERIAL.begin(250000);

  while (!PC_SERIAL || !BUS_SERIAL) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

void loop() {
  controller.process();
}
