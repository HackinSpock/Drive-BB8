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
        void setup(DomeMovement* dome, IMU* imu, uint8_t leanServoPin) 
        {   
            this->dome = dome;
            this->imu = imu;

            // lean
            this->leanServoPin = leanServoPin;

            imu->IMUtimeout();
            dome->pwm.begin(); // Enable pwm board
            dome->pwm.setPWMFreq(300);  // Analog servos run at ~50 Hz updates, digital at ~300Hz updates.

            //center lean
            dome->pwm.setPWM(this->leanServoPin, 0, LEAN_CENTER);
            //delay(1000); // wait for adjustment
        }
        
        void setDriveSpeed(int16_t speed) 
        {   
            //this->rawSpeed = speed;
            int driveRange = map(speed, 172, 1811, 988, 2012);
            //targetDriveSpeed = map(driveRange, 988, 2012, -127, 127);
            targetDriveSpeed = map(driveRange, 988, 2012, -25, 25);
        }

        void setFlywheelSpeed(int16_t speed) 
        {   
            int flywheelRange = map(speed, 172, 1811, 988, 2012);
            this->targetFlywheelSpeed = map(flywheelRange, 988, 2012, -127, 127);
        }

        void setTilt(int16_t x) 
        {   
            //this->rawLean = x;
            int tiltRange = map(x, 172, 1811, 988, 2012);
            this->targetLean = map(tiltRange, 988, 2012, LEAN_LEFT, LEAN_RIGHT);
        }

        void task() // Main loop
        {
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= _DRIVE_TASK_INTERVAL) {
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
            float torque = drivePID(targetDriveSpeed);
            torque = constrain(torque, -127, 127);
            //uint16_t torque = targetDriveSpeed;

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
            //delay(100);

        }
        void flywheel()  // Drives BB8 flywheel with pot
        {
            if(enabled == true)
            {
                if(targetFlywheelSpeed > -10 && targetFlywheelSpeed < 10){
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
            //delay(100);
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
            unsigned long previousMillis = 0; // Used to determine if loop should run
            uint8_t leanServoPin;

            int16_t targetDriveSpeed = 0;
            int16_t targetFlywheelSpeed = 0;
            int16_t targetLean = 0;
            
            IMU* imu;
            DomeMovement* dome;
            
            Sabertooth ST = Sabertooth(128, Serial1); // Address 128, and use SWSerial as the serial port.

            bool enabled = false;

            float kP = 2;
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