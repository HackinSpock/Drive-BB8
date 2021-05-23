#include <Arduino.h>
#include <ArduinoLog.h>

#include "BB8Config.h"
#include "Droid.h"
#include "RCTransmitter.h"

//#define DEBUG_CONTROL

Droid droid = Droid();
Controller* controller = new RCTransmitter();

void setup()
{   
    //Log.notice(F("BB8 Drive Control Started...\n"));

    droid.dome.setup(
        DOME_PITCH_SERVO_PIN, 
        DOME_ROLL_SERVO_PIN, 
        DOME_SPIN_SERVO_PIN, 
        DOME_SPIN_POT_PIN
    );

    // add &droid.imu, as second parameter once fixed
    droid.drive.setup(
        &droid.dome,
        &droid.imu,
        DRIVE_LEAN_SERVO_PIN
    );

    droid.sfx.setup();

    controller->setup(&droid);

    Serial.begin(115200);
    Serial1.begin(9600);
    Serial2.begin(9600);
    Serial3.begin(9600);

    //Log.notice(F("BB8 Drive Ready...\n"));
}

void loop()
{   
    controller->task();
    droid.task();
}