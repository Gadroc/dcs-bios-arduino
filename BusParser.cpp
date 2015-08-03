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

#include "BusParser.h"

enum packet_state {
    BUS_S_START,        // Waiting for start byte
    BUS_S_LEADIN,       // Waiting for lead in byte
    BUS_S_ADDR,         // Waiting for addresss
    BUS_S_BANK,         // Waiting for bank / data length
    BUS_S_DATA,         // Waiting on data load
    BUS_S_CHECKSUM      // Waiting on checksum
};

bool BusParser::processByte(uint8_t in) {
    switch(_rxState) {
        case BUS_S_START:
            if (in == DCSBIOS_PACKET_START_BYTE) {
                _rxState = BUS_S_LEADIN;
            }
            break;

        case BUS_S_LEADIN:
            if (in == DCSBIOS_PACKET_LEADIN_BYTE)
            {
                _rxState = BUS_S_ADDR;
                _checksum = 0;
            } else {
                reset();
            }
            break;

        case BUS_S_ADDR:
            _rxState = BUS_S_BANK;
            _rxCommand = (in & 0xE0) >> 5;
            _rxAddress = (in & 0x1f);
            _checksum += in;
            break;

        case BUS_S_BANK:
            _rxBank = (in & 0xE0) >> 5;
            _rxSize = (in & 0x1F);
            _checksum += in;

            if (_rxSize > DCSBIOS_MAX_DATA_SIZE) {
                _rxErrors++;
                reset();
            }

            if (_rxSize > 0) {
                _rxState = BUS_S_DATA;
            } else {
                _rxState = BUS_S_CHECKSUM;
            }
            break;

        case BUS_S_DATA:
            _checksum += in;
            _rxBuffer[_rxPointer++];
            if (_rxPointer == _rxSize) {
                _rxState = BUS_S_CHECKSUM;
            }
            break;          

        case BUS_S_CHECKSUM:    
            if (in == _checksum)
            {
                reset();
                return true;
            } else {
                _rxErrors++;
                reset();
            }
            break;
    }

    return false;
}

void BusParser::reset() {
    _rxState = BUS_S_START;
    _rxPointer = 0;
}