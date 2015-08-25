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
#ifndef _DCSBIOS_DIRECTINPUTPIN_H_
#define _DCSBIOS_DIRECTINPUTPIN_H_

#include <Arduino.h>
#include "InputPin.h"

class DirectInputPin : public InputPin
{
private:		
	uint8_t _bitMask;					// Bit mask of pin in register
	volatile uint8_t *_inputRegister;   // Read register for pin

	static void turnOffPWM(uint8_t timer);

public:
	DirectInputPin();
	DirectInputPin(uint8_t pin, uint8_t debounceTime = 10);
	void setPin(uint8_t pin, uint8_t debounceTime = 10);

	uint8_t readState();
};

inline DirectInputPin::DirectInputPin() {}

inline uint8_t DirectInputPin::readState()
{
	return (*_inputRegister & _bitMask) ? HIGH : LOW;
}

#endif