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
#include "DirectInputPin.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

uint8_t DirectInputPin::_dummyRegister = 0;

DirectInputPin::DirectInputPin(uint8_t pin) :  _bitMask(digitalPinToBitMask(pin))
{
    _lastRead = millis();
    _readBuffer = 0;
    _state = HIGH;

    uint8_t port = digitalPinToPort(pin);

    if (port == NOT_A_PIN) {
        _inputRegister = &DirectInputPin::_dummyRegister;
        return;
    }

    volatile uint8_t *modeRegister = portModeRegister(port);
    volatile uint8_t *outputRegister = portOutputRegister(port);

    _inputRegister = portInputRegister(port);

    uint8_t oldSREG = SREG;
    cli();
    *modeRegister &= ~_bitMask;
    *outputRegister |= _bitMask;
    SREG = oldSREG;

    uint8_t timer = digitalPinToTimer(pin);
    if (timer != NOT_ON_TIMER) turnOffPWM(timer);
}

bool DirectInputPin::isValid()
{
    return (_inputRegister != &DirectInputPin::_dummyRegister);
}

uint8_t DirectInputPin::readState()
{
    if (millis() - _lastRead >= _readInterval) {
        _readBuffer = (_readBuffer << 1u) | ((*_inputRegister & _bitMask) ? 1u : 0u) | 0xe000u;
        if      (_readBuffer == 0xf000u) { _state = LOW; }
        else if (_readBuffer == 0xffffu) { _state = HIGH; }
    }
    return _state;
}

void DirectInputPin::turnOffPWM(uint8_t timer)
{
	switch (timer)
	{
		#if defined(TCCR1A) && defined(COM1A1)
		case TIMER1A:   cbi(TCCR1A, COM1A1);    break;
		#endif
		#if defined(TCCR1A) && defined(COM1B1)
		case TIMER1B:   cbi(TCCR1A, COM1B1);    break;
		#endif
		
		#if defined(TCCR2) && defined(COM21)
		case  TIMER2:   cbi(TCCR2, COM21);      break;
		#endif
		
		#if defined(TCCR0A) && defined(COM0A1)
		case  TIMER0A:  cbi(TCCR0A, COM0A1);    break;
		#endif
		
		#if defined(TIMER0B) && defined(COM0B1)
		case  TIMER0B:  cbi(TCCR0A, COM0B1);    break;
		#endif
		#if defined(TCCR2A) && defined(COM2A1)
		case  TIMER2A:  cbi(TCCR2A, COM2A1);    break;
		#endif
		#if defined(TCCR2A) && defined(COM2B1)
		case  TIMER2B:  cbi(TCCR2A, COM2B1);    break;
		#endif
		
		#if defined(TCCR3A) && defined(COM3A1)
		case  TIMER3A:  cbi(TCCR3A, COM3A1);    break;
		#endif
		#if defined(TCCR3A) && defined(COM3B1)
		case  TIMER3B:  cbi(TCCR3A, COM3B1);    break;
		#endif
		#if defined(TCCR3A) && defined(COM3C1)
		case  TIMER3C:  cbi(TCCR3A, COM3C1);    break;
		#endif

		#if defined(TCCR4A) && defined(COM4A1)
		case  TIMER4A:  cbi(TCCR4A, COM4A1);    break;
		#endif					
		#if defined(TCCR4A) && defined(COM4B1)
		case  TIMER4B:  cbi(TCCR4A, COM4B1);    break;
		#endif
		#if defined(TCCR4A) && defined(COM4C1)
		case  TIMER4C:  cbi(TCCR4A, COM4C1);    break;
		#endif			
		#if defined(TCCR4C) && defined(COM4D1)
		case TIMER4D:	cbi(TCCR4C, COM4D1);	break;
		#endif			
			
		#if defined(TCCR5A)
		case  TIMER5A:  cbi(TCCR5A, COM5A1);    break;
		case  TIMER5B:  cbi(TCCR5A, COM5B1);    break;
		case  TIMER5C:  cbi(TCCR5A, COM5C1);    break;
		#endif
	}
}