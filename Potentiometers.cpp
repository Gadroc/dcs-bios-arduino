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
#include "Potentiometers.h"
#include "DirectAnalogInput.h"

Potentiometer::Potentiometer(const char* message, uint8_t pin, int threshold) : ReadInput(message) {
    Potentiometer(message, new DirectAnalogInput(pin));
}

Potentiometer::Potentiometer(const char* message, AnalogInput* input, int threshold) : ReadInput(message) {
    _input = input;
    _threshold = threshold;
    _lastState = _input->read();
}

void Potentiometer::readInput() {
    unsigned int reading = _input->read();
    if (abs(reading-_lastState) > _threshold) {
        sendMessage(map(reading, 0, _input->maxValue(), 0, 65535));
        _lastState = reading;
    }
}