/*
    Copyright 2015 Craig Courtney

    This file is part of DcsBios-Firmware.

    DcsBios-Firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DcsBios-Firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DcsBios-Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _DCSBIOS_BUS_H_
#define _DCSBIOS_BUS_H_

#include <Arduino.h>
#include "FastPin.h"

#define DCSBIOS_BUFFER_SIZE 31

namespace DcsBios {
    class Bus 
    {
    private:
        HardwareSerial* _serial;
        FastPin _txPin;
        uint8_t _address;
        uint8_t _bank;
        uint8_t _rxBuffer[DCSBIOS_BUFFER_SIZE];

        uint8_t _txPointer;
        uint8_t _txBuffer[DCSBIOS_BUFFER_SIZE];

        uint16_t _bufferOverruns;
        uint16_t _rxErrors;

    public:
        Bus(uint8_t address, uint8_t bank);

        void begin(HardwareSerial *serialPort, int txPin);

        // Must be called oftern in main processing loop.  This routine
        // will process incoming data, notify outputs and send inputs
        // back when necessary.
        void process();

        // Queues command data to send back to the simulation when we are
        // polled.
        void queueCommand(uint8_t buffer, uint8_t size);
    };
};

#endif