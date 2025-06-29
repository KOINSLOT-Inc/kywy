// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Scene.hpp"

void Scene::add(Actor::Actor *actor) {
  uint8_t i = 0;
  while (i < MAX_ACTORS && actors[i] != nullptr) {
    if (actors[i] == actor) {
      // TODO: error
      return;  // already subscribed
    }
    i++;
  }

  if (i == MAX_ACTORS) {
    // TODO: error
    return;  // subscriber limit reached
  }

  actors[i] = actor;
  if (active) {
    actors[i]->enable();
  } else {
    actors[i]->disable();
  }
}

void Scene::remove(Actor::Actor *actor) {
  uint8_t i = 0;
  bool removed = false;
  while (i < MAX_ACTORS && actors[i] != nullptr) {
    if (removed) {
      // shift remaining actors down so that loops don't have to iterate
      // over the entire list, just the beginning populated section
      actors[i - 1] = actors[i];
      actors[i] = nullptr;
    } else if (actors[i] == actor) {
      actors[i] = nullptr;
      removed = true;
    }
    i++;
  }
}

void Scene::enter() {
  active = true;

  uint8_t i = 0;
  while (i < MAX_ACTORS) {

    if (actors[i] == nullptr) {
      // reached end of actors list,
      // no need to iterate over rest of nullptrs in the array
      return;
    }

    actors[i]->enable();
    actors[i]->dispatch(&sceneEnterMessage);

    i++;
  }
}

void Scene::exit() {
  active = false;

  uint8_t i = 0;
  while (i < MAX_ACTORS) {

    if (actors[i] == nullptr) {
      // reached end of actors list,
      // no need to iterate over rest of nullptrs in the array
      return;
    }

    actors[i]->disable();
    actors[i]->dispatch(&sceneExitMessage);

    i++;
  }
}
