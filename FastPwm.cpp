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
#include "FastPwm.h"
#include "DcsBiosCommon.h"

FastPwm::FastPwm(uint8_t pin) {
	setPin(pin);
}

void FastPwm::setPin(uint8_t pin) 
{
	FastPin::setPin(pin, OUTPUT);
	_timer = digitalPinToTimer(pin);
}

void FastPwm::write(int val) {
	switch(_timer) {
		// XXX fix needed for atmega8
		#if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
		case TIMER0A:
			// connect pwm to pin on timer 0
			sbi(TCCR0, COM00);
			OCR0 = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR0A) && defined(COM0A1)
		case TIMER0A:
			// connect pwm to pin on timer 0, channel A
			sbi(TCCR0A, COM0A1);
			OCR0A = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR0A) && defined(COM0B1)
		case TIMER0B:
			// connect pwm to pin on timer 0, channel B
			sbi(TCCR0A, COM0B1);
			OCR0B = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR1A) && defined(COM1A1)
		case TIMER1A:
			// connect pwm to pin on timer 1, channel A
			sbi(TCCR1A, COM1A1);
			OCR1A = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR1A) && defined(COM1B1)
		case TIMER1B:
			// connect pwm to pin on timer 1, channel B
			sbi(TCCR1A, COM1B1);
			OCR1B = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR1A) && defined(COM1C1)
		case TIMER1C:
			// connect pwm to pin on timer 1, channel B
			sbi(TCCR1A, COM1C1);
			OCR1C = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR2) && defined(COM21)
		case TIMER2:
			// connect pwm to pin on timer 2
			sbi(TCCR2, COM21);
			OCR2 = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR2A) && defined(COM2A1)
		case TIMER2A:
			// connect pwm to pin on timer 2, channel A
			sbi(TCCR2A, COM2A1);
			OCR2A = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR2A) && defined(COM2B1)
		case TIMER2B:
			// connect pwm to pin on timer 2, channel B
			sbi(TCCR2A, COM2B1);
			OCR2B = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR3A) && defined(COM3A1)
		case TIMER3A:
			// connect pwm to pin on timer 3, channel A
			sbi(TCCR3A, COM3A1);
			OCR3A = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR3A) && defined(COM3B1)
		case TIMER3B:
			// connect pwm to pin on timer 3, channel B
			sbi(TCCR3A, COM3B1);
			OCR3B = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR3A) && defined(COM3C1)
		case TIMER3C:
			// connect pwm to pin on timer 3, channel C
			sbi(TCCR3A, COM3C1);
			OCR3C = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR4A)
		case TIMER4A:
			//connect pwm to pin on timer 4, channel A
			sbi(TCCR4A, COM4A1);
			#if defined(COM4A0)		// only used on 32U4
			cbi(TCCR4A, COM4A0);
			#endif
			OCR4A = val;	// set pwm duty
			break;
		#endif
		
		#if defined(TCCR4A) && defined(COM4B1)
		case TIMER4B:
			// connect pwm to pin on timer 4, channel B
			sbi(TCCR4A, COM4B1);
			OCR4B = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR4A) && defined(COM4C1)
		case TIMER4C:
			// connect pwm to pin on timer 4, channel C
			sbi(TCCR4A, COM4C1);
			OCR4C = val; // set pwm duty
			break;
		#endif
			
		#if defined(TCCR4C) && defined(COM4D1)
		case TIMER4D:				
			// connect pwm to pin on timer 4, channel D
			sbi(TCCR4C, COM4D1);
			#if defined(COM4D0)		// only used on 32U4
			cbi(TCCR4C, COM4D0);
			#endif
			OCR4D = val;	// set pwm duty
			break;
		#endif

						
		#if defined(TCCR5A) && defined(COM5A1)
		case TIMER5A:
			// connect pwm to pin on timer 5, channel A
			sbi(TCCR5A, COM5A1);
			OCR5A = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR5A) && defined(COM5B1)
		case TIMER5B:
			// connect pwm to pin on timer 5, channel B
			sbi(TCCR5A, COM5B1);
			OCR5B = val; // set pwm duty
			break;
		#endif

		#if defined(TCCR5A) && defined(COM5C1)
		case TIMER5C:
			// connect pwm to pin on timer 5, channel C
			sbi(TCCR5A, COM5C1);
			OCR5C = val; // set pwm duty
			break;
		#endif

		case NOT_ON_TIMER:
		default:
			if (val < 128) {
				clear();
			} else {
				set();
			}		
	}
}