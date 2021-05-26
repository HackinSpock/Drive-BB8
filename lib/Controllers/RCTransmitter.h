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
        x8r.begin();
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

    void processInput() // Sbus input
    {
        if(x8r.read(&channels[0], &failSafe, &lostFrame)) 
        {
          droid->dome.setDomePosition(channels[4]); // inputs left joystick potentionmeter data for dome spin
          droid->dome.setDomeXY(channels[0], channels[1]); // inputs from left joystick pitch and roll for dome pitch and roll

          droid->drive.setDriveSpeed(map_drive_inputs(channels[2])); // inputs from right joystick pitch for body drive forward/reverse
          droid->drive.setFlywheelSpeed(channels[5]); // inputs from right joystick potentiemter for flywheel spin
          droid->drive.setTilt(channels[3]); // inputs from right joystick roll for drive lean

          // droid->sfx.playFile(channels[6]); // play sounds from DFplayer Mini
          // droid->sfx.soundType(channels[9]); // choose sound type from 3 different positions
          // droid->sfx.adjustVolume(channels[10]); // adjust volume from 3 different positions  
        }

        if(channels[8] <= 175)
        {
          droid->drive.setEnable(true);
          droid->dome.setEnable(true);
        }
        else if(channels[8] >= 1800)
        {
          droid->drive.setEnable(false);
          droid->dome.setEnable(false);
        }
        else
        {
          droid->drive.setEnable(false);
          droid->drive.setEnable(true);
        }         
    }

    int16_t map_drive_inputs(int16_t input) {
            int8_t x = 0;
                if (input > 1000) {
                    x = map(input, 1000, 1811, 0, 127);
                }
                else if(input < 980)
                {
                  x = map(input, 172, 980, -127, 0);
                }
                
            return x;
        }

    int16_t map_servo_inputs(int16_t input, int16_t dead_zone) {
            int8_t x = 0;
                if  (input > 1005) {
                    x = map(input, 1005, 1811, 1475, 1811);
                }
                else if(input < 975)
                {
                  x = map(input, 172, 975, 544, 1475);
                }           
            return x;
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
