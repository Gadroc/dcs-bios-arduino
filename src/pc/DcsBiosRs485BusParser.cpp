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
#include "DcsBiosRs485BusParser.h"

DcsBiosRs485BusParser::DcsBiosRs485BusParser() {
    _packetState = PACKET_START;
    _packetDataByteReady = false;
}

void DcsBiosRs485BusParser::processByte(int in) {
    _packetDataByteReady = false;
    if (_packetState == PACKET_COMPLETE || _packetState == PACKET_TIMEOUT) {
        _packetState = PACKET_START;        
    }

    if (in > -1) {
        switch(_packetState) {
            case PACKET_START:
                if (in == DCSBIOS_RS485_PACKET_START_BYTE) {
                    _packetState = PACKET_LEADIN;
                }
                break;

            case PACKET_LEADIN:
                if (in == DCSBIOS_RS485_PACKET_LEADIN_BYTE)
                {
                    _packetState = PACKET_ADDRTYPE;
                } else {
                    _packetState = PACKET_START;
                }
                break;

            case PACKET_ADDRTYPE:
                _packetState = PACKET_SIZE;
                _packetAddressType = in;
                break;

            case PACKET_SIZE:
                _packetDataSize = in;
                _packetDataRemaining = in;

                if (_packetDataSize > DCSBIOS_RS485_MAX_PACKET_DATA_SIZE) {
                    _packetState = PACKET_START;
                }

                if (_packetDataSize > 0) {
                    _packetState = PACKET_DATA;
                } else {
                    _packetState = PACKET_COMPLETE;
                }
                break;

            case PACKET_DATA:
                _packetDataRemaining--;
                _packetDataByteReady = true;
                _packetDataByte = in;
                if (_packetDataRemaining == 0) {
                    _packetState = PACKET_COMPLETE;
                }
                break;
        }

        if (_packetState != PACKET_START) {
            _packetTimeout = millis() + PACKET_TIMEOUT_DURATION;
        }

    } else if (_packetState != PACKET_START && (long)(millis() - _packetTimeout) >= 0) {
        _packetState = PACKET_TIMEOUT;
    }
}

void DcsBiosRs485BusParser::sendPacket(Stream& stream, uint8_t packetType, uint8_t address, uint8_t* data, uint8_t size, uint8_t offset) {
    stream.write(DCSBIOS_RS485_PACKET_START_BYTE);
    stream.write(DCSBIOS_RS485_PACKET_LEADIN_BYTE);
    stream.write((address & 31) | (packetType << 5));
    if (data == 0 || size == 0) {
        stream.write((uint8_t)0);
    } else {
        stream.write(size);
        for (int i=0;i<size;i++) {
            stream.write(data[offset+i]);
        }
    }
}