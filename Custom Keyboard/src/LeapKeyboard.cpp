#include <windows.h>
#include <windowsx.h>

#include <tchar.h>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

const TCHAR szAppName [] =_T ( "TransparentGL" );
const TCHAR wcWndName [] = _T ( "WS_EX_LAYERED OpenGL" );

#define IDT_WINDOWCLOSETIMER 999

#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include <list>
#include <algorithm>

#include "Resources.h"
#include "LeapKeyboardLetters.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LeapKeyboardApp : public AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void layoutWords( vector<string> words, float radius );	
	void setup();
	void initialize();
	void enableSelections() { mEnableSelections = true; }
	void mouseMove( MouseEvent event );	
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	float getLayoutRadius(){ return getWindowHeight() * 0.415f; }
    void GenerateKey ( int vk, BOOL bExtended );
	void selectNode( list<WordNode>::iterator selectedWord );

	void draw();

	list<WordNode>::iterator	getNodeAtPoint( const Vec2f &point );
	
	list<WordNode>				mNodes, mDyingNodes;
	list<WordNode>::iterator	mMouseOverNode;

	gl::Texture					mBgTex;
	gl::Texture					mCircleTex;
	gl::Texture					mSmallCircleTex;
	
	bool						mEnableSelections;
	WordNode					mCurrentNode;
	float						mCurrentCircleRadius;

    bool                        upperCase = false;
    bool                        numberPad = false;
};

void LeapKeyboardApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1550, 200 );
    settings->setBorderless ();
}

void LeapKeyboardApp::layoutWords( vector<string> words, float radius )
{
	mCurrentCircleRadius = 25;
    
    Vec2f positionCounter = Vec2f ( 0, 0 );

    // Loop through all the letters that are present in the list and perform the necessary 
    // actions on it.
	for ( size_t w = 0; w < words.size(); ++w ) 
    {
        // Set the new position of the letter
        Vec2f pos = getWindowCenter () - Vec2f ( getWindowWidth () / 2 - 20, 0 ) + radius * positionCounter;

        // Create the WordNode object and push it on to the mNodes list vector
		mNodes.push_back ( WordNode( words[w] ) );
        
        // Set the starting position of the letter, used to perform a transition to resting
        // position from this starting position.
        mNodes.back ().mPos = getWindowCenter () + radius * positionCounter;

        // Set the starting color of the letter, used to perform a color change from
        // gray (0,0,0) to black (-1,-1,-1).
        mNodes.back ().mColor = ColorA ( 0, 0, 0 );

        // Set the starting and ending radius for the square where the letter will reside.
        mNodes.back ().mRadiusDest = mCurrentCircleRadius;
		mNodes.back().mRadius = 0;
        
        // Used to set the distance between the letters, the factor of 0.647 is specifically 
        // determined based on visual aspects with distance between the letters.
        positionCounter = positionCounter + Vec2f ( 0.647, 0 ) ;

        // This timeline apply call is used to enlarge the square radius from starting to destination.
        // In this case the box that surrounds the letter expands from starting radius(0) to destination 
        // radius(25). This action is performed using a duration factor of 0.4, which is 0.4 times a sec.
		timeline ().apply ( &mNodes.back ().mRadius,   // Starting radius 
                            mNodes.back().mRadiusDest, // Target radius
                            0.4f,                      // Duration factor
                            EaseOutAtan( 10 )          // This is used to perform a decelerating from zero velocity of the action
                          ).timelineEnd( -0.39f );

        // This timeline apply call is used to move the letter (with the square) from starting position to ending position.
        // In this case the box that surrounds the letter and the letter moves from starting position (mPos) to ending position 
        // (pos). This action is performed using a duration factor of 0.4, which is 0.4 times a sec.
		timeline ().apply ( &mNodes.back ().mPos, // Starting position
                            pos,                  // Target position 
                            0.4f,                 // Duration factor
                            EaseOutAtan( 10 )     // This is used to perform a decelerating from zero velocity of the action
                          ).timelineEnd( -0.39f );

        // Applying the color to the node (letter) at a specific speed. This adds the color black to the letter
        timeline ().apply ( &mNodes.back ().mColor, // Starting color
                            ColorA ( -1, -1, -1 ),  // Target color
                            0.4f,                   // Duration factor
                            EaseOutAtan ( 10 )      // This is used to perform a decelerating from zero velocity of the action
                          ).timelineEnd ( -0.39f );
	}
}

