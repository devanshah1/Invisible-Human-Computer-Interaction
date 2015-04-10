/*******************************************************************************************
*
*  Source File Name = common.cpp
*
*  Descriptive Name = Common functions
*
*  List of Functions:
*
*     LeapDesktopAppFull::defaultEnvironmentSetup ()
*     LeapDesktopAppFull::getEnvironmentVariables ()
*     LeapDesktopAppFull::runGestureAction ( std::string gestureAction )
*     LeapDesktopAppFull::executeAction ( LPTSTR executionAction )
*     LeapDesktopAppFull::freeEnvironmentBuffers ()
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
*  07/10/2014  Initial drop - File contains common functions          Devan Shah 100428864
*
*  12/10/2014  Adding functions - Added functions to read environment Devan Shah 100428864
*                                 variables
*
*  05/11/2014  Adding functions - Added functions to free up memory   Devan Shah 100428864
*                                 after reading the environment 
*                                 variable.
*
*  02/12/2014  Commenting - Adding commenting for all the functions  Devan Shah 100428864
*                           and updating the change log to
*                           represent the change made to the file
*                           over the months
*
*  04/04/2015  Changing - The action that SCREEP_TAP does and the    Devan Shah 100428864
*                         default action that is perform when 
*                         SCREEP_TAP is detected
*
*  04/05/2015 Commenting - Adding addition comments and fixing the   Devan Shah 100428864
*                          function descriptions
*
*  07/04/2015  Updating - Changing LeapGestureFeedBack class and     Devan Shah 100428864
*                         file name to LeapDesktopAppFull
*
*******************************************************************************************/
#include "commonUtils.h"

/**********************************************************************************

Function Name = LeapDesktopAppFull::defaultEnvironmentSetup

Descriptive Name = Used to Set up the environment variables

Function =

    This function is used to set up the default environment variables that are used
    to pick up the actions that gesture are suppose to do.

Input =
    
    N/A

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Return =
    
    N/A

******************************************************************************/
void LeapDesktopAppFull::defaultEnvironmentSetup ()
{
    // Pushing environment variables for testing purposes, The UI will do this in the future

    // Set actions for different types of circle gesture.
    SetEnvironmentVariable ( TEXT ( CIRCLE_COUNTERCLOKWISE ), TEXT ( VOLUME_STEP_DOWN ) ) ;
    SetEnvironmentVariable ( TEXT ( CIRCLE_CLOCKWISE ), TEXT ( VOLUME_STEP_UP ) ) ;

    // Set actions for different types of swipe actions.
    SetEnvironmentVariable ( TEXT ( SWIPE_LEFT ), TEXT ( SUPPORTED_GESTURES ) );
    SetEnvironmentVariable ( TEXT ( SWIPE_RIGHT ), TEXT ( LOCK_WORK_STATION ) );
    SetEnvironmentVariable ( TEXT ( SWIPE_UP ), TEXT ( HIDE_KEYBOARD ) ) ;
    SetEnvironmentVariable ( TEXT ( SWIPE_DOWN ), TEXT ( SHOW_KEYBOARD ) ) ;

    // Set actions for different types of tap actions.
    SetEnvironmentVariable ( TEXT ( KEY_TAP ), TEXT ( VOLUME_MUTE_UNMUTE ) ) ;
    SetEnvironmentVariable ( TEXT ( SCREEP_TAP ), TEXT ( OPEN_CALCULATOR ) ) ;
}

