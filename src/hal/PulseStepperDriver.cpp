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
#include "PulseStepperDriver.h"

PulseStepperDriver::PulseStepperDriver(OutputPin &stepPin, OutputPin &directionPin) :
        PulseStepperDriver(stepPin, directionPin, *(new DirectOutputPin(0xff)))
{ }

PulseStepperDriver::PulseStepperDriver(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin) :
        PulseStepperDriver(*(new DirectOutputPin(stepPin)), *(new DirectOutputPin(directionPin)), *(new DirectOutputPin(enablePin)))
{ }

PulseStepperDriver::PulseStepperDriver(OutputPin &stepPin, OutputPin &directionPin, OutputPin &enablePin) :
    _stepPin(stepPin), _directionPin(directionPin), _enablePin(enablePin)
{
    _pulseWidth = 1;

    _invertDirection = false;
    _invertEnable = false;

    _enabled = false;
    setEnablePin(_enabled);

    _forward = true;
    setDirectionPin(_forward);

    _stepPin.clear();
}

void PulseStepperDriver::setInvertDirection(bool value) {
    _invertDirection = value;
    setDirectionPin(_forward);
}

void PulseStepperDriver::setInvertEnable(bool value) {
    _invertEnable = value;
    setEnablePin(_enabled);
}

void PulseStepperDriver::setPulseWidth(uint8_t pulseWidth) {
    _pulseWidth = pulseWidth;
}

void PulseStepperDriver::setDirection(int8_t dir) {
    if (dir == 1) {
        _forward = true;
    } else if (dir == -1) {
        _forward = false;
    }
    setDirectionPin(_forward);
}

void PulseStepperDriver::step() {
    _stepPin.set();
    delayMicroseconds(_pulseWidth);
    _stepPin.clear();
}

void PulseStepperDriver::setDirectionPin(bool forward) {
    if (forward) {
        _directionPin.setState(_invertDirection ? LOW : HIGH);
    } else {
        _directionPin.setState(_invertDirection ? HIGH : LOW);
    }
}

void PulseStepperDriver::setEnablePin(bool enabled) {
    if (_enablePin.isValid()) {
        if (enabled) {
            _enablePin.setState(_invertEnable ? HIGH : LOW);
        } else {
            _enablePin.setState(_invertEnable ? LOW : HIGH);
        }
    }
}

void PulseStepperDriver::enable() {
    _enabled = true;
    setEnablePin(true);
}

void PulseStepperDriver::disable() {
    _enabled = false;
    setEnablePin(false);
}