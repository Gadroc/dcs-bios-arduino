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
#ifndef _DCSBIOS_FASTPIN_H_
#define _DCSBIOS_FASTPIN_H_

#include <Arduino.h>

namespace DcsBios {
	// Helper class to do highspeed digital input/output on the arduino.
	// Built in routines are incredibly slow due to significant saftey 
	// checks and runtime lookup of port and masks.  This class sacrifices
	// some memory in order to do look up once of these values, and only
	// do pin setup in begin call.
	class FastPin 
	{
	private:		
		uint8_t _pinNumber;					// Pin to set high for TX
		uint8_t _bitMask;					// Bit mask of TX pin
		volatile uint8_t *_inputRegister;   // Register for TX pin
		volatile uint8_t *_outputRegister;	// Register for TX pin

	public:
		FastPin();

		uint8_t getPin();
		void setPin(uint8_t pin);
		void setMode(uint8_t mode);

		void clear();
		void set();
		uint8_t read();
	};

	inline FastPin::FastPin() 
	{
		_pinNumber = 0xff;
	}

	inline uint8_t FastPin::getPin()
	{
		return _pinNumber;
	}

	inline void FastPin::setPin(uint8_t pin) 
	{
		_pinNumber = pin;
	}

	inline void FastPin::clear()
	{
		uint8_t oldSREG = SREG;
		cli();
		 *_outputRegister &= ~_bitMask; 
	 	SREG = oldSREG;		
	}

	inline void FastPin::set()
	{
		uint8_t oldSREG = SREG;
		cli();
		*_outputRegister |= _bitMask;
	 	SREG = oldSREG;		
	}

	inline uint8_t FastPin::read()
	{
		return (*_inputRegister & _bitMask) ? 0 : 1;
	}
}

#endif