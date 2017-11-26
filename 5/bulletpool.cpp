#include <iostream>
#include <cmath>
#include "ioMod.h"
#include "bulletpool.h"
#include "imageFactory.h"
#include "collisionStrategy.h"
#include "gamedata.h"

BulletPool::~BulletPool() {};

BulletPool::BulletPool(const std::string& s) :
  name(s),
  myVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
             Gamedata::getInstance().getXmlInt(name+"/speedY")),
  bulletImages (ImageFactory::getInstance().getImages(name)),
  bulletList(),
  freeList(),
  strategy(NULL)
{
  strategy = new PerPixelCollisionStrategy;
}

BulletPool::BulletPool(const BulletPool& pool) :
  name(pool.name),
  myVelocity(pool.myVelocity),
  bulletImages(pool.bulletImages),
  bulletList(pool.bulletList),
  freeList(pool.freeList),
  strategy(pool.strategy)
{}

void BulletPool::shoot(const Vector2f& pos, const Vector2f& velocity) {
  if(freeList.empty()) {
    Bullet b(name, pos, velocity);
    bulletList.push_back(b);
  }
  else{
    Bullet b = freeList.front();
    freeList.pop_front();
    b.reset();
    b.setVelocity(velocity);
    b.setPosition(pos);
    bulletList.push_back(b);
  }
}

bool BulletPool::collided(const Drawable* obj) const {
  for(const auto& b : bulletList)
    if(strategy->execute(b, *obj))
      return true;
  return false;
}

void BulletPool::draw() const {
  for(const auto& b : bulletList)
    b.draw();
}

void BulletPool::update(Uint32 ticks) {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }
    else
      ptr++;
  }
}
