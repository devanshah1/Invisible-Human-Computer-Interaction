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
*     LeapGestureFeedBack::moveMouse ( const Controller& controller, std::string mouseAction )
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
*  04/04/2015  Fixing - Mouse detection issues, revert changes form  Devan Shah 100428864
*                      SendInput to SetCursor
*
*  04/04/2015  Change - Change mouse diff from 10 to 7 and previous  Devan Shah 100428864
*                       frame checking from 10 to 5. 
* 
*  05/04/2015  Adding - Debug info and starting adding code for      Devan Shah 100428864
*                       using thumb and index fingers for mouse
*                       move and click.
* 
*  05/04/2015  Adding - Additional code for thumb and index finger   Devan Shah 100428864
*                       mouse move and click. 
*
*  06/04/2015  Commenting - Adding commenting for all the extra      Devan Shah 100428864
*                           code for thumb and index finger for
*                           mouse and click changes.
*
*  06/04/2015  Adding - Check for thumb and index finger shift       Devan Shah 100428864
*                       from current and previous frames and 
*                       determine which mouse action to perform.
*
*  06/04/2015  Changing - Increase mouse sensitivity and reduce      Devan Shah 100428864
*                         amount of mouse movement difference
*                         between frames. Comment out all uses
*                         of index finger calculations not needed
*                         at this point.
*
*  07/04/2015  Adding - New code to detect thumb to index finger     Devan Shah 100428864
*                       distance change.
*
*  07/04/2015  Adding - New code to improve checking for left click  Devan Shah 100428864
*                       better.
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
        const Hand hand = *singleHand ;

        // Based on the hand that was detected figure out which fingers are active and perform the necessary actions
        determineFingerAndPerformAction ( controller, hand ) ;
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
    // Vector Declaration
    Vector thumbFingerCurrent ; // Stores the current stabilized thumb position 
    Vector indexFingerCurrent ; // Stores the current stabilized index position 

    Vector thumbFingerPrevious ; // Stores the previous stabilized thumb position 
    Vector indexFingerPrevious ; // Stores the previous stabilized index position 

    // Get the current and previous 5th frame from the leap controller
    const Frame currentFrame = controller.frame () ;
    const Frame previousFrame = controller.frame ( 30 ) ;

    // Get the list of extended fingers that are visible for the hand on the frame
    const FingerList fingersExtendedCurrent = currentFrame.hand ( hand.id () ).fingers ().extended () ;
    const FingerList fingersExtendedPrevious = previousFrame.hand ( hand.id () ).fingers ().extended () ;

    // Determine which hand is detected by leap
    string handOrientation = hand.isLeft () ? "LeftHand" : "RightHand" ;

    // Get the right and left most fingers from the current frame
    const Finger rightMostFingerCurrent = fingersExtendedCurrent.rightmost () ;
    const Finger leftMostFingerCurrent = fingersExtendedCurrent.leftmost () ;

    // Get the right and left most fingers from the previous frame
    const Finger rightMostFingerPrevious = fingersExtendedPrevious.rightmost () ;
    const Finger leftMostFingerPrevious = fingersExtendedPrevious.leftmost () ;

    // Only perform a mouse action based on the fact that the index finger and the thumb are present in both current and previous
    // frames. 
    //    1. Check is to make sure that there are 2 extended fingers in both current and previous frames
    //    2. Check to make sure that both current and previous frames have a thumb and index finger
    // Note: At this point the assumption is that there was one hand detected and all theses checks are performed on the single
    //       hand.
    if ( fingersExtendedCurrent.count () == 2 && fingersExtendedPrevious.count () == 2 &&
         ( rightMostFingerCurrent.type () == Finger::TYPE_THUMB || rightMostFingerCurrent.type () == Finger::TYPE_INDEX ) &&
         ( leftMostFingerCurrent.type () == Finger::TYPE_THUMB || leftMostFingerCurrent.type () == Finger::TYPE_INDEX ) &&
         ( rightMostFingerPrevious.type () == Finger::TYPE_THUMB || rightMostFingerPrevious.type () == Finger::TYPE_INDEX ) &&
         ( leftMostFingerPrevious.type () == Finger::TYPE_THUMB || leftMostFingerPrevious.type () == Finger::TYPE_INDEX )
       )
    {
        // Debug info 
        this->console () << "Hand Orientation: " << handOrientation << "\n"
                         << "Current RightMost Finger Type: " << fingerNames [rightMostFingerCurrent.type ()] << "\n"
                         << "Current LeftMost Finger Type: " << fingerNames [leftMostFingerCurrent.type ()] << "\n"
                         << "Previous RightMost Finger Type: " << fingerNames [rightMostFingerPrevious.type ()] << "\n"
                         << "Previous LeftMost Finger Type: " << fingerNames [leftMostFingerPrevious.type ()] << "\n"
                         << endl;

        // Detect if the current hand is left and perform the action based on left hand detected 
        if ( handOrientation == "LeftHand" )
        {
            // Once again check to make sure that for a left hand we have thumb at the most right 
            // and index finger at the most left. This will only be the case for left hand.
            if ( rightMostFingerCurrent.type () == Finger::TYPE_THUMB &&
                 leftMostFingerCurrent.type () == Finger::TYPE_INDEX
               )
            {
                // Get the current frame stabilized tip position of the thumb and index finger
                thumbFingerCurrent = rightMostFingerCurrent.stabilizedTipPosition () ;
                indexFingerCurrent = leftMostFingerCurrent.stabilizedTipPosition () ;

                // Get the previous frame stabilized tip position of the thumb and index finger
                thumbFingerPrevious = rightMostFingerPrevious.stabilizedTipPosition () ;
                indexFingerPrevious = leftMostFingerPrevious.stabilizedTipPosition () ;
            }
        }
        // Detect if the current hand is right and perform the action based on right hand detected 
        else if ( handOrientation == "RightHand" )
        {
            // Once again check to make sure that for a right hand we have index finger at the most 
            // right and thumb finger at the most left. This will only be the case for right hand.
            if ( leftMostFingerCurrent.type () == Finger::TYPE_THUMB &&
                 rightMostFingerCurrent.type () == Finger::TYPE_INDEX
               )
            {
                // Get the current frame stabilized tip position of the thumb and index finger
                thumbFingerCurrent = leftMostFingerCurrent.stabilizedTipPosition () ;
                indexFingerCurrent = rightMostFingerCurrent.stabilizedTipPosition () ;

                // Get the previous frame stabilized tip position of the thumb and index finger
                thumbFingerPrevious = leftMostFingerPrevious.stabilizedTipPosition ();
                indexFingerPrevious = rightMostFingerPrevious.stabilizedTipPosition ();
            }
        }

        // Get the difference between thumb movement from current and previous frame
        int thumbXDifference = ( int ) abs ( thumbFingerPrevious.x - thumbFingerCurrent.x ) ;
        int thumbYDifference = ( int ) abs ( thumbFingerPrevious.y - thumbFingerCurrent.y );
        int thumbZDifference = ( int ) abs ( thumbFingerPrevious.z - thumbFingerCurrent.z );

        // Get the difference between index movement from current and previous frame
        int indexXDifference = ( int ) abs ( indexFingerPrevious.x - indexFingerCurrent.x );
        int indexYDifference = ( int ) abs ( indexFingerPrevious.y - indexFingerCurrent.y );
        int indexZDifference = ( int ) abs ( indexFingerPrevious.z - indexFingerCurrent.z );

        // Get the distance between the thumb vector and the index finger vector. This will in turn
        // Give the distance difference between the thumb and the index finger.
        float currentDistance = thumbFingerCurrent.distanceTo ( indexFingerCurrent ) ;
        float previousDistance = thumbFingerPrevious.distanceTo ( indexFingerPrevious ) ;

        // Find the distance difference between the thumb and the index finger to see of the thumb has
        // moved closer to the index finger. Thumb moving close to the index finger signifies a left click.
        int thumbToIndexFingerDistance = ( int ) abs ( previousDistance - currentDistance ) ;

        //// Debug info 
        //this->console () << "Hand Orientation: " << handOrientation << "\n"
        //    		     << "Current Thumb Vector: " << thumbFingerCurrent << "\n"
        //	    	     << "Current Index Vector: " << indexFingerCurrent << "\n"
        //		         << "Previous Thumb Vector: " << thumbFingerPrevious << "\n"
        //		         << "Previous Index Vector: " << indexFingerPrevious << "\n"
        //                 << "Difference Thumb Vector: " << Vector ( thumbXDifference, thumbYDifference, thumbZDifference ) << "\n"
        //                 << "Difference Index Vector: " << Vector ( indexXDifference, indexYDifference, indexZDifference ) << "\n"
        //                 << "Current Distance from Thumb to index Finger: " << currentDistance << "\n"
        //                 << "Previous Distance from Thumb to index Finger: " << previousDistance << "\n"
        //                 <<  "Difference from Thumb to index Finger: " << thumbToIndexFingerDistance << "\n"
        //		         << endl ;

        // Only move the mouse if both index finger and thumb are extended and also there is less then 3
        // difference in the x direction for the thumb from current and previous frame.
        if ( thumbXDifference < 3 && thumbToIndexFingerDistance < 3 )
        {
            moveMouse ( controller, MOVE_MOUSE );
        }
        // Only perform a left mouse click if both index finger and thumb are extended and also there is less then 3
        // is greater then 25 difference in the x direction for the thumb from current and previous frame. This would
        // signify that the thumb has moved close to the index finger, representing a left click by the user.
        else if ( thumbXDifference > 25 && thumbToIndexFingerDistance > 25 && !isMoseLeftClickEnabled )
        {
            moveMouse ( controller, MOUSE_LEFT_CLICK ) ;
            
            isMoseLeftClickEnabled = true ;

            // Debug info 
           this->console () << "Hand Orientation: " << handOrientation << "\n"
               		        << "Current Thumb Vector: " << thumbFingerCurrent << "\n"
        	                << "Current Index Vector: " << indexFingerCurrent << "\n"
        		            << "Previous Thumb Vector: " << thumbFingerPrevious << "\n"
        		            << "Previous Index Vector: " << indexFingerPrevious << "\n"
                            << "Difference Thumb Vector: " << Vector ( thumbXDifference, thumbYDifference, thumbZDifference ) << "\n"
                            << "Difference Index Vector: " << Vector ( indexXDifference, indexYDifference, indexZDifference ) << "\n"
                            << "Current Distance from Thumb to index Finger: " << currentDistance << "\n"
                            << "Previous Distance from Thumb to index Finger: " << previousDistance << "\n"
                            << "Difference from Thumb to index Finger: " << thumbToIndexFingerDistance << "\n"
        		            << endl ;
        }
        else if ( thumbXDifference > 25 && thumbToIndexFingerDistance > 25 && isMoseLeftClickEnabled )
        {
            
            isMoseLeftClickEnabled = false;

            // Debug info 
           this->console () << "Hand Orientation: " << handOrientation << "\n"
               		        << "Current Thumb Vector: " << thumbFingerCurrent << "\n"
        	                << "Current Index Vector: " << indexFingerCurrent << "\n"
        		            << "Previous Thumb Vector: " << thumbFingerPrevious << "\n"
        		            << "Previous Index Vector: " << indexFingerPrevious << "\n"
                            << "Difference Thumb Vector: " << Vector ( thumbXDifference, thumbYDifference, thumbZDifference ) << "\n"
                            << "Difference Index Vector: " << Vector ( indexXDifference, indexYDifference, indexZDifference ) << "\n"
                            << "Current Distance from Thumb to index Finger: " << currentDistance << "\n"
                            << "Previous Distance from Thumb to index Finger: " << previousDistance << "\n"
                            << "Difference from Thumb to index Finger: " << thumbToIndexFingerDistance << "\n"
                            << "Mouse Left Clicked: " << isMoseLeftClickEnabled << "\n"
        		            << endl ;
        }
        // TODO: Not Supported yet, need to figure out best way to perform a right click which this new algo
        else if ( fingersExtendedCurrent.count () == 3 )
        {
            //moveMouse ( controller, MOUSE_RIGHT_CLICK ) ;
        }
    }
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
    const Frame currentFrame = controller.frame () ;
    const Frame previousFrame = controller.frame ( 7 ) ;

    // Get maximum width and height of the screens
    int maxScreenWidth = GetSystemMetrics ( SM_CXSCREEN ) ;
    int maxScreenHeight = GetSystemMetrics ( SM_CYSCREEN ) ;

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
    normalizedPointCurrent *= 4.1 ; // scale
    normalizedPointCurrent -= Leap::Vector ( .25, .25, .25 ) ; // re-center

    // Increase the sensitivity of the mouse movement for previous frame.
    normalizedPointPrevious *= 4.1 ; // scale
    normalizedPointPrevious -= Leap::Vector ( .25, .25, .25 ) ; // re-center


    /*
    * The reason to calculate current and previous mouse locations is to make sure that
    * the mouse is not moved while performing a left click or right click. This help to
    * increase the accuracy of the mouse movement and accuracy.
    */
    int currentCorospondingMouseX = ( int ) ( normalizedPointCurrent.x * maxScreenWidth ) ;
    int currentCorospondingMouseY = ( int ) ( maxScreenHeight - ( normalizedPointCurrent.y * maxScreenHeight ) ) ;
    int previousCorospondingMouseX = ( int ) ( normalizedPointPrevious.x * maxScreenWidth ) ;
    int previousCorospondingMouseY = ( int ) ( maxScreenHeight - ( normalizedPointPrevious.y * maxScreenHeight ) ) ;

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

        // Only move the mouse to the current location if the difference is greater then 5 pixels
        if ( mouseYDifference > 5 || mouseXDifference > 5 )
        {
            SetCursorPos ( currentCorospondingMouseX, currentCorospondingMouseY ) ;
        }
    }
    else if ( mouseAction == MOUSE_LEFT_CLICK )
    {
        // Perform a left click where the mouse is currently located at.
        mouse_event ( MOUSEEVENTF_LEFTDOWN, 0, currentCorospondingMouseX, currentCorospondingMouseY, 0 ) ;

        // Calculate the difference between previous mouse location and current mouse location.
        int mouseXDifference = abs ( previousCorospondingMouseX - currentCorospondingMouseX );
        int mouseYDifference = abs ( previousCorospondingMouseY - currentCorospondingMouseY );

        // When a drag is detected while mouse is clicked then perform a dragging action.
        if ( mouseYDifference > 5 || mouseXDifference > 5 )
        {
            SetCursorPos ( currentCorospondingMouseX, currentCorospondingMouseY ) ;
        }

        // Release the left click
        mouse_event ( MOUSEEVENTF_LEFTUP, 0, currentCorospondingMouseX, currentCorospondingMouseY, 0 ) ;
    }
    else if ( mouseAction == MOUSE_RIGHT_CLICK )
    {
        // Perform a right click where the mouse is currently located at.
        mouse_event ( MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, currentCorospondingMouseX, currentCorospondingMouseY, 0, 0 ) ;
    }
}