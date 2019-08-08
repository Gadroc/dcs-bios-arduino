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
    static   uint8_t _dummyRegister;                   // Dummy register to read from if bad pin config
    static constexpr unsigned long _readInterval = 5;  // Interval between reads of pin

	const    uint8_t _bitMask;			// Bit mask of pin in register
	volatile uint8_t *_inputRegister;   // Read register for pin

	unsigned long _lastRead;       // Last read time in millis
	unsigned int  _readBuffer;     // Buffer of bit reads for the pin
	bool          _state;          // Last known state

	static void turnOffPWM(uint8_t timer);

public:
	explicit DirectInputPin(uint8_t pin);

	bool    isValid() override;
	uint8_t readState() override;
};

#endif