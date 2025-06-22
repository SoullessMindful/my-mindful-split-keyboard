#include <Arduino.h>
#include "key.h"

// Base Key
Key::Key() {}

Key::Key(char key) {
  this->key = key;
}

void Key::press() {
   Keyboard.press(this->key);
}

void Key::release() {
   Keyboard.release(this->key);
}

// Key With Lock
KeyWithLock::KeyWithLock(char key, bool *lock, unsigned long *lastTime): Key(key) {
  this->lock = lock;
  this->lastTime = lastTime;
}

void KeyWithLock::press() {
  Key::press();

  unsigned long time = millis();
  if (time - *lastTime < LOCK_TIME_BOUND) {
    *lock = !*lock;
  }
  *lastTime = time;
}

void KeyWithLock::release() {
  if (!*lock) {
    Key::release();
  }
}

// Key Scroll
KeyScroll::KeyScroll(char scroll) {
  this->scroll = scroll;
}

void KeyScroll::press() {
  Mouse.move(0, 0, this->scroll);
}

void KeyScroll::release() {}

// Key Layer
KeyLayer::KeyLayer(bool *on) {
  this->on = on;
}

void KeyLayer::press() {
  *on = true;
}

void KeyLayer::release() {
  *on = false;
}

// Key Layer With Lock
KeyLayerWithLock::KeyLayerWithLock(bool *on, bool *lock, unsigned long *lastTime): KeyLayer(on) {
  this->lock = lock;
  this->lastTime = lastTime;
}

void KeyLayerWithLock::press() {
  KeyLayer::press();

  unsigned long time = millis();
  if (time - *lastTime < LOCK_TIME_BOUND) {
    *lock = !*lock;
  }
  *lastTime = time;
}

// Key Macro
KeyMacro::KeyMacro(String text) {
  this->text = text;
}

void KeyMacro::press() {
  Keyboard.print(text);
}

void KeyMacro::release() {}
