#ifndef IMU_H_
#define IMU_H_

#include <Arduino.h>
#include "EasyTransfer.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


//#define DEBUG_IMU

class IMU {
    public:
        IMU() {}
        ~IMU() {;}

        /**
        * Setup the class.
        */

       void setup() {
           bno.begin();
       }

       /**
        * Recieves the data from the IMU
        */

       void task(){
           if((millis() - lastRecievedMillis) >= 10){
                sensors_event_t orientationData; 
                bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
                y_orientation = printEvent(&orientationData);
           }
           
       }

        double printEvent(sensors_event_t* event) {
            //double x = -1000000; 
            double y = -1000000; 
            //double z = -1000000; //dumb values, easy to spot problem
            if (event->type == SENSOR_TYPE_ORIENTATION) {
                //x = event->orientation.x;
                y = event->orientation.y;
                //z = event->orientation.z;
            }
            else {
                //Serial.print("Unk:");
            }
            return y;
        }

        double getYOrientation()
        {
            return y_orientation;
        }
       
        void IMUtimeout() {
            if(!bno.begin())
            {
                /* There was a problem detecting the BNO055 ... check your connections */
                //Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
                while(1);
            }
        }

    private:
        unsigned long lastRecievedMillis;
        Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

        float y_orientation;
};

#endif //IMU_H_