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

#include "BusDevice.h"
#include "BufferListener.h"

void BusDevice::begin(Stream *busStream, int txPin, uint8_t address, uint8_t bank) {
    _address = address;
    _bank = bank;

    _txPin.setPin(txPin, OUTPUT);
    _txPin.clear();

    _bus = busStream;
}

void BusDevice::process() {
    int data = _bus->read();
    if (data > -1 && _parser.processByte(data)) {
        processPacket();
    }
}

void BusDevice::processPacket() {
    if (_parser.getBank() == _bank && _parser.hasData()) {
        BufferListener::handleBufferReady(_parser.getBuffer());
    }
    if (_parser.getAddress() == _address && _parser.getCommand() == 0x04) {
        if (_txPin.isSetup()) {
            _txPin.set();
        }
        _bus->write(DCSBIOS_PACKET_TRAIL_BYTE);
        _bus->write(DCSBIOS_PACKET_START_BYTE);
        _bus->write(DCSBIOS_PACKET_LEADIN_BYTE);
        _bus->write(_address);
        _bus->write((uint8_t)0);
        _bus->write(_address);
        _bus->write(DCSBIOS_PACKET_TRAIL_BYTE);
        _bus->flush();
        if (_txPin.isSetup()) {
            _txPin.clear();
        }
    }
}

BusDevice DcsBiosDevice;