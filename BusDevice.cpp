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
#include "PollingInput.h"
#include "ReadInput.h"

void BusDevice::begin(Stream *busStream, int txPin, uint8_t address, uint8_t bank) {
    initBus(busStream, txPin);
    _address = address;
    _bank = bank;
}

void BusDevice::process() {
    if (processBus()) {
        processPacket();
    }
    PollingInput::pollInputs();
}

void BusDevice::processPacket() {
    if (getPacketType() <= DCSBIOS_PACKETYPE_POLLING_UPDATE_BANK3) {
        if (getPacketBank() == _bank && getPacketDataSize() > 0) {
            BufferListener::handleBufferReady(getPacketDataBuffer());
        }
        if (getPacketAddress() == _address) {
            ReadInput::readInputs();
            sendPacket(DCSBIOS_PACKETYPE_SIMULATION_DATA, _address, Input::CommandBuffer, Input::CommandBufferSize);
            Input::CommandBufferSize = 0;
        }
    }
}

BusDevice DcsBiosDevice;