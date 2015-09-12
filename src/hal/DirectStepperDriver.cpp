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
#include "DirectStepperDriver.h"

DirectStepperDriver::DirectStepperDriver(uint8_t stepPin, uint8_t directionPin, uint8_t enablePin) {
    _stepPin.setPin(stepPin);
    _directionPin.setPin(directionPin);
    if (enablePin != 0xff) {
        _enablePin.setPin(enablePin);
        _useEnable = true;
    } else {
        _useEnable = false;
    }

    _pulseWidth = 1;
    _enabled = false;
    _forward = true;
    setEnablePin(_enabled);
    setDirectionPin(_forward);
    
    _stepPin.clear();
}

void DirectStepperDriver::setInvertDirection(bool value) {
    _invertDirection = value;
    setDirectionPin(_forward);
}

void DirectStepperDriver::setInvertEnable(bool value) {
    _invertEnable = value;
    setEnablePin(_enabled);
}

void DirectStepperDriver::setPulseWidth(uint8_t pulseWidth) {
    _pulseWidth = pulseWidth;
}

void DirectStepperDriver::setDirection(int8_t dir) {
    if (dir == 1) {
        _forward = true;
    } else if (dir == -1) {
        _forward = false;
    }
    setDirectionPin(_forward);
}

void DirectStepperDriver::step() {
    _stepPin.set();
    delayMicroseconds(_pulseWidth);
    _stepPin.clear();
}

void DirectStepperDriver::setDirectionPin(bool forward) {
    if (forward) {
        _directionPin.setState(_invertDirection ? LOW : HIGH);
    } else {
        _directionPin.setState(_invertDirection ? HIGH : LOW);
    }
}

void DirectStepperDriver::setEnablePin(bool enabled) {
    if (enabled) {
        _enablePin.setState(_invertEnable ? HIGH : LOW);
    } else {
        _enablePin.setState(_invertEnable ? LOW : HIGH);
    }
}

void DirectStepperDriver::enable() {
    if (_useEnable) {
        _enabled = true;
        setEnablePin(true);
    }
}

void DirectStepperDriver::disable() {
    if (_useEnable) {
        _enabled = false;
        setEnablePin(false);
    }
}