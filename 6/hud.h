#ifndef HUD__H
#define HUD__H
#include <SDL.h>
#include <SDL_ttf.h>
#include "player.h"

class Hud{
public:
  Hud(SDL_Renderer*, int, int, Player*);
  void toggleHud();
  void draw() const;

private:
  SDL_Renderer* renderer;
  Player* player;
  bool showHud = true;
  int width;
  int height;
};
#endif
