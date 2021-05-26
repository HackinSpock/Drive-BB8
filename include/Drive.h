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
#include <MovementUtils.h>

//#define DEBUG_DRIVE_MOVEMENT

#define _DRIVE_TASK_INTERVAL 15

class Drive {
    public:
        Drive() {}
        ~Drive() {;}
       
        // add  IMU* imu, once fixed later
        void setup(DomeMovement* dome, IMU* imu, uint8_t leanServoPin) 
        {   
            this->dome = dome;
            this->imu = imu;

            // lean
            this->leanServoPin = leanServoPin;

            //center lean
            dome->pwm.setPWM(this->leanServoPin, 0, LEAN_CENTER);
        }
        
        void setDriveSpeed(int16_t speed) 
        {   
            targetDriveSpeed = constrain(speed, -127, 127);
            //targetDriveSpeed = map(driveRange, 988, 2012, -25, 25);
            driveEase = MovementUtils::ease(speed, targetDriveSpeed, 10);
        }

        void setFlywheelSpeed(int16_t speed) 
        {   
            targetFlywheelSpeed = constrain(speed, -127, 127);
            flywheelEase = MovementUtils::ease(speed, targetFlywheelSpeed, 10);
        }

        void setTilt(int16_t x) 
        {   
            this->targetLean = map(x, 172, 1811, LEAN_LEFT, LEAN_RIGHT);
        }

        void task() // Main loop
        {
            if(millis() >= nextMillis) {
                nextMillis = millis() + _DRIVE_TASK_INTERVAL;
                drive();
                tilt();
                flywheel();
            }
        }

        void setEnable(bool enabled) {
            this->enabled = enabled;
        }

        void drive() // Drives the BB unit FWD and Reverse
        {   
            //float torque = drivePID(targetDriveSpeed);
            //torque = constrain(torque, -127, 127);
            int16_t torque = driveEase;

            if(enabled == true)
            {
                if(torque > -10 && torque < 10){
                    ST.motor(1, 0);
                }
                else {
                    ST.motor(1, -torque);
                }
            }
            else
            {
                ST.motor(1, 0);
            }

        }
        void flywheel()  // Drives BB8 flywheel with pot
        {
            if(enabled == true)
            {
                if(flywheelEase > -10 && flywheelEase < 10){
                    ST.motor(2, 0);
                }
                else {
                    ST.motor(2, -targetFlywheelSpeed);
                }
            }
            else
            {
                ST.motor(2, 0);
            }
        }

        void tilt() // drive body roll servo
        { 
            dome->pwm.setPWM(leanServoPin, 0, targetLean);
        }

        float drivePID(int16_t throttleAngle)
        {   
            this->y_orientation = imu->getYOrientation();
            errorX = throttleAngle - y_orientation; // set error to angle of IMU
            integral = integral + (errorX * dTime); // calculate Integral
            derivative = (errorX-previousError) / dTime; // // calculate Derivitive
            outputX = (kP * errorX)+(kI * integral)+(kD * derivative); // Set output (CV)
            previousError = errorX; // set previous error to current error. This is for the derivitive to check if the system is going too fast   

            return outputX;
        }

        private:
            unsigned long nextMillis = 0;
            uint8_t leanServoPin;

            int16_t targetDriveSpeed = 0;
            int16_t targetFlywheelSpeed = 0;
            int16_t targetLean = 0;
            
            IMU* imu;
            DomeMovement* dome;
            
            Sabertooth ST = Sabertooth(128, Serial1); // Address 128, and use HardwareSerial as the serial port.

            int16_t driveEase;
            int16_t flywheelEase;

            bool enabled = false;

            float kP = 0;
            float kI = 0;
            float kD = 0;

            float errorX = 0.0; // Error for X-Axis
            float previousError = 0.0;
            float derivative = 0.0;
            float dTime = 1;
            float integral = 0.0;
            float setPoint = 0.0;
            float current = 0.0;
            float outputX = 0.0;

            double y_orientation;

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