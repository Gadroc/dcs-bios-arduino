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
#ifndef _DCSBIOS_ACTIONBUTTON_H_
#define _DCSBIOS_ACTIONBUTTON_H_

#include <stdint.h>
#include "dcs/PollingInput.h"
#include "hal/InputPin.h"

class ActionButton : public PollingInput {
private:
    const char* _arg;
    InputPin& _pin;
    uint8_t _lastState;
    void initInput() override;
    void pollInput() override;

public:
    ActionButton(const char message[], const char arg[], uint8_t pin);
    ActionButton(const char message[], const char arg[], InputPin& pin);
};

#endif