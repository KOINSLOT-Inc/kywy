// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

#include "assets/promo_keyframes.hpp"
#include "kywy/Events.hpp"
#include <cstdio>

Kywy::Engine engine;

SpriteSheet promoSpriteSheet =
    ::SpriteSheet(promo_keyframes_sprite_sheet_data, Display::Bitmap::GRAYSCALE_4_BIT, 128 * 6, 128 * 6, 32);
const uint8_t *promoFrames[] = {};
Sprite promo(promoFrames, Display::Bitmap::GRAYSCALE_4_BIT, 32, 128, 128);

class Presenter : public Actor::Actor {
public:
  const char *getName() { return "presenter"; };

  void initialize() { currentSprite = &promo; }

  Sprite *currentSprite;

  int framesHeld = 0;

  void handle(::Actor::Message *message) {
    switch (message->signal) {
    case Kywy::Events::TICK:
    case Kywy::Events::BUTTON_RIGHT_RELEASED: {
      framesHeld += 1;
      if (currentSprite->frame == 0 && framesHeld == 48) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 1 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 2 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 3 && framesHeld == 24) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 4 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 5 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 6 && framesHeld == 12) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 7 && framesHeld == 2) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 8 && framesHeld == 6) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 9 && framesHeld == 2) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 10 && framesHeld == 2) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 11 && framesHeld == 12) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 12 && framesHeld == 2) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 13 && framesHeld == 6) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 14 && framesHeld == 3) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 15 && framesHeld == 4) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 16 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 17 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 18 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 19 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 20 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 21 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 22 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 23 && framesHeld == 12) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 24 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 25 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 26 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 27 && framesHeld == 1) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 28 && framesHeld == 4) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 29 && framesHeld == 6) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 30 && framesHeld == 36) {
        currentSprite->setFrame(currentSprite->frame + 1);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      } else if (currentSprite->frame == 31 && framesHeld == 96) {
        currentSprite->setFrame(0);
        currentSprite->render();
        engine.display.update();
        framesHeld = 0;
      }
      break;
    }
    }
  }
} presenter;

extern "C" void app_main(void) {
  engine.start();
  engine.clock.setTickDuration(40);

  promoSpriteSheet.addFrames(0, 0, 128, 128, 32);
  promo.frames = promoSpriteSheet.frames;
  promo.setDisplay(&engine.display);
  promo.setVisible(true);
  promo.render();

  engine.display.update();

  presenter.subscribe(&engine.input);
  presenter.subscribe(&engine.clock);
  presenter.start();
}
