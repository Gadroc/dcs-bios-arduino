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

DcsBiosRs485Controller::DcsBiosRs485Controller(Stream* busStream, int txPin, Stream* pcStream) {
    _busStream = busStream;
    _busTxPin.setPin(txPin);
    _busTxPin.clear();
    _busBufferSize = 0;

    _pcStream = pcStream;

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
    if (_pc.bufferReady()) {
        _bus.sendPacket(_busStream, DCSBIOS_RS485_PACKETYPE_POLLING_REQUEST, _busPollingAddress, _pc.getCurrentBuffer(), _pc.getCurrentBufferSize());
        _pc.markBufferComplete();
    } else {
        _bus.sendPacket(_busStream, DCSBIOS_RS485_PACKETYPE_POLLING_REQUEST, _busPollingAddress);        
    }
    _busStream->flush();
    _busTxPin.clear();

    _busPollingAddress++;

    if (_busPollingAddress > DCSBIOS_RS485_MAX_DEVICE_ADDRESS) {
        _busPollingAddress = 0;
    }
  
    _busWaitingResponse = true;
    _busPollingTimeout = millis() + DCSBIOS_RS485_DEVICE_RESPONSE_TIMEOUT;
}

void DcsBiosRs485Controller::processBusInput() {
    int in = _busStream->read();

    do {
        _bus.processByte(in);

        if (_bus.hasPacketDataByte() && _busBufferSize < DCSBIOS_RS485_MAX_PACKET_DATA_SIZE) {
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
    if (_busBufferSize > 0 && _bus.getPacketType() == DCSBIOS_RS485_PACKETYPE_POLLING_RESPONSE) {
        _pcStream->write('m');
        _pcStream->write(_busBufferSize);
        _pcStream->write(_busBuffer, _busBufferSize);
        _busBufferSize = 0;
    }
}

void DcsBiosRs485Controller::processPcInput() {
    _pc.processByte(_pcStream->read());
    switch (_pc.getState()) {
        case PC_STATUSREADY:
            _pcStream->write(PC_NOTIFICATION_STATUSREADY);
            break;

        case PC_STATUSFULL:
            _pcStream->write(PC_NOTIFICATION_STATUSFULL);
            break;

        case PC_DATARECEIVED:
            _pcStream->write(PC_NOTIFICATION_DATARECEIVED);
            break;

        case PC_DATAERROR:
            _pcStream->write(PC_NOTIFICATION_DATAERROR);
            break;
    }
}