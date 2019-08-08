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
#include "Switch3Pos.h"
#include "hal/DirectInputPin.h"

Switch3Pos::Switch3Pos(const char message[], uint8_t pinA, uint8_t pinB) : PollingSwitch(message), _inputPinA(*(new DirectInputPin(pinA))), _inputPinB(*(new DirectInputPin(pinB))) {
}

Switch3Pos::Switch3Pos(const char message[], InputPin& inputPinA, InputPin& inputPinB) : PollingSwitch(message), _inputPinA(inputPinA), _inputPinB(inputPinB) {
}

uint8_t Switch3Pos::readState() {
    if (_inputPinA.readState() == LOW) return 0;
    if (_inputPinB.readState() == LOW) return 2;
    return 1;
}