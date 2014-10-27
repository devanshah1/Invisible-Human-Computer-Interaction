/*******************************************************************************************
*
*  Source File Name = volumeController.cpp
*
*  Descriptive Name = Functions used to control volume for workstations
*
*  List of Functions:
*
*     void applicationWindowManipulation ( string controlOption )
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
#include "commonUtils.h"

using namespace Leap;

class MotionListener : public Listener
{
    public:
    virtual void onConnect ( const Controller& );
    virtual void onFrame ( const Controller& );

    private:
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
int main ( int argc, char** argv )
{
    defaultEnvironmentSetup ();
    getEnvironmentVariables ();

    // Create a sample listener and controller
    MotionListener listener;
    Controller controller;

    // Have the sample listener receive events from the controller
    controller.addListener ( listener );

    // Keep this process running until Enter is pressed
    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get ();

    // Remove the sample listener when done
    controller.removeListener ( listener );

    return 0;
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
void MotionListener::onConnect ( const Controller& controller )
{
    std::cout << "Connected" << std::endl;
    controller.enableGesture ( Gesture::TYPE_CIRCLE );
    controller.enableGesture ( Gesture::TYPE_KEY_TAP );
    controller.enableGesture ( Gesture::TYPE_SCREEN_TAP );
    controller.enableGesture ( Gesture::TYPE_SWIPE );
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
void MotionListener::onFrame ( const Controller& controller )
{
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame ();

    // Get gestures
    const GestureList gestures = frame.gestures ();

    for ( int g = 0; g < gestures.count (); ++g )
    {
        Gesture gesture = gestures [g];

        switch ( gesture.type () )
        {
            case Gesture::TYPE_CIRCLE:
            {
                circleGestures ( gesture, controller );

                break;
            }
            case Gesture::TYPE_SWIPE:
            {

                swipeGesture ( gesture, controller );
                break;
            }
            case Gesture::TYPE_KEY_TAP:
            {
                keyTapGesture ( gesture, controller );
                break;
            }
            case Gesture::TYPE_SCREEN_TAP:
            {
                screenTapGesture ( gesture, controller );
                break;
            }
            default:
                std::cout << std::string ( 2, ' ' ) << "Unknown gesture type." << std::endl;
                break;
        }
    }

    if ( !frame.hands ().isEmpty () || !gestures.isEmpty () )
    {
        std::cout << std::endl;
    }
}