/**********************************************************************************

Function Name = LeapDesktopAppFull::getEnvironmentVariables

Descriptive Name = Fetch all the require environment variables

Function =

    This function is used to fetch all the environment variables for the different 
    gestures actions that are to be performed.

Input =
    
    N/A

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

******************************************************************************/
void LeapDesktopAppFull::getEnvironmentVariables ()
{
    // variable Declaration
    DWORD  dwRet = NULL ;

    // Allocate memory for environment variable retrieval
    CIRCLE_CLOCKWISE_SET = ( LPTSTR ) malloc ( BUFSIZE*sizeof ( TCHAR ) );
    CIRCLE_COUNTERCLOKWISE_SET = ( LPTSTR ) malloc ( BUFSIZE*sizeof ( TCHAR ) );
    SWIPE_LEFT_SET = ( LPTSTR ) malloc ( BUFSIZE*sizeof ( TCHAR ) );
    SWIPE_RIGHT_SET = ( LPTSTR ) malloc ( BUFSIZE*sizeof ( TCHAR ) );
    SWIPE_UP_SET = ( LPTSTR ) malloc ( BUFSIZE*sizeof ( TCHAR ) );
    SWIPE_DOWN_SET = ( LPTSTR ) malloc ( BUFSIZE*sizeof ( TCHAR ) );
    KEY_TAP_SET = ( LPTSTR ) malloc ( BUFSIZE*sizeof ( TCHAR ) );
    SCREEP_TAP_SET = ( LPTSTR ) malloc ( BUFSIZE*sizeof ( TCHAR ) );

    /**************************** GESTURE TYPE_CIRCLE START *****************************/

    // Get environment variable value for CIRCLE_CLOCKWISE
    dwRet = GetEnvironmentVariable ( TEXT ( CIRCLE_CLOCKWISE ), CIRCLE_CLOCKWISE_SET, BUFSIZE );

    // Get environment variable value for CIRCLE_COUNTERCLOKWISE
    dwRet = GetEnvironmentVariable ( TEXT ( CIRCLE_COUNTERCLOKWISE ), CIRCLE_COUNTERCLOKWISE_SET, BUFSIZE );

    /**************************** GESTURE TYPE_CIRCLE END *****************************/

    /**************************** GESTURE TYPE_SWIPE START *****************************/

    // Get environment variable value for SWIPE_LEFT
    dwRet = GetEnvironmentVariable ( TEXT ( SWIPE_LEFT ), SWIPE_LEFT_SET, BUFSIZE );

    // Get environment variable value for SWIPE_RIGHT
    dwRet = GetEnvironmentVariable ( TEXT ( SWIPE_RIGHT ), SWIPE_RIGHT_SET, BUFSIZE );

    // Get environment variable value for SWIPE_UP
    dwRet = GetEnvironmentVariable ( TEXT ( SWIPE_UP ), SWIPE_UP_SET, BUFSIZE );

    // Get environment variable value for SWIPE_DOWN
    dwRet = GetEnvironmentVariable ( TEXT ( SWIPE_DOWN ), SWIPE_DOWN_SET, BUFSIZE );

    /**************************** GESTURE TYPE_SWIPE END ******************************/

    /************************** GESTURE TYPE_KEY_TAP START ****************************/

    // Get environment variable value for KEY_TAP_SET
    dwRet = GetEnvironmentVariable ( TEXT ( KEY_TAP ), KEY_TAP_SET, BUFSIZE );

    /**************************** GESTURE TYPE_KEY_TAP END *****************************/

    /************************** GESTURE TYPE_SCREEN_TAP START ***************************/

    // Get environment variable value for SCREEP_TAP
    dwRet = GetEnvironmentVariable ( TEXT ( SCREEP_TAP ), SCREEP_TAP_SET, BUFSIZE );

    /************************** GESTURE TYPE_SCREEN_TAP END *****************************/
}

/**********************************************************************************

Function Name = void LeapDesktopAppFull::runGestureAction

Descriptive Name = Execute an action based on the gesture 

Function =

    This function is used to execute an action based on the environment variable 
    that is set for that gesture.

Input =
    
    std::string gestureAction - The gesture option that needs to execute the specific
                                action for.

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

******************************************************************************/
void LeapDesktopAppFull::runGestureAction ( std::string gestureAction )
{
    // Execute the action for a gesture based on the environment variable that was set
    if ( gestureAction == CIRCLE_CLOCKWISE )
    {
        executeAction ( CIRCLE_CLOCKWISE_SET );
    }
    else if ( gestureAction == CIRCLE_COUNTERCLOKWISE )
    {
        executeAction ( CIRCLE_COUNTERCLOKWISE_SET );
    }
    else if ( gestureAction == SWIPE_LEFT )
    {
        executeAction ( SWIPE_LEFT_SET );
    }
    else if ( gestureAction == SWIPE_RIGHT )
    {
        executeAction ( SWIPE_RIGHT_SET );
    }
    else if ( gestureAction == SWIPE_UP )
    {
        executeAction ( SWIPE_UP_SET );
    }
    else if ( gestureAction == SWIPE_DOWN )
    {
        executeAction ( SWIPE_DOWN_SET );
    }
    else if ( gestureAction == KEY_TAP )
    {
        executeAction ( KEY_TAP_SET );
    }
    else if ( gestureAction == SCREEP_TAP )
    {
        executeAction ( SCREEP_TAP_SET );
    }
}