void LeapKeyboardApp::setup()
{	
	// load textures
	mBgTex = gl::Texture( loadImage( loadResource( RES_BACKGROUND_IMAGE ) ) );
	gl::Texture::Format fmt;
	fmt.enableMipmapping();
	mCircleTex = gl::Texture( loadImage( loadResource( RES_CIRCLE_IMAGE ) ), fmt );
	mSmallCircleTex = gl::Texture( loadImage( loadResource( RES_SMALL_CIRCLE_IMAGE ) ), fmt );

	// give the WordNodes their font
	WordNode::setFont( gl::TextureFont::create( Font( loadResource( RES_FONT ), 120 ), gl::TextureFont::Format().enableMipmapping( true ) ) );
	
	initialize();
}

void LeapKeyboardApp::initialize()
{
	mNodes.clear();

	vector<string> initialWords;
    
    if ( upperCase )
    {
        // Add the necessary letters for the keyboard in the appropriate order 
        initialWords.push_back ( string ( 1, ( char ) ( '^' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'Q' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'A' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'Z' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'W' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'S' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'X' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'E' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'D' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'C' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'R' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'F' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'V' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'T' ) ) );

        initialWords.push_back ( string ( 1, ( char ) ( ' ' ) ) );

        initialWords.push_back ( string ( 1, ( char ) ( 'G' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'B' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'Y' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'H' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'N' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'U' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'J' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'M' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'I' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'K' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'O' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'L' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'P' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '#' ) ) );
    }
    else if ( numberPad )
    {
        initialWords.push_back ( string ( 1, ( char ) ( '^' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '1' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '2' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '3' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '4' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '5' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '6' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '7' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '8' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '9' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '0' ) ) );

        initialWords.push_back ( string ( 1, ( char ) ( ' ' ) ) );

        initialWords.push_back ( string ( 1, ( char ) ( '-' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '+' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '[' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( ']' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '\\' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( ';' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '\'' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( ',' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '.' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '/' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'A' ) ) );
    }
    else
    {
        // Add the necessary letters for the keyboard in the appropriate order 
        initialWords.push_back ( string ( 1, ( char ) ( '^' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'q' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'a' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'z' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'w' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 's' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'x' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'e' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'd' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'c' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'r' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'f' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'v' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 't' ) ) );

        initialWords.push_back ( string ( 1, ( char ) ( ' ' ) ) );

        initialWords.push_back ( string ( 1, ( char ) ( 'g' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'b' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'y' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'h' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'n' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'u' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'j' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'm' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'i' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'k' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'o' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'l' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( 'p' ) ) );
        initialWords.push_back ( string ( 1, ( char ) ( '#' ) ) );
    }

	layoutWords( initialWords, getLayoutRadius() );
	
	// mark our currently highlighted node as "none"
	mMouseOverNode = mNodes.end();
	
	mEnableSelections = true;
}

list<WordNode>::iterator LeapKeyboardApp::getNodeAtPoint( const Vec2f &point )
{
	for ( list<WordNode>::iterator nodeIt = mNodes.begin(); nodeIt != mNodes.end(); ++nodeIt ) 
    {
		if ( nodeIt->isPointInside( point ) )
			return nodeIt;
	}
	
	return mNodes.end();
}


void LeapKeyboardApp::keyDown( KeyEvent event )
{
	if( ! mEnableSelections )
		return;
	
	if( isalpha( event.getChar() ) ){
		// see if we can find a word that ends with this letter
		list<WordNode>::iterator foundWord = mNodes.end();
		for( foundWord = mNodes.begin(); foundWord != mNodes.end(); ++foundWord ) {
			if( foundWord->getWord()[foundWord->getWord().size()-1] == event.getChar() )
				break;
		}
		
		if( foundWord != mNodes.end() )
			selectNode( foundWord );
	} else {
		if( event.getCode() == KeyEvent::KEY_BACKSPACE ){
			initialize();
		}
	}
}

void LeapKeyboardApp::mouseMove ( MouseEvent event )
{
    if ( !mEnableSelections )
        return;

    list<WordNode>::iterator currentMouseOver = getNodeAtPoint ( event.getPos () );

    if ( currentMouseOver != mMouseOverNode )
    {
        mMouseOverNode = currentMouseOver;

        // make all the circles not moused-over normal size, and the mouse-over big
        for ( list<WordNode>::iterator nodeIt = mNodes.begin (); nodeIt != mNodes.end (); ++nodeIt )
        {
            if ( mMouseOverNode == nodeIt )
            {
                timeline ().apply ( &nodeIt->mRadius, 
                                    mCurrentCircleRadius * 2.35f, 
                                    0.25f, 
                                    EaseOutElastic ( 2.0f, 1.2f ) 
                                  );
            }
            else
            {
                timeline ().apply ( &nodeIt->mRadius, 
                                    mCurrentCircleRadius, 
                                    0.5f, 
                                    EaseOutAtan ( 10 ) 
                                  );
            }
        }
    }
}

void LeapKeyboardApp::mouseDown( MouseEvent event )
{
	list<WordNode>::iterator clickedNode = getNodeAtPoint( event.getPos() );
	if( clickedNode != mNodes.end() ){
		selectNode( clickedNode );
	} /*else {
		if( ( event.getPos() - getWindowCenter() ).length() < 180.0f )
			initialize();
	}*/
}

void LeapKeyboardApp::selectNode( list<WordNode>::iterator selectedNode )
{	
	mCurrentNode = *selectedNode;
    mCurrentNode.setSelected ();

	// mark our currently highlighted node as "none"
	mMouseOverNode = mNodes.end();
    
    if ( mCurrentNode.getWord () == "^" )
    {
       // When the shift key is pressed re-initialize the keyboard with upper case letters
       // also account for the case if shift is already pressed.
       if ( upperCase ) { upperCase = false; }
       else { upperCase = true; }

       initialize ();
    }
    else if ( mCurrentNode.getWord () == "#" )
    {
       // When the shift key is pressed re-initialize the keyboard with upper case letters
       // also account for the case if shift is already pressed.
       if ( numberPad ) { numberPad = false; }
       else { numberPad = true; }

       initialize ();
    }
    else
    {
        HWND currentActiveWindow = GetForegroundWindow ();
        HWND windowSelection = NULL;

        windowSelection = GetNextWindow ( currentActiveWindow, GW_HWNDNEXT );

        // Only perform the window change if another window is available
        if ( windowSelection )
        {
            // 
            //SetForegroundWindow ( windowSelection );
            //SetActiveWindow ( windowSelection );
            //SetFocus ( windowSelection );
            //EnableWindow ( windowSelection, TRUE );
            //ShowWindow ( currentActiveWindow, SW_RESTORE );
        }

        //app::WindowRef activeWindow = getWindow ();
        //activeWindow->setFullScreen ();

        //app::WindowRef foregroundWin = getForegroundWindow ();
        //foregroundWin->setFullScreen ();

        //GenerateKey ( VK_CAPITAL, TRUE );
        //GenerateKey ( 'I', FALSE );
        //GenerateKey ( ' ', FALSE );

        //GenerateKey ( VK_CAPITAL, TRUE );
        //GenerateKey ( 'A', FALSE );
        //GenerateKey ( 'M', FALSE );
        //GenerateKey ( ' ', FALSE );

        //GenerateKey ( 'C', FALSE );
        //GenerateKey ( 'O', FALSE );
        //GenerateKey ( 'O', FALSE );
        //GenerateKey ( 'L', FALSE );
        //GenerateKey ( 'E', FALSE );
        //GenerateKey ( 'R', FALSE );
        //GenerateKey ( ' ', FALSE );

        //GenerateKey ( 'T', FALSE );
        //GenerateKey ( 'H', FALSE );
        //GenerateKey ( 'A', FALSE );
        //GenerateKey ( 'N', FALSE );
        //GenerateKey ( ' ', FALSE );

        //GenerateKey ( 'Y', FALSE );
        //GenerateKey ( 'O', FALSE );
        //GenerateKey ( 'U', FALSE );
        //GenerateKey ( ' ', FALSE );

        //GenerateKey ( 'W', FALSE );
        //GenerateKey ( 'I', FALSE );
        //GenerateKey ( 'L', FALSE );
        //GenerateKey ( 'L', FALSE );
        //GenerateKey ( ' ', FALSE );

        //GenerateKey ( 'E', FALSE );
        //GenerateKey ( 'V', FALSE );
        //GenerateKey ( 'E', FALSE );
        //GenerateKey ( 'R', FALSE );
        //GenerateKey ( ' ', FALSE );

        //GenerateKey ( 'B', FALSE );
        //GenerateKey ( 'E', FALSE );
        //GenerateKey ( 'n', FALSE );
        //GenerateKey ( ' ', FALSE );

        //GenerateKey ( 0x3A, FALSE ); /* period key */
        //GenerateKey ( 0x0D, FALSE ); /* enter key */
    }

    // once everything is done animating, then we can allow selections, but for now, disable them
    mEnableSelections = true;
    std::function<void ()> cueAction = bind ( &LeapKeyboardApp::enableSelections, this );
    timeline ().add ( cueAction, timeline ().getEndTime () - 2.0f );
}

void LeapKeyboardApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	gl::enableAlphaBlending();
	
	// draw background image
	gl::color( Color::white() );
	mBgTex.enableAndBind();
	gl::drawSolidRect( getWindowBounds() );
	
	
	mCircleTex.bind();
	
	// draw the dying nodes
	mSmallCircleTex.bind();
	for( list<WordNode>::const_iterator nodeIt = mDyingNodes.begin(); nodeIt != mDyingNodes.end(); ++nodeIt )
		nodeIt->draw();
	
	// draw all the non-mouseOver nodes
	for( list<WordNode>::const_iterator nodeIt = mNodes.begin(); nodeIt != mNodes.end(); ++nodeIt ){
		if( nodeIt != mMouseOverNode )
			nodeIt->draw();
	}
	
	// if there is a mouseOverNode, draw it last so it is 'above' the non-mouseOver nodes
	if( mMouseOverNode != mNodes.end() )
		mMouseOverNode->draw();
	
	// if there is a currentNode (previously selected), draw it
	if( ! mCurrentNode.getWord().empty() )
		mCurrentNode.draw();
	
	mSmallCircleTex.disable();
}

//LRESULT CALLBACK WindowFunc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
//{
//    PAINTSTRUCT ps;
//
//    switch ( msg )
//    {
//        case WM_ERASEBKGND:
//            return 0;
//            break;
//
//        case WM_CREATE:
//            break;
//
//        case WM_DESTROY:
//            break;
//
//        case WM_PAINT:
//            break;
//
//        case WM_SIZE:
//            break;
//
//        case WM_TIMER:
//
//            switch ( wParam )
//            {
//                case IDT_WINDOWCLOSETIMER:
//                    DestroyWindow ( hWnd );
//
//                    return 0;
//            }
//
//        default:
//            return DefWindowProc ( hWnd, msg, wParam, lParam );
//    }
//
//    return 0;
//}
//
//int WINAPI WinMain ( HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR str, int nWinMode )
//{
//    WNDCLASSEX wc;
//    memset ( &wc, 0, sizeof ( wc ) );
//    wc.cbSize = sizeof ( WNDCLASSEX );
//    wc.hIconSm = LoadIcon ( NULL, IDI_APPLICATION );
//    wc.style = CS_HREDRAW | CS_VREDRAW;
//    wc.lpfnWndProc = ( WNDPROC ) WindowFunc;
//    wc.cbClsExtra = 0;
//    wc.cbWndExtra = 0;
//    wc.hInstance = hThisInst;
//    wc.hIcon = LoadIcon ( NULL, IDI_APPLICATION );
//    wc.hCursor = LoadCursor ( NULL, IDC_ARROW );
//    wc.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW );
//    wc.lpszClassName = szAppName;
//
//    if ( !RegisterClassEx ( &wc ) )
//    {
//        MessageBox ( NULL, _T ( "RegisterClassEx - failed" ), _T ( "Error" ), MB_OK | MB_ICONERROR );
//        return FALSE;
//    }
//
//    HWND hWnd = CreateWindowEx ( WS_EX_TOPMOST, szAppName, wcWndName,
//                                 WS_VISIBLE | WS_POPUP, 200, 150, 800, 800,
//                                 NULL, NULL, hThisInst, NULL );
//    if ( !hWnd )
//    {
//        MessageBox ( NULL, _T ( "CreateWindowEx - failed" ), _T ( "Error" ), MB_OK | MB_ICONERROR );
//        return FALSE;
//    }
//
//    SetTimer ( hWnd, IDT_WINDOWCLOSETIMER, 10000, ( TIMERPROC ) NULL );
//
//    MSG msg;
//    while ( 1 )
//    {
//        while ( PeekMessage ( &msg, NULL, 0, 0, PM_NOREMOVE ) )
//        {
//            if ( GetMessage ( &msg, NULL, 0, 0 ) )
//            {
//                TranslateMessage ( &msg );
//                DispatchMessage ( &msg );
//            }
//            else return 0;
//        }
//    }
//
//    return ( FALSE );
//}

/* This is a function to simplify usage of sending keys */
void LeapKeyboardApp::GenerateKey ( int vk, BOOL bExtended )
{

    KEYBDINPUT  kb = {0};
    INPUT       Input = {0};

    /* Generate a "key down" */
    if ( bExtended )
    {
        kb.dwFlags = KEYEVENTF_EXTENDEDKEY;
    }
    kb.wVk = vk;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    SendInput ( 1, &Input, sizeof ( Input ) );

    /* Generate a "key up" */
    ZeroMemory ( &kb, sizeof ( KEYBDINPUT ) );
    ZeroMemory ( &Input, sizeof ( INPUT ) );
    kb.dwFlags = KEYEVENTF_KEYUP;
    if ( bExtended )
    {
        kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    }
    kb.wVk = vk;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    SendInput ( 1, &Input, sizeof ( Input ) );

    return;
}

CINDER_APP_BASIC( LeapKeyboardApp, RendererGl )
