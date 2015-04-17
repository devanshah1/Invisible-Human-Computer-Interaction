#include "LeapKeyboardLetters.h"
#include "cinder/gl/gl.h"

#include <iostream>

using namespace std;
using namespace ci;

gl::TextureFontRef	WordNode::sFont;

WordNode::WordNode( const string &word )
	: mWord( word ), mShouldBeDeleted( false ), mIsSelected( false )
{
	mWordPixelLength	= sFont->measureString( mWord ).x;
}

bool WordNode::isPointInside( const Vec2f &pt ) const
{
	return pt.distance( mPos ) < mRadius;
}

void WordNode::setShouldBeDeleted()
{
	mShouldBeDeleted = true;
}

bool WordNode::shouldBeDeleted() const
{
	return mShouldBeDeleted;
}

void WordNode::setSelected()
{
	mIsSelected = true;
}

bool WordNode::isSelected() const
{
	return mIsSelected;
}

void WordNode::draw() const
{
    int windowHeight = 200;

	// draw shadows
	gl::color( ColorA( 0, 0, 0 ) );
	Vec2f p	= mPos();
	p += Vec2f( 1.0f, 1.0f );
	gl::drawSolidRect ( Rectf ( p.x - mRadius, 
                                ( p.y - mRadius ) - ( windowHeight - 15 ) / 2, 
                                p.x + mRadius, 
                                ( p.y + mRadius ) + ( windowHeight - 15 ) / 2 
                              ) 
                      );
	p += Vec2f( 1.0f, 1.0f );
	gl::drawSolidRect ( Rectf ( p.x - mRadius, 
                                ( p.y - mRadius ) - ( windowHeight - 15 ) / 2, 
                                p.x + mRadius, 
                                ( p.y + mRadius ) + ( windowHeight - 15 ) / 2 
                              ) 
                      );

	
	// draw color circle
    gl::color ( 0, 45, 255, 1.0f );
	p = mPos();
    gl::drawSolidRect ( Rectf ( p.x - mRadius, 
                                ( p.y - mRadius ) - ( windowHeight - 15 ) / 2, 
                                p.x + mRadius, 
                                ( p.y + mRadius ) + ( windowHeight - 15 ) / 2 
                              ) 
                      );

	
	// draw string
	// biggest square that can fit in the circle is radius * sqrt(2) per side  x^2 = (r^2)/2
	const float squareSide = sqrtf( ( mRadius * mRadius ) / 2.0f );
	float pixelScale = std::min( squareSide / mWordPixelLength, squareSide / 72 ) * 2.25f;
	gl::TextureFont::DrawOptions options = gl::TextureFont::DrawOptions().scale( pixelScale ).pixelSnap( false );

	const Vec2f offset = (Vec2f)mPos + Vec2f( -mRadius + ( mRadius * 2 - mWordPixelLength * pixelScale ) / 2, mRadius - (mRadius * 2.0f - 20 * pixelScale ) / 2 );

	gl::color( ColorA( Color::black(), mColor().a * 0.5f ) );
	sFont->drawString( mWord, offset + Vec2f( pixelScale, pixelScale ) * 1.5f, options );

	gl::color( ColorA( ( Color::white() ) * 0.65f, mColor().a ) );
	sFont->drawString( mWord, offset, options );
}

void WordNode::setFont( gl::TextureFontRef font )
{
	sFont = font;
}
