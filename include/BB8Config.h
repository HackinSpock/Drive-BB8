#ifndef BB8_CONFIG_H_
#define BB8_CONFIG_H_

/**
*  DO NOT OVERRIDE VALUES IN THIS FILE!!!
*
*  This is a default reference for all of the possible configurations you can
*  change in your BB unit.  This file will change over time and it's defaults
*  will likely overwrite changes you have made.
*
*  To change any config items create a file called MyConfig.h in the same
*  folder as your .ino file.  Options copied there will override those defined
*  here and importantly will not overriden the next time you update the code.
*/

/**
* Dome position and spin settings
*/ 
#define DOME_PITCH_SERVO_PIN 5 
#define DOME_ROLL_SERVO_PIN 6

//Pitch position values
#define DOME_TILT_PITCH_FRONT 2350
#define DOME_TILT_PITCH_CENTER 2000
#define DOME_TILT_PITCH_BACK 1300

//Roll position values
#define DOME_TILT_ROLL_LEFT 1000
#define DOME_TILT_ROLL_CENTER 1900
#define DOME_TILT_ROLL_RIGHT 2800

#define DOME_SPIN_SERVO_PIN 4
#define DOME_SPIN_POT_PIN A4
#define DOME_SPIN_CENTER_POSITION 512


/**
* Main drive configuration.
*/
#define MAIN_DRIVE_NUM 1
#define FLYWHEEL_NUM 2
#define DRIVE_LEAN_SERVO_PIN 7
#define DRIVE_LEAN_LEFT 1800
#define DRIVE_LEAN_CENTER 2300
#define DRIVE_LEAN_RIGHT 2600

/**
* Sound FX
**/
// sound board configuration
#define SFX_FADE_PIN A2

/**
* DEBUGGING FLAGS
* These exist in the headers of the project and are repeated here for usability.
**/
//#define DEBUG_CONTROL
//#define DEBUG_DRIVE_MOVEMENT
//#define DEBUG_DOME_MOVEMENT
//#define DEBUG_SFX
//#define DEBUG_IMU

#if defined(__has_include)
  #if __has_include("MyConfig.h")
    #include "MyConfig.h"
  #else
    #warning "       DO NOT OVERRIDE VALUES IN THIS FILE"
    #warning "--------------------------------------------------"
    #warning "A custom configuration header does not exist."
    #warning "Create a file called MyConfig.h and ovverride the "
    #warning "directives (config items) listed here."
  #endif
#endif


#endif // BB_CONFIG_H_
