#ifndef IMU_H_
#define IMU_H_

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


//#define DEBUG_IMU

#define _IMU_TASK_INTERVAL 10

class IMU {
    public:
        IMU() {}
        ~IMU() {;}

       void setup() {
           is_imuInit = bno.begin();
       }

       void task(){
           if(millis() >= nextMillis) {
                nextMillis = millis() + _IMU_TASK_INTERVAL;
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

    private:
        unsigned long nextMillis;
        Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

        float y_orientation;
        boolean is_imuInit = false;
};

#endif //IMU_H_