#include "commonUtils.h"

using namespace ci;
using namespace ci::app;
using namespace std;

// The window-specific data for each window
class WindowData
{
    public:
        WindowData ( gl::Texture image )
            : windowBackgroundColor ( Color ( 0, 0, 0 ) ) // Default to white background
        {
            windowImageToLoad = image;
        }

        Color			windowBackgroundColor;
        gl::Texture     windowImageToLoad;
};

void LeapGestureFeedBack::setup ()
{
    // for the default window we need to provide an instance of WindowData
    createMainApplicationWindow ();
}

void LeapGestureFeedBack::draw ()
{
    gl::clear ( Color ( 255, 255, 255 ) );

    // Get the most recent frame and report some basic information
    const Leap::Frame frame = leap.frame ();

    // Sort through the hands and perform the necessary actions that are associated to the hand actions.
    determineHandAndPerformAction ( frame, leap );

    // Sort through the gestures and perform the necessary actions that are associated to the gestures.
    determineGestureAndPerformAction ( frame, leap );

    WindowData *data = getWindow ()->getUserData<WindowData> ();

    gl::color ( data->windowBackgroundColor );
    gl::draw ( data->windowImageToLoad, Vec2f ( 0, 0 ) );
    gl::enableAlphaBlending ();
    glColor4f ( 1.0f, 1.0f, 1.0f, 0.5f );
    glEnable ( GL_BLEND );
    gl::end ();
}

void LeapGestureFeedBack::createUserFeedBackWindow ( std::string userFeedBackImagePath, int windowWidth, int windowHeight )
{
    gl::Texture userFeedBackImage = gl::Texture ( loadImage ( userFeedBackImagePath ) );

    app::WindowRef newWindow = createWindow ( Window::Format ().size ( windowWidth, windowHeight ) );
    newWindow->setUserData ( new WindowData ( userFeedBackImage ) );
    newWindow->setBorderless ( true );

    // for demonstration purposes, we'll connect a lambda unique to this window which fires on close
    int uniqueId = getNumWindows ();
    newWindow->getSignalClose ().connect (
        [uniqueId, this]
    {
        this->console () << "You closed window #" << uniqueId << std::endl;
    }
    );
}

void LeapGestureFeedBack::createMainApplicationWindow ()
{
    app::WindowRef newWindow = createWindow ( Window::Format ().size ( 10, 10 ) );
    newWindow->setBorderless ( true );

    // for demonstration purposes, we'll connect a lambda unique to this window which fires on close
    int uniqueId = getNumWindows ();
    newWindow->getSignalClose ().connect (
        [uniqueId, this]
    {
        this->console () << "You closed window #" << uniqueId << std::endl;
    }
    );
}