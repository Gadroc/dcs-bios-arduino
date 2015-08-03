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
#ifndef _DCSBIOS_BUSPARSER_H_
#define _DCSBIOS_BUSPARSER_H_

#include <Arduino.h>

#define DCSBIOS_MAX_DATA_SIZE 31
#define DCSBIOS_PACKET_START_BYTE 0xbb
#define DCSBIOS_PACKET_LEADIN_BYTE 0x88
#define DCSBIOS_PACKET_TRAIL_BYTE 0xFF

class BusParser {
    private:
        uint8_t _rxState;
        uint8_t _rxAddress;
        uint8_t _rxCommand;
        uint8_t _rxBank;
        uint8_t _rxPointer;
        uint8_t _rxSize;
        uint8_t _rxBuffer[DCSBIOS_MAX_DATA_SIZE];
        uint8_t _checksum;
        uint16_t _rxErrors;

    public:
        uint8_t getAddress();
        uint8_t getCommand();
        uint8_t getBank();
        uint8_t hasData();
        uint8_t* getBuffer();

        bool processByte(uint8_t data);
        void reset();
};

inline uint8_t BusParser::getAddress() {
    return _rxAddress;
}

inline uint8_t BusParser::getCommand() {
    return _rxCommand;
}

inline uint8_t BusParser::getBank() {
    return _rxBank;
}

inline uint8_t BusParser::hasData() {
    return _rxSize > 0;
}

inline uint8_t* BusParser::getBuffer() {
    return _rxBuffer;
}

#endif