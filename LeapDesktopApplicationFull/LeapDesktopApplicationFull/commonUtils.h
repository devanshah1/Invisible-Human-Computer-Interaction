/*******************************************************************************************
*
*  Source File Name = commonUtils.h
*
*  Descriptive Name = Contains the global definitions, common includes, common required
*                     linker configuration, global classes and also any globally
*                     accessible functions.
*
*  List of Functions/Classes:
*
*     LeapGestureFeedBack - Class used to construct the feed back window
*
*     WindowData - Class is used to construct the custom windows for the gestures
*
*  Dependencies:
*
*     Make sure that all the used header files and default c++ headers are available.
*     This includes Leap libraries and OpenGL libraries.
*
*  Restrictions: 
*
*     Make sure that all linker options are set correctly to avoid failures. Compiling supported 
*     only on windows 32 bit.
*     
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
*  21/10/2014  Adding Constants - Added some constants arrays        Devan Shah 100428864
*                                 used through out the project
*                                 
*
*  04/11/2014  Adding Definitions - Added some definitions for       Devan Shah 100428864
*                                   gestures SWIPE_LEFT, 
*                                   SWIPE_RIGHT, SWIPE_UP, 
*                                   SWIPE_DOWN also the
*                                   equivalent action definition
*
*  11/11/2014  Adding Definitions - Added some definitions for       Devan Shah 100428864
*                                   mouse movements and actions
*                                   MOVE_MOUSE, MOUSE_LEFT_CLICK
*                                   and MOUSE_RIGHT_CLICK
*
*  18/11/2014  Adding Definitions - Added some definitions for       Devan Shah 100428864
*                                   gesture actions 
*                                   LOCK_WORK_STATION_ACTION and
*                                   OPEN_CALCULATOR_ACTION as 
*                                   sample actions to perform
*                                   when gesture is performed
*
*  25/11/2014  Adding Definitions - Added some definitions for       Devan Shah 100428864
*                                   gestures CIRCLE_CLOCKWISE,
*                                   CIRCLE_COUNTERCLOKWISE,
*                                   KEY_TAP and SCREEP_TAP also
*                                   the equivalent action definition
*
*  02/12/2014  Commenting - Adding commenting for all the definitions Devan Shah 100428864
*                           and the classes that are present in this 
*                           file.
*
*  02/02/2015  Resource header - Adding the resource header file to   Devan Shah 100428864
*                                the master header file for all cpp
*                                files to use.

*  02/03/2015  Updating - Changing parameter of function              Devan Shah 100428864
*                         createUserFeedBackWindow from string path
*                         to a cinder DataSourceRef to load the 
*                         image resources correctly.
*
*  04/01/2015  Fixing - Minor indentation issues in the public        Devan Shah 100428864
*                       LeapGestureFeedBack class.
*
*******************************************************************************************/

// Include the necessary files to be used in the entire project

// General C++/windows header files that are needed
#include <iostream>
#include <string.h>
#include <windows.h>
#include <endpointvolume.h> 
#include <mmdeviceapi.h>
#include <stdlib.h>
#include <list>
#include <algorithm>

// General Leap header files that are needed
#include "Leap.h"
#include "LeapMath.h"

// General cinder header files that are needed
#include "cinder/app/AppNative.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/Cinder.h"
#include "cinder/Timeline.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/gl/TextureFont.h"

// Include internal resource file for leap application
#include "resource.h"

// Define global namespace used through out the project to use specific variable/functions
// without specifying the full namespace for each call/variable declaration.
using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gl;
using namespace Leap;

// Make use of the name-space Leap to avoid having to use Leap::
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

// Buffer for environment variable retrieval
#define BUFSIZE 4096

// Constant for states available, fingers available and finger bones available.
const std::string stateNames []  = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};
const std::string fingerNames [] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames []   = {"Metacarpal", "Proximal", "Middle", "Distal"};

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
#define SUPPORTED_GESTURES  "SUPPORTED_GESTURES"

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
#define SUPPORTED_GESTURE_ACTION   supportedGestures() ;

