// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_SCENE
#define KYWY_LIB_SCENE 1

#include "Actor.hpp"
#include "Events.hpp"

const uint8_t MAX_ACTORS = 5;

class Scene {
private:
  Actor::Actor *actors[MAX_ACTORS] = {};
  bool active = false;

  ::Actor::Message sceneEnterMessage = ::Actor::Message(Kywy::Events::KywyEvents::SCENE_ENTER);
  ::Actor::Message sceneExitMessage = ::Actor::Message(Kywy::Events::KywyEvents::SCENE_EXIT);

public:
  Scene() {};
  ~Scene() {};

  void add(Actor::Actor *actor);
  void remove(Actor::Actor *actor);

  void enter();
  void exit();
};

#endif
