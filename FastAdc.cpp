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
#include "FastAdc.h"
#include "DcsBiosCommon.h"

FastAdc::FastAdc(uint8_t pin, uint8_t analog_reference) {
    setPin(pin, analog_reference);
}

void FastAdc::setPin(uint8_t pin, uint8_t analog_reference) 
{
    if (_pinNumber = 0xff) {
        _pinNumber = pin;
        _reference = analog_reference;

#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
    if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#endif
    pin = analogPinToChannel(pin);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    if (pin >= 54) pin -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
    if (pin >= 18) pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
    if (pin >= 24) pin -= 24; // allow for channel or pin numbers
#else
    if (pin >= 14) pin -= 14; // allow for channel or pin numbers
#endif

        _channel = pin;
    }
}

int FastAdc::read() {
    uint8_t low, high;

#if defined(ADCSRB) && defined(MUX5)
    // the MUX5 bit of ADCSRB selects whether we're reading from channels
    // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
    ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((_channel >> 3) & 0x01) << MUX5);
#endif
  
    // set the analog reference (high two bits of ADMUX) and select the
    // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
    // to 0 (the default).
#if defined(ADMUX)
    ADMUX = (_reference << 6) | (_channel & 0x07);
#endif

    // without a delay, we seem to read from the wrong channel
    //delay(1);

#if defined(ADCSRA) && defined(ADCL)
    // start the conversion
    sbi(ADCSRA, ADSC);

    // ADSC is cleared when the conversion finishes
    while (bit_is_set(ADCSRA, ADSC));

    // we have to read ADCL first; doing so locks both ADCL
    // and ADCH until ADCH is read.  reading ADCL second would
    // cause the results of each conversion to be discarded,
    // as ADCL and ADCH would be locked when it completed.
    low  = ADCL;
    high = ADCH;
#else
    // we dont have an ADC, return 0
    low  = 0;
    high = 0;
#endif

    // combine the two bytes
    return (high << 8) | low;    
}