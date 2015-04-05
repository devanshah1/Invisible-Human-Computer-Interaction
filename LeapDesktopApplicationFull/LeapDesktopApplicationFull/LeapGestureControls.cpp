/*******************************************************************************************
*
*  Source File Name = LeapGestureControls.cpp
*
*  Descriptive Name = Functions used to control and detect the gestures.
*
*  List of Functions:
*
*     LeapGestureFeedBack::determineGestureAndPerformAction ( const Frame& frame, const Controller& controller )
*     LeapGestureFeedBack::circleGestures ( const Gesture& gesture, const Controller& controller )
*     LeapGestureFeedBack::swipeGesture ( const Gesture& gesture, const Controller& controller )
*     LeapGestureFeedBack::keyTapGesture ( const Gesture& gesture, const Controller& controller )
*     LeapGestureFeedBack::screenTapGesture ( const Gesture& gesture, const Controller& controller )
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
*  29/10/2014  Initial drop - File contains functions for controlling Devan Shah 100428864
*                             the gestures that are performed 
*                             and also performs the actions that are 
*                             needed to be performed.
*
*  02/12/2014  Commenting - Adding commenting for all the functions  Devan Shah 100428864
*                           and updating the change log to
*                           represent the change made to the file
*                           over the months
*
*  02/03/2015   Updating - Changing how the image are loaded, making Devan Shah 100428864
*                          use of the defined resources.
*
*  04/04/2015   Updating - Changing how the image for                Devan Shah 100428864
*                          screenTapGesture is opened. 
*
*******************************************************************************************/
#include "commonUtils.h"

using namespace Leap;

/**********************************************************************************

Function Name = LeapGestureFeedBack::determineGestureAndPerformAction

Descriptive Name = Used to detect the gesture and perform the action

Function =

    This function is used to detect the gesture and then perform the action 
    for the gesture that was detected in the frame.

Dependencies =

    N/A

Restrictions =

    N/A

Input =

    const Frame& frame - The frame object
    const Controller& controller  - The leap controller object that stores all the information
                                    about the leap motions.

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::determineGestureAndPerformAction ( const Frame& frame, const Controller& controller )
{
    // Get gestures
    const GestureList gestures = frame.gestures ();

    // Iterate through the gestures and perform the necessary actions
    for ( int g = 0; g < gestures.count (); ++g )
    {
        Gesture gesture = gestures [g];

        // Switch block to perform the action based on the gesture type that is detected.
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
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::circleGestures

Descriptive Name = Handles performing the action for the circle gesture

Function =

    This function is responsible for performing the action of the circle
    gesture, this supports counter clockwise and also clockwise circle 
    actions.

Dependencies =

    N/A

Restrictions =

    N/A

Input =
    
    const Gesture& gesture - The leap object for gestures
    const Controller& controller  - The leap controller object that stores all the information
                                    about the leap motions.

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::circleGestures ( const Gesture& gesture, const Controller& controller )
{
    // Variable Declaration
    CircleGesture circle = gesture;
    std::string clockwiseness;

    // Detect if the single finger motion was in the clock wise direction 
    if ( circle.pointable ().direction ().angleTo ( circle.normal () ) <= PI / 2 && stateNames [gesture.state ()] == "STATE_END" )
    {
        clockwiseness = "counterclockwise";

        // Handle the action when clockwise circle action is detected from Leap Motion.
        runGestureAction ( CIRCLE_CLOCKWISE );
        createUserFeedBackWindow ( loadResource ( RES_CLOCKWISE_IMAGE ), 150, 131 );
    }
    else if ( stateNames [gesture.state ()] == "STATE_END" )
    {
        clockwiseness = "clockwise";
        runGestureAction ( CIRCLE_COUNTERCLOKWISE );
        createUserFeedBackWindow ( loadResource ( RES_COUNTERCLOCKWISE_IMAGE ), 150, 131 );
    }

    // Calculate angle swept since last frame
    float sweptAngle = 0;
    if ( circle.state () != Gesture::STATE_START )
    {
        CircleGesture previousUpdate = CircleGesture ( controller.frame ( 1 ).gesture ( circle.id () ) );
        sweptAngle = ( circle.progress () - previousUpdate.progress () ) * 2 * PI;
    }

    // Debug info
    std::cout << std::string ( 2, ' ' )
        << "Circle id: " << gesture.id ()
        << ", state: " << stateNames [gesture.state ()]
        << ", progress: " << circle.progress ()
        << ", radius: " << circle.radius ()
        << ", angle " << sweptAngle * RAD_TO_DEG
        << ", " << clockwiseness;
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::swipeGesture

Descriptive Name = This function is used to perform the actions for the swipe gesture,
                   supports multiple direction of the swipe. There is a detection setup
                   for each of the directions.

Function =

    This function is used to perform the action swipe gestures, the supported gestures
    are up/down/right/left. 

Dependencies =

    N/A

Restrictions =

    N/A

Input =
    
    const Gesture& gesture - The leap object for gestures
    const Controller& controller  - The leap controller object that stores all the information
                                    about the leap motions.

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::swipeGesture ( const Gesture& gesture, const Controller& controller )
{
    // Switch the gesture input to swipe gesture object
    SwipeGesture swipe = gesture;

    // Debug info
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
    *        swipe you are increasing the x coordinate and the y should stay restively the same.
    */
    if ( abs ( swipe.direction ().x ) > abs ( swipe.direction ().y ) )
    {
        /*
        * Handle Right Swipe:
        *	   - Perform an action when user has performed a right swipe on leap.
        *      - The right swipe from the user is detected based on the x coordinates
        *        that are detected during the action, this is pulled from the SwipeGestures API
        *        using the direction function which provides data of the x, y, and z coordinates
        *        of a swipe direction.
        *      - Right swipe is performed in a horizontal manner therefore only the x coordinate
        *        would increase in a positive direction. (greater then 0)
        */
        if ( swipe.direction ().x > 0 )
        {
            runGestureAction ( SWIPE_RIGHT );
            createUserFeedBackWindow ( loadResource ( RES_SWIPERIGHT_IMAGE ), 150, 131 );
        }
        /*
        * Handle Left Swipe:
        *	   - Perform an action when user has performed a left swipe on leap.
        *      - The left swipe from the user is detected based on the x coordinates
        *        that are detected during the action, this is pulled from the SwipeGestures API
        *        using the direction function which provides data of the x, y, and z coordinates
        *        of a swipe direction.
        *      - Left swipe is performed in a horizontal manner therefore only the x coordinate
        *        would decrease in a negative direction. (less then 0)
        */
        else
        {
            runGestureAction ( SWIPE_LEFT );
            createUserFeedBackWindow ( loadResource ( RES_SWIPELEFT_IMAGE ), 150, 131 );
        }
    }
    /*
    * Handle Vertical Swipe detection:
    *	   - Vertical swipe is detected based on the x and y coordinate that are detected
    *        during the action. The direction function from SwipeGestures API is used to
    *        determine the x and y coordinates of the swipe.
    *      - Using the x and y coordinates we can determine the orientation of the swipe
    *      - When the y value of the swipe is greater then the x value of the swipe this
    *        show that the swipe is vertical. This is true because when you perform a vertical
    *        swipe you are increasing the y coordinate and the x should stay restively the same.
    */
    else
    {
        if ( swipe.direction ().y > 0 )
        {
            runGestureAction ( SWIPE_DOWN );
            createUserFeedBackWindow ( loadResource ( RES_SWIPEDOWN_IMAGE ), 150, 131 );
        }
        else
        {
            runGestureAction ( SWIPE_UP );
            createUserFeedBackWindow ( loadResource ( RES_SWIPEUP_IMAGE ), 150, 131 );
        }
    }
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::keyTapGesture

