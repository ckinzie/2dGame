#ifndef MULTISPRITE2D__H
#define MULTISPRITE2D__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class MultiSprite2d : public Drawable {
public:
  MultiSprite2d(const std::string&);
  MultiSprite2d(const MultiSprite2d&);
  ~MultiSprite2d();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }
  void direction(int d) {
    dir = d; }

  virtual void explode();

  bool dead = false;

protected:
  std::vector<Image *> images;
  std::vector<Image *> imagesR;
  std::vector<Image *> imagesL;
  std::vector<Image *> imagesD;
  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  bool facingRight;
  int dir = 2;

  void advanceFrame(Uint32 ticks);
  MultiSprite2d& operator=(const MultiSprite2d&);
};
#endif
