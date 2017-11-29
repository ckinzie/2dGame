#include "player.h"
#include "gamedata.h"

Player::Player( const std::string& s) :
  MultiSprite2d(s + "R", s),
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

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    if(!east) {
      east = true;
      flipSprite();
    }
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    if(east) {
      east = false;
      flipSprite();
    }
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
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

void Player::shoot() {
  if (timeSinceLastBullet > bulletInterval) {
    Vector2f vel = getVelocity();
    float x = getPosition()[0];
    float y = getPosition()[1] + Gamedata::getInstance().getXmlInt(getName() +"/imageHeight")/2;
    int minBulletSpeed = Gamedata::getInstance().getXmlInt("Bullet/minSpeed");
    if (vel[0] > 0 || !facingRight) {
      x += Gamedata::getInstance().getXmlInt(getName() +"/imageWidth");
      vel[0] += minBulletSpeed;
    }
    else if (vel[0] < 0 || facingRight) {
      vel[0] -= minBulletSpeed;
    }
    bullets.shoot(Vector2f(x, y), Vector2f(vel[0], 0));
    timeSinceLastBullet = 0;
  }
}

void Player::draw() const{
  MultiSprite2d::draw();
  bullets.draw();
}

void Player::update(Uint32 ticks) {
  if ( !collision ) advanceFrame(ticks);
  timeSinceLastBullet += ticks;
  bullets.update(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if (getY() < 0)
    setVelocityY(std::abs(getVelocityY()));
  if (getY() > worldHeight-getScaledHeight())
    setVelocityY(-std::abs(getVelocityY()));
  if (getX() < 0)
    setVelocityX(std::abs(getVelocityX()));
  if (getY() > worldHeight-getScaledHeight())
    setVelocityX(-std::abs(getVelocityX()));

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }

  stop();
}

