/*******************************************************************************************
*
*  Source File Name = LeapGestureFeedBack.cpp
*
*  Descriptive Name = Used to start the application and create the main rendering of the application
*                     sets up all the required functions also that are needed for the rendering.
*                     This is the main file that handles all of the listening from leap and 
*                     displaying the user feedback information to the user.
*
*  List of Functions/Classes:
*
*     LeapGestureFeedBack::prepareSettings ( Settings *settings )
*     LeapGestureFeedBack::setup ()
*     LeapGestureFeedBack::draw ()
*     LeapGestureFeedBack::createUserFeedBackWindow ( cinder::DataSourceRef userFeedBackImage, int windowWidth, int windowHeight )
*     LeapGestureFeedBack::createMainApplicationWindow ()
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
*  29/10/2014  Initial drop - File contains functions for main     Devan Shah 100428864
*                             user feedback creation and also 
*                             starting of the Leap motion 
*                             listeners.
*
*  25/11/2014  Adding Functions - Added functions to create custom Devan Shah 100428864
*                                 windows for the user feedback.
*
*  02/12/2014  Commenting - Adding commenting for all the functions  Devan Shah 100428864
*                           and updating the change log to
*                           represent the change made to the file
*                           over the months
*
*******************************************************************************************/
#include "commonUtils.h"

/**********************************************************************************

Function Name = LeapGestureFeedBack::prepareSettings

Descriptive Name = Used to setup the settings for the main application window

Function =

    This function is used to prepare the main settings for the window

Dependencies =

    N/A

Restrictions =

    N/A

Input =
    
    Settings *settings - The settings variable that is created on class creation.

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A


******************************************************************************/
void LeapGestureFeedBack::prepareSettings ( Settings *settings )
{
    settings->setWindowSize ( 200, 200 );
    settings->setFrameRate ( 60.0f );
    settings->setBorderless ( true );
    settings->setWindowPos ( 0, 0 );
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::setup

Descriptive Name = Setup function for environment settings and also for creating the
                   main screen objects.

Function =

    This function is responsible for performing the setup that is required for the
    environment settings and also construction of the windows.

Dependencies =

    N/A

Restrictions =

    N/A

Input =
    
    N/A

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::setup ()
{ 
    defaultEnvironmentSetup ();
    getEnvironmentVariables ();

    // for the default window we need to provide an instance of WindowData
    createMainApplicationWindow ();
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::draw

Descriptive Name = Draw the windows that have been created, by default it will create the
                   the main application window

Function =

    This function is responsible for drawing the major windows with all the data that is
    present in the windowData objects. This function also sets up the leap motion listener 
    and enables all the options for gestures and enables background policy so leap can
    listening in the background.

Dependencies =

    N/A

Restrictions =

    N/A

Input =
    
    N/A

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::draw ()
{
    // Set the color to white for the background
    gl::clear ( Color ( 255, 255, 255 ) );

    // Enable the required Gestures
    leap.enableGesture ( Gesture::TYPE_CIRCLE );
    leap.enableGesture ( Gesture::TYPE_KEY_TAP );
    leap.enableGesture ( Gesture::TYPE_SCREEN_TAP );
    leap.enableGesture ( Gesture::TYPE_SWIPE );

    // Set the policy flag to make sure that the application is able to listen for background frames
    leap.setPolicyFlags ( Leap::Controller::POLICY_BACKGROUND_FRAMES );

    // Get the most recent frame and report some basic information
    const Leap::Frame frame = leap.frame ();

    // Sort through the hands and perform the necessary actions that are associated to the hand actions.
    determineHandAndPerformAction ( frame, leap );

    // Sort through the gestures and perform the necessary actions that are associated to the gestures.
    determineGestureAndPerformAction ( frame, leap );

    // Get the data for the window 
    WindowData *data = getWindow ()->getUserData<WindowData> ();

    // Only try to extract and draw the window if there is window data available
    if ( data != NULL )
    {
        // Set the color based of the data in the window object
        gl::color ( data->windowBackgroundColor );

        // Load the image that is available in the window data object
        gl::draw ( data->windowImageToLoad, Vec2f ( 0, 0 ) );
    }

    // Enable alpha bending, and set some color conversion settings
    gl::enableAlphaBlending ();
    glColor4f ( 1.0f, 1.0f, 1.0f, 0.5f );
    glEnable ( GL_BLEND );
    gl::end ();
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::createUserFeedBackWindow

Descriptive Name = Create custom image with data inside.

Function =

    This function is used to create windows based on the configuration that is passed in
    to the function. Also supports adding images into the windows while creating the image.

Dependencies =

    N/A

Restrictions =

    N/A

Input =
    
    std::string userFeedBackImagePath  - The location of the image that needs to be displayed
    int windowWidth                    - The width of the window
    int windowHeight                   - The height of the window

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A


******************************************************************************/
void LeapGestureFeedBack::createUserFeedBackWindow ( cinder::DataSourceRef userFeedBackImageRef, int windowWidth, int windowHeight )
{
    // Load the image that was passed in into a proper format
    gl::Texture userFeedBackImage = gl::Texture ( loadImage ( userFeedBackImageRef ) );
    
    // Create the window of size passed in, set the image into the windowData object and set it as borderless and
    app::WindowRef newWindow = createWindow ( Window::Format ().size ( windowWidth, windowHeight ) );
    newWindow->setUserData ( new WindowData ( userFeedBackImage ) );
    newWindow->setBorderless ( true );

    // Get a unique id for the window that is used to identify the window
    int uniqueId = getNumWindows ();
    
    // Set the unique number for the new window that was created
    newWindow->getSignalClose ().connect (
        [uniqueId, this]
        {
            this->console () << "You closed window #" << uniqueId << std::endl; // Action performed when the window is closed
        }
    );
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::createMainApplicationWindow

Descriptive Name = Creates the main window of the application

Function =

    This function is used to create the main application window

Dependencies =

    N/A

Restrictions =

    N/A

Input =
    
    N/A

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::createMainApplicationWindow ()
{
    // Create the window of size 10X10 and set it as borderless
    app::WindowRef newWindow = createWindow ( Window::Format ().size ( 10, 10 ) );
    newWindow->setBorderless ( true );
        
    // Get a unique id for the window that is used to identify the window
    int uniqueId = getNumWindows ();
    
    // Set the unique number for the new window that was created
    newWindow->getSignalClose ().connect (
        [uniqueId, this]
        {
            this->console () << "You closed window #" << uniqueId << std::endl; // Action performed when the window is closed
        }
    );
}

// Start the rendering of the application
CINDER_APP_NATIVE ( LeapGestureFeedBack, RendererGl )
