/*******************************************************************************************
*
*  Source File Name = ApplicationWindowController.cpp
*
*  Descriptive Name = Functions used to control volume for workstations
*
*  List of Functions:
*
*     LeapGestureFeedBack::applicationWindowManipulation ( std::string windowOption )
*     LeapGestureFeedBack::keyboardManipulation ( std::string keyboardOption )
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
*  07/10/2014  Initial drop - File contains application switching    Devan Shah 100428864
*                             functions.
*
*  12/10/2014  Adding functions - Added functions switch application Devan Shah 100428864
*
*  14/11/2014  Adding functions - Added functions enable keyboard    Devan Shah 100428864
*
*  02/12/2014  Commenting - Adding commenting for all the functions  Devan Shah 100428864
*                           and updating the change log to
*                           represent the change made to the file
*                           over the months
*
*******************************************************************************************/
#include "commonUtils.h"

using namespace std;

/**********************************************************************************

Function Name = LeapGestureFeedBack::applicationWindowManipulation

Descriptive Name = 

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

Function Name = LeapGestureFeedBack::keyboardManipulation

Descriptive Name = 

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
