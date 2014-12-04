/*******************************************************************************************
*
*  Source File Name = LeapHandControls.cpp
*
*  Descriptive Name = Functions used to control volume for workstations
*
*  List of Functions/Classes:
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
*  29/10/2014  Initial drop - File contains functions for hands     Devan Shah 100428864
*                             control.
*******************************************************************************************/
#include "commonUtils.h"

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
void LeapGestureFeedBack::prepareSettings ( Settings *settings )
{
    settings->setWindowSize ( 0, 0 );
    settings->setFrameRate ( 60.0f );
    settings->setBorderless ( true );
    settings->setWindowPos ( 0, 0 );
}

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
void LeapGestureFeedBack::setup ()
{ 
    defaultEnvironmentSetup ();
    getEnvironmentVariables ();

    // for the default window we need to provide an instance of WindowData
    createMainApplicationWindow ();
}

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
void LeapGestureFeedBack::draw ()
{
    gl::clear ( Color ( 255, 255, 255 ) );

    //leap.enableGesture ( Gesture::TYPE_CIRCLE );
    //leap.enableGesture ( Gesture::TYPE_KEY_TAP );
    //leap.enableGesture ( Gesture::TYPE_SCREEN_TAP );
    leap.enableGesture ( Gesture::TYPE_SWIPE );

    // Set the policy flag to make sure that the application is able to listen for background frames
    leap.setPolicyFlags ( Leap::Controller::POLICY_BACKGROUND_FRAMES );

    // Get the most recent frame and report some basic information
    const Leap::Frame frame = leap.frame ();

    // Sort through the hands and perform the necessary actions that are associated to the hand actions.
    determineHandAndPerformAction ( frame, leap );

    // Sort through the gestures and perform the necessary actions that are associated to the gestures.
    determineGestureAndPerformAction ( frame, leap );

    WindowData *data = getWindow ()->getUserData<WindowData> ();

    gl::color ( data->windowBackgroundColor );
    gl::draw ( data->windowImageToLoad, Vec2f ( 0, 0 ) );
    gl::enableAlphaBlending ();
    glColor4f ( 1.0f, 1.0f, 1.0f, 0.5f );
    glEnable ( GL_BLEND );
    gl::end ();
}

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
void LeapGestureFeedBack::createUserFeedBackWindow ( std::string userFeedBackImagePath, int windowWidth, int windowHeight )
{
    gl::Texture userFeedBackImage = gl::Texture ( loadImage ( userFeedBackImagePath ) );

    app::WindowRef newWindow = createWindow ( Window::Format ().size ( windowWidth, windowHeight ) );
    newWindow->setUserData ( new WindowData ( userFeedBackImage ) );
    newWindow->setBorderless ( true );

    // for demonstration purposes, we'll connect a lambda unique to this window which fires on close
    int uniqueId = getNumWindows ();
    newWindow->getSignalClose ().connect (
        [uniqueId, this]
    {
        this->console () << "You closed window #" << uniqueId << std::endl;
    }
    );
}

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
void LeapGestureFeedBack::createMainApplicationWindow ()
{
    app::WindowRef newWindow = createWindow ( Window::Format ().size ( 10, 10 ) );
    newWindow->setBorderless ( true );

    // for demonstration purposes, we'll connect a lambda unique to this window which fires on close
    int uniqueId = getNumWindows ();
    newWindow->getSignalClose ().connect (
        [uniqueId, this]
    {
        this->console () << "You closed window #" << uniqueId << std::endl;
    }
    );
}

CINDER_APP_NATIVE ( LeapGestureFeedBack, RendererGl )
