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
        void setup(DomeMovement* dome, uint8_t drivePin, uint8_t flywheelPin, uint8_t leanServoPin) 
        {   
            this->dome = dome;
            //this->imu = imu;

            // drive & flywheel
            this->drivePin = drivePin;
            this->flywheelPin = flywheelPin;
            // lean
            this->leanServoPin = leanServoPin;

            //center lean
            dome->pwm.setPWM(this->flywheelPin, 0, LEAN_CENTER);
        }
        
        void setDriveSpeed(int16_t speed) 
        {
            this->targetDriveSpeed = map(speed, 992, 2003, -255, 255);
        }

        void setFlywheelSpeed(int16_t speed) 
        {
             this->targetFlywheelSpeed = map(speed, 0, 1023, -255, 255);
        }

        void setTilt(int16_t x) 
        {   
            this->rawLean = x;
            this->targetLean = map(x, 1108, 2003, LEAN_LEFT, LEAN_RIGHT);
        }

        void task() // Main loop
        {
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= _DRIVE_TASK_INTERVAL) {
                //drive();
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
                //dome->pwm.setPWM(this->leanServoPin, 0, targetLean);

                // VIEW MAPPED PITCH & ROLL VALUES
                //Serial.println(this->targetLean);

                // VIEW RAW PITCH & ROLL VALUES
                Serial.println(rawLean);
                //delay(15);
            }

        private:
            unsigned long previousMillis = 0; // Used to determine if loop should run
            uint8_t drivePin; 
            uint8_t flywheelPin;
            uint8_t leanServoPin;

            int16_t targetDriveSpeed = 0;
            int16_t targetFlywheelSpeed = 0;
            int16_t targetLean = 0;
            int16_t rawLean = 0;
            
            IMU* imu;
            DomeMovement* dome;
            SoftwareSerial SWSerial = SoftwareSerial(NOT_A_PIN, 11); // RX on no pin (unused), TX on pin 11 (to S1).
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