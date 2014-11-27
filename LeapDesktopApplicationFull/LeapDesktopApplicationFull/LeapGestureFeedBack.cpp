#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Leap.h"
#include "LeapMath.h"
#include "commonUtils.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LeapGestureFeedBack : public AppNative
{
    public:
        void setup ();
        void draw ();
        void prepareSettings ( Settings *settings );
        
        // Stores the maximum screen size
        int maxWindowWidth;
        int maxWindowHeight;

        // Setup the 
        Leap::Controller leap;
};

void LeapGestureFeedBack::prepareSettings ( Settings *settings )
{
    // Retrieve the maximum screen size for the computer that is running this application
    maxWindowWidth = GetSystemMetrics ( SM_CXSCREEN );
    maxWindowHeight = GetSystemMetrics ( SM_CYSCREEN );

    // Setting the window size, frame rate for processing and making the window borderless.
    settings->setWindowSize ( 150, 130 );
    settings->setFrameRate ( 100.0f );
    settings->setBorderless ( true );
}

void LeapGestureFeedBack::setup ()
{
    gl::enableAlphaBlending ();
}

void LeapGestureFeedBack::draw ()
{
    gl::clear ( Color ( 255, 255, 255 ) );
    Leap::PointableList pointables = leap.frame ().pointables ();
    Leap::InteractionBox iBox = leap.frame ().interactionBox ();

    // Get the most recent frame and report some basic information
    const Leap::Frame frame = leap.frame ();

    // Sort through the hands and perform the necessary actions that are associated to the hand actions.
    determineHandAndPerformAction ( frame, leap );

    // Sort through the gestures and perform the necessary actions that are associated to the gestures.
    determineGestureAndPerformAction ( frame, leap );

    //for ( int p = 0; p < pointables.count (); p++ )
    //{
    //    Leap::Pointable pointable = pointables [p];
    //    Leap::Vector normalizedPosition = iBox.normalizePoint ( pointable.stabilizedTipPosition () );
    //    float x = normalizedPosition.x * maxWindowWidth;
    //    float y = maxWindowHeight - normalizedPosition.y * maxWindowHeight;

    //    if ( pointable.touchDistance () > 0 && pointable.touchZone () != Leap::Pointable::Zone::ZONE_NONE )
    //    {
    //        gl::color ( 0, 1, 0, 1 - pointable.touchDistance () );
    //    }
    //    else if ( pointable.touchDistance () <= 0 )
    //    {
    //        gl::color ( 1, 0, 0, -pointable.touchDistance () );
    //    }
    //    else
    //    {
    //        gl::color ( 0, 0, 1, .05 );
    //    }

    //    gl::drawSolidCircle ( Vec2f ( x, y ), 40 );
    //}
}

CINDER_APP_NATIVE ( LeapGestureFeedBack, RendererGl )