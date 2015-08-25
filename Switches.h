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
#ifndef _DCSBIOS_SWITCHES_H_
#define _DCSBIOS_SWITCHES_H_

#include <Arduino.h>
#include "PollingInput.h"
#include "InputPin.h"

class PollingSwitch : public PollingInput {
private:    
    uint8_t _lastState;
    virtual void pollInput();

protected:
    void init();

public:
    PollingSwitch(const char* message);
    virtual uint8_t readState() = 0;
};

class Switch2Pos : public PollingSwitch {
private:
    InputPin* _inputPin;

public:
    Switch2Pos(const char* message, uint8_t pin, int debounceTime = 10);
    Switch2Pos(const char* message, InputPin* inputPin);
    virtual uint8_t readState();
};

class Switch3Pos : public PollingSwitch {
private:
    InputPin* _inputPinA;
    InputPin* _inputPinB;
    
public:
    Switch3Pos(const char* message, uint8_t pinA, uint8_t pinB, int debounceTime = 10);
    Switch3Pos(const char* message, InputPin* inputPinA, InputPin* inputPinB);
    virtual uint8_t readState();
};

class SwitchMultiPos : public PollingSwitch {
private: 
    InputPin* _inputPins;
    uint8_t _numberOfPins;

public:
    SwitchMultiPos(const char* message, const uint8_t* pins, uint8_t numberOfPins, int debounceTime = 10);
    SwitchMultiPos(const char* message, InputPin* inputPins, uint8_t numberOfPins);
    virtual uint8_t readState();
};

#endif