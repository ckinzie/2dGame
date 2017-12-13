#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "smartSprite.h"
#include "multisprite.h"
#include "multisprite2d.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "collisionStrategy.h"


Engine::~Engine() { 
  delete player;
  for (auto x:sprites)
    delete x;
  delete strategy;
  std::cout << "Terminating program" << std::endl;
  delete hud;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  background("background", Gamedata::getInstance().getXmlInt("background/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("Pirate")),
  strategy(),
  collision(false),
  currentSprite(0),
  makeVideo( false )
{
  hud = new Hud(renderer, Gamedata::getInstance().getXmlInt("HUD/width"),Gamedata::getInstance().getXmlInt("HUD/height"), player);
  int n = Gamedata::getInstance().getXmlInt("numberOfWasps");
  sprites.reserve(n);
  for (int i = 0; i < n; ++i) {
    sprites.push_back( new Sprite("Bee", true) );
    enemyCount++;
  }
  x = new Sprite("X");
  chest = new Sprite("Chest");
  shovel = new Sprite("Shovel");
  key = new Sprite("Key");
  gold = new Sprite("Gold");
  strategy = new PerPixelCollisionStrategy;
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  background.draw();

  switch(stage) {
    case 0:
      x->draw();
      IOmod::getInstance().writeText("QUEST: Find the RED X", 10, 440, {0,0,0,255});
      break;
    case 1:
      x->draw();
      shovel->draw();
      IOmod::getInstance().writeText("QUEST: Find the SHOVEL", 10, 440, {0,0,0,255});
      break;
    case 2:
      x->draw();
      IOmod::getInstance().writeText("QUEST: Dig at the RED X", 10, 440, {0,0,0,255});
      break;
    case 3:
      chest->draw();
      if(enemyCount == 0) {
        key->draw();
      }
      IOmod::getInstance().writeText("QUEST: One of the wasps must have taken the key.", 10, 420, {0,0,0,255});
      IOmod::getInstance().writeText("               Kill them all to find it!", 10, 440, {0,0,0,255});
      break;
    case 4:
      chest->draw();
      IOmod::getInstance().writeText("QUEST: Unlock the CHEST", 10, 440, {0,0,0,255});
      break;
    case 5:
      gold->draw();
      IOmod::getInstance().writeText("YOU WIN! Press R to restart", 230, 180, {255,255,0,255});
      clock.pause();
      break;
  }
  
  for (auto x : sprites)
    x->draw();
  if (player->dead) {
    IOmod::getInstance().writeText("Uh-Oh! You died. Press R to try again!", 230, 220, {0,0,0,255});
    clock.pause();
  }
  else
    player->draw();
  hud->draw();

  strategy->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  collision = false;
  for ( const Drawable* d : sprites ) {
    if ( strategy->execute(*player, *d) ) {
      collision = true;
    }
  }
  if ( collision ) {
    player->collided();
  }
  else {
    player->missed();
    collision = false;
  }

  switch(stage) {
    case 0:
      if ( strategy->execute(*player, *x))
        stage++;
      break;
    case 1:
      if ( strategy->execute(*player, *shovel))
        stage++;
      break;
    case 2:
      if ( strategy->execute(*player, *x))
        stage++;
      break;
    case 3:
      if ( strategy->execute(*player, *key) && enemyCount == 0)
        stage++;
      break;
    case 4:
      if ( strategy->execute(*player, *chest)) {
        gold->setPosition(player->getPosition());
        sound[1];
        stage++;
      }
      break;
  }

  for ( Sprite* s : sprites ) {
    if ( player->getBulletPool().collided(s) ) {
      enemyCount--;
      sound[0];
      if(enemyCount == 0)
        key->setPosition(s->getPosition());
      s->explode();
    }
  }
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  for (auto x : sprites)
    x->update(ticks);
  player->update(ticks);
  background.update();
  viewport.update(); // always update viewport last
}

bool Engine::play() {
  bool flag = true;
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {

      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_F1] ) {
          hud->toggleHud();
          flag = false;
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          sound.toggleMusic();
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        if ( keystate[SDL_SCANCODE_G] ) {
          player->toggleGod();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    if(clock.getSeconds() > 3 && flag) {
      hud->toggleHud();
		flag = false;
    }
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      if (keystate[SDL_SCANCODE_UP]) {
		   if (keystate[SDL_SCANCODE_RIGHT]) {
		     player->shoot(1,1);
		   }
		   else if (keystate[SDL_SCANCODE_LEFT]) {
		     player->shoot(-1,1);
		   }
         else
           player->shoot(0,1);
      }
      if (keystate[SDL_SCANCODE_RIGHT]) {
		   if (keystate[SDL_SCANCODE_UP]) {
		     player->shoot(1,1);
		   }
		   else if (keystate[SDL_SCANCODE_DOWN]) {
		     player->shoot(1,-1);
		   }
         else
           player->shoot(1,0);
      }
      if (keystate[SDL_SCANCODE_LEFT]) {
		   if (keystate[SDL_SCANCODE_UP]) {
		     player->shoot(-1,1);
		   }
		   else if (keystate[SDL_SCANCODE_DOWN]) {
		     player->shoot(-1,-1);
		   }
         else
           player->shoot(-1,0);
      }
      if (keystate[SDL_SCANCODE_DOWN]) {
         if (keystate[SDL_SCANCODE_RIGHT]) {
		     player->shoot(1,-1);
		   }
		   else if (keystate[SDL_SCANCODE_LEFT]) {
		     player->shoot(-1,-1);
		   }
         else
           player->shoot(0,-1);
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
