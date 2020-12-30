#ifndef IMU_H_
#define IMU_H_

#include <Arduino.h>
#include "EasyTransfer.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// default settings, call setup to change these
#define _IMU_SERIAL &Serial2
#define _IMU_BAUD 115200

//#define DEBUG_IMU

class IMU {
    public:
        IMU() {
            setup(_IMU_SERIAL, _IMU_BAUD);
        }
        ~IMU() {;}

        struct RECEIVE_DATA_STRUCTURE_IMU{
            float IMUloop;
            float pitch;
            float roll;
        } data;

        byte status;

        /**
        * Setup the class.
        */

       void setup(HardwareSerial* serial, long baud_rate) {
           serial->begin(baud_rate);
           bno.setExtCrystalUse(true);
       }

       /**
        * Recieves the data from the IMU
        */

       void task(){
           if(millis() - lastReceivedMillis >= 10){
                sensors_event_t event; 
                bno.getEvent(&event);
           }
           IMUtimeout();
       }

       private:
        unsigned long lastRecievedMillis;
        uint16_t BNO055_SAMPLERATE_DELAY_MS = 10; //how often to read data from the board

        Adafruit_BNO055 bno = Adafruit_BNO055(55);

        void IMUtimeout() {
            if(!bno.begin())
            {
                /* There was a problem detecting the BNO055 ... check your connections */
                Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
                while(1);
            }
        }

};

#endif //IMU_H_