#ifndef SFX_H_
#define SFX_H_

#include "Arduino.h"
#include "ArduinoLog.h"
#include "Stream.h"
  
#include "DFRobotDFPlayerMini.h"
//#include "Adafruit_Soundboard.h"

// default settings, call setup to change these
#define _SFX_RST 4
#define _SFX_FADE_PIN A1
#define _SFX_BAUD_RATE 9600

// internal settings
#define _SFX_TASK_INTERVAL 50

class SoundFX {
  public:
    SoundFX() {
    }
    ~SoundFX() {;}

    /**
    * Sets up the soundFX class.
    */
    void setup(
        uint8_t sfx_fade_pin = _SFX_FADE_PIN
      ) {
      myMP3.begin(Serial1);

      this->sfx_fade_pin = sfx_fade_pin;
      pinMode(this->sfx_fade_pin, INPUT);

      //this->myMP3.reset();
      myMP3.volume(30);
      myMP3.play(1);
    }

    /**
    * Work.  Run this repeatidly in the main loop.
    */
    void task() {
      unsigned long currentMillis = millis();
      if (currentMillis - this->previousMillis >= _SFX_TASK_INTERVAL) {
        this->previousMillis = currentMillis;
        this->psiActivity();
      }
    }

    /**
    * Convience method to play a track in bank given a range.
    */
    void playTrack(uint16_t min, uint8_t max, boolean psiEnabled) {
      this->playFile(random(min, max), psiEnabled);
    }

    void playFile(uint16_t filename, boolean psiEnabled = true) {
      // if (this->myMP3.available()) 
      // {
      //   this->myMP3.stop(); // stop the current sound that is playing
      //   this->myMP3.play(filename); // play new filename
      // }
      this->myMP3.play(filename); // play new filename
      this->psi_enabled = psiEnabled;
    }

    /**
    * Raises volume to 205 (0...205 is the range)
    */
    void volUp(uint16_t volume) {
        this->myMP3.volume(volume);
    }

    /**
    * Lower volume in increments of 10 (0...205 is the range)
    */
    void volDown(uint16_t volume) {
        this->myMP3.volume(volume);
    }

  private:
    unsigned long previousMillis = 0; // used to determine if loop should run
    unsigned long lastSound = 0;
    //SoftwareSerial serial = SoftwareSerial(SFX_TX, SFX_RX); // TX, RX
    //HardwareSerial* serial;
    //Adafruit_Soundboard fxSound = Adafruit_Soundboard(&Serial2, NULL, _SFX_RST);
    DFRobotDFPlayerMini myMP3 = DFRobotDFPlayerMini();
    uint8_t sfx_fade_pin, sfx_reset_pin;
    bool psi_enabled = false; 
    uint8_t psi_level = 0;

    void psiActivity() {
      if ((millis() - this->lastSound) < 500 && this->psi_enabled) {
        readLevel();
      } else if (this->psi_enabled) {
        if (!this->myMP3.available()) {
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