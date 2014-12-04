/*******************************************************************************************
*
*  Source File Name = VolumeController.cpp
*
*  Descriptive Name = Functions used to control volume for workstations
*
*  List of Functions/Classes:
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
void LeapGestureFeedBack::volumeManipulation ( string controlOption )
{
    // Variable Declaration
    HRESULT              volumeManipulationResults = NULL;  // Stores detailed information for the volume communication interface
    IMMDeviceEnumerator  *deviceEnumerator = NULL;  // Stores 
    IMMDevice            *defaultDevice = NULL;  //
    IAudioEndpointVolume *endpointVolume = NULL;  //
    BOOL                 pbMute = FALSE; //

    CoInitialize ( NULL );
    volumeManipulationResults = CoCreateInstance ( __uuidof( MMDeviceEnumerator ),
                                                   NULL,
                                                   CLSCTX_INPROC_SERVER,
                                                   __uuidof( IMMDeviceEnumerator ),
                                                   ( LPVOID * ) &deviceEnumerator
                                                   );

    volumeManipulationResults = deviceEnumerator->GetDefaultAudioEndpoint ( eRender, eConsole, &defaultDevice );
    deviceEnumerator->Release ();
    deviceEnumerator = NULL;

    volumeManipulationResults = defaultDevice->Activate ( __uuidof( IAudioEndpointVolume ), CLSCTX_INPROC_SERVER, NULL, ( LPVOID * ) &endpointVolume );
    defaultDevice->Release ();
    defaultDevice = NULL;

    endpointVolume->GetMute ( &pbMute );
    if ( ( pbMute && controlOption == VOLUME_MUTE_UNMUTE ) )
    {
        volumeManipulationResults = endpointVolume->SetMute ( 0, NULL );
        createUserFeedBackWindow ( "C:\\Users\\100428864\\Desktop\\FeedBackImages\\keyTap1.png", 150, 131 );
    }
    else if ( !pbMute && controlOption == VOLUME_MUTE_UNMUTE )
    {
        volumeManipulationResults = endpointVolume->SetMute ( 1, NULL );
        createUserFeedBackWindow ( "C:\\Users\\100428864\\Desktop\\FeedBackImages\\keyTap2.png", 150, 131 );
    }

    if ( controlOption == VOLUME_STEP_UP )
    {
        volumeManipulationResults = endpointVolume->VolumeStepUp ( NULL );
    }
    else if ( controlOption == VOLUME_STEP_DOWN )
    {
        volumeManipulationResults = endpointVolume->VolumeStepDown ( NULL );
    }

    endpointVolume->Release ();
    CoUninitialize ();
}