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
#ifndef _DCSBIOS_SWITCHMULTIPOS_H_
#define _DCSBIOS_SWITCHMULTIPOS_H_

#include <Arduino.h>
#include "PollingSwitch.h"
#include "hal/InputPin.h"

class SwitchMultiPos : public PollingSwitch {
private: 
    InputPin** _inputPins;
    uint8_t _numberOfPins;

public:
    SwitchMultiPos(const char message[], const uint8_t pins[], uint8_t numberOfPins);
    SwitchMultiPos(const char message[], InputPin** inputPins, uint8_t numberOfPins);
    uint8_t readState() override;
};

#endif