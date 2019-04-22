# **dStepper**
Simple Dithering Stepper Driver Library

* Designed for interupt driven stepping
* Pulse dithering for more accurate speed control and smoother high speed operation
* Efficient functions will perform well with either 8 bit or 32 bit microcontrollers
* Works with any stepper driver that accepts step and direction input (see: stepStick)
* Handles setup and control of hardware pins
* Position estimation to the sub-microstep resolution minimizes discrete noise
* Frequent setSpeed updates will not cause stuttering or motion artifacts
* Acceleration ramping and path planning are not performed by this library ( see `EasyMove` for basic 3 axis motion control )
* Designed to be compatible with any micro controller that is either part of or "friendly" with the Arduino ecosystem.  Specifically tested with Arduno Uno, Arduino Due, and Teensy 3.x devices.

## Basic Usage Documentation

### Object Creation:
```
dStepper your_motor( stepsPerMM, direction, tickRateHz, stepPin, directionPin, enablePin);
```
* stepsPerMM -- usually defined by motor steps, pully size and micro-stepping ( A 200 step motor, with 20T GT2 pully with 1/16 microstepping will have 80 steps per mm)
* direction -- changing between 0 and 1 will change the direction of rotation
* tickRateHz -- how often is the step() function called per second
* stepPin -- which hardware pin is used to send the step pulse
* directionPin -- which hardware pin is used to send the direction signal
* enablePin -- which hardware pin us used to enable/disable the stepper driver


### Sending Step Pulses
```
your_motor.step();
```
* This should be called at a fixed frequency.  Generally higher frequencies will give smoother motion and high max speeds.
* The call rate should match the rate specified at object creation (tickRateHz)
* A step is only output when needed.  If the speed is set to zero, no pulses will be sent.
* This should be run from an interrupt for best performance.  For the less experienced user, a library that can make this easier is: FrequencyTimer2 https://github.com/PaulStoffregen/FrequencyTimer2
* Jitter from imperfect call rates will not cause adverse software function but may affect motor function/smoothness
* This has low computational requrements, so high call rates should be attainable in most micro controllers
* The step pin is set low one call after it is set high to insure that the stepper driver does not miss a step pulse at high frequencies.  This is generally only a consideration for 32bit microcontrollers operating at high frequencies, but this strategy has no disadvantages for low power micros like the Arduino.


### Enable/Disable Stepper
```
your_motor.enable();
your_motor.disable();
```

### Set Motor Speed:
```
your_motor.setSpeed( speed );
```
* Speed is in MM per min
* Negative speed values will reverse motor direction
* The direction pin is controlled from this function
* Should be called after enable() has been called
* Will operate at the request speed until called again with a different speed
```
your_motor.setSpeedByPositionMM( targetPositionMM );
your_motor.setSpeedByPositionMM( targetPositionMM, Hz );
```
* Pass in the target position at regular intervals and this function will set the motor speed to minimize error
* The target position must move smoothly to prevent unreasonable motor speed requests
* The starting motor position should be set to starting motion control position using `setPositionMM()`
* Using the second option and passing the expected motion control frequency saves ~40us per motor compared to first option which computes it each execution


### Set Motor Position:
```
your_motor.setPositionMM( position );
```
* Passing a float will set the position in MM
```
your_motor.setPositionSteps( position );
```
* Passing an integer will set the position in steps

### Get Motor Position:
```
float   position = your_motor.getPositionMM();
int32_t position = your_motor.getPositionSteps();
```
* MM are returned as a 32bit float. The position returned combines both the integer microstep position and the sub-microstep position estimate to reduce discrete noise generated by only using the integer microsteps.  
* Steps are return as an signed 32 bit int


### Get Maxiumum Speed
```
float speedLimit = getMaxFeedRate();
```
* This function returns the maximum theoretical speed of the motor taking into account the tick rate and steps per MM.
* Calls to setSpeed that are higher than this limit will automatically be constrained.
* It is possible that many motors do not have the torque required to reach this max speed, so testing should be performed to establish the real limits of the system.

