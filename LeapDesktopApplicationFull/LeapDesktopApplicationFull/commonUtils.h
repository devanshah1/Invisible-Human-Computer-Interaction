/*******************************************************************************************
*
*  Source File Name = commonUtils.h
*
*  Descriptive Name = Functions used to control volume for workstations
*
*  List of Functions:
*
*     void volumeManipulation ( string controlOption )
*
*  Dependencies: See function dependencies
*
*  Restrictions: See function dependencies
*
*  Supported Platforms:
*       Windows 32 bit
*
*  Project Developers:
*      Devan Shah        (100428864)
*      Ravikumar Patel   (100423830)
*      Parth Patel       (100392782)
*      Miguel Arindaeng  (100394094)
*
*  Change Log:
*    DATE      DESCRIPTION                                           Name
*  dd/mmm/yyyy
*  ========================================================================================
*  07/10/2014  Initial drop - File contains functions for volume     Devan Shah 100428864
*                             control.
*******************************************************************************************/
#include <iostream>
#include <string.h>
#include "Leap.h"
#include "LeapMath.h"
#include <windows.h>
#include <endpointvolume.h> 
#include <mmdeviceapi.h>
#include <stdlib.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comdlg32.lib")

using namespace Leap;

// Buffer for envirenment variable retrival
#define BUFSIZE 4096

// Constant for states available, fingers detactable and the bones that are detectable
const std::string stateNames [] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};
const std::string fingerNames [] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames [] = {"Metacarpal", "Proximal", "Middle", "Distal"};

// Definations for available gestures
#define SWIPE_LEFT             "S_LEFT"
#define SWIPE_RIGHT            "S_RIGHT"
#define SWIPE_UP               "S_UP"  
#define SWIPE_DOWN             "S_DOWN" 
#define CIRCLE_CLOCKWISE       "C_CLOCKWISE"
#define CIRCLE_COUNTERCLOKWISE "C_COUNTERCLOKWISE"
#define KEY_TAP                "KEY_TAP"
#define SCREEP_TAP             "SCREEN_TAP"

// Definations for actions available
#define VOLUME_STEP_UP      "VOL_INCREASE"
#define VOLUME_STEP_DOWN    "VOL_DECREASE"
#define VOLUME_MUTE_UNMUTE  "VOL_MUTE_UNMUTE"
#define LOCK_WORK_STATION   "LOCK"          
#define OPEN_CALCULATOR     "OPEN_CAL"
#define APP_WINDOW_NEXT     "WIN_NEXT"
#define APP_WINDOW_PREVIOUS "WIN_PRV"
#define SHOW_KEYBOARD       "SHOW_KEYBOARD"
#define HIDE_KEYBOARD       "HIDE_KEYBOARD"
#define MOVE_MOUSE          "MOVE_MOUSE"
#define MOUSE_LEFT_CLICK    "MOUSE_LEFT_CLICK"
#define MOUSE_RIGHT_CLICK   "MOUSE_RIGHT_CLICK"

// Definations for actions to be performed when set
#define VOLUME_STEP_UP_ACTION      volumeManipulation( VOLUME_STEP_UP );
#define VOLUME_STEP_DOWN_ACTION    volumeManipulation( VOLUME_STEP_DOWN );
#define VOLUME_MUTE_UNMUTE_ACTION  volumeManipulation( VOLUME_MUTE_UNMUTE );
#define LOCK_WORK_STATION_ACTION   LockWorkStation();          
#define OPEN_CALCULATOR_ACTION     system("calc");
#define SHOW_KEYBOARD_ACTION       keyboardManipulation ( SHOW_KEYBOARD );
#define HIDE_KEYBOARD_ACTION       keyboardManipulation ( HIDE_KEYBOARD );
#define APP_WINDOW_NEXT_ACTION     applicationWindowManipulation ( APP_WINDOW_NEXT );
#define APP_WINDOW_PREVIOUS_ACTION applicationWindowManipulation ( APP_WINDOW_PREVIOUS );

// Function decleration
void volumeManipulation ( std::string controlOption );
void applicationWindowManipulation ( std::string windowOption );
void defaultEnvironmentSetup ();
void getEnvironmentVariables ();
void freeEnvironmentBuffers ();
void circleGestures ( const Gesture& gesture, const Controller& controller );
void swipeGesture ( const Gesture& gesture, const Controller& controller );
void keyTapGesture ( const Gesture& gesture, const Controller& controller );
void screenTapGesture ( const Gesture& gesture, const Controller& controller );
void runGestureAction ( std::string gestureAction );
void executeAction ( LPTSTR executionAction );
void keyboardManipulation ( std::string keyboardOption );
void determineGestureAndPerformAction ( const Frame& frame, const Controller& controller );
void determineHandAndPerformAction ( const Frame& frame, const Controller& controller );
void determineFingerAndPerformAction ( const Controller& controller, const Hand& hand );
void moveMouse ( const Controller& controller, std::string mouseAction );