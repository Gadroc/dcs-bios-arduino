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
#ifndef _DCSBIOS_RS485BUSPARSER_H_
#define _DCSBIOS_RS485BUSPARSER_H_

#include <Arduino.h>

#define DCSBIOS_RS485_PACKET_START_BYTE 0xbb
#define DCSBIOS_RS485_PACKET_LEADIN_BYTE 0x88

#define DCSBIOS_RS485_MAX_DEVICE_ADDRESS 31
#define DCSBIOS_RS485_MAX_PACKET_DATA_SIZE 64

#define DCSBIOS_RS485_PACKETYPE_POLLING_REQUEST 0
#define DCSBIOS_RS485_PACKETYPE_POLLING_RESPONSE 1

#define PACKET_START 0    // Waiting for Packet Start
#define PACKET_LEADIN 1   // Waiting for Packet Lead In
#define PACKET_ADDRTYPE 2 // Waiting for Packet Address/Type
#define PACKET_SIZE 3     // Waiting for Packet Size
#define PACKET_DATA 4     // Waiting for Packet Data
#define PACKET_COMPLETE 5 // Packet is complete
#define PACKET_TIMEOUT 6  // Packet has timedout

#define PACKET_TIMEOUT_DURATION 2000

class DcsBiosRs485BusParser {
    private:
        uint8_t _packetState;        
        uint8_t _packetAddressType;
        uint8_t _packetDataSize;
        uint8_t _packetDataRemaining;
        bool _packetDataByteReady;
        uint8_t _packetDataByte;
        unsigned long _packetTimeout;

    public:
        DcsBiosRs485BusParser();
        
        uint8_t getPacketState();
        uint8_t getPacketAddress();
        uint8_t getPacketType();
        uint8_t getPacketDataSize();
        bool hasPacketDataByte();
        uint8_t getPacketDataByte();

        void sendPacket(Stream& stream, uint8_t packetType, uint8_t address, uint8_t* data = 0, uint8_t size = 0, uint8_t offset = 0);

        void processByte(int in);
};

inline bool DcsBiosRs485BusParser::hasPacketDataByte() {
    return _packetDataByteReady;
}

inline uint8_t DcsBiosRs485BusParser::getPacketDataByte() {
    return _packetDataByte;
}

inline uint8_t DcsBiosRs485BusParser::getPacketState() {
    return _packetState;
}

inline uint8_t DcsBiosRs485BusParser::getPacketAddress() {
    return _packetAddressType & DCSBIOS_RS485_MAX_DEVICE_ADDRESS;
}

inline uint8_t DcsBiosRs485BusParser::getPacketType() {
    return _packetAddressType >> 5;
}

inline uint8_t DcsBiosRs485BusParser::getPacketDataSize() {
    return _packetDataSize;
}

#endif