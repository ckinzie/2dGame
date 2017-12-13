#include "multisprite2d.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"

void MultiSprite2d::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite2d::~MultiSprite2d( ) { if (explosion) delete explosion; }

MultiSprite2d::MultiSprite2d( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  imagesR( RenderContext::getInstance()->getImages(name + "R") ),
  imagesL( RenderContext::getInstance()->getImages(name + "L") ),
  imagesD( RenderContext::getInstance()->getImages(name + "D") ),
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  facingRight(true)
{ }

MultiSprite2d::MultiSprite2d(const MultiSprite2d& s) :
  Drawable(s), 
  images(s.images),
  imagesR(s.imagesR),
  imagesL(s.imagesL),
  imagesD(s.imagesD),
  explosion(s.explosion),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  facingRight(s.facingRight)
  { }

MultiSprite2d& MultiSprite2d::operator=(const MultiSprite2d& s) {
  Drawable::operator=(s);
  images = (s.images);
  imagesR = (s.imagesR);
  imagesL = (s.imagesL);
  imagesD = (s.imagesD);
  explosion = s.explosion;
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  facingRight = s.facingRight;
  return *this;
}

void MultiSprite2d::explode() {
  if ( !explosion ) {
    if (facingRight) {
      Sprite sprite(getName(), getPosition(), getVelocity(), imagesR[currentFrame]);
      explosion = new ExplodingSprite(sprite);
    }
    else {
      Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
      explosion = new ExplodingSprite(sprite);
    }
  }
}

void MultiSprite2d::draw() const { 
  if ( explosion ) explosion->draw();
  else {
    if (dir == 0)
      images[currentFrame]->draw(getX(), getY(), getScale());
    else if (dir == 1)
      imagesR[currentFrame]->draw(getX(), getY(), getScale());
    else if (dir == 3)
      imagesL[currentFrame]->draw(getX(), getY(), getScale());
    else
      imagesD[currentFrame]->draw(getX(), getY(), getScale());
  }
}

void MultiSprite2d::update(Uint32 ticks) { 
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }
  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
