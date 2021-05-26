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
        DomeMovement() {}
        ~DomeMovement() {;}

        Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // make public so it can be pointed to and called

        /**
         * Setup the class.
        */
       void setup(uint8_t domePitchPin, uint8_t domeRollPin, uint8_t domeSpinPin, uint8_t domeSpinPotPin) 
       {
           this->domePitchPin = domePitchPin;
           this->domeRollPin = domeRollPin;
           this->domeSpinPin = domeSpinPin;
           this->domeSpinPotPin = domeSpinPotPin;

            pwm.begin(); // Enable pwm board
            pwm.setPWMFreq(300);  // Analog servos run at ~50 Hz updates, digital at ~300Hz updates.
            
            // center dome (X,Y)
            pwm.setPWM(this->domePitchPin, 0, PITCH_CENTER);
            pwm.setPWM(this->domeRollPin, 0, ROLL_CENTER);   
       }

       /**
        * Repeatably call this method to enable dome movement.
        */
       void task() 
       {   
           if(millis() >= nextMillis) {
                nextMillis = millis() + DOME_TASK_INTERVAL;
                //domeSpinPotPos = analogRead(domeSpinPotPin);
                move();
                //spin();
           }
        }

        void setDomeXY(int16_t pitch, int16_t roll) 
        {   
            targetPitch = map(pitch, 172, 1811, PITCH_BACK, PITCH_FRONT);
            targetRoll = map(roll, 172, 1811, ROLL_LEFT, ROLL_RIGHT);    
        }

        void setDomePosition(int16_t x)
        {   
            targetSpin = map(x, 172, 1811, 600, 2400); // Map input range from min and max to -180 and 180 (Clockwise and counter)
        }

        float getDomeSpinPosition() {
            return domeSpinPotPos;
        }

        void move()
        {   
            if(enabled == true)
            {
                pwm.setPWM(domePitchPin, 0, targetPitch);
                pwm.setPWM(domeRollPin, 0, targetRoll);
            }
        }

        void spin()
        {   
            if(targetSpin < 1600 && targetSpin > 1400)
            {
                pwm.writeMicroseconds(domeSpinPin, 1500);
            }
            else
            {
                pwm.writeMicroseconds(domeSpinPin, targetSpin);
            }
        }

        void setEnable(bool enabled) {
            this->enabled = enabled;
        }


    private:
        unsigned long nextMillis = 0;

        uint8_t receivedPitch;
        uint8_t receivedRoll;
        uint8_t receivedSpin;

        uint8_t domePitchPin;
        uint8_t domeRollPin;
        uint8_t domeSpinPin;
        uint8_t domeSpinPotPin;

        int16_t targetPitch = 0;
        int16_t targetRoll = 0;
        int16_t targetSpin = 0;
        int16_t domeSpinPotPos = 0;

        bool enabled = false;

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
};
#endif //DomeMovement_H_