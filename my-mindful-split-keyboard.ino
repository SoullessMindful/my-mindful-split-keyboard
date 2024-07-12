#include <Keyboard.h>

const byte OUTPUT_COUNT = 6;
const byte OUTPUTS[OUTPUT_COUNT] = {A0, A1, A2, A3, A4, A5};

const byte INPUT_COUNT = 8;
const byte SPECIAL_SECTION_LEFT = 6;
const byte SPECIAL_SECTION_RIGHT = 7;
const byte INPUTS[INPUT_COUNT] = {
  0, 1, 2, 4, 5, SPECIAL_SECTION_LEFT, SPECIAL_SECTION_RIGHT
};

const byte LAYERS = 2;

const unsigned long LOCK_TIME_BOUND = 250;

const char keys[LAYERS][INPUT_COUNT][OUTPUT_COUNT] = {
  {
    {'r', 'f'},
  },
  {
    {'t', 'g'},
  },
};


bool keyDown[INPUT_COUNT][OUTPUT_COUNT];
bool keyDownPrev[INPUT_COUNT][OUTPUT_COUNT];
byte layer = 0;

unsigned long shiftLastTime;
bool shiftLock = false;

unsigned long firstLayerLastTime;
bool firstLayerLock = false;

void setup() {
  for (byte i = 0; i < OUTPUT_COUNT; i++) {
    pinMode(OUTPUTS[i], OUTPUT);
    digitalWrite(OUTPUTS[i], HIGH);
  }

  for (byte j = 0; j < INPUT_COUNT; j++) {
    pinMode(INPUTS[j], INPUT_PULLUP);
  }

  Keyboard.begin();
}

void loop() {
  for (byte i = 0; i < OUTPUT_COUNT; i++) {
    for (byte j = 0; j < INPUT_COUNT; j++) {
      keyDownPrev[j][i] = keyDown[j][i];
    }
  }

  for (byte i = 0; i < OUTPUT_COUNT; i++) {
    digitalWrite(OUTPUTS[i], LOW);
    delayMicroseconds(5);
    for (byte j = 0; j < INPUT_COUNT; j++) {
      keyDown[j][i] = (digitalRead(INPUTS[j]) == LOW);
    }
    digitalWrite(OUTPUTS[i], HIGH);
  }

  handleLayers();

  for (byte i = 0; i < OUTPUT_COUNT; i++) {
    for (byte j = 0; j < INPUT_COUNT; j++) {
      if (keyDown[j][i] && !keyDownPrev[j][i]) {
        handlePressOrRelease(j, i, true);
      }

      if (!keyDown[j][i] && keyDownPrev[j][i]) {
        handlePressOrRelease(j, i, false);
      }
    }
  }
}

void handlePressOrRelease(byte section, byte row, bool press) {
  switch (section) {
    case SPECIAL_SECTION_LEFT:
      switch (row) {
        case 0:
          porl(KEY_LEFT_SHIFT, press, &shiftLock, &shiftLastTime);
          break;

        case 1:
          handleLock(&firstLayerLock, &firstLayerLastTime);
          break;
      }
      break;

    case SPECIAL_SECTION_RIGHT:
      break;

    default:
      por(keys[layer][section][row], press);
      break;
  }
}

void handleLayers() {
  if (firstLayerLock || keyDown[SPECIAL_SECTION_LEFT][1]) {
    layer = 1;
    return;
  }

  layer = 0;
}

// Press or Release
void por(char key, bool press) {
  if (press) {
    Keyboard.press(key);
  } else {
    Keyboard.release(key);
  }
}

// Press or Release with lock
void porl(char key, bool press, bool *lock, unsigned long *lastTime) {
  if (press) {
    Keyboard.press(key);

    handleLock(lock, lastTime);
  } else if (!*lock) {
    Keyboard.release(key);
  }
}

void handleLock(bool *lock, unsigned long *lastTime) {
    unsigned long time = millis();
    if (time - *lastTime < LOCK_TIME_BOUND) {
      *lock = !*lock;
    }
    *lastTime = time;
}
