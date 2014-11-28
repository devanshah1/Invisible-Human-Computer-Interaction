/*******************************************************************************************
*
*  Source File Name = commonUtils.h
*
*  Descriptive Name = Functions used to control volume for workstations
*
*  List of Functions:
*
*     N/A
*
*  Dependencies:
*   
*     Make sure that all the used header files and default c++ headers are available. 
*     This includes Leap libraries and OpenGL libraries. 
*
*  Restrictions: N/A
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
*  07/10/2014  Initial drop - File contains global definitions,      Devan Shah 100428864
*                             common includes, common required
*                             linker configurations and global 
*                             functions.
*  
*******************************************************************************************/

// Include the necessary files to be used in the entire project
#include <iostream>
#include <string.h>
#include "Leap.h"
#include "LeapMath.h"
#include <windows.h>
#include <endpointvolume.h> 
#include <mmdeviceapi.h>
#include <stdlib.h>
#include "cinder/app/AppNative.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include <list>
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace Leap;

/*
 * Define the programming directives for making sure that the linker knows the lib's
 * listed need to be added to the list of libraries dependencies.
 * Need to make sure that the core windows lib's are added to linker to avoid compilation 
 * failures.
 */
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comdlg32.lib")

// Make use of the name-space Leap to avoid having to use Leap::
using namespace Leap;

// Buffer for environment variable retrieval
#define BUFSIZE 4096

// Constant for states available, fingers available and finger bones available.
const std::string stateNames []  = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };
const std::string fingerNames [] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames []   = { "Metacarpal", "Proximal", "Middle", "Distal" };

// Definitions for available gestures in the application currently
#define SWIPE_LEFT             "S_LEFT"
#define SWIPE_RIGHT            "S_RIGHT"
#define SWIPE_UP               "S_UP"  
#define SWIPE_DOWN             "S_DOWN" 
#define CIRCLE_CLOCKWISE       "C_CLOCKWISE"
#define CIRCLE_COUNTERCLOKWISE "C_COUNTERCLOKWISE"
#define KEY_TAP                "KEY_TAP"
#define SCREEP_TAP             "SCREEN_TAP"

// Definitions for actions available to be performed by the application currently
#define VOLUME_STEP_UP      "VOL_INCREASE"
#define VOLUME_STEP_DOWN    "VOL_DECREASE"
#define VOLUME_MUTE_UNMUTE  "VOL_MUTE_UNMUTE"
#define LOCK_WORK_STATION   "LOCK_WORK_STATION" 
#define OPEN_CALCULATOR     "OPEN_CAL"
#define APP_WINDOW_NEXT     "WIN_NEXT"
#define APP_WINDOW_PREVIOUS "WIN_PRV"
#define SHOW_KEYBOARD       "SHOW_KEYBOARD"
#define HIDE_KEYBOARD       "HIDE_KEYBOARD"
#define MOVE_MOUSE          "MOVE_MOUSE"
#define MOUSE_LEFT_CLICK    "MOUSE_LEFT_CLICK"
#define MOUSE_RIGHT_CLICK   "MOUSE_RIGHT_CLICK"

// Definitions for action to be performed when specific action is called by the application currently
#define VOLUME_STEP_UP_ACTION      volumeManipulation( VOLUME_STEP_UP );
#define VOLUME_STEP_DOWN_ACTION    volumeManipulation( VOLUME_STEP_DOWN );
#define VOLUME_MUTE_UNMUTE_ACTION  volumeManipulation( VOLUME_MUTE_UNMUTE );
#define LOCK_WORK_STATION_ACTION   LockWorkStation();          
#define OPEN_CALCULATOR_ACTION     system("calc");
#define SHOW_KEYBOARD_ACTION       keyboardManipulation ( SHOW_KEYBOARD );
#define HIDE_KEYBOARD_ACTION       keyboardManipulation ( HIDE_KEYBOARD );
#define APP_WINDOW_NEXT_ACTION     applicationWindowManipulation ( APP_WINDOW_NEXT );
#define APP_WINDOW_PREVIOUS_ACTION applicationWindowManipulation ( APP_WINDOW_PREVIOUS );

/**********************************************************************************

Function Name = volumeManipulation

Descriptive Name = Increase/decrease/mute/unmute the volume

Function =



Dependencies =
None

Restrictions =
None

Input =

Output =
See function description.

Normal Return =
0 -

Error Return =
None

******************************************************************************/
class LeapGestureFeedBack : public ci::app::AppNative
{
    public:
        void setup ();
        void draw ();
        void createUserFeedBackWindow ( std::string imagePath, int windowWidth, int windowHeight );
        void createMainApplicationWindow ();
        void preperSettings ( Settings *settings );

        // Global Function Deceleration for actions supported by the application
        void volumeManipulation ( std::string controlOption );
        void applicationWindowManipulation ( std::string windowOption );
        void keyboardManipulation ( std::string keyboardOption );
        void moveMouse ( const Controller& controller, std::string mouseAction );

        // Supported Gestures Function Deceleration
        void circleGestures ( const Gesture& gesture, const Controller& controller );
        void swipeGesture ( const Gesture& gesture, const Controller& controller );
        void keyTapGesture ( const Gesture& gesture, const Controller& controller );
        void screenTapGesture ( const Gesture& gesture, const Controller& controller );

        // General Function Deceleration
        void runGestureAction ( std::string gestureAction );
        void executeAction ( LPTSTR executionAction );
        void determineGestureAndPerformAction ( const Frame& frame, const Controller& controller );
        void determineHandAndPerformAction ( const Frame& frame, const Controller& controller );
        void determineFingerAndPerformAction ( const Controller& controller, const Hand& hand );

        // Setup Function Deceleration 
        void defaultEnvironmentSetup ();
        void getEnvironmentVariables ();
        void freeEnvironmentBuffers ();

        // Stores the maximum screen size
        int maxWindowWidth;
        int maxWindowHeight;

        // Constants used to store environment variables for each gesture
        LPTSTR SWIPE_LEFT_SET;
        LPTSTR SWIPE_RIGHT_SET;
        LPTSTR SWIPE_UP_SET;
        LPTSTR SWIPE_DOWN_SET;
        LPTSTR CIRCLE_CLOCKWISE_SET;
        LPTSTR CIRCLE_COUNTERCLOKWISE_SET;
        LPTSTR KEY_TAP_SET;
        LPTSTR SCREEP_TAP_SET;

        // Setup the leap controller
        Leap::Controller leap;
};

/**********************************************************************************

Function Name = volumeManipulation

Descriptive Name = Increase/decrease/mute/unmute the volume

Function =



Dependencies =
None

Restrictions =
None

Input =

Output =
See function description.

Normal Return =
0 -

Error Return =
None

******************************************************************************/
class WindowData
{
    public:
        WindowData ( gl::Texture image )
            : windowBackgroundColor ( Color ( 255, 255, 255 ) ) // Default to white background
        {
            windowImageToLoad = image;
        }

        Color			windowBackgroundColor;
        gl::Texture     windowImageToLoad;
};