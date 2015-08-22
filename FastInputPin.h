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
#ifndef _DCSBIOS_FASTINPUTPIN_H_
#define _DCSBIOS_FASTINPUTPIN_H_

#include <Arduino.h>
#include "FastPin.h"
#include "InputPin.h"

// Implementation of an input pin based on direct reading of a Arduino pin status using FastPin.
class FastInputPin : public InputPin {
private:
    FastPin _pin;
    uint8_t _debounceTime;  // Amount of time to read same value before switching current state
    uint8_t _currentState; // Current state returned to input
    uint8_t _lastRead; // Last value read from pin
    long _lastDebounceTime; // Time since last state change on pin

public:
    FastInputPin();
    FastInputPin(uint8_t pin, uint8_t debounceTime = 10);
    void setPin(uint8_t pin, uint8_t debounceTime = 10);
    virtual uint8_t readState();
};

#endif