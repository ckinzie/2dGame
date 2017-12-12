#include <SDL.h>
#include "hud.h"
#include "ioMod.h"
#include <sstream>

Hud::Hud(SDL_Renderer* r, int w, int h, Player* p) : renderer(r), player(p), width(w), height(h){}

void Hud::draw() const { 
  if(showHud) {
    SDL_Rect r = {5, 5, width, height};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 150 );
    SDL_RenderFillRect( renderer, &r );
    IOmod::getInstance().writeText("HUD:", 10, 10, {255,255,255,255});
    IOmod::getInstance().writeText("Use wasd to ", 10, 50, {255,255,255,255});
    IOmod::getInstance().writeText("move the goomba.", 10,70, {255,255,255,255});
    IOmod::getInstance().writeText("E explodes player.", 10,90, {255,255,255,255});
    std::ostringstream ss;
    ss << "Bullet list: " << player->bulletCount();
    IOmod::getInstance().writeText(ss.str(), 10,110, {255,255,255,255});
    std::ostringstream ss2;
    ss2 << "Free list: " << player->freeCount();
    IOmod::getInstance().writeText(ss2.str(), 10,130, {255,255,255,255});
  }
}

void Hud::toggleHud() {
  showHud = !showHud;
}
