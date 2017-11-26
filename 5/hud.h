#ifndef HUD__H
#define HUD__H
#include <SDL.h>
#include <SDL_ttf.h>

class Hud{
public:
  Hud(SDL_Renderer*, int, int);
  void toggleHud();
  void draw() const;

private:
  SDL_Renderer* renderer;
  bool showHud = true;
  int width;
  int height;
};
#endif
