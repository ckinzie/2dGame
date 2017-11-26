#include <SDL.h>
#include "hud.h"
#include "ioMod.h"

Hud::Hud(SDL_Renderer* r, int w, int h) : renderer(r), width(w), height(h){}

void Hud::draw() const { 
  if(showHud) {
    SDL_Rect r = {5, 5, width, height};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 0, 0, 255, 150 );
    SDL_RenderFillRect( renderer, &r );
    IOmod::getInstance().writeText("HUD:", 10, 10, {255,255,255,255});
    IOmod::getInstance().writeText("Use wasd to ", 10, 50, {255,255,255,255});
    IOmod::getInstance().writeText("move the goomba.", 10,70, {255,255,255,255});
  }
}

void Hud::toggleHud() {
  showHud = !showHud;
}
