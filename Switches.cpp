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

#include "Switches.h"
#include "FastInputPin.h"

PollingSwitch::PollingSwitch(char* message) {
    _message = message;
}

void PollingSwitch::init() {
    _lastState = readState();
}

void PollingSwitch::pollInput() {
    uint8_t state = readState();
    if (state != _lastState) {
        sendMessage(_message, state);
        _lastState = state;
    }
}

Switch2Pos::Switch2Pos(char* message, uint8_t pin, int debounceTime) : PollingSwitch(message) {
    Switch2Pos(message, new FastInputPin(pin, debounceTime));
}

Switch2Pos::Switch2Pos(char* message, InputPin* inputPin) : PollingSwitch(message) {
    _inputPin = inputPin;
    init();
}

uint8_t Switch2Pos::readState() {
    return _inputPin->readState() == HIGH ? 0 : 1;
}

Switch3Pos::Switch3Pos(char* message, uint8_t pinA, uint8_t pinB, int debounceTime) : PollingSwitch(message) {
    Switch3Pos(message, new FastInputPin(pinA, debounceTime), new FastInputPin(pinB, debounceTime));
}

Switch3Pos::Switch3Pos(char* message, InputPin* inputPinA, InputPin* inputPinB) : PollingSwitch(message) {
    _inputPinA = inputPinA;
    _inputPinB = inputPinB;
    init();
}

uint8_t Switch3Pos::readState() {
    if (_inputPinA->readState() == LOW) return 0;
    if (_inputPinB->readState() == LOW) return 2;
    return 1;
}

SwitchMultiPos::SwitchMultiPos(char* message, uint8_t* pins, uint8_t numberOfPins, int debounceTime) : PollingSwitch(message) {
    FastInputPin* fastPins = new FastInputPin[numberOfPins];
    _numberOfPins = numberOfPins;
    for(uint8_t i=0; i<numberOfPins; i++) {
        fastPins[i].setPin(pins[i], debounceTime);
    }
    SwitchMultiPos(message, fastPins, numberOfPins);
}

SwitchMultiPos::SwitchMultiPos(char* message, InputPin* inputPins, uint8_t numberOfPins) : PollingSwitch(message) {
    _inputPins = inputPins;
    _numberOfPins = numberOfPins;
    init();
}

uint8_t SwitchMultiPos::readState() {
    for (uint8_t i=0; i<_numberOfPins; i++) {
        if (_inputPins[i].readState() == LOW) return i;
    }
    return 0;
}