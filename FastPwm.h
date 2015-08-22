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
#ifndef _DCSBIOS_FASTPWM_H_
#define _DCSBIOS_FASTPWM_H_

#include <Arduino.h>
#include "FastPin.h"

// Helper class to do highspeed analog output on the arduino.
// Built in routines are incredibly slow due to significant saftey 
// checks and runtime lookup of port and masks.  This class sacrifices
// some memory in order to do look ups once on port and mask values, and only
// does pin setup in begin call.
class FastPwm : private FastPin
{
private:		
	uint8_t _timer;

public:
	FastPwm();
	FastPwm(uint8_t pin);

	uint8_t getPin();
	void setPin(uint8_t pin);

	bool isSetup();

	void write(int value);
};

inline FastPwm::FastPwm() : FastPin() {
}

inline uint8_t FastPwm::getPin()
{
	return FastPin::getPin();
}

inline bool FastPwm::isSetup() {
	return FastPin::isSetup();
}

#endif