/**********************************************************************************

Class Name = LeapGestureFeedBack

Descriptive Name = This class is used to draw the necessary objects while also 
                   acting as a leap motion listener.

Functions:

    // General Cinder functions
    setup ();
    draw ();
    prepareSettings ( Settings *settings );

    // General Cinder window creation functions
    createUserFeedBackWindow ( cinder::DataSourceRef userFeedBackImageRef, int windowWidth, int windowHeight );
    createMainApplicationWindow ();

    // Global Function Deceleration for actions supported by the application
    volumeManipulation ( std::string controlOption );
    applicationWindowManipulation ( std::string windowOption );
    keyboardManipulation ( std::string keyboardOption );
    moveMouse ( const Controller& controller, std::string mouseAction );

    // Supported Gestures Function Deceleration
    circleGestures ( const Gesture& gesture, const Controller& controller );
    swipeGesture ( const Gesture& gesture, const Controller& controller );
    keyTapGesture ( const Gesture& gesture, const Controller& controller );
    screenTapGesture ( const Gesture& gesture, const Controller& controller );

    // General Function Deceleration
    runGestureAction ( std::string gestureAction );
    executeAction ( LPTSTR executionAction );
    determineGestureAndPerformAction ( const Frame& frame, const Controller& controller );
    determineHandAndPerformAction ( const Frame& frame, const Controller& controller );
    determineFingerAndPerformAction ( const Controller& controller, const Hand& hand );
    supportedGestures ();

    // Setup Function Deceleration
    defaultEnvironmentSetup ();
    getEnvironmentVariables ();
    freeEnvironmentBuffers ();
    
Dependencies =
    
    None

Restrictions =

    Must have the cinder lib's available in the linker and c/c++ configurations. 

******************************************************************************/
class LeapGestureFeedBack : public ci::app::AppNative
{
    public:

       // General Cinder functions
       void setup () ;
       void draw () ;
       void prepareSettings ( Settings *settings ) ;

       // Custom Cinder window creation functions
       void createUserFeedBackWindow ( cinder::DataSourceRef userFeedBackImageRef, int windowWidth, int windowHeight ) ;
       void createMainApplicationWindow () ;

       // Global Function Deceleration for actions supported by the application
       void volumeManipulation ( std::string controlOption ) ;
       void applicationWindowManipulation ( std::string windowOption ) ;
       void keyboardManipulation ( std::string keyboardOption ) ;
       void moveMouse ( const Controller& controller, std::string mouseAction ) ;

       // Supported Gestures Function Deceleration
       void circleGestures ( const Gesture& gesture, const Controller& controller ) ;
       void swipeGesture ( const Gesture& gesture, const Controller& controller ) ;
       void keyTapGesture ( const Gesture& gesture, const Controller& controller ) ;
       void screenTapGesture ( const Gesture& gesture, const Controller& controller ) ;

       // General Function Deceleration
       void runGestureAction ( std::string gestureAction ) ;
       void executeAction ( LPTSTR executionAction ) ;
       void determineGestureAndPerformAction ( const Frame& frame, const Controller& controller ) ;
       void determineHandAndPerformAction ( const Frame& frame, const Controller& controller ) ;
       void determineFingerAndPerformAction ( const Controller& controller, const Hand& hand ) ;
       void supportedGestures () ;

       // Setup Function Deceleration 
       void defaultEnvironmentSetup () ;
       void getEnvironmentVariables () ;
       void freeEnvironmentBuffers () ;

       // Stores the maximum screen size
       int maxWindowWidth ;
       int maxWindowHeight ;

       // Constants used to store environment variables for each gesture
       LPTSTR SWIPE_LEFT_SET ;
       LPTSTR SWIPE_RIGHT_SET ;
       LPTSTR SWIPE_UP_SET ;
       LPTSTR SWIPE_DOWN_SET ;
       LPTSTR CIRCLE_CLOCKWISE_SET ;
       LPTSTR CIRCLE_COUNTERCLOKWISE_SET ;
       LPTSTR KEY_TAP_SET ;
       LPTSTR SCREEP_TAP_SET ;

       // Setup the leap controller
       Controller leap ;

       // Stores the default windows settings
       Settings *settings ;
};

/**********************************************************************************

Class Name = WindowData

Descriptive Name = This class is used to define the window to be created and stores
                   the color and the texture (image) that the window is to display.

Function =

    Only has a public constructor and some object declarations (Color and Texture).

Dependencies =
    
    None

Restrictions =

    Must have the cinder lib's available in the linker and c/c++ configurations. 

******************************************************************************/
class WindowData
{
    public:
        // Constructor of the class that is used to store the color of the window and the image to display
        WindowData ( Texture image )
            : windowBackgroundColor ( Color ( 255, 255, 255 ) ) // Default to white background
        {
            // Store the image in the global variable to access when needed
            windowImageToLoad = image ;
        }

        // Variables to store the object in the class
        Color   windowBackgroundColor ; 
        Texture windowImageToLoad ;
};