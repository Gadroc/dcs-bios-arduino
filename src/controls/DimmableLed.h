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
#ifndef _DCSBIOS_DIMMABLELED_H_
#define _DCSBIOS_LED_H_

#include <stdint.h>
#include "hal/AnalogOutput.h"
#include "dcs/IntegerListener.h"

class DimmableLed : public IntegerListener {
private:
    AnalogOutput& _output;

public:
    DimmableLed(unsigned int address, unsigned int mask, uint8_t shift, uint8_t pin);
    DimmableLed(unsigned int address, unsigned int mask, uint8_t shift, AnalogOutput& output);

    void onDcsBiosFrameSync() override;
};

#endif