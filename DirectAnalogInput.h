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
#ifndef _DCSBIOS_DIRECTANALOGINPUT_H_
#define _DCSBIOS_DIRECTANALOGINPUT_H_

#include <Arduino.h>
#include <AnalogInput.h>

// Helper class to do highspeed analog input on the arduino.
// Built in routines are slow due to significant saftey 
// checks and runtime lookup of port and masks.  This class sacrifices
// some memory in order to do look ups once on port and mask values, and only
// does pin setup in begin call.
class DirectAnalogInput : public AnalogInput
{
private:
    uint8_t _channel;       // Analog channel to read from for pin
    uint8_t _reference;

public:
    DirectAnalogInput();
    DirectAnalogInput(uint8_t pin, uint8_t analog_reference = DEFAULT);
    
    void setPin(uint8_t pin, uint8_t analog_reference = DEFAULT);

    virtual int maxValue();
    virtual int read();
};

inline DirectAnalogInput::DirectAnalogInput() {}

inline int DirectAnalogInput::maxValue() { return 1023; }

#endif