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

using namespace std;

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
void LeapGestureFeedBack::applicationWindowManipulation ( std::string windowOption )
{

    HWND currentActiveWindow = GetForegroundWindow ();
    HWND windowSelection = NULL;

    if ( windowOption.compare ( APP_WINDOW_NEXT ) )
    {
        windowSelection = GetNextWindow ( currentActiveWindow, GW_HWNDNEXT );
    }
    else if ( windowOption.compare ( APP_WINDOW_PREVIOUS ) )
    {
        windowSelection = GetWindow ( currentActiveWindow, GW_HWNDPREV );
    }

    // Only perform the window change if another window is available
    if ( windowSelection )
    {
        // 
        SetForegroundWindow ( windowSelection );
        SetActiveWindow ( windowSelection );
        SetFocus ( windowSelection );
        //ShowWindow ( currentActiveWindow, SW_MAXIMIZE );
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
void LeapGestureFeedBack::keyboardManipulation ( std::string keyboardOption )
{
    // Find the window that contains the windows on screen keyboard
    HWND onScreenKeyboard = FindWindow ( TEXT ( "OSKMainClass" ), NULL );

    if ( keyboardOption.compare ( SHOW_KEYBOARD ) )
    {
        if ( onScreenKeyboard != NULL )
        {
            ShowWindow ( onScreenKeyboard, SW_SHOW );
        }
        else
        {
            system ( "osk" );
        }
    }
    else if ( keyboardOption.compare ( HIDE_KEYBOARD ) )
    {
        if ( onScreenKeyboard != NULL )
        {
            ShowWindow ( onScreenKeyboard, SW_HIDE );
        }
    }
}
