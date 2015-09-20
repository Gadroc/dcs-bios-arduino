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
#ifndef _DCSBIOS_ACCELSTEPPEROUTPUT_H_
#define _DCSBIOS_ACCELSTEPPEROUTPUT_H_

#include <Arduino.h>
#include "StepperDriver.h"
#include "StepperOutput.h"

// Maths constants. To simplify maths when calculating in speed_cntr_Move().
#define ALPHA (2*3.14159/_stepsPerRevolution)    // 2*pi/spr
#define A_T_x100 ((long)(ALPHA*_frequency*100))  // (ALPHA / T1_FREQ)*100
#define T1_FREQ_148 ((int)((_frequency*0.676)/100)) // divided by 100 and scaled by 0.676
#define A_SQ (long)(ALPHA*2*10000000000)         // ALPHA*2*10000000000
#define A_x20000 (int)(ALPHA*20000)              // ALPHA*20000

enum ACCEL_STEPPER_RUNSTATES {
    STOP,
    ACCEL,
    DECEL,
    RUN
};

// Stepper output based which implements acceleration / deceleration.
class AcceleratedStepperOutput : public StepperOutput {
private:
    StepperDriver& _driver;

    // Minimum distance before we start moving
    unsigned int _threshold;

    // Frequency of the step count timer
    unsigned long _frequency;

    // Number of steps per revolution for this stepper
    unsigned int _stepsPerRevolution;

    // Acceleration in 0.01*rad/sec^2
    unsigned int _acceleration;

    // Maximum speed in 0.01*rad/sec
    unsigned int _maxSpeed;

    // First step delay
    unsigned int _initialStepDelay;

    // Minimum step delay
    unsigned int _minStepDelay;

    // Current state of the running stepper
    ACCEL_STEPPER_RUNSTATES _runState;

    // Current position of the stepper (in steps)
    signed long _currentPosition;

    // Current direction we are moving
    int8_t _currentDirection;

    // Target position for the stepper (in steps)
    signed long _targetPosition;

    // Position where decelration will start
    long _decelerationTarget;

    // Step delay for last step
    unsigned int _lastStepDelay;

    // Remember the last acceleration step dealy before full speed
    unsigned int _lastAccelStepDelay;

    // Remainder from step delay to increase accuracy
    unsigned int _stepDelayRemainder;

    // Counter for acceleration / deceleration steps
    signed int _accelerationStepCount;

    // Next step time for this stepper
    unsigned long _nextStepTime;

    // Steps the output and returns the next step delay.
    unsigned int step();

    // Plans out a move for a given distance
    void planMove(signed long distance);

public:
    // driver - Stepper driver used to control this stepper motor.
    // stepsPerRevolution - Native steps per revolution for the stepper motor (1.8 degeree stepper is 200)
    // microsteps - How many microsteps does the driver apply for this motor (1 = no microstepping)
    // maxSpeed - Maximum rotation speed for this motor in 0.01*rad/sec (6.28 radian/sec would be 628)
    // acceleration - Acceleration/Deceleration rate for this motor in 0.01*rad/sec^2 (8 radion/sec^s would be 800) (65535 disables acceleration which is the default)
    // frequency - Step counter frequency.  Default is appropraite for poll based timing like the run function on this class.  Set appropriately if calling step function from a timer interrupt.
    AcceleratedStepperOutput(StepperDriver& driver, unsigned int stepsPerRevolution, unsigned int microsteps, unsigned int maxSpeed, unsigned int acceleration = 65535, unsigned long frequency = 1000000L);

    // Checks time and steps the motor if necessary.  Must be called as frequent as possible
    virtual void run();

    // Sets a new target step position
    virtual void setTargetPosition(long newPosition);

    // Sets the current step position of the motor
    virtual void setCurrentPosition(long currentPosition);  
};

#endif
