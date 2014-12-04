/*******************************************************************************************
*
*  Source File Name = LeapHandControls.cpp
*
*  Descriptive Name = Functions used to control volume for workstations
*
*  List of Functions/Classes:
*
*     LeapGestureFeedBack::determineHandAndPerformAction ( const Frame& frame, const Controller& controller )
*     LeapGestureFeedBack::determineFingerAndPerformAction ( const Controller& controller, const Hand& hand )
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
*  13/11/2014  Initial drop - File contains functions for hands      Devan Shah 100428864
*                             control.
*
*  11/11/2014  Adding functions - Added functions                    Devan Shah 100428864
*                                 determineHandAndPerformAction and
*                                 determineFingerAndPerformAction
*                                 for hand and finger detection.
*
*  11/11/2014  Adding Functions - Adding mouse function              Devan Shah 100428864
*
*  17/11/2014  Fixing Functions - Improving the detection for the    Devan Shah 100428864
*                                 mouse and fixing some bug fixes.
*
*  02/12/2014  Commenting - Adding commenting for all the functions  Devan Shah 100428864
*                           and updating the change log to
*                           represent the change made to the file
*                           over the months
*
*******************************************************************************************/
#include "commonUtils.h"

using namespace Leap;

/**********************************************************************************

Function Name = LeapGestureFeedBack::determineHandAndPerformAction

Descriptive Name = Detects hands that are found in the leap motion frames.

Function =

    This function is responsible for performing the actions that are outlined for
    for each of the hands. When a hand is detected it move towards the action of
    performing an action based on each of the fingers also. Note: Checks all hands
    that are found in the frame.

Dependencies =
    
    Depends on if a hand was detected in the leap motion listener.

Restrictions =

    N/A

Input =

    const Frame& frame            - The frame object that is detected by leap
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
void LeapGestureFeedBack::determineHandAndPerformAction ( const Frame& frame, const Controller& controller )
{
    // Get Hands
    const HandList hands = frame.hands ();

    // Detect all the hands that are present in the frame and perform the necessary actions
    for ( HandList::const_iterator singleHand = hands.begin (); singleHand != hands.end (); ++singleHand )
    {
        // Get the hand
        const Hand hand = *singleHand;

        /******************************************** DEBUG INFORMATION START ******************************************
        
        std::string handOrientation = hand.isLeft () ? "Left hand" : "Right hand";
        std::cout << std::string ( 2, ' ' ) << handOrientation << ", id: " << hand.id ()
            << ", palm position: " << hand.palmPosition () << std::endl;
        
        ********************************************* DEBUG INFORMATION STOP *******************************************/

        // Based on the hand that was detected figure out which fingers are active and perform the necessary actions
        determineFingerAndPerformAction ( controller, hand );
    }
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::determineFingerAndPerformAction

Descriptive Name = Determine the orientation of the fingers and performs the actions
                   based of this.

Function =

    This function is responsible for determine the finger orientation and performing
    the action that is outlined. Following is the supported action that can be performed
    based on the finger orientation:
        One finger extended    - Move the mouse based on the location of the finger in the leap
                                 orientation.
        Two fingers extended   - Perform a left click at the location where the 2 fingers are extended
                                 in the leap orientation.
        Three fingers extended - Perform a right click at the location where the 3 fingers are extended
                                 in the leap orientation.

Dependencies =

    Depends on if a hand was detected in the leap motion listener.

Restrictions =

    Can only perform 3 actions when fingers are detected on the leap motion listener.
        MOVE_MOUSE         - Performs the action of moving the mouse
        MOUSE_LEFT_CLICK   - Performs the action of doing a left click with the mouse
                             when the left click is continuously click and a dragging 
                             action is performed, this will simulate a dragging action
                             with the mouse. (i.e. moving folders, drawing in paint)
        MOUSE_RIGHT_CLICK  - Performs the action of doing a right click with the mouse

