/*******************************************************************************************
*
*  Source File Name = VolumeControllers.cpp
*
*  Descriptive Name = Functions used to control volume for windows workstations. 
*
*  List of Functions/Classes:
*
*     LeapDesktopAppFull::volumeManipulation ( string controlOption )
*
*  Dependencies: See function dependencies
*
*  Restrictions: 
*
*    Makes use of windows calls to increase/decrease volume and to mute/unmute 
*    the volume. Make sure that audio input is enabled so that volume actions 
*    can be performed.
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
*  10/10/2014  Initial drop - File contains functions for volume     Devan Shah 100428864
*                             control.
*
*  24/10/2014  Adding volume support - Added the ability to interact Devan Shah 100428864
*                                      with the volume devices on 
*                                      the machine to increase and
*                                      decrease the volume.
*
*  05/11/2014  Adding mute/unmute - Adding the ability to perform    Devan Shah 100428864
*                                   mute and unmute actions.
*
*  02/12/2014  Commenting - Adding commenting for all the functions  Devan Shah 100428864
*                           and updating the change log to 
*                           represent the change made to the file
*                           over the months
*
*  02/03/2015  Updating - Changing how the image are loaded, making  Devan Shah 100428864 
*                         use of the defined resources.
*
*  04/04/2015  Updating - Minor changes to formatting and comments   Devan Shah 100428864 
*
*  07/04/2015  Updating - Changing LeapGestureFeedBack class and     Devan Shah 100428864
*                         file name to LeapDesktopAppFull
*
*******************************************************************************************/
#include "commonUtils.h"

using namespace std;

/**********************************************************************************

Function Name = LeapDesktopAppFull::volumeManipulation

Descriptive Name = Increase/decrease/mute/unmute the volume

Function =

    This functions is responsible for performing the actions of increasing/
    decreasing/muting/unmuting the volume. Makes use of the available functions on 
    windows to construct and initialize the interface of audio devices for communication.

Dependencies =

    None

Restrictions =

    Must have audio devices connected, running and capable of being accessed through the 
    IMMDeviceEnumerator interface.

Input =
    
    string controlOption - The volume action to be performed, supports the following options:
                                VOLUME_STEP_UP      - This option will increase the volume by 1
                                VOLUME_STEP_DOWN    - This option will decrease the volume by 1
                                VOLUME_MUTE_UNMUTE  - This option will mute or unmute based on 
                                                      what the state of the volume currently is.

Output =
   
   Currently there are no outputs from this functions as it only performs the action. 
   Future TODO is to make sure errors are handled and appropriate response is returned.

Normal Return =
    
    N/A

Error Return =

    N/A

******************************************************************************/
void LeapDesktopAppFull::volumeManipulation ( string controlOption )
{
    // Variable Declaration
    HRESULT              volumeManipulationResults = NULL ;  // Stores detailed information for the volume communication interface
    IMMDeviceEnumerator  *deviceEnumerator         = NULL ;  // Stores the interface pointer of enumerating audio devices
    IMMDevice            *defaultDevice            = NULL ;  // Stores all the default audio devices resources
    IAudioEndpointVolume *endpointVolume           = NULL ;  // Stores the volume controls on the audio streams 
    BOOL                 pbMute                    = FALSE ; //

    // Initialize the COM library on the current thread
    CoInitialize ( NULL ) ;

    // Create an instance of a single uninitialized object of audio devices, a success instance creation will return "S_OK"
    volumeManipulationResults = CoCreateInstance ( __uuidof( MMDeviceEnumerator ),    // The CLSID associated with the data and code that is used to create the object
                                                   NULL,                              // Set to NULL to identify that object is not being created as an aggregate
                                                   CLSCTX_INPROC_SERVER,              // Identifies how the new object will run, in this case it is a DLL that runs in the same process
                                                   __uuidof( IMMDeviceEnumerator ),   // The identifier of the interface that will be used to communicate with the object
                                                   ( LPVOID * ) &deviceEnumerator     // Pointer that will store the interface pointer that was requested
                                                 ) ;

    // Using the device enumerator interface get the default audio endpoint for the provided data-flow direction
    volumeManipulationResults = deviceEnumerator->GetDefaultAudioEndpoint ( eRender,          // Set to eRender to represent an audio rendering stream
                                                                            eConsole,         // Set to eConsole to represent the role that is assigned to the endpoint device
                                                                            &defaultDevice    // Pointer to which the device address is stored of the endpoint object
                                                                          ) ;

    // No longer need the pointer for the device enumerator interface so release it to free up memory
    deviceEnumerator->Release () ;
    deviceEnumerator = NULL ;  // Force un-initialization

    // Activate the device that was retrieved above, a success activation will return "S_OK"
    volumeManipulationResults = defaultDevice->Activate ( __uuidof( IAudioEndpointVolume ), // The interface identifier, in this case this is an audio endpoint device
                                                          CLSCTX_INPROC_SERVER,             // Identifies how the new object will run, in this case it is a DLL that runs in the same process
                                                          NULL,                             // Set to NULL to activate an IAudioEndpointVolume interface on an audio endpoint device
                                                          ( LPVOID * ) &endpointVolume      // Pointer which stores the address of the IAudioEndpointVolume interface
                                                        ) ;

    // No longer need the pointer for the device interface so release it to free up memory
    defaultDevice->Release () ;
    defaultDevice = NULL ; // Force un-initialization

    // From the endpointVolume interface get the mute status
    // GetMute returns TRUE for muted and FALSE for the stream not muted
    endpointVolume->GetMute ( &pbMute ) ;

    // Based on the mute status perform the action of muting or unmuting the stream
    if ( ( pbMute && controlOption == VOLUME_MUTE_UNMUTE ) )
    {
        // Found that the stream is already currently muted so un-mute the stream
        volumeManipulationResults = endpointVolume->SetMute ( 0, NULL ) ;

        // Display user feed back image based on the action performed
        createUserFeedBackWindow ( loadResource ( RES_KEYTAP_IMAGE ), 150, 131 ) ;
    }
    else if ( !pbMute && controlOption == VOLUME_MUTE_UNMUTE )
    {
        // Found that the stream is not mute so mute the stream
        volumeManipulationResults = endpointVolume->SetMute ( 1, NULL ) ;

        // Display user feed back image based on the action performed
        createUserFeedBackWindow ( loadResource ( RES_KEYTAPRELEASE_IMAGE ), 150, 131 ) ;
    }

    // Based on the volume increase/decrease option, perform the appropriate action
    if ( controlOption == VOLUME_STEP_UP )
    {
        // Increase the volume on the stream by one, this also will update any connected system notifications
        volumeManipulationResults = endpointVolume->VolumeStepUp ( NULL );
        volumeManipulationResults = endpointVolume->VolumeStepUp ( NULL );
        volumeManipulationResults = endpointVolume->VolumeStepUp ( NULL );
        volumeManipulationResults = endpointVolume->VolumeStepUp ( NULL );
    }
    else if ( controlOption == VOLUME_STEP_DOWN )
    {
        // Decrease the volume on the stream by one, this also will update any connected system notifications
        volumeManipulationResults = endpointVolume->VolumeStepDown ( NULL );
        volumeManipulationResults = endpointVolume->VolumeStepDown ( NULL );
        volumeManipulationResults = endpointVolume->VolumeStepDown ( NULL );
        volumeManipulationResults = endpointVolume->VolumeStepDown ( NULL );
    }

    // No longer need the endpointVolume interface so release it to free up memory
    endpointVolume->Release ();

    // All the actions that are to be performed on the end point device are done so un-initialize the instance
    CoUninitialize ();
}