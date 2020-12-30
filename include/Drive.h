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
        Adafruit_PWMServoDriver* pwm = Adafruit_PWMServoDriver();

        void setup(IMU* imu, uint8_t drivePin, uint8_t flywheelPin, uint8_t leanServoPin) {

            this->imu = imu;
            // drive & flywheel
            this->drivePin = drivePin;
            this->flywheelPin = flywheelPin;
            // lean
            this->leanServoPin = leanServoPin;

            //center lean
            pwm.setPWM(this->flywheelPin, 0, LEAN_CENTER)

            void setDriveSpeed(int16_t speed) {
                this->targetDriveSpeed = constrain(speed, -255, 255);
            }

            void setFlywheelSpeed(int16_t speed) {
                this->targetFlywheelSpeed = constrain(speed, -255, 255);
            }

            void setTilt(int16_t x) {
                this->targetLeanPosition = constrain(x, this->DRIVE_LEAN_LEFT, this->DRIVE_LEAN_RIGHT);
            }

            void task() {
                unsigned long currentMillis = millis();
                if (currentMillis - previousMillis >= _DRIVE_TASK_INTERVAL) {
                    drive();
                    flywheel();
                    tilt();
                }
            }

            bool isEnabled() {
                return this->enabled;
            }

            void setEnable(bool enabled) {
                this->enabled = enabled;
            }

        } // end of Public

        private:
            unsigned long previousMillis = 0; // Used to determine if loop should run
            uint8_t drivePin; 
            uint8_t flywheelPin,
            uint8_t leanServoPin;

            int16_t targetDriveSpeed = 0;
            int16_t targetFlywheelSpeed = 0;
            int16_t targetLeanPosition = 0;
            IMU* imu;
            SoftwareSerial SWSerial(NOT_A_PIN, 11); // RX on no pin (unused), TX on pin 11 (to S1).
            Sabertooth ST(128, SWSerial); // Address 128, and use SWSerial as the serial port.

            enabled = false;

           void drive() // Drives the BB unit FWD and Reverse
           { 
                if(this->targetDriveSpeed > 10) {
                    ST.motor(MAIN_DRIVE_NUM, this->targetDriveSpeed);
                }
                else if(this->targetDriveSpeed < -10) {
                    ST.motor(MAIN_DRIVE_NUM, this->targetDriveSpeed);
                }
                else {
                    ST.motor(MAIN_DRIVE_NUM, 0);
                }

            void flywheel() {
                if(this->targetFlywheelSpeed > 10) {
                    ST.motor(FLYWHEEL_NUM, this->targetFlywheelSpeed);
                }
                else if(this->targetFlywheelSpeed < -10) {
                    ST.motor(FLYWHEEL_NUM, this->targetFlywheelSpeed);
                }
                else {
                    ST.motor(FLYWHEEL_NUM, 0);
                }
            }

            void tilt() // drive body roll servo
            { 
                pwm.setPWM(this->leanServoPin, 0, targetLeanPosition);
            }

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

           } // end of Private
    
};
#endif