/**********************************************************************************

Function Name = LeapDesktopAppFull::executeAction

Descriptive Name = Executes the action for gesture that was detected

Function =

    This function is used to execute the action based on which action was
    set for a specific gesture. Execute different action based on the action
    that is passed in.

Input =
    
    LPTSTR executionAction - Used to identify which action to perform

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

******************************************************************************/
void LeapDesktopAppFull::executeAction ( LPTSTR executionAction )
{
    // Perform the actions based on the execution Action that is passed in to the function
    if ( ( lstrcmpi ( TEXT ( VOLUME_STEP_UP ), executionAction ) ) == 0 && executionAction != NULL )
    {
        VOLUME_STEP_UP_ACTION;
    }
    else if ( ( lstrcmpi ( TEXT ( VOLUME_STEP_DOWN ), executionAction ) ) == 0 && executionAction != NULL )
    {
        VOLUME_STEP_DOWN_ACTION;
    }
    else if ( ( lstrcmpi ( TEXT ( APP_WINDOW_NEXT ), executionAction ) ) == 0 && executionAction != NULL )
    {
        APP_WINDOW_NEXT_ACTION;
    }
    else if ( ( lstrcmpi ( TEXT ( APP_WINDOW_PREVIOUS ), executionAction ) ) == 0 && executionAction != NULL )
    {
        APP_WINDOW_PREVIOUS_ACTION;
    }
    else if ( ( lstrcmpi ( TEXT ( LOCK_WORK_STATION ), executionAction ) ) == 0 && executionAction != NULL )
    {
        LOCK_WORK_STATION_ACTION;
    }
    else if ( ( lstrcmpi ( TEXT ( OPEN_CALCULATOR ), executionAction ) ) == 0 && executionAction != NULL )
    {
        OPEN_CALCULATOR_ACTION;
    }
    else if ( ( lstrcmpi ( TEXT ( VOLUME_MUTE_UNMUTE ), executionAction ) ) == 0 && executionAction != NULL )
    {
        VOLUME_MUTE_UNMUTE_ACTION;
    }
    else if ( ( lstrcmpi ( TEXT ( SHOW_KEYBOARD ), executionAction ) ) == 0 && executionAction != NULL )
    {
        SHOW_KEYBOARD_ACTION;
    }
    else if ( ( lstrcmpi ( TEXT ( HIDE_KEYBOARD ), executionAction ) ) == 0 && executionAction != NULL )
    {
        HIDE_KEYBOARD_ACTION;
    }
    else if ( ( lstrcmpi ( TEXT ( SUPPORTED_GESTURES ), executionAction ) ) == 0 && executionAction != NULL )
    {
        SUPPORTED_GESTURE_ACTION;
    }
}

/**********************************************************************************

Function Name = LeapDesktopAppFull::freeEnvironmentBuffers

Descriptive Name = Used to free the environment variable storage buffers

Function =

    This function is used to free up the memory allocation buffers for the environment
    variables that were used to get the environment variable for the gesture actions.

******************************************************************************/
void LeapDesktopAppFull::freeEnvironmentBuffers ()
{
    // Free the memory buffer for each of the variables where memory was allocated
    free ( CIRCLE_CLOCKWISE_SET );
    free ( CIRCLE_COUNTERCLOKWISE_SET );
    free ( SWIPE_LEFT_SET );
    free ( SWIPE_RIGHT_SET );
    free ( SWIPE_UP_SET );
    free ( SWIPE_DOWN_SET );
    free ( KEY_TAP_SET );
    free ( SCREEP_TAP_SET );
}
