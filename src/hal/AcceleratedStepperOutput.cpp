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
#include "AcceleratedStepperOutput.h"

AcceleratedStepperOutput::AcceleratedStepperOutput(StepperDriver& driver, unsigned int stepsPerRevolution, unsigned int microsteps, unsigned int acceleration, unsigned int maxSpeed, unsigned long frequency) : _driver(driver) {
    _frequency = frequency;
    _stepsPerRevolution = stepsPerRevolution * microsteps;    
    _acceleration = acceleration;
    _maxSpeed = maxSpeed;
    _currentPosition = 0;
    _targetPosition = 0;
    _currentDirection = 1;

    // Default to half stepping as threshold for starting movement.  Starting movement with 
    // less steps available create jerky pointer movement.
    _threshold = microsteps / 2;
    if (_threshold == 0) {
        _threshold = 1;
    }

    _runState = STOP;

    // Lets calculate all initial values now
    _minStepDelay = A_T_x100 / _maxSpeed;
    _initialStepDelay = (T1_FREQ_148 * sqrt(A_SQ / _acceleration))/100;
    _maxAccelerationSteps = (long)_maxSpeed*_maxSpeed/(long)(((long)A_x20000*_acceleration)/100);
    if(_maxAccelerationSteps == 0){
        _maxAccelerationSteps = 1;
    }
}

void AcceleratedStepperOutput::setTargetPosition(long newPosition) {
    _targetPosition = newPosition;
    planMove(_targetPosition - _currentPosition);
}

void AcceleratedStepperOutput::planMove(long distance) {
    int8_t targetDirection = distance < 0 ? -1 : 1;
    long absDistance = abs(distance);

    if (_runState == STOP) {
        // We don't want to start moving unless we cross the step threshold.  Moving individual
        // microsteps creates a very jerky movement.
        if (absDistance >= _threshold) {

            // Set drivers to step in the right direction
            _currentDirection = targetDirection;
            _driver.setDirection(_currentDirection);

            if (distance == 1) {
                // When distance is only step construct the scenario appropiatley
                // so that only one step happens and then stop.
                _accelerationStepCount = 1;
                _runState = DECEL;
                _lastStepDelay = 1000;
            } else {
                // Moving more than once reset counters for steps and remainder
                _accelerationStepCount = 0;
                _stepDelayRemainder = 0;
                if (_initialStepDelay <= _minStepDelay) {
                    // if intial speed is faster than max speed go straight to max speed
                    _lastStepDelay = _minStepDelay;
                    _runState = RUN;
                } else {
                    // Seed last step delay with pre-caclulated initial step speed and turn on acceleration
                    _lastStepDelay = _initialStepDelay;
                    _runState = ACCEL;
                }
                _driver.enable();
            }
            _nextStepTime = micros();
        }
    } else if (targetDirection == _currentDirection) {        
        // Direction stayed the same so make sure acceleration / deceleration are correct with new target
        switch (_runState) {
            case DECEL:
                // When extending a move beyond the current deceleration target accelerate back up to speed
                if (absDistance > _accelerationStepCount) {
                    _runState = ACCEL;
                }
                // If the distance is not greater decelerate to an overshoot and step funciton will plan correction
                // move when deceleration is done.
                break;

            case RUN:
                // When shortening a move start decelerating if necessary
                // We may overshoot which is ok.. then step loop will swing us back round.
                if (absDistance <= _accelerationStepCount) {
                    _lastStepDelay = _lastAccelStepDelay;
                    _runState = DECEL;
                }
                break;

            case ACCEL:
                // When shortening a move start decelerating if necessary
                // We may overshoot which is ok.. then step loop will swing us back round.
                if (absDistance <= _accelerationStepCount) {
                    _runState = DECEL;
                }
                break;
        }
    } else {
        // New target is switching directions so decelerate to stop.  Step function will plan next move
        // when deceleration is finished.
        switch (_runState) {
            case RUN:
            case ACCEL:
                _runState = DECEL;
                break;
        }
    }
}

void AcceleratedStepperOutput::setCurrentPosition(long currentPosition) {
    _currentPosition = currentPosition;    
    setTargetPosition(currentPosition);
}

unsigned int AcceleratedStepperOutput::step() {
    unsigned int newStepDelay;

    _driver.step();
    _currentPosition += _currentDirection;
    long distance = _targetPosition - _currentPosition;

    switch(_runState) {
        case ACCEL:
            _accelerationStepCount++;
            newStepDelay = _lastStepDelay - (((2 * (long)_lastStepDelay) + _stepDelayRemainder)/(4 * _accelerationStepCount +1));
            _stepDelayRemainder = ((2 * (long)_lastStepDelay) + _stepDelayRemainder) % (4 * _accelerationStepCount + 1);
            if (_accelerationStepCount >= abs(distance)) {
                _runState = DECEL;
            } else if (newStepDelay <= _minStepDelay) {
                _lastAccelStepDelay = _lastStepDelay;
                newStepDelay = _minStepDelay;
                _stepDelayRemainder = 0;
                _runState = RUN;
            }
            break;

        case RUN:
            newStepDelay = _minStepDelay;
            if (_accelerationStepCount >= abs(distance)) {
                newStepDelay = _lastAccelStepDelay;
                _runState = DECEL;
            }
            break;

        case DECEL:
            _accelerationStepCount--;
            newStepDelay = _lastStepDelay - (((2 * (long)_lastStepDelay) + _stepDelayRemainder)/(4 * -_accelerationStepCount + 1));
            _stepDelayRemainder = ((2 * (long)_lastStepDelay) + _stepDelayRemainder) % (4 * -_accelerationStepCount + 1);
            if (_accelerationStepCount <= 0) {
                _runState = STOP;
                if (distance != 0) {
                    planMove(distance);
                }
            }
            break;
    }

    _lastStepDelay = newStepDelay;
    return newStepDelay;
}

void AcceleratedStepperOutput::run() {    
    if (_runState != STOP) {
        unsigned long current = micros();
        if ((long)(current - _nextStepTime) >= 0) {
            unsigned int delay = step();
            _nextStepTime = current + delay;
        }
    }
}