/*
	Copyright 2013 Craig Courtney

    This file is part of EOS-Firmware.

    EOS-Firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    EOS-Firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with EOS-Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _DCSBIOS_BUS_H_
#define _DCSBIOS_BUS_H_

#include <Arduino.h>
#include "FastPin.h"

#define BUFFER_SIZE 31

namespace DcsBios {
    class Bus 
    {
    private:
        HardwareSerial* _serial;
        FastPin _txPin;
        uint8_t _address;
        uint8_t _bank;
        uint8_t _rxBuffer[BUFFER_SIZE];
        uint8_t _txBuffer[BUFFER_SIZE];

    public:
        EosBusSerial(uint8_t address, uint8_t bank);
        void begin(HardwareSerial *serialPort, uint8_t txPin);

        // Reads data from the bus and returns true if the buffer data
        // is in a state which updates to outputs should occur.  Buffer will only be
        // in a consistent state until the next isDataReay call.
        bool isDataReady();
    };
};

#endif