Descriptive Name = Used to perform the key tap action

Function =

    This function is used to perform the key tap actions.

Dependencies =

    N/A

Restrictions =

    N/A

Input =
    
    const Controller& controller  - The leap controller object that stores all the information
                                    about the leap motions.
    const Gesture& gesture - The leap object for gestures

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::keyTapGesture ( const Gesture& gesture, const Controller& controller )
{
    // Variable Declaration
    KeyTapGesture tap = gesture ;

    // Debug info
    std::cout << std::string ( 2, ' ' )
        << "Key Tap id: " << gesture.id ()
        << ", state: " << stateNames [gesture.state ()]
        << ", position: " << tap.position ()
        << ", direction: " << tap.direction ();

    // Perform key tap action
    runGestureAction ( KEY_TAP );
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::screenTapGesture

Descriptive Name = performs action when screen Tap gesture is detected

Function =

    This function is responsible of perform the action when the screen tap
    gesture is performed. The action is performed based on the 

Dependencies =

    N/A

Restrictions =

    N/A

Input =
    
    const Gesture& gesture - The leap object for gestures
    const Controller& controller  - The leap controller object that stores all the information
                                    about the leap motions.

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::screenTapGesture ( const Gesture& gesture, const Controller& controller )
{
    // Variable Declaration
    ScreenTapGesture screentap = gesture;

    // Debug info
    std::cout << std::string ( 2, ' ' )
        << "Screen Tap id: " << gesture.id ()
        << ", state: " << stateNames [gesture.state ()]
        << ", position: " << screentap.position ()
        << ", direction: " << screentap.direction ();

    // Perform the action for screen tap
    runGestureAction ( SCREEP_TAP );
}