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

    // droid.dome.setup(
    //     DOME_PITCH_SERVO_PIN, 
    //     DOME_ROLL_SERVO_PIN, 
    //     DOME_SPIN_SERVO_PIN, 
    //     DOME_SPIN_POT_PIN
    // );

    controller->setup(&droid);

    // add &droid.imu, as second parameter once fixed
    droid.drive.setup(
        &droid.dome,
        MAIN_DRIVE_NUM,
        FLYWHEEL_NUM,
        DRIVE_LEAN_SERVO_PIN
    );

    //droid.sfx.setup(SFX_SERIAL, SFX_RST, SFX_BAUD_RATE);

    //Log.notice(F("BB8 Drive Ready...\n"));

    Serial.begin(115200);
}

void loop()
{   
    controller->task();
    droid.task();
    
}