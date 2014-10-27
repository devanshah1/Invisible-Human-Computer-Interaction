/*******************************************************************************************
*
*  Source File Name = LeapGestureControls.cpp
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
#include "commonUtils.h"

using namespace Leap;

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
void circleGestures ( Gesture gesture, const Controller& controller )
{
    CircleGesture circle = gesture;
    std::string clockwiseness;

    // Detect if the single fingure motion was in the clock wise direction 
    if ( circle.pointable ().direction ().angleTo ( circle.normal () ) <= PI / 2 && stateNames [gesture.state ()] == "STATE_END" )
    {
        clockwiseness = "counterclockwise";

        // handle the action when clockwise circle action is detected from Leap Motion.
        runGestureAction ( CIRCLE_CLOCKWISE );
    }
    else if ( stateNames [gesture.state ()] == "STATE_END" )
    {
        clockwiseness = "clockwise";
        runGestureAction ( CIRCLE_COUNTERCLOKWISE );
    }

    // Calculate angle swept since last frame
    float sweptAngle = 0;
    if ( circle.state () != Gesture::STATE_START )
    {
        CircleGesture previousUpdate = CircleGesture ( controller.frame ( 1 ).gesture ( circle.id () ) );
        sweptAngle = ( circle.progress () - previousUpdate.progress () ) * 2 * PI;
    }

    std::cout << std::string ( 2, ' ' )
        << "Circle id: " << gesture.id ()
        << ", state: " << stateNames [gesture.state ()]
        << ", progress: " << circle.progress ()
        << ", radius: " << circle.radius ()
        << ", angle " << sweptAngle * RAD_TO_DEG
        << ", " << clockwiseness;
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
void swipeGesture ( Gesture& gesture, const Controller& controller )
{
    SwipeGesture swipe = gesture;

    std::cout << std::string ( 2, ' ' )
        << "Swipe id: " << gesture.id ()
        << ", state: " << stateNames [gesture.state ()]
        << ", direction: " << swipe.direction ()
        << ", speed: " << swipe.speed ();
    
    /*
    * Handle Horizontal Swipe detection:
    *	   - Horizontal swipe is detected based on the x and y coordinate that are detected
    *        during the action. The direction function from SwipeGestures API is used to 
    *        determine the x and y coordinates of the swipe.
    *      - Using the x and y coordinates we can determine the orientation of the swipe
    *      - When the x value of the swipe is greater then the y value of the swipe this 
    *        show that the swipe is horizontal. This is true because when you perform a horizontal 
    *        swipe you are increasing the x coordinate and the y should stay retively the same.
    */
    if ( abs ( swipe.direction().x ) > abs ( swipe.direction().y ) )
    {
        /*
        * Handle Right Swipe:
        *	   - Perform an action when user has performed a right swipe on leap.
        *      - The right swipe from the user is detected based on the x coordinates
        *        that are detected during the action, this is pulled from the SwipeGestures API
        *        using the direction function wich provides data of the x, y, and z coordinates
        *        of a swipe direction.
        *      - Right swipe is performed in a horizontal manner therefore only the x coordinate 
        *        would increase in a positive direction. (greater then 0)
        */
        if ( swipe.direction ().x > 0 )
        {
            runGestureAction ( SWIPE_RIGHT );
        }
        /*
        * Handle Left Swipe:
        *	   - Perform an action when user has performed a left swipe on leap.
        *      - The left swipe from the user is detected based on the x coordinates
        *        that are detected during the action, this is pulled from the SwipeGestures API
        *        using the direction function wich provides data of the x, y, and z coordinates
        *        of a swipe direction.
        *      - Left swipe is performed in a horizontal manner therefore only the x coordinate
        *        would decrease in a negitive direction. (less then 0)
        */
        else
        {
            runGestureAction ( SWIPE_LEFT );
        }
    }
    /*
    * Handle Vertical Swipe detection:
    *	   - Vertical swipe is detected based on the x and y coordinate that are detected
    *        during the action. The direction function from SwipeGestures API is used to
    *        determine the x and y coordinates of the swipe.
    *      - Using the x and y coordinates we can determine the orientation of the swipe
    *      - When the y value of the swipe is greater then the x value of the swipe this
    *        show that the swipe is vertical. This is true because when you perform a vertival
    *        swipe you are increasing the y coordinate and the x should stay retively the same.
    */
    else
    {
        if ( swipe.direction ().y > 0 )
        {
            //runGestureAction ( SWIPE_UP );
            runGestureAction ( SWIPE_DOWN );
        }
        else
        {
            //runGestureAction ( SWIPE_DOWN );
            runGestureAction ( SWIPE_UP );
        }
    }
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
void keyTapGesture ( Gesture& gesture, const Controller& controller )
{
    KeyTapGesture tap = gesture;
    std::cout << std::string ( 2, ' ' )
        << "Key Tap id: " << gesture.id ()
        << ", state: " << stateNames [gesture.state ()]
        << ", position: " << tap.position ()
        << ", direction: " << tap.direction ();

    runGestureAction ( KEY_TAP );
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
void screenTapGesture ( Gesture& gesture, const Controller& controller )
{
    ScreenTapGesture screentap = gesture;
    std::cout << std::string ( 2, ' ' )
        << "Screen Tap id: " << gesture.id ()
        << ", state: " << stateNames [gesture.state ()]
        << ", position: " << screentap.position ()
        << ", direction: " << screentap.direction ();

    runGestureAction ( SCREEP_TAP );
}