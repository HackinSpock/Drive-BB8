#include <Arduino.h>
#include <ArduinoLog.h>

#include "BBConfig.h"
#include "Droid.h"

//#define DEBUG_CONTROL

Droid droid = Droid();

void setup()
{
    Serial.begin(115200);
    Log.notice(F("BB8 Drive Control Started...\n"));

    droid.dome.setup(
        DOME_PITCH_SERVO_PIN, 
        DOME_ROLL_SERVO_PIN, 
        DOME_SPIN_SERVO_PIN, 
        DOME_SPIN_POT_PIN
    );

    droid.drive.setup(
        &droid.imu,
        MAIN_DRIVE_NUM,
        FLYWHEEL_NUM,
        DRIVE_LEAN_SERVO_PIN
    );

    droid.sfx.setup(SFX_SERIAL, SFX_RST, SFX_BAUD_RATE);



    Log.notice(F("BB8 Drive Ready...\n"));
}

void loop()
{
    droid.task();
}