Input =

    const Controller& controller  - The leap controller object that stores all the information
                                    about the leap motions.
    const Hand& hand - The Hand object that was detected from Leap motions listeners

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::determineFingerAndPerformAction ( const Controller& controller, const Hand& hand )
{
    // Get the list of extended fingers that are visible for the hand on the frame
    const FingerList fingersExtended = hand.fingers ().extended ();

    // Only move the mouse when one single finger is extended.
    if ( fingersExtended.count () == 1 )
    {
        moveMouse ( controller, MOVE_MOUSE );
    }
    // Only perform a left click if 2 fingers are extended.
    else if ( fingersExtended.count () == 2 )
    {
        moveMouse ( controller, MOUSE_LEFT_CLICK );
    }
    // Only perform a right click if 3 fingers are extended.
    else if ( fingersExtended.count () == 3 )
    {
        moveMouse ( controller, MOUSE_RIGHT_CLICK );
    }

    /******************************************** DEBUG INFORMATION START ******************************************
    for ( FingerList::const_iterator fl = fingersExtended.begin (); fl != fingersExtended.end (); ++fl )
    {
        const Finger finger = *fl;
        std::cout << std::string ( 4, ' ' ) << fingerNames [finger.type ()]
            << " finger, id: " << finger.id ()
            << ", length: " << finger.length ()
            << "mm, width: " << finger.width () << std::endl;
    }
    ********************************************* DEBUG INFORMATION STOP *******************************************/
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::moveMouse

Descriptive Name = Moves the pointer from one position to another based on the leap
                   input points and calculated 2D rendering of the point on the 
                   screen.

Function =

    This function is responsible for perform the action of moving the pointer from 
    one point to another based on the leap data. The leap data is converted from 3D
    to 2D and the tip of the finger is used to identify the point where the pointer
    should be on the screen. Also supports perform an Left and Right click using the 
    pointer. Furthermore also supports dragging from one point to another using the left 
    click.

Dependencies =

    This function depends on the windows aspect of moving the mouse pointer. Makes use of
    windows function calls to achieve the task of moving and clicking of the pointer.

Restrictions =

    Must have a mouse pointer available to use this function

Input =
    
    const Controller& controller  - The leap controller object that stores all the information
                                    about the leap motions.
    std::string mouseAction - This specifies the mouse action to perform, following are the supported 
                              actions: 
                                MOVE_MOUSE         - Performs the action of moving the mouse
                                MOUSE_LEFT_CLICK   - Performs the action of doing a left click with the mouse
                                                     when the left click is continuously click and a dragging 
                                                     action is performed, this will simulate a dragging action
                                                     with the mouse. (i.e. moving folders, drawing in paint)
                                MOUSE_RIGHT_CLICK  - Performs the action of doing a right click with the mouse

Output =

    Currently there are no outputs from this functions as it only performs the action.
    Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =

    N/A

Error Return =

    N/A

Future Work
    
    Enhance the mouse detection and clicking algorithm to make sure that it does not 
    perform multiple clicks and move to fast.

******************************************************************************/
void LeapGestureFeedBack::moveMouse ( const Controller& controller, std::string mouseAction )
{
    // Get the current and previous 10th frame from the leap controller
    const Frame currentFrame = controller.frame ();
    const Frame previousFrame = controller.frame ( 10 );

    // Get maximum width and height of the screens
    int maxScreenWidth = GetSystemMetrics ( SM_CXSCREEN );
    int maxScreenHeight = GetSystemMetrics ( SM_CYSCREEN );

    /*
    * Get the current interactionBox object from the current frame object
    * to be used to map positions in the Leap Motion coordinate system to
    * 2D coordinate system used to determine the locations of the mouse moving action
    * moving action on the screen.
    */
    InteractionBox leapInteractionBoxCurrent = currentFrame.interactionBox ();
    Pointable pointableCurrent = currentFrame.pointables ().frontmost ();

    /*
    * Get the previous interactionBox object from the previous frame object
    * to be used to map positions in the Leap Motion coordinate system to
    * 2D coordinate system used to determine the locations of the mouse moving action
    * moving action on the screen.
    */
    InteractionBox leapInteractionBoxPrevious = previousFrame.interactionBox ();
    Pointable pointablePrevious = previousFrame.pointables ().frontmost ();

    /*
    * Retrieve the current stabilized leap point (2D orientation and smooth).
    * Using the stabilized leap points determine the normalized points with the
    * use of the Leap's interaction box.
    */
    Vector leapPointCurrent = pointableCurrent.stabilizedTipPosition ();
    Vector normalizedPointCurrent = leapInteractionBoxCurrent.normalizePoint ( leapPointCurrent, false );

    /*
    * Retrieve the previous stabilized leap point (2D orientation and smooth).
    * Using the stabilized leap points determine the normalized points with the
    * use of the Leap's interaction box.
    */
    Vector leapPointPrevious = pointablePrevious.stabilizedTipPosition ();
    Vector normalizedPointPrevious = leapInteractionBoxPrevious.normalizePoint ( leapPointPrevious, false );

    // Increase the sensitivity of the mouse movement for current frame.
    normalizedPointCurrent *= 1.5; //scale
    normalizedPointCurrent -= Leap::Vector ( .25, .25, .25 ); // re-center

    // Increase the sensitivity of the mouse movement for previous frame.
    normalizedPointPrevious *= 1.5; //scale
    normalizedPointPrevious -= Leap::Vector ( .25, .25, .25 ); // re-center


    /*
    * The reason to calculate current and previous mouse locations is to make sure that
    * the mouse is not moved while performing a left click or right click. This help to
    * increase the accuracy of the mouse movement and accuracy.
    */
    int currentCorospondingMouseX = ( int ) ( normalizedPointCurrent.x * maxScreenWidth );
    int currentCorospondingMouseY = ( int ) ( ( 1 - normalizedPointCurrent.y ) * maxScreenHeight );
    int previousCorospondingMouseX = ( int ) ( normalizedPointPrevious.x * maxScreenWidth );
    int previousCorospondingMouseY = ( int ) ( ( 1 - normalizedPointPrevious.y ) * maxScreenHeight );

    /********************************* TODO START *******************************
    
    Avoid too fast clicking and movement by checking the previous frames for specific
    actions.

    // Get Hands
    const HandList previousHands = controller.frame ( 1 ).hands ();
    FingerList fingersExtended;
    bool leftClicked = false;

    // Detect all the hands that are present in the frame
    for ( HandList::const_iterator singleHand = previousHands.begin (); singleHand != previousHands.end (); ++singleHand )
    {
        // Get the hand
        const Hand hand = *singleHand;

        fingersExtended = hand.fingers ().extended ();
    }
   
    *********************************** TODO END ********************************/

    // Move the mouse
    if ( mouseAction == MOVE_MOUSE )
    {
        // Calculate the difference between previous mouse location and current mouse location.
        int mouseXDifference = abs ( previousCorospondingMouseX - currentCorospondingMouseX );
        int mouseYDifference = abs ( previousCorospondingMouseY - currentCorospondingMouseY );

        // Only move the mouse to the current location if the difference is greater then 10 pixels
        if ( mouseYDifference > 1 || mouseXDifference > 1 )
        {
            // Set the new corresponding cursor position for the mouse
            SetCursorPos ( currentCorospondingMouseX, currentCorospondingMouseY );
        }
    }
    else if ( mouseAction == MOUSE_LEFT_CLICK )
    {
        // Perform a left click where the mouse is currently located at.
        mouse_event ( MOUSEEVENTF_LEFTDOWN, 0, currentCorospondingMouseX, currentCorospondingMouseY, 0 );

        // Calculate the difference between previous mouse location and current mouse location.
        int mouseXDifference = abs ( previousCorospondingMouseX - currentCorospondingMouseX );
        int mouseYDifference = abs ( previousCorospondingMouseY - currentCorospondingMouseY );

        // When a drag is detected while mouse is clicked then perform a dragging action.
        if ( mouseYDifference > 5 || mouseXDifference > 5 )
        {
            // Set the new corresponding cursor position for the mouse
            SetCursorPos ( currentCorospondingMouseX, currentCorospondingMouseY );
        }

        // Release the left click
        mouse_event ( MOUSEEVENTF_LEFTUP, 0, currentCorospondingMouseX, currentCorospondingMouseY, 0 );
    }
    else if ( mouseAction == MOUSE_RIGHT_CLICK )
    {
        // Perform a right click where the mouse is currently located at.
        mouse_event ( MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, currentCorospondingMouseX, currentCorospondingMouseY, 0, 0 );
    }
}