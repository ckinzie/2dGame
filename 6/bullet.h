#ifndef BULLET_H
#define BULLET_H

#include "multisprite.h"

class Bullet : public MultiSprite {
public:
  Bullet (const std::string&, const Vector2f&, const Vector2f&);
  Bullet (const Bullet&);
  Bullet& operator=(const Bullet&) = delete;
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset();

private:
  float distance;
  float maxDistance;
  bool tooFar;
};
#endif
