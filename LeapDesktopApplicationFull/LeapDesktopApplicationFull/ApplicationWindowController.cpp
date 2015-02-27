/*******************************************************************************************
*
*  Source File Name = ApplicationWindowController.cpp
*
*  Descriptive Name = Functions used to control switching between windows and opening new
*                     windows.
*
*  List of Functions:
*
*     LeapGestureFeedBack::applicationWindowManipulation ( std::string windowOption )
*     LeapGestureFeedBack::keyboardManipulation ( std::string keyboardOption )
*
*  Dependencies: See function dependencies
*
*  Restrictions: See function Restrictions
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
*  25/02/2015  Commenting - Adding commenting for all the functions  Devan Shah 100428864
*                           
*******************************************************************************************/
#include "commonUtils.h"

using namespace std;

/**********************************************************************************

Function Name = LeapGestureFeedBack::applicationWindowManipulation

Descriptive Name = Switch between windows. Next and previous from main window.

Function =

    This function is used to switch between windows based on the function parameter.
    The windows are switched using the main window as the pivot point (next and previous)
    from this point.

Dependencies =

    N/A

Restrictions =

    Needs to have a main window active to allow to get the next and previous windows
    from the main starting point. Also need to make sure that there were actual next
    and previous windows to even switch to. Windows stack needs to have previous and
    next.

Input =

    string windowOption - Which direction to switch the window to, supports the following options:
                                APP_WINDOW_NEXT      - This option will set the active window as 
                                                       the next window in the stack from the main
                                APP_WINDOW_PREVIOUS  - This option will set the active window as 
                                                       the previous window in the stack from the main

Output =

    Currently there are no outputs from this functions as it only performs the action.
    Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =

    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::applicationWindowManipulation ( std::string windowOption )
{
    // Get the current foreground window and set the new window to NULL
    HWND currentActiveWindow = GetForegroundWindow ();
    HWND windowSelection = NULL;

    // Based on the function parameter get the next or previous window from the
    // main window
    if ( windowOption.compare ( APP_WINDOW_NEXT ) )
    {
        // Retrieve the next window from the current active window
        windowSelection = GetNextWindow ( currentActiveWindow, GW_HWNDNEXT );
    }
    else if ( windowOption.compare ( APP_WINDOW_PREVIOUS ) )
    {
        // Retrieve the previous window from the current active window
        windowSelection = GetWindow ( currentActiveWindow, GW_HWNDPREV );
    }

    // Only perform the window change if another window is available
    if ( windowSelection )
    {
        // Move the new selected window (next or previous) to foreground
        SetForegroundWindow ( windowSelection );

        // Set this new window as active
        SetActiveWindow ( windowSelection );

        // Set focus to this window
        SetFocus ( windowSelection );

        // Allow to changing the size of the window from the current size if needed.
        //ShowWindow ( currentActiveWindow, SW_MAXIMIZE );
    }
}

/**********************************************************************************

Function Name = LeapGestureFeedBack::keyboardManipulation

Descriptive Name = Switch between opening/showing the On Screen Keyboard or hiding
                   the On Screen Keyboard.

Function =

    This function is used to open/show/hide the window's On Screen Keyboard

Dependencies =

    N/A

Restrictions =

    Needs to have the window's On Screen Keyboard installed and available in the path.
    Calls osk to open the on screen keyboard and uses window class name OSKMainClass 
    to find out if it is hidden or already open.

Input =

    string keyboardOption - Specifies to open keyboard or hide, supports the following options:
                                SHOW_KEYBOARD  - This option will SHOW/OPEN the On Screen Keyboard
                                HIDE_KEYBOARD  - This option will hide the On Screen Keyboard

Output =

    Currently there are no outputs from this functions as it only performs the action.
    Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =

    N/A

Error Return =

    N/A

******************************************************************************/
void LeapGestureFeedBack::keyboardManipulation ( std::string keyboardOption )
{
    // Find the window that contains the windows On Screen Keyboard
    HWND onScreenKeyboard = FindWindow ( TEXT ( "OSKMainClass" ), NULL );

    // Based on the function parameter open/show/hide the On Screen Keyboard
    if ( keyboardOption.compare ( SHOW_KEYBOARD ) )
    {
        // In the case that the On Screen Keyboard is hidden simply show it
        // there is no need to open it again with the osk system call
        if ( onScreenKeyboard != NULL )
        {
            ShowWindow ( onScreenKeyboard, SW_SHOW );
        }
        else
        {
            // In the case that the On Screen Keyboard is not open run the 
            // system command to start up the On Screen Keyboard.
            system ( "osk" );
        }
    }
    else if ( keyboardOption.compare ( HIDE_KEYBOARD ) )
    {
        // If the On Screen Keyboard is active hide it
        if ( onScreenKeyboard != NULL )
        {
            // Hide the On Screen Keyboard
            ShowWindow ( onScreenKeyboard, SW_HIDE );
        }
    }
}