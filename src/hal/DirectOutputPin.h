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
#ifndef _DCSBIOS_DIRECTOUTPUTPIN_H_
#define _DCSBIOS_DIRECTOUTPUTPIN_H_

#include <stdint.h>
#include "OutputPin.h"

// Helper class to do high-speed digital input/output on the Arduino.
// Built in routines are incredibly slow due to significant safety
// checks and runtime lookup of port and masks.  This class sacrifices
// some memory in order to do look ups once on port and mask values, and only
// does pin setup in begin call.
class DirectOutputPin : public OutputPin
{
private:
    static   uint8_t _dummyRegister;    // Dummy register to manipulate if pin number is bad
	const    uint8_t _bitMask;			// Bit mask of pin in register
	volatile uint8_t *_outputRegister;	// Write register for pin

	static void turnOffPWM(uint8_t timer);

public:
	explicit DirectOutputPin(uint8_t pin);

    bool isValid() override;
	void clear() override;
	void set() override;
};

#endif