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
    _txPin.setPin(txPin, OUTPUT);
    _txPin.clear();

    _bus = busStream;
    _pc = pcStream;

    for (uint8_t i=0; i<DCSBIOS_BANK_COUNT; i++) {
        for (uint8_t j=0; j<DCSBIOS_MAX_DATA_SIZE; j++) {
            _bank[i][j] = 0;
        }
        _bankReady[i] = false;
        _bankChecksum[i] = 0;
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
    uint8_t address = 0x80 | _pollingAddress;
    
    if (_txPin.isSetup()) {
        _txPin.set();
    }

    _bus->write(DCSBIOS_PACKET_START_BYTE);
    _bus->write(DCSBIOS_PACKET_LEADIN_BYTE);
    _bus->write(address);
    if (_bankReady[_pollingBank]) {
        uint8_t size = (_pollingBank << 5) | DCSBIOS_MAX_DATA_SIZE;
        _bus->write(size);
        _bus->write(_bank[_pollingBank], DCSBIOS_MAX_DATA_SIZE);
        _bus->write(address + size + _bankChecksum[_pollingBank]);
        _bankReady[_pollingBank] = false;
    } else {
        _bus->write((uint8_t)0x00);
        _bus->write(address);
    }
    _bus->write(DCSBIOS_PACKET_TRAIL_BYTE);
    _bus->flush();
    if (_txPin.isSetup()) {
        _txPin.clear();
    }

    _pollingAddress++;
    _pollingBank++;

    if (_pollingBank >= DCSBIOS_BANK_COUNT) {
        _pollingBank = 0;
    }
    if (_pollingAddress > DCSBIOS_MAX_DEVICE_ADDRESS) {
        _pollingAddress = 0;
    }
  
    _waitingResponse = true;
    _timeout = millis() + DCSBIOS_DEVICE_RESPONSE_TIMEOUT;
}

void BusController::processBusInput() {
    bool packetReady = _parser.processByte(_bus->read());

    if (packetReady) {
        processBusPacket();
        _waitingResponse = false;
    } else if ((long)(millis() - _timeout) >= 0) {
        _deviceTimeouts++;
        _waitingResponse = false;
    }
}

void BusController::processBusPacket() {
    // TODO Hmm
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
    for (uint8_t i=0; i<DCSBIOS_BANK_COUNT; i++) {
        int amountRead = _pc->readBytes(_bank[i], DCSBIOS_MAX_DATA_SIZE);
        if (amountRead == DCSBIOS_MAX_DATA_SIZE) {
            uint8_t checksum = 0;
            for (uint8_t j=0; j<DCSBIOS_MAX_DATA_SIZE; j++) {
                checksum += _bank[i][j];
            }
            _bankReady[i] = true;
            _bankChecksum[i] = checksum;
        } else {
            break;
        }        
    }
}

BusController DcsBiosController;