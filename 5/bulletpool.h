#ifndef BULLETS_H
#define BULLETS_H

#include <list>
#include "bullet.h"

class CollisionStrategy;

class BulletPool {
public:
  BulletPool(const std::string&);
  BulletPool(const BulletPool&);
  BulletPool& operator=(const BulletPool&) = delete;
  ~BulletPool();
  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& velocity);
  int bulletCount() const { return bulletList.size(); }
  int freeCount() const { return freeList.size(); }
  bool shooting() const {return !bulletList.empty(); }
  bool collided(const Drawable*) const;
private:
  std::string name;
  Vector2f myVelocity;
  std::vector<Image*> bulletImages;
  std::list<Bullet> bulletList;
  std::list<Bullet> freeList;
  CollisionStrategy* strategy;
};
#endif
