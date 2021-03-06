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
#ifndef _DCSBIOS_DIRECTSTEPPERDRIVER_H_
#define _DCSBIOS_DIRECTSTEPPERDRIVER_H_

#include <Arduino.h>
#include "StepperDriver.h"
#include "DirectOutputPin.h"

// Class to drive an external stepper motor driver chip.
class PulseStepperDriver : public StepperDriver {
private:
    OutputPin& _enablePin;
    OutputPin& _directionPin;
    OutputPin& _stepPin;

    uint8_t _pulseWidth;

    bool _invertEnable;
    bool _invertDirection;

    bool _enabled;
    bool _forward;

    void setDirectionPin(bool forward);
    void setEnablePin(bool enabled);

public:
    PulseStepperDriver(OutputPin& stepPin, OutputPin& directionPin);
    PulseStepperDriver(OutputPin& stepPin, OutputPin& directionPin, OutputPin& enablePin);
    PulseStepperDriver(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin = 0xff);

    // By default driver sets direction pin high to enable forward motion.
    // Set to true to invert this behavior.
    void setInvertDirection(bool value);

    // By default driver sets invert low to enable motor.  Set
    // to true to invert this logic.
    void setInvertEnable(bool value);

    // Sets the pulse time in microseconds. Defaults to 1.  Be careful
    // increasing this value as long pulses will through off timing
    // for micros() and millis() functions.
    void setPulseWidth(uint8_t pulseWidth);

    // Sets the direction to step. (1 CW/Forward, -1 CCW/Backward)
    void setDirection(int8_t dir) override;

    // Steps the motor in the current direction
    void step() override;

    // Enables the stepper motor
    void enable() override;

    // Disables the stepper motor
    void disable() override;
};

#endif