#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "sound.h"


class CollisionStrategy;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
  Hud* hud;

  SDL_Renderer * const renderer;
  World background;
  Viewport& viewport;
  Player *player;
  CollisionStrategy *strategy;
  bool collision;

  std::vector<Sprite*> sprites;
  int currentSprite;

  bool makeVideo;
  SDLSound sound;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&);
  void printScales() const;
  void checkForCollisions();
};
