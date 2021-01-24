#ifndef DomeMovement_H_
#define DomeMovement_H_

#include <Arduino.h>
#include <ArduinoLog.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
//#include "ServoEasing.h"

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

           //Wire.begin(); // Starts with 100 kHz. Clock will eventually be increased at first attach() except for ESP32.
           //Wire.beginTransmission(PCA9685_DEFAULT_ADDRESS);

           //pitchServo.attach(domePitchPin);
           //rollServo.attach(domeRollPin);

           //pitchServo.setSpeed(180); // set servo speed to 180 degrees per second
           //rollServo.setSpeed(180);

            pwm.begin(); // Enable pwm board
            pwm.setPWMFreq(300);  // Analog servos run at ~50 Hz updates, digital at ~300Hz updates.
            
            // center dome (X,Y)
            pwm.setPWM(this->domePitchPin, 0, PITCH_CENTER);
            pwm.setPWM(this->domeRollPin, 0, ROLL_CENTER);
            //pitchServo.write(0); // Move to center with 30 degree per second using interrupts
            //rollServo.write(0);
            delay(1000); // Wait for servos to reach start position
            
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

                //domeSpinPotPos = analogRead(domeSpinPotPin);

                move();
                //spin();
           }
        }

        void setDomeXY(int16_t pitch, int16_t roll) 
        {   
            //this->receivedPitch = pitch; // Look at raw pitch value for testing
            //this->receivedRoll = roll; // Look at raw roll value for testing
            
            targetPitch = map(pitch, 172, 1811, PITCH_BACK, PITCH_FRONT);
            targetRoll = map(roll, 172, 1811, ROLL_LEFT, ROLL_RIGHT);
            
        }

        void setDomePosition(int16_t x)
        {
            targetSpin = map(x, 2412, 1100, 0, 1023);
        }

        float getDomeSpinPosition() {
            return domeSpinPotPos;
        }

        void move()
        {   
            // USE MAPPED PITCH & ROLL VALUES 
            pwm.setPWM(domePitchPin, 0, targetPitch);
            pwm.setPWM(domeRollPin, 0, targetRoll);

            //pitchServo.easeTo(targetPitch);

            // VIEW MAPPED PITCH & ROLL VALUES
            //Serial.println(this->targetPitch);
            //Serial.println(this->targetRoll); 

            // VIEW RAW PITCH & ROLL VALUES
            //Serial.println(receivedPitch);
            //Serial.println(receivedRoll);
            //delay(15);
        }

        void spin()
        {
            //pwm.setPWM(domeSpinPin, 0, this->targetSpin);

            // VIEW MAPPED SPIN VALUE
            //Serial.println(this->targetSpin);

            // VIEW RAW SPIN VALUE
            //Serial.println(receivedSpin);
        }


    private:
        unsigned long previousMillis = 0; // used to determine if loop should run

        //ServoEasing pitchServo = ServoEasing(PCA9685_DEFAULT_ADDRESS, &Wire);
        //ServoEasing rollServo = ServoEasing(PCA9685_DEFAULT_ADDRESS, &Wire);
        //ServoEasing spinServo = ServoEasing(PCA9685_DEFAULT_ADDRESS, &Wire);

        //uint8_t deadzone = 10;

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