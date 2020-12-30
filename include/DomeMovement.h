#ifndef DomeMovement_H_
#define DomeMovement_H_

#include <Arduino.h>
#include <ArduinoLog.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define DOME_TASK_INTERVAL 25 // in millis

//#define DEBUG_DOME_MOVEMENT

class DomeMovement {

    public:
        unsigned long previousMillis = 0; // used to determine if loop should run

        uint8_t domePitchPin;
        uint8_t domeRollPin;
        uint8_t domeSpinPin;
        uint8_t domeSpinPotPin;

        int16_t targetPitch = 0;
        int16_t targetRoll = 0;
        int16_t targetSpin = 0;
        int16_t domeSpinPotPos = 0;

        #ifdef DEBUG_DOME_MOVEMENT
            int count = 0;
        #endif

        // Pitch front, center, back values.
        #ifdef DOME_TILT_PITCH_FRONT
            int16_t PITCH_FRONT = DOME_TILT_PITCH_FRONT;
        #endif
        #ifdef DOME_TILT_PITCH_CENTER
            int16_t PITCH_CENTER = DOME_TILT_PITCH_CENTER;
        #endif
        #ifdef DOME_TILT_PITCH_BACK
            int16_t PITCH_BACK = DOME_TILT_PITCH_BACK;
        #endif

        // Roll left, center, right values.
        #ifdef DOME_TILT_ROLL_LEFT
            int16_t ROLL_LEFT = DOME_TILT_ROLL_LEFT;
        #endif
        #ifdef DOME_TILT_ROLL_CENTER
            int16_t ROLL_CENTER = DOME_TILT_ROLL_CENTER;
        #endif
        #ifdef DOME_TILT_ROLL_RIGHT
            int16_t ROLL_RIGHT = DOME_TILT_ROLL_RIGHT;
        #endif

        DomeMovement() {}
        ~DomeMovement() {;}
        Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
        pwm.setPWMFreq(300);  // Analog servos run at ~50 Hz updates, digital at ~300Hz updates.

        /**
         * Setup the class.
        */
       void setup(uint8_t domePitchPin, uint8_t domeRollPin, uint8_t domeSpinPin, uint8_t domeSpinPotPin) 
       {
           this->domePitchPin = domePitchPin;
           this->domeRollPin = domeRollPin;
           this->domeSpinPin = domeSpinPin;
           this->domeSpinPotPin = domeSpinPotPin;
            
            // center dome (X,Y)
            pwm.setPWM(this->domePitchPin, 0, this->PITCH_CENTER);
            pwm.setPWM(this->domeRollPin, 0, this->ROLL_CENTER);
            
            // center dome spin servo        
       }

       /**
        * Repeatably call this method to enable dome movement.
        */
       void task() 
       {
           unsigned long currentMillis = millis();
           if(currentMillis - previousMillis >= DOME_TASK_INTERVAL) {
                previousMillis = currentMillis;

                domeSpinPotPos = analogRead(domeSpinPotPin);

                move();
                spin();
           }
        }

        void setDomeXY(int16_t pitch, int16_t roll) 
        {
            targetPitch = constrain(pitch, PITCH_FRONT, PITCH_BACK);
            targetRoll = constrain(roll, ROLL_LEFT, ROLL_RIGHT);
        }

        void setDomePosition(int16_t x)
        {
            targetSpin = constrain(x, 0, 1023);
        }

        float getDomeSpinPosition() {
            return domeSpinPotPos;
        }

         void move()
        {
            pwm.setPWM(domePitchPin, 0,targetPitch);
            pwm.setPWM(domeRollPin, 0, targetRoll);
        }

        void spin()
        {
            pwm.setPWM(domeSpinPin, 0, targetSpin);
        }
    } // end to public

    //     private:
        
    // } // end of private  
     
};
#endif //DomeMovement_H_