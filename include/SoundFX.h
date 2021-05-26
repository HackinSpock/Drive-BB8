#ifndef SFX_H_
#define SFX_H_

#include "Arduino.h"
#include "ArduinoLog.h"
#include "Stream.h"
  
#include "DFRobotDFPlayerMini.h"

#define _SFX_FADE_PIN A2

// internal settings
#define _SFX_TASK_INTERVAL 50

class SoundFX {
  public:
    SoundFX() {}
    ~SoundFX() {;}

    /**
    * Sets up the soundFX class.
    */
   // add uint8_t sfx_fade_pin = _SFX_FADE_PIN to parameters
    void setup() {
      // this->sfx_fade_pin = sfx_fade_pin;
      // pinMode(this->sfx_fade_pin, INPUT);

      myDFPlayer.begin(Serial3);
      myDFPlayer.volume(30);
    }

    /**
    * Work.  Run this repeatidly in the main loop.
    */
    void task() {
      if(millis() >= nextMillis) {
        nextMillis = millis() + _SFX_TASK_INTERVAL;
        //this->psiActivity();
      }
    }

    void playFile(uint16_t isPlaying) {
      if(isPlaying >= 1800 && millis() - lastrun > debounce_time)
      {
        myDFPlayer.play(random(this->trackMin, this->trackMax)); // play track between min and max values
        //Serial.println("Sound played");
        psi_enabled = true;
        lastrun = millis();
      }
    }

    void soundType(uint16_t type)
    {
      if(type <= 180)
      {
        trackMin = 1;
        trackMax = 7;
      } 
      else if(type <= 1000 && type >= 985)
      {
        trackMin = 8;
        trackMax = 15;
      }
      else //if(type >= 1800)
      {
        trackMin = 16;
        trackMax = 24;
      }
    }

    void adjustVolume(uint16_t volume) { // 0 - 30
      if(volume <= 180)
      {
        myDFPlayer.volume(10);
      } 
      else if(volume <= 1000 && volume >= 985)
      {
        myDFPlayer.volume(20);
      }
      else //if(type >= 1800)
      {
        myDFPlayer.volume(30);
      }
    }

  private:
    unsigned long nextMillis = 0; // used to determine if loop should run
    unsigned long lastSound = 0;

    DFRobotDFPlayerMini myDFPlayer = DFRobotDFPlayerMini();
    uint8_t sfx_fade_pin;
    bool psi_enabled = false; 

    uint8_t psi_level = 0;
    
    uint16_t trackMin;
    uint16_t trackMax;
    uint16_t oldIsPlaying;
    uint16_t debounce_time = 500;
    unsigned long lastrun;


    void psiActivity() {
      if ((millis() - this->lastSound) < 500 && this->psi_enabled) {
        readLevel();
      } else if (this->psi_enabled) {
        if (!this->myDFPlayer.available()) {
          readLevel();
        } else {
          this->psi_enabled = false;
          this->psi_level = 0;
        }
      }
    }

    /**
    * Read the level and apply and multiplier.  Sets the level between 0.255.
    */
    void readLevel() {
      this->psi_level = constrain(
        analogRead(this->sfx_fade_pin) * 10,
        0, 255);
    }
};

#endif //SFX_H_