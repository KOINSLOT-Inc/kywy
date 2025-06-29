// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Actor.hpp"

namespace Actor {

rtos::Mutex handlerMutex;

void queueEventCallback(Actor *actor, Message *message) {
  handlerMutex.lock();
  switch (message->directive) {
    case DIRECTIVE_HANDLE:
      actor->handle(message);
      break;
    case DIRECTIVE_EXIT:
      actor->teardown();
      return;
    default:
      break;
  }
  handlerMutex.unlock();
};

void Actor::initialize(){};
void Actor::teardown(){};

void Actor::enable() {
  enabled = true;
};
void Actor::disable() {
  enabled = false;
};

Actor::Actor() {
  this->event_handler = new events::Event<void(Actor *, Message *)>(&this->queue, queueEventCallback);
}

Actor::~Actor() {
  delete this->event_handler;
}

void Actor::start() {
  initialize();
  thread.start(mbed::callback(&queue, &events::EventQueue::dispatch_forever));
}

void Actor::dispatch(Message *message) {
  // user cannot dispatch special system directives
  message->directive = DIRECTIVE_HANDLE;
  this->event_handler->post(this, message);
}

void Actor::stop() {
  Message message;
  message.directive = DIRECTIVE_EXIT;
  this->event_handler->post(this, &message);
}

void Actor::addSubscriber(Actor *actor) {
  if (actor == this) {
    // TODO: error
    return;  // cannot subscribe to self
  }

  uint8_t i = 0;
  while (i < MAX_SUBSCRIBERS && subscribers[i] != nullptr) {
    if (subscribers[i] == actor) {
      // TODO: error
      return;  // already subscribed
    }
    i++;
  }

  if (i == MAX_SUBSCRIBERS) {
    // TODO: error
    return;  // subscriber limit reached
  }

  subscribers[i] = actor;
}

void Actor::subscribe(Actor *actor) {
  actor->addSubscriber(this);
}

void Actor::removeSubscriber(Actor *actor) {
  if (actor == this) {
    // TODO: error
    return;  // cannot unsubscribe from self
  }

  uint8_t i = 0;
  bool removed = false;
  while (i < MAX_SUBSCRIBERS && subscribers[i] != nullptr) {
    if (removed) {
      // shift remaining subscribers down so that `publish` calls don't have
      // to iterate over the entire list, just the beginning populated section
      subscribers[i - 1] = subscribers[i];
      subscribers[i] = nullptr;
    } else if (subscribers[i] == actor) {
      subscribers[i] = nullptr;
      removed = true;
    }
    i++;
  }
}

void Actor::unsubscribe(Actor *actor) {
  actor->removeSubscriber(this);
}

void Actor::publish(Message *message) {
  uint8_t i = 0;
  while (i < MAX_SUBSCRIBERS) {

    if (subscribers[i] == nullptr) {
      // reached end of subscriber list,
      // no need to iterate over rest of nullptrs in the array
      return;
    }

    if (subscribers[i]->enabled) {  // don't send events to disabled actors
      subscribers[i]->dispatch(message);
    }

    i++;
  }
}

}  // namespace Actor
