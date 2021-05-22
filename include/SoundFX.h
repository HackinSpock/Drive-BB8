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
    void setup(
        uint8_t sfx_fade_pin = _SFX_FADE_PIN
      ) {
      mp3Timout(); // check if DFplayer is properly connected

      // this->sfx_fade_pin = sfx_fade_pin;
      // pinMode(this->sfx_fade_pin, INPUT);

      myDFPlayer.volume(30);
    }

    /**
    * Work.  Run this repeatidly in the main loop.
    */
    void task() {
      unsigned long currentMillis = millis();
      if (currentMillis - this->previousMillis >= _SFX_TASK_INTERVAL) {
        this->previousMillis = currentMillis;
        //this->psiActivity();
      }
    }

    void playFile(uint16_t isPlaying, boolean psiEnabled) {
      if(isPlaying > (oldIsPlaying + 1000))
      {
        this->myDFPlayer.play(random(this->trackMin, this->trackMax)); // play track between min and max values
        this->psi_enabled = psiEnabled;
      }
      else if(isPlaying < (oldIsPlaying - 1000))
      {
        oldIsPlaying = isPlaying;
      }
    }

    void soundType(uint16_t type)
    {
      if(type <= 180)
      {
        this->trackMin = 1;
        this->trackMax = 7;
      } 
      else if(type <= 1000 && type >= 985)
      {
        this->trackMin = 8;
        this->trackMax = 15;
      }
      else //if(type >= 1800)
      {
        this->trackMin = 16;
        this->trackMax = 24;
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

    void mp3Timout()
    {
      if (!myDFPlayer.begin(Serial3)) {  //Use hardwareserial to communicate with mp3.
        while(true){
          //delay
        }
      }
    }

  private:
    unsigned long previousMillis = 0; // used to determine if loop should run
    unsigned long lastSound = 0;

    DFRobotDFPlayerMini myDFPlayer = DFRobotDFPlayerMini();
    uint8_t sfx_fade_pin;
    bool psi_enabled = false; 

    uint8_t psi_level = 0;
    uint16_t trackMin;
    uint16_t trackMax;
    uint16_t oldIsPlaying;

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

    // void reset() {
    //   digitalWrite(this->sfx_reset_pin, LOW);
    //   pinMode(this->sfx_reset_pin, OUTPUT);
    //   //delay(10);
    //   pinMode(this->sfx_reset_pin, INPUT);
    //   // commenting out for now, Adafruit gives this as an example, but I do
    //   // not think we need it.
    //   // delay(1000); // give a bit of time to 'boot up'
    // }
};

#endif //SFX_H_