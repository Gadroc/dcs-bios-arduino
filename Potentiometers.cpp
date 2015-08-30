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

Potentiometer::Potentiometer(const char* message, uint8_t pin, unsigned int threshold, unsigned int pollingInterval) : PollingInput(message) {
    _input = new DirectAnalogInput(pin);
    _threshold = threshold;
    _pollingInterval = pollingInterval;
}

Potentiometer::Potentiometer(const char* message, AnalogInput* input, unsigned int threshold, unsigned int pollingInterval) : PollingInput(message) {
    _input = input;
    _threshold = threshold;
    _pollingInterval = pollingInterval;
}

void Potentiometer::initInput() {
    _nextPoll = millis() + _pollingInterval;
    _lastState = _input->read();
}

void Potentiometer::pollInput() {
    unsigned long current = millis();
    if ((long)(current-_nextPoll) >= 0) {
        unsigned int reading = _input->read();
        if (abs(reading-_lastState) > _threshold) {
            sendMessage(map(reading, 0, _input->maxValue(), 0, 65535));
            _lastState = reading;
        }
        _nextPoll = current + _pollingInterval;        
    }
}