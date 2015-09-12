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
#include "ActionButton.h"
#include "hal/DirectInputPin.h"

ActionButton::ActionButton(const char message[], const char arg[], uint8_t pin, int debounceTime) : PollingInput(message), _pin(*(new DirectInputPin(pin, debounceTime))) {
    _arg = arg;
}

ActionButton::ActionButton(const char message[], const char arg[], InputPin& pin) : PollingInput(message), _pin(pin) {
    _arg = arg;
}

void ActionButton::initInput() {
    _lastState = _pin.readState();
}

void ActionButton::pollInput() {
    uint8_t state = _pin.readState();
    if (state != _lastState) {
        if (state == LOW) {
            sendMessage(_arg);
        }
        _lastState = state;
    }
}