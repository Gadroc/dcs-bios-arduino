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
#ifndef _DCSBIOS_ANALOGINPUT_H_
#define _DCSBIOS_ANALOGINPUT_H_

// Interface for all AnalogInputs
class AnalogInput
{
public:
    // Returns the maximum value that this adc can return.
    virtual int maxValue() = 0;

    // Returns a value between 0 and maxValue() as the measurement from the ADC.
    virtual int read() = 0;
};

#endif