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

#include "BusController.h"

// Command values from host system
#define PC_COMMAND_UPDATEDATA 98

void BusController::begin(Stream* busStream, int txPin, Stream* pcStream) {
    initBus(busStream, txPin);
    _pcReceivePin.setPin(13, OUTPUT);
    _pcReceivePin.clear();

    _pc = pcStream;

    for (uint8_t i=0; i<DCSBIOS_BANK_COUNT; i++) {
        for (uint8_t j=0; j<DCSBIOS_BANK_SIZE; j++) {
            _bank[i][j] = 0;
        }
        _bankReady[i] = false;
    }

    _waitingResponse = false;
    _pollingAddress = 0;
    _pollingBank = 0;
    _deviceTimeouts = 0;
}

void BusController::process() {
    processPcInput(_pc->read());
    if (_waitingResponse) {
        processBusInput();
    } else {
        sendPollingPacket();
    }
}

void BusController::sendPollingPacket() {
    uint8_t address = _pollingAddress;
    
    if (_bankReady[_pollingBank]) {
        sendPacket(_pollingBank, _pollingAddress, _bank[_pollingBank], DCSBIOS_BANK_SIZE);
        _bankReady[_pollingBank] = false;
    } else {
        sendPacket(DCSBIOS_PACKETYPE_POLLING_UPDATE_BANK0, _pollingAddress);
    }

    _pollingAddress++;
    _pollingBank++;

    if (_pollingBank >= DCSBIOS_BANK_COUNT) {
        _pollingBank = 0;
    }
    if (_pollingAddress > 15) {
        _pollingAddress = 0;
    }
  
    _waitingResponse = true;
    _timeout = millis() + DCSBIOS_DEVICE_RESPONSE_TIMEOUT;
}

void BusController::processBusInput() {
    bool packetReady = processBus();

    if (packetReady) {
        processBusPacket();
        _waitingResponse = false;
    } else if ((long)(millis() - _timeout) >= 0) {
        _deviceTimeouts++;
        _waitingResponse = false;
    }
}

void BusController::processBusPacket() {
    if (getPacketType() == DCSBIOS_PACKETYPE_SIMULATION_DATA) {
        uint8_t size = getPacketDataSize();
        if (size > 0) {
            _pc->write(DCSBIOS_PACKET_START_BYTE);
            _pc->write(DCSBIOS_PACKET_LEADIN_BYTE);
            uint8_t checksum = getPacketAddress();
            _pc->write(checksum);
            checksum += size;
            _pc->write(size);
            uint8_t* data = getPacketDataBuffer();
            for (int i=0;i<size;i++) {
                checksum += data[i];
                _pc->write(data[i]);
            }
            _pc->write(checksum);        
        }
    }
}

void BusController::processPcInput(int in) {
    if (in == -1) {
        return;
    }

    if (in == PC_COMMAND_UPDATEDATA) {
        loadPcData();
    }
}

void BusController::loadPcData() {
    _pcReceivePin.set();
    for (uint8_t i=0; i<DCSBIOS_BANK_COUNT; i++) {
        int amountRead = _pc->readBytes(_bank[i], DCSBIOS_BANK_SIZE);
        if (amountRead == DCSBIOS_BANK_SIZE) {
            _bankReady[i] = true;
        } else {
            break;
        }        
    }
    _pcReceivePin.clear();
}

BusController DcsBiosController;