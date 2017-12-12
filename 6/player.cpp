#include "player.h"
#include "gamedata.h"

Player::Player( const std::string& s) :
  MultiSprite2d(s),
  name(s),
  collision(false),
  initialVelocity(getVelocity()),
  observers(),
  bullets("Bullet"),
  bulletInterval(Gamedata::getInstance().getXmlInt("Bullet/bulletInterval"))
   
{ }

Player::Player(const Player& s) :
  MultiSprite2d(s),
  name(s.getName()),
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  observers(s.observers),
  bullets(s.bullets),
  bulletInterval(s.bulletInterval)
{ }

Player& Player::operator=(const Player& s) {
  MultiSprite2d::operator=(s);
  name = s.getName();
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  return *this;
}

void Player::stop() { 
  setVelocity(Vector2f(0,0));
}

void Player::toggleGod() { 
  godMode = !godMode;
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    direction(1);
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    direction(3);
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
    direction(0);
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
    direction(2);
  }
}

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::shoot(int a, int b) {
  if (timeSinceLastBullet > bulletInterval) {
    Vector2f vel {0,0};
    float x = getPosition()[0];
    float y = getPosition()[1];
    int minBulletSpeed = Gamedata::getInstance().getXmlInt("Bullet/minSpeed");
    if (a > 0) {
      x += Gamedata::getInstance().getXmlInt(getName() +"/imageWidth");
      vel[0] = minBulletSpeed;
    }
    else if (a < 0) {
      vel[0] = -minBulletSpeed;
    }
    else
      x += Gamedata::getInstance().getXmlInt(getName() +"/imageWidth")/2;

    if (b > 0) {
      vel[1] = -minBulletSpeed;
    }
    else if (b < 0) {
      y += Gamedata::getInstance().getXmlInt(getName() +"/imageHeight");
      vel[1] = minBulletSpeed;
    }
    else
      y += Gamedata::getInstance().getXmlInt(getName() +"/imageHeight")/2;
	
    bullets.shoot(Vector2f(x, y), Vector2f(vel[0], vel[1]));
    timeSinceLastBullet = 0;
  }
}

void Player::draw() const{
  MultiSprite2d::draw();
  bullets.draw();
}

void Player::update(Uint32 ticks) {
  if ( !collision )
    advanceFrame(ticks);
  else
    if(!godMode)
      MultiSprite2d::explode();
  timeSinceLastBullet += ticks;
  bullets.update(ticks);
  MultiSprite2d::update(ticks);

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }

  stop();
}
