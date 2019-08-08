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
#include "Switch2Pos.h"
#include "hal/DirectInputPin.h"

Switch2Pos::Switch2Pos(const char message[], uint8_t pin) : PollingSwitch(message), _inputPin(*(new DirectInputPin(pin))) {
}

Switch2Pos::Switch2Pos(const char message[], InputPin& inputPin) : PollingSwitch(message), _inputPin(inputPin) {
}

uint8_t Switch2Pos::readState() {
    return _inputPin.readState() == HIGH ? 0 : 1;
}