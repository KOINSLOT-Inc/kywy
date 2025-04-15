// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_ACTOR
#define KYWY_LIB_ACTOR 1

#include "mbed.h"
#include "EventQueue.h"

namespace Actor {
const uint8_t MAX_SUBSCRIBERS = 5;

typedef enum : uint8_t {
  DIRECTIVE_HANDLE,
  DIRECTIVE_EXIT,
} Directive;

struct Message {
  Message() : directive(DIRECTIVE_HANDLE), signal(0), data(nullptr){};
  Message(int signal)
      : directive(DIRECTIVE_HANDLE), signal(signal), data(nullptr){};
  Message(int signal, void *data)
      : directive(DIRECTIVE_HANDLE), signal(signal), data(data){};
  Directive directive;
  int signal;
  void *data;
};

extern rtos::Mutex handlerMutex; // mutex to lock so that handler functions run from start to finish before going to another actor.

class Actor {
private:
  rtos::Thread thread;
  events::EventQueue queue;
  events::Event<void(Actor*, Message*)> *event_handler;

  Actor *subscribers[MAX_SUBSCRIBERS] = {};

public:
  Actor();
  ~Actor();

  virtual void handle(Message *message) = 0;

  void start();
  void stop();

  virtual void initialize();
  virtual void teardown();

  void dispatch(Message *message);
  void publish(Message *message);

  void subscribe(Actor *actor);
  void addSubscriber(Actor *actor);
  void unsubscribe(Actor *actor);
  void removeSubscriber(Actor *actor);
};

// Queue event callback that handles events, or passes them to a user defined function to handle
void queueEventCallback(Actor *actor, Message *message);

} // namespace Actor

#endif