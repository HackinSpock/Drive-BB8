#ifndef RCTransmitter_H_
#define RCTransmitter_H_
#include <ArduinoLog.h>
#include <SPI.h>

#include "Controller.h"
#include "SBUS.h"
#include "Droid.h"

class RCTransmitter: public Controller {
  public:
    void setup(Droid* droid) override {
        this->droid = droid;
        // pinMode(5, INPUT);
        // pinMode(6, INPUT);
        // pinMode(7, INPUT);
        // pinMode(8, INPUT);
        // pinMode(9, INPUT);
        // pinMode(10, INPUT);
        // pinMode(11, INPUT);
    }

    void task() override {
      this->processInput();
    }
      

  private:
    SBUS x8r = SBUS(Serial2);
    Droid* droid;

    uint16_t channels[16];
    bool failSafe;
    bool lostFrame;

    // Declaration of each channels function
    // uint16_t domePitch = channels[0];
    // uint16_t domeRoll = channels[1];
    // int16_t domeSpin = channels[4];

    // uint16_t drive = channels[3];
    // uint16_t lean = channels[2];
    // uint16_t flywheel = channels[5];

    // uint16_t START = channels[6]; // switch type channel

    void processInput() // Sbus input
    {
        x8r.begin();

        if(x8r.read(&channels[0], &failSafe, &lostFrame)) 
        {
          droid->dome.setDomePosition(channels[4]); // inputs left joystick potentionmeter data for dome spin
          droid->dome.setDomeXY(channels[0], channels[1]); // inputs from left joystick pitch and roll for dome pitch and roll

          droid->drive.setDriveSpeed(channels[3]); // inputs from right joystick pitch for body drive forward/reverse
          droid->drive.setFlywheelSpeed(channels[5]); // inputs from right joystick potentiemter for flywheel spin
          droid->drive.setTilt(channels[2]); // inputs from right joystick roll for drive lean

          if(channels[11] >= 1800) // if flip switch is clicked
          {
          // All possible mood types if switch 1/2 is forward, center or back
          if(channels[9] <= 180)
          {
            droid->sfx.playTrack(4, 0, 9, true);
          }
          else if(channels[9] <= 1000 && channels[9] >= 985)
          {
            droid->sfx.playTrack(0, 0, 9, true);
          }
          else //if(channels[9] >= 1800)
          {
            droid->sfx.playTrack(6, 0, true);
          }
          // else if(channels[10] <= 180)
          // {
          //   droid->sfx.playTrack(5, 1, false);
          // }
          // else if(channels[10] <= 1000 && channels[9] >= 985)
          // {
          //   droid->sfx.playTrack(1, 0, 9, true);
          // }
          // else if(channels[10] >= 1800)
          // {
          //   droid->sfx.playTrack(5, 4, false);
          // }
          }
        }
        // else
        // {
        //   droid->dome.setDomePosition(0);
        //   droid->dome.setDomeXY(PITCH_CENTER, ROLL_CENTER);

        //   droid->drive.setDriveSpeed(0);
        //   droid->drive.setFlywheelSpeed(0);
        //   droid->drive.setTilt(LEAN_CENTER);
        // }
    
        if(channels[8] <= 175)
        {
          droid->drive.setEnable(true);
        }
        else
        {
          droid->drive.setEnable(false);
        }
        
    }

    // void processInput()
    // {
    //     domePitch = pulseIn(5, HIGH, 25000);
    //     domeRoll = pulseIn(6, HIGH, 25000);
    //     domeSpin = pulseIn(7, HIGH, 25000);
    //     drive = pulseIn(8, HIGH, 25000);
    //     lean = pulseIn(9, HIGH, 25000);
    //     flywheel = pulseIn(10, HIGH, 25000);
    //     START = pulseIn(11, HIGH, 25000);

    //     //droid->dome.setDomePosition(domeSpin); // inputs left joystick potentionmeter data for dome spin
    //     droid->dome.setDomeXY(domePitch, domeRoll); // inputs from left joystick pitch and roll for dome pitch and roll

    //     droid->drive.setDriveSpeed(drive); // inputs from right joystick pitch for body dr ive forward/reverse
    //     droid->drive.setTilt(lean); // inputs from right joystick roll for drive lean

    //     if(START < 990)
    //     {
    //       droid->drive.setEnable(true);
    //     }
    //     else
    //     {
    //       droid->drive.setEnable(false);
    //     }
        
    // }

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
