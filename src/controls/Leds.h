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
#include "hal/OutputPin.h"
#include "hal/AnalogOutput.h"
#include "dcs/IntegerListener.h"


class Led : public IntegerListener {
private:
	OutputPin* _pin;

public:
	Led(unsigned int address, unsigned int mask, uint8_t shift, uint8_t pin);
    Led(unsigned int address, unsigned int mask, uint8_t shift, OutputPin* pin);

    virtual void onDcsBiosFrameSync();    
};

class DimmableLed : public IntegerListener {
private:
    AnalogOutput* _output;

public:
    DimmableLed(unsigned int address, unsigned int mask, uint8_t shift, uint8_t pin);
    DimmableLed(unsigned int address, unsigned int mask, uint8_t shift, AnalogOutput* output);

    virtual void onDcsBiosFrameSync();    
};

#endif