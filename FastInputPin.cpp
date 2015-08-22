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

#include "FastInputPin.h"

FastInputPin::FastInputPin() {}

FastInputPin::FastInputPin(uint8_t pin, uint8_t debounceTime) {
    setPin(pin, debounceTime);
}

void FastInputPin::setPin(uint8_t pin, uint8_t debounceTime) {
    _pin.setPin(pin, INPUT_PULLUP);
    _debounceTime = debounceTime;
    _currentState = _pin.read();
    _lastRead = _pin.read();
}

uint8_t FastInputPin::readState() {
    uint8_t reading = _pin.read();

    if (reading != _lastRead) {
        _lastDebounceTime = millis();
    }

    if (((millis() - _lastDebounceTime) > _debounceTime) && reading != _currentState) {
        _currentState = reading;               
    }

    _lastRead = reading;

    return _currentState;
}