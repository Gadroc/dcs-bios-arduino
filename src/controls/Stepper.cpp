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
#include <Arduino.h>
#include "Stepper.h"

Stepper::Stepper(unsigned int address, unsigned int mask, uint8_t shift, long minPosition, long maxPosition, StepperOutput& stepperOutput) : IntegerListener(address, mask, shift), _output(stepperOutput) {
    _minPosition = minPosition;
    _maxPosition = maxPosition;
    _maxValue = 65535u>>shift;
}

void Stepper::onDcsBiosFrameSync() {
    _output.setTargetPosition(map(getData(), 0, _maxValue, _minPosition, _maxPosition));
}