// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

#include "assets/gunner_concept.hpp"
#include "assets/kywy_main_character.hpp"
#include "assets/platformer_concept.hpp"
#include "kywy/Events.hpp"

Kywy::Engine engine;

SpriteSheet mainCharacterSpriteSheet =
    ::SpriteSheet(kywy_main_character_sprite_sheet_data, Display::Bitmap::GRAYSCALE_4_BIT, 128 * 2, 128, 2);
const uint8_t *mainCharacterFrames[] = {};
Sprite mainCharacter(mainCharacterFrames, Display::Bitmap::GRAYSCALE_4_BIT, 2, 128, 128);

SpriteSheet gunnerConceptSpriteSheet =
    ::SpriteSheet(gunner_concept_sprite_sheet_data, Display::Bitmap::GRAYSCALE_4_BIT, 128 * 8, 128, 8);
const uint8_t *gunnerConceptFrames[] = {};
Sprite gunnerConcept(gunnerConceptFrames, Display::Bitmap::GRAYSCALE_4_BIT, 8, 128, 128);

SpriteSheet platformerConceptSpriteSheet =
    ::SpriteSheet(platformer_concept_sprite_sheet_data, Display::Bitmap::GRAYSCALE_4_BIT, 128 * 8, 128, 8);
const uint8_t *platformerConceptFrames[] = {};
Sprite platformerConcept(platformerConceptFrames, Display::Bitmap::GRAYSCALE_4_BIT, 8, 128, 128);

class Presenter : public Actor::Actor {
public:
  const char *getName() { return "presenter"; };

  void initialize() { currentSprite = &mainCharacter; }

  Sprite *currentSprite;

  void handle(::Actor::Message *message) {
    switch (message->signal) {
    case Kywy::Events::BUTTON_LEFT_RELEASED: {
      currentSprite->setVisible(false);
      currentSprite->render();

      if (currentSprite == &mainCharacter) {
        currentSprite = &gunnerConcept;
      } else if (currentSprite == &gunnerConcept) {
        currentSprite = &platformerConcept;
      } else if (currentSprite == &platformerConcept) {
        currentSprite = &mainCharacter;
      }

      currentSprite->setVisible(true);
      currentSprite->setFrame(0);
      currentSprite->render();

      engine.display.update();
      break;
    }
    case Kywy::Events::BUTTON_RIGHT_RELEASED: {
      currentSprite->advanceFrame(0, currentSprite->numFrames - 1, 1, FrameLoopMode::PERIODIC);
      currentSprite->render();
      engine.display.update();
      break;
    }
    }
  }
} presenter;

extern "C" void app_main(void) {
  engine.start();

  mainCharacterSpriteSheet.addFrames(0, 0, 128, 128, 3);
  mainCharacter.frames = mainCharacterSpriteSheet.frames;
  mainCharacter.setDisplay(&engine.display);
  mainCharacter.setVisible(true);
  mainCharacter.render();

  gunnerConceptSpriteSheet.addFrames(0, 0, 128, 128, 8);
  gunnerConcept.frames = gunnerConceptSpriteSheet.frames;
  gunnerConcept.setDisplay(&engine.display);

  platformerConceptSpriteSheet.addFrames(0, 0, 128, 128, 8);
  platformerConcept.frames = platformerConceptSpriteSheet.frames;
  platformerConcept.setDisplay(&engine.display);

  engine.display.update();

  presenter.subscribe(&engine.input);
  presenter.start();
}
