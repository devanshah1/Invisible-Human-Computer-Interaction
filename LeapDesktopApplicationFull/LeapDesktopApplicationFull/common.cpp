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
void LeapGestureFeedBack::defaultEnvironmentSetup ()
{
    // Pushing environment variables for testing purposes, The UI will do this in the future

    // Set actions for different types of circle gesture.
    SetEnvironmentVariable ( TEXT ( CIRCLE_COUNTERCLOKWISE ), TEXT ( VOLUME_STEP_DOWN ) );
    SetEnvironmentVariable ( TEXT ( CIRCLE_CLOCKWISE ), TEXT ( VOLUME_STEP_UP ) );

    // Set actions for different types of swipe actions.
    SetEnvironmentVariable ( TEXT ( SWIPE_LEFT ), TEXT ( APP_WINDOW_NEXT ) );
    SetEnvironmentVariable ( TEXT ( SWIPE_RIGHT ), TEXT ( APP_WINDOW_PREVIOUS ) );
    SetEnvironmentVariable ( TEXT ( SWIPE_UP ), TEXT ( SHOW_KEYBOARD ) );
    SetEnvironmentVariable ( TEXT ( SWIPE_DOWN ), TEXT ( HIDE_KEYBOARD ) );

    // Set actions for different types of tap actions.
    SetEnvironmentVariable ( TEXT ( KEY_TAP ), TEXT ( VOLUME_MUTE_UNMUTE ) );
    SetEnvironmentVariable ( TEXT ( SCREEP_TAP ), TEXT ( VOLUME_MUTE_UNMUTE ) );
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
void LeapGestureFeedBack::getEnvironmentVariables ()
{
    // 
    DWORD  dwRet = NULL;

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
void LeapGestureFeedBack::runGestureAction ( std::string gestureAction )
{
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
void LeapGestureFeedBack::executeAction ( LPTSTR executionAction )
{
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
void LeapGestureFeedBack::freeEnvironmentBuffers ()
{
    free ( CIRCLE_CLOCKWISE_SET );
    free ( CIRCLE_COUNTERCLOKWISE_SET );
    free ( SWIPE_LEFT_SET );
    free ( SWIPE_RIGHT_SET );
    free ( SWIPE_UP_SET );
    free ( SWIPE_DOWN_SET );
    free ( KEY_TAP_SET );
    free ( SCREEP_TAP_SET );
}
