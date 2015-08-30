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
#include "DcsBiosRs485Device.h"

uint8_t DcsBiosRs485Device::pollingResponseBuffer[DCSBIOS_RS485_MAX_PACKET_DATA_SIZE];
uint8_t DcsBiosRs485Device::pollingResponseBufferSize;

void DcsBiosRs485Device::begin(Stream* busStream, int txPin, uint8_t address) {
    _busStream = busStream;
    _busTxPin.setPin(txPin);
    _busTxPin.clear();
    _address = address;
}

void DcsBiosRs485Device::process() {
    int in = _busStream->read();
    do {
        _bus.processByte(in);

        if (_bus.getPacketState() == PACKET_COMPLETE 
            && _bus.getPacketType() == DCSBIOS_RS485_PACKETYPE_POLLING_REQUEST 
            && _bus.getPacketAddress() == _address) {
            sendPollResponse();
        }
        
        if (_bus.hasPacketDataByte()) {
            _parser.processByte(_bus.getPacketDataByte());
        }

        in = _busStream->read();
    } while (in > -1);
}

void DcsBiosRs485Device::sendPollResponse() {
    delayMicroseconds(10);
    _busTxPin.set();
    _bus.sendPacket(_busStream, DCSBIOS_RS485_PACKETYPE_POLLING_RESPONSE, _address, pollingResponseBuffer, pollingResponseBufferSize);
    _busStream->flush();
    _busTxPin.clear();
    pollingResponseBufferSize = 0;
}

void DcsBiosRs485Device::addPollingResponseString(const char* string) {
    uint8_t i = 0;
    uint8_t data = string[i++];
    while(data > 0) {
        pollingResponseBuffer[pollingResponseBufferSize++] = data;
        data = string[i++];
    }
}

void DcsBiosRs485Device::sendDcsBiosMessage(const char* message, const char* arg) {
    uint8_t totalSize = sizeof(message) + sizeof(arg) + 2;
    if (pollingResponseBufferSize + totalSize < DCSBIOS_RS485_MAX_PACKET_DATA_SIZE) {
        addPollingResponseString(message);
        addPollingResponseString(" ");
        addPollingResponseString(arg);
        addPollingResponseString("\n");
    }       
}