#ifndef Drive_H_
#define Drive_H_

#include <Arduino.h>
#include <ArduinoLog.h>

#include "IMU.h"
#include <SoftwareSerial.h>
#include <Sabertooth.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <DomeMovement.h>

//#define DEBUG_DRIVE_MOVEMENT

#define _DRIVE_TASK_INTERVAL 15

class Drive {
    public:
        Drive() {}
        ~Drive() {;}
       
        // add  IMU* imu, once fixed later
        void setup(DomeMovement* dome, uint8_t leanServoPin) 
        {   
            this->dome = dome;
            //this->imu = imu;

            // lean
            this->leanServoPin = leanServoPin;

            SWSerial.begin(9600);
            ST.autobaud();

            dome->pwm.begin(); // Enable pwm board
            dome->pwm.setPWMFreq(300);  // Analog servos run at ~50 Hz updates, digital at ~300Hz updates.

            //center lean
            dome->pwm.setPWM(this->leanServoPin, 0, LEAN_CENTER);
            delay(1000); // wait for adjustment
        }
        
        void setDriveSpeed(int16_t speed) 
        {
            this->targetDriveSpeed = map(speed, 990, 2003, -127, 127);
        }

        void setFlywheelSpeed(int16_t speed) 
        {
             this->targetFlywheelSpeed = map(speed, 0, 1023, -127, 127);
        }

        void setTilt(int16_t x) 
        {   
            //this->rawLean = x;
            this->targetLean = map(x, 1108, 2003, LEAN_LEFT, LEAN_RIGHT);
        }

        void task() // Main loop
        {
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= _DRIVE_TASK_INTERVAL) {
                drive();
                //flywheel();
                tilt();
            }
        }

        bool isEnabled() {
            return this->enabled;
        }

        void setEnable(bool enabled) {
            this->enabled = enabled;
        }

        void drive() // Drives the BB unit FWD and Reverse
           { 
                if(targetDriveSpeed > 10) {
                    ST.motor(DRIVE_NUM, this->targetDriveSpeed);
                }
                else if(targetDriveSpeed < -10) {
                    ST.motor(DRIVE_NUM, this->targetDriveSpeed);
                }
                else {
                    ST.motor(DRIVE_NUM, 0);
                }
           }

            void flywheel()  // Drives BB8 flywheel with pot
            {
                if(targetFlywheelSpeed > 10) {
                    ST.motor(FLYWHEEL_NUMBER, this->targetFlywheelSpeed);
                }
                else if(targetFlywheelSpeed < -10) {
                    ST.motor(FLYWHEEL_NUMBER, this->targetFlywheelSpeed);
                }
                else {
                    ST.motor(FLYWHEEL_NUMBER, 0);
                }
            }

            void tilt() // drive body roll servo
            { 
                dome->pwm.setPWM(this->leanServoPin, 0, targetLean);

                // VIEW MAPPED PITCH & ROLL VALUES
                //Serial.println(this->targetLean);

                // VIEW RAW PITCH & ROLL VALUES
                //Serial.println(rawLean);
                //delay(15);
            }

        private:
            unsigned long previousMillis = 0; // Used to determine if loop should run
            uint8_t leanServoPin;

            int16_t targetDriveSpeed = 0;
            int16_t targetFlywheelSpeed = 0;
            int16_t targetLean = 0;
            int16_t rawLean = 0;
            
            IMU* imu;
            DomeMovement* dome;
            SoftwareSerial SWSerial = SoftwareSerial(12, 11); // RX on no pin (unused), TX on pin 11 (to S2).
            Sabertooth ST = Sabertooth(128, SWSerial); // Address 128, and use SWSerial as the serial port.

            bool enabled = false;

            // Lean left, center, right values.
            #ifdef DRIVE_LEAN_LEFT
                int16_t LEAN_LEFT = DRIVE_LEAN_LEFT;
            #endif
            #ifdef DRIVE_LEAN_CENTER
                int16_t LEAN_CENTER = DRIVE_LEAN_CENTER;
            #endif
            #ifdef DRIVE_LEAN_RIGHT
                int16_t LEAN_RIGHT = DRIVE_LEAN_RIGHT;
            #endif 

            #ifdef MAIN_DRIVE_NUM
                int16_t DRIVE_NUM = MAIN_DRIVE_NUM;
            #endif
            #ifdef FLYWHEEL_NUM
                int16_t FLYWHEEL_NUMBER = FLYWHEEL_NUM;
            #endif

};
#endif