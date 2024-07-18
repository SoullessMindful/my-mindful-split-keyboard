#ifndef key_h
#define key_h

const unsigned long LOCK_TIME_BOUND = 250;

#include <Arduino.h>
#include <Keyboard.h>
#include <Mouse.h>

class Key {
  public:
    Key();
    Key(char key);
    virtual void press();
    virtual void release();
  private:
    char key;
};

class KeyWithLock : public Key {
  public:
    KeyWithLock(char key, bool *lock, unsigned long *lastTime);
    void press() override;
    void release() override;
  private:
    bool *lock;
    unsigned long *lastTime;
};

class KeyScroll : public Key {
  public:
    KeyScroll(char scroll);
    void press() override;
    void release() override;
  private:
    char scroll;
};

class KeyLayer : public Key {
  public:
    KeyLayer(bool *on);
    void press() override;
    void release() override;
  private:
    bool *on;
};

class KeyLayerWithLock : public KeyLayer {
  public:
    KeyLayerWithLock(bool *on, bool *lock, unsigned long *lastTime);
    void press() override;
  private:
    bool *lock;
    unsigned long *lastTime;
};

class KeyMacro : public Key {
  public:
    KeyMacro(string text);
    void press() override;
    void release() override;
  private:
    string text;
};

#endif
