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
#ifndef _DCSBIOS_STEPPERDRIVER_H_
#define _DCSBIOS_STEPPERDRIVER_H_

class StepperDriver {
public:
    // Enables the stepper motor
    virtual void enable() = 0;

    // Disables the stepper motor
    virtual void disable() = 0;

    // Sets the direction to step. (1 CW/Forward, -1 CCW/Backward)
    virtual void setDirection(int8_t dir) = 0;

    // Steps the motor in the current direction
    virtual void step() = 0;
};

#endif