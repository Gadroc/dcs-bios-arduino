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
#ifndef _DCSBIOS_STEPPEROUTPUT_H_
#define _DCSBIOS_STEPPEROUTPUT_H_

#include <Arduino.h>

// StepperOutput classes generate pulse trains from absolute position indicators.
class StepperOutput {
private:
    StepperOutput* nextStepperOutput;

public:
    static StepperOutput* firstStepperOutput;
    static void runSteppers();
    
    StepperOutput();

    // Check to see if pulse needs to be generated and do so if necessary
    virtual void run() = 0;

    // Sets a new target step position
    virtual void setTargetPosition(long newPosition) = 0;

    // Sets the current step position of the motor
    virtual void setCurrentPosition(long currentPosition) = 0;
};

#endif