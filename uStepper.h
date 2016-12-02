/*
      uStepper -- Simple Stepper Driver Library
      Copyright (C) 2016  Phillip J Schmidt
      
         This program is free software: you can redistribute it and/or modify
         it under the terms of the GNU General Public License as published by
         the Free Software Foundation, either version 3 of the License, or
         (at your option) any later version.
         
         This program is distributed in the hope that it will be useful,
         but WITHOUT ANY WARRANTY; without even the implied warranty of
         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
         GNU General Public License for more details.
         
         You should have received a copy of the GNU General Public License
         along with this program.  If not, see <http://www.gnu.org/licenses/>

 */
 
#ifndef uStepper_h
   #define uStepper_h
   
   #include <arduino.h>

   class uStepper
   {
      public:
      
         uStepper(float _stepsPerMM, int direction, float _tickRateHz, int _stepPin, int _dirPin, int _enablePin);
         ~uStepper();
         
         void setSpeed(int32_t feedRate);
         float positionFloat();
         void setZero();
         void enable();
         void disable();
         inline void step();  // call from ISR


      private:
      
         void stepPulseOff();
         void stepPulseOn();
         
         boolean stepPinOn;
         
         float stepsPerMM, MMPerStep;
         int directionPin, stepPin, enablePin;
         int FORWARD, REVERSE;
         
         int ditherTotalSteps, ditherLongSteps, ditherCounter;
         
         int32_t maxFeedRate;
         
         volatile int32_t  position;
          
         unsigned int tickCounter, tickPerStep;
         float tickPerMin;
         
         enum moveDir_t {
            Positive,
            Negative,
            Stopped
         } moveDirection;

   };
   
   
   // defined in header to allow "inline" declaration
   inline void uStepper::step(){  // call from ISR
      
      // This is kept fast by only executing an increment and a comparison on most calls (depending on speed)
      tickCounter++;

      if(tickCounter >= tickPerStep) // send step to motor
      { 
         // This is executed twice:
         //    * first to set the tick pin high
         //    * second to set it low (on the following call)
         //
         // This insures sufficient time for the stepper controller to see the tick, without adding an artificial delay into the ISR
      
         if(stepPinOn)    // check if pin is already high 
         { 
            stepPulseOff();
         }
         else
         {
            stepPulseOn();
         }
      }
   }

   
#endif