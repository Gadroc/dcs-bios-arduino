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
#ifndef _DCSBIOS_POTENTIOMETER_H_
#define _DCSBIOS_POTENTIOMETER_H_

#include "hal/AnalogInput.h"
#include "dcs/PollingInput.h"

class Potentiometer : public PollingInput {
private:
    AnalogInput& _input;
    unsigned int _threshold;
    unsigned int _lastState;
    unsigned long _nextPoll;
    unsigned int _pollingInterval;

public:
    Potentiometer(const char message[], uint8_t pin, unsigned int threshold = 50, unsigned int pollingInterval = 100);
    Potentiometer(const char message[], AnalogInput& input, unsigned int threshold = 50, unsigned int pollingInterval = 100);

    void initInput() override;
    void pollInput() override;
};

#endif