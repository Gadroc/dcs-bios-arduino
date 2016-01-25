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
#include "ExportStreamParser.h"
#include "PollingInput.h"
#include "ExportStreamListener.h"

extern void onDcsBiosFrameSync();

ExportStreamParser::ExportStreamParser() {
    _state = DCSBIOS_STATE_WAIT_FOR_SYNC;
    _sync_byte_count = 0;
}

void ExportStreamParser::processByte(uint8_t c) {
    switch(_state) {
    case DCSBIOS_STATE_WAIT_FOR_SYNC:
        /* do nothing */
        break;

    case DCSBIOS_STATE_ADDRESS_LOW:
        _address = (unsigned int)c;
        _state = DCSBIOS_STATE_ADDRESS_HIGH;
        break;

    case DCSBIOS_STATE_ADDRESS_HIGH:
        _address = (c << 8) | _address;
            if (_address != 0x5555) {
            _state = DCSBIOS_STATE_COUNT_LOW;
        } else {
            _state = DCSBIOS_STATE_WAIT_FOR_SYNC;
        }
        break;

    case DCSBIOS_STATE_COUNT_LOW:
        _count = (unsigned int)c;
        _state = DCSBIOS_STATE_COUNT_HIGH;
        break;

    case DCSBIOS_STATE_COUNT_HIGH:
        _count = (c << 8) | _count;
        _state = DCSBIOS_STATE_DATA_LOW;
        break;

    case DCSBIOS_STATE_DATA_LOW:
        _data = (unsigned int)c;
        _count--;
        _state = DCSBIOS_STATE_DATA_HIGH;
        break;

    case DCSBIOS_STATE_DATA_HIGH:
        _data = (c << 8) | _data;
        _count--;
        ExportStreamListener::handleDcsBiosWrite(_address, _data);
        if (_count == 0) {
            _state = DCSBIOS_STATE_ADDRESS_LOW;

            // Frame sync moved to end of frame.  All time consumeing updates should
            // be handled in framesync during the down time between frame transmissions.
            if (_address == 0xfffe) {
                ExportStreamListener::handleDcsBiosFrameSync();
                onDcsBiosFrameSync();
            }
        } else {
            _address += 2;
            _state = DCSBIOS_STATE_DATA_LOW;
        }
        break;
    }

    if (c == 0x55) {
        _sync_byte_count++;
    } else {
        _sync_byte_count = 0;
    }

    if (_sync_byte_count == 4) {
        _state = DCSBIOS_STATE_ADDRESS_LOW;
        _sync_byte_count = 0;        
    } 
}