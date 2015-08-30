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
#include "DcsBiosRs485Controller.h"

// Command values from host system
#define PC_COMMAND_STATUS 's'
#define PC_COMMAND_ADD_EXPORT_DATA 'e'

#define PC_NOTIFICATION_SEND_DATA 'r'
#define PC_NOTIFICATION_TRANSMITTING_DATA 't'
#define PC_NOTIFICATION_COMMAND_TIMEOUT 'x'

#define PC_STATE_WAITING 0
#define PC_STATE_EXPORT_DATA_SIZE 1
#define PC_STATE_EXPORT_DATA 2

#define PC_COMMAND_TIMEOUT 250

void DcsBiosRs485Controller::begin(Stream* busStream, int txPin, Stream* pcStream) {
    _busStream = busStream;
    _busTxPin.setPin(txPin);
    _busTxPin.clear();
    _busBufferSize = 0;

    _pcStream = pcStream;
    _pcBufferSize = 0;
    _pcBufferLoadIndex = 0;
    _pcBufferTxIndex = 0;
    _pcCommandState = PC_STATE_WAITING;

    _busWaitingResponse = false;
    _busPollingAddress = 0;
}

void DcsBiosRs485Controller::process() {
    processPcInput();
    if (_busWaitingResponse) {
        processBusInput();
    } else {
        sendPollingPacket();
    }        
}

void DcsBiosRs485Controller::sendPollingPacket() {
    uint8_t address = _busPollingAddress;    

    // Make sure device has enough time to turn off transmit.
    delayMicroseconds(10);

    _busTxPin.set();
    if (_pcCommandState == PC_STATE_WAITING && _pcBufferSize > 0) {
        uint8_t size = _pcBufferSize < DCSBIOS_MAX_PACKET_DATA_SIZE ? _pcBufferSize : DCSBIOS_MAX_PACKET_DATA_SIZE;
        _bus.sendPacket(_busStream, DCSBIOS_PACKETYPE_POLLING_REQUEST, _busPollingAddress, _pcBuffer, size);
        _pcBufferSize -= size;
        _pcBufferTxIndex += size;
        if (_pcBufferSize == 0) {
            _pcStream->write(PC_NOTIFICATION_SEND_DATA);
        }
    } else {
        _bus.sendPacket(_busStream, DCSBIOS_PACKETYPE_POLLING_REQUEST, _busPollingAddress);        
    }
    _busStream->flush();
    _busTxPin.clear();

    _busPollingAddress++;

    if (_busPollingAddress > DCSBIOS_MAX_DEVICE_ADDRESS) {
        _busPollingAddress = 0;
    }
  
    _busWaitingResponse = true;
    _busPollingTimeout = millis() + DCSBIOS_RS485_DEVICE_RESPONSE_TIMEOUT;
}

void DcsBiosRs485Controller::processBusInput() {
    int in = _busStream->read();

    do {
        _bus.processByte(in);

        if (_bus.hasPacketDataByte() && _busBufferSize < DCSBIOS_MAX_PACKET_DATA_SIZE) {
            _busBuffer[_busBufferSize++] = _bus.getPacketDataByte();
        }

        switch (_bus.getPacketState()) {
            case PACKET_COMPLETE:
                processBusPacket();
            case PACKET_TIMEOUT:
                _busWaitingResponse = false;
                break;
            case PACKET_START:
                if ((long)(millis() - _busPollingTimeout) >= 0) {
                    _busWaitingResponse = false;
                }
                break;
        }

        in = _busStream->read();
    } while (in > -1);
}

void DcsBiosRs485Controller::processBusPacket() {
    if (_busBufferSize > 0 && _bus.getPacketType() == DCSBIOS_PACKETYPE_POLLING_RESPONSE) {
        _pcStream->write('m');
        _pcStream->write(_busBufferSize);
        _pcStream->write(_busBuffer, _busBufferSize);
        _busBufferSize = 0;
    }
}

void DcsBiosRs485Controller::processPcInput() {
    unsigned long current = millis();
    int in = _pcStream->read();
    if (in == -1) {
        if (_pcCommandState != PC_STATE_WAITING && (long)(current - _pcCommandTimeout) >= 0) {
            _pcCommandState = PC_STATE_WAITING;
            _pcStream->write(PC_NOTIFICATION_COMMAND_TIMEOUT);
        }
    } else {
        switch(_pcCommandState) {
            case PC_STATE_WAITING:
                parseCommand(in);
                break;

            case PC_STATE_EXPORT_DATA_SIZE:
                _pcCommandTimeout = current + PC_COMMAND_TIMEOUT;
                parseExportDataSize(in);
                break;

            case PC_STATE_EXPORT_DATA:
                _pcCommandTimeout = current + PC_COMMAND_TIMEOUT;
                parseExportData(in);
                break;
        }
    }
}

void DcsBiosRs485Controller::parseCommand(uint8_t in) {
    if (in == PC_COMMAND_STATUS) {
        if (_pcBufferSize > 0) {
            _pcStream->write(PC_NOTIFICATION_TRANSMITTING_DATA);
        } else {            
            _pcStream->write(PC_NOTIFICATION_SEND_DATA);
        }
    }

    if (in == PC_COMMAND_ADD_EXPORT_DATA && _pcBufferSize == 0) {
        _pcCommandState = PC_STATE_EXPORT_DATA_SIZE;
    }
}

void DcsBiosRs485Controller::parseExportDataSize(uint8_t in) {
    _pcBufferSize = in;
    _pcBufferLoadIndex = 0;
    if (_pcBufferSize == 0) {
        _pcCommandState = PC_STATE_WAITING;
    } else {
        _pcCommandState = PC_STATE_EXPORT_DATA;
    }
}

void DcsBiosRs485Controller::parseExportData(uint8_t in) {
    _pcBuffer[_pcBufferLoadIndex++] = in;
    if (_pcBufferLoadIndex == _pcBufferSize) {
        _pcBufferTxIndex = 0;
        _pcCommandState = PC_STATE_WAITING;
    }
}