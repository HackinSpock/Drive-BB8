#ifndef RCTransmitter_H_
#define RCTransmitter_H_
#include <ArduinoLog.h>
#include <SPI.h>

#include "Controller.h"
//#include "SBUS.h"
#include "Droid.h"

class RCTransmitter: public Controller {
  public:
    void setup(Droid* droid) override {
        this->droid = droid;
        pinMode(5, INPUT);
        pinMode(6, INPUT);
        pinMode(7, INPUT);
    }

    void task() override {
      this->processInput();
    }
      

  private:
    //SBUS xsr(Serial1);
    Droid* droid;

    //bool failSafe;
    //bool lostFrame;

    // Declaration of each channels function
    int16_t domePitch;
    int16_t domeRoll;
    int16_t domeSpin;

    int16_t drive;
    int16_t lean;
    int16_t flywheelSpin;

    int16_t START; // switch type channel

    // void processInput() // Sbus input
    // {
    //     xsr.begin();

    //     if(xsr.read(&channels[0], &failSafe, &lostFrame)) 
    //     {
    //       droid->dome.setDomePosition(domeSpin); // inputs left joystick potentionmeter data for dome spin
    //       droid->dome.setDomeXY(domePitch, domeRoll); // inputs from left joystick pitch and roll for dome pitch and roll

    //       droid->drive.setDriveSpeed(drive); // inputs from right joystick pitch for body drive forward/reverse
    //       droid->drive.setFlywheelSpeed(flywheelSpin); // inputs from right joystick potentiemter for flywheel spin
    //       droid->drive.setTilt(lean); // inputs from right joystick roll for drive lean

    //       if(START == HIGH)
    //       {
    //         this->isEnabled(!droid->drive.isEnabled());
    //       }
    //     }
    //     else
    //     {
    //       droid->dome.setDomePosition(0);
    //       droid->dome.setDomeXY(PITCH_CENTER, ROLL_CENTER);

    //       droid->drive.setDriveSpeed(0);
    //       droid->drive.setFlywheelSpeed(0);
    //       droid->drive.setTilt(LEAN_CENTER);
    //     }
        
    // }

    void processInput()
    {
        domePitch = pulseIn(5, HIGH, 25000);
        domeRoll = pulseIn(6, HIGH, 25000);
        //domeSpin = pulseIn(7, HIGH, 25000);

        //droid->dome.setDomePosition(domeSpin); // inputs left joystick potentionmeter data for dome spin
        droid->dome.setDomeXY(domePitch, domeRoll); // inputs from left joystick pitch and roll for dome pitch and roll

        // if(START == HIGH)
        // {
        //   this->isEnabled(!droid->drive.isEnabled());
        // }
    }

    #ifdef DOME_TILT_PITCH_CENTER
        int16_t PITCH_CENTER = DOME_TILT_PITCH_CENTER;
    #endif
    #ifdef DOME_TILT_ROLL_CENTER
        int16_t ROLL_CENTER = DOME_TILT_ROLL_CENTER;
    #endif
    #ifdef DRIVE_LEAN_CENTER
        int16_t LEAN_CENTER = DRIVE_LEAN_CENTER;
    #endif
    
};
#endif // RCTransmitter_H_
