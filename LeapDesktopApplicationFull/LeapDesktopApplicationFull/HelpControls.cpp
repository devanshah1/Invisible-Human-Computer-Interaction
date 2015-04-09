/*******************************************************************************************
*
*  Source File Name = HelpControls.cpp
*
*  Descriptive Name = Functions used to provide help to the user. 
*
*  List of Functions:
*
*     LeapDesktopAppFull::supportedGestures ( std::string windowOption )
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
*  04/04/2014  Initial drop - File contains application help    Devan Shah 100428864
*                             functions.
*                           
*  07/04/2015  Updating - Changing LeapGestureFeedBack class and     Devan Shah 100428864
*                         file name to LeapDesktopAppFull
*
*******************************************************************************************/
#include "commonUtils.h"

using namespace std;

/**********************************************************************************

Function Name = LeapDesktopAppFull::supportedGestures

Descriptive Name = Used to open up the supported gesture window

Function =

    This function is used to provide help to the user, such as the supported gestures.

Output =

    Currently there are no outputs from this functions as it only performs the action.
    Future TODO is to make sure errors are handled and appropriate response is returned.

******************************************************************************/
void LeapDesktopAppFull::supportedGestures ()
{
    // Open up a window that displays the supported gestures
    createUserFeedBackWindow ( loadResource ( RES_SUPPORTEDGESTURES_IMAGE ), 729, 335 ) ;
}