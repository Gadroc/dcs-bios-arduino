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

#include "DcsBiosBus.h"

enum packet_state {
    BUS_S_START,        // Waiting for start byte
    BUS_S_LEADIN,       // Waiting for lead in byte
    BUS_S_ADDR,         // Waiting for addresss
    BUS_S_SIZE,         // Waiting for bank / data length
    BUS_S_DATA,         // Waiting on data load
    BUS_S_CHECKSUM      // Waiting on checksum
};

void DcsBiosBus::initBus(Stream* busStream, int txPin) {
    _txPin.setPin(txPin, OUTPUT);
    _txPin.clear();
    _stream = busStream;
    resetBus();
}

bool DcsBiosBus::processBus() {
    int in = _stream->read();
    if (in > -1) {
        switch(_packetState) {
            case BUS_S_START:
                if (in == DCSBIOS_PACKET_START_BYTE) {
                    _packetState = BUS_S_LEADIN;
                }
                break;

            case BUS_S_LEADIN:
                if (in == DCSBIOS_PACKET_LEADIN_BYTE)
                {
                    _packetState = BUS_S_ADDR;
                } else {
                    resetBus();
                }
                break;

            case BUS_S_ADDR:
                _packetState = BUS_S_SIZE;
                _packetAddressType = in;
                _packetChecksum = in;
                break;

            case BUS_S_SIZE:
                _packetDataSize = in;
                _packetChecksum += in;

                if (_packetDataSize > DCSBIOS_PACKETYPE_SIMULATION_DATA) {
                    _rxErrors++;
                    resetBus();
                }

                if (_packetDataSize > 0) {
                    _packetState = BUS_S_DATA;
                } else {
                    _packetState = BUS_S_CHECKSUM;
                }
                break;

            case BUS_S_DATA:
                _packetChecksum += in;
                _packetDataBuffer[_packetDataPointer++] = in;
                if (_packetDataPointer == _packetDataSize) {
                    _packetState = BUS_S_CHECKSUM;
                }
                break;          

            case BUS_S_CHECKSUM:    
                if (in == _packetChecksum)
                {
                    resetBus();
                    return true;
                } else {
                    _rxErrors++;
                    resetBus();
                }
                break;
        }
    }
    return false;
}

void DcsBiosBus::resetBus() {
    _packetState = BUS_S_START;
    _packetDataPointer = 0;
}

void DcsBiosBus::sendPacket(uint8_t packetType, uint8_t address, uint8_t* data, uint8_t size) {
    _txPin.set();
    _stream->write(DCSBIOS_PACKET_TRAIL_BYTE);
    _stream->write(DCSBIOS_PACKET_START_BYTE);
    _stream->write(DCSBIOS_PACKET_LEADIN_BYTE);
    uint8_t checksum = (address & 31) | (packetType << 5);
    _stream->write(checksum);
    if (data == 0 || size == 0) {
        _stream->write((uint8_t)0);
    } else {
        checksum += size;
        _stream->write(size);
        for (int i=0;i<size;i++) {
            checksum += data[i];
            _stream->write(data[i]);
        }
    }
    _stream->write(checksum);
    _stream->write(DCSBIOS_PACKET_TRAIL_BYTE);
    _stream->flush();
    _txPin.clear();
}