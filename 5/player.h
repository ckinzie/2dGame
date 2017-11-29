#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <list>
#include <cmath>
#include "multisprite2d.h"
#include "smartSprite.h"
#include "bulletpool.h"

class Player : public MultiSprite2d {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);
  void attach( SmartSprite* o ) { observers.push_back(o); } 
  void detach( SmartSprite* o );
  void shoot();
  const std::string getName() const{ return name; }

  int bulletCount() const { return bullets.bulletCount(); }
  int freeCount() const { return bullets.freeCount(); }

  void right();
  void left();
  void up();
  void down();
  void stop();
private:
  std::string name;
  bool collision;
  bool east = true;
  Vector2f initialVelocity;
  std::list<SmartSprite*> observers;
  BulletPool bullets;
  int timeSinceLastBullet;
  int bulletInterval;
  std::string bulletName;
};
#endif
