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
#include "DcsBiosCommon.h"
#include "FastPin.h"

#define DCSBIOS_PACKET_START_BYTE 0xbb
#define DCSBIOS_PACKET_LEADIN_BYTE 0x88
#define DCSBIOS_PACKET_TRAIL_BYTE 0xFF

class DcsBiosBus {
    private:
        Stream* _stream;
        FastPin _txPin;

        uint8_t _packetState;
        uint8_t _packetAddress;
        uint8_t _packetType;
        uint8_t _packetBank;
        uint8_t _packetDataSize;
        uint8_t _packetDataPointer;
        uint8_t _packetDataBuffer[DCSBIOS_BUS_BUFFER_SIZE];
        uint8_t _packetChecksum;
        uint16_t _rxErrors;

        void resetBus();

    protected:
        void initBus(Stream* busStream, int txPin);

        uint8_t getPacketAddress();
        uint8_t getPacketType();
        uint8_t getPacketBank();
        uint8_t getPacketDataSize();
        uint8_t* getPacketDataBuffer();

        void sendPacket(uint8_t packetType, uint8_t address, uint8_t* data = 0, uint8_t size = 0);

        bool processBus();
};

inline uint8_t DcsBiosBus::getPacketAddress() {
    return _packetAddress;
}

inline uint8_t DcsBiosBus::getPacketType() {
    return _packetType;
}

inline uint8_t DcsBiosBus::getPacketBank() {
    return _packetBank;
}

inline uint8_t DcsBiosBus::getPacketDataSize() {
    return _packetDataSize;
}

inline uint8_t* DcsBiosBus::getPacketDataBuffer() {
    return _packetDataBuffer;
}

#endif