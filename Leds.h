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
#ifndef _DCSBIOS_LEDS_H_
#define _DCSBIOS_LEDS_H_

#include <Arduino.h>
#include "OutputPin.h"
#include "AnalogOutput.h"
#include "BufferListener.h"


class Led : BufferListener {
private:
	OutputPin* _pin;
	uint8_t _address;
	uint8_t _mask;

	void onBufferReady(uint8_t *buffer);

public:
	Led(uint8_t address, uint8_t mask, uint8_t pin);
    Led(uint8_t address, uint8_t mask, OutputPin* pin);
};

class DimmableLed : BufferListener {
private:
    AnalogOutput* _output;
    uint8_t _address;
    uint8_t _mask;

    void onBufferReady(uint8_t *buffer);

public:
    DimmableLed(uint8_t address, uint8_t pin);
    DimmableLed(uint8_t address, AnalogOutput* output);
};

#endif