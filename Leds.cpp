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
#include "Leds.h"
#include "FastOutputPin.h"

Led::Led(uint8_t address, uint8_t mask, uint8_t pin) {
    Led(address, mask, new FastOutputPin(pin));
}

Led::Led(uint8_t address, uint8_t mask, OutputPin* pin) {    
    _address = address;
    _mask = mask;
    _pin = pin;
}

void Led::onBufferReady(uint8_t *buffer) {
    if (buffer[_address] & _mask) {
        _pin->set();
    } else {
        _pin->clear();
    }
}

PwmLed::PwmLed(uint8_t address, uint8_t pin) {
    _address = address;
    _pin.setPin(pin);
    _pin.write(0);
}

void PwmLed::onBufferReady(uint8_t *buffer) {
    unsigned int value = (buffer[_address]) << 8 | buffer[_address+1];
    _pin.write(map(value, 0, 65535, 0, 255));
}