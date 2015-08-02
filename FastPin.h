/*
	Copyright 2013 Craig Courtney

    This file is part of EOS-Firmware.

    EOS-Firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    EOS-Firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with EOS-Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _DCSBIOS_FASTPIN_H_
#define _DCSBIOS_FASTPIN_H_

#include <Arduino.h>

namespace DcsBios {
	class FastPin 
	{
	private:		
		uint8_t _pinNumber;					// Pin to set high for TX
		uint8_t _bitMask;					// Bit mask of TX pin
		volatile uint8_t *_inputRegister;   // Register for TX pin
		volatile uint8_t *_outputRegister;	// Register for TX pin

	public:
		EOSDigitalPin();

		uint8_t getPin();
		void setPin(uint8_t pin);
		void setMode(uint8_t mode);

		void clear();
		void set();
		uint8_t read();
	};

	inline EOSDigitalPin::EOSDigitalPin() 
	{
		_pinNumber = 0xff;
	}

	inline uint8_t EOSDigitalPin::getPin()
	{
		return _pinNumber;
	}

	inline void EOSDigitalPin::setPin(uint8_t pin) 
	{
		_pinNumber = pin;
	}

	inline void EOSDigitalPin::clear()
	{
		uint8_t oldSREG = SREG;
		cli();
		 *_outputRegister &= ~_bitMask; 
	 	SREG = oldSREG;		
	}

	inline void EOSDigitalPin::set()
	{
		uint8_t oldSREG = SREG;
		cli();
		*_outputRegister |= _bitMask;
	 	SREG = oldSREG;		
	}

	inline uint8_t EOSDigitalPin::read()
	{
		return (*_inputRegister & _bitMask) ? 0 : 1;
	}
}

#endif