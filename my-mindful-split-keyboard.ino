#include "key.h"

const byte OUTPUT_COUNT = 6;
const byte OUTPUTS[OUTPUT_COUNT] = {A0, A1, A2, A3, A4, A5};

const byte INPUT_COUNT = 8;
const byte INPUTS[INPUT_COUNT] = {
  0, 1, 2, 4, 5, 6, 7
};

const byte LAYER_COUNT = 2;
const byte FUNCTION_LAYER = 1;
const byte BASE_LAYER = 0;

/*const char keys[LAYER_COUNT][INPUT_COUNT][OUTPUT_COUNT] = {
  {
    {KEY_TAB, KEY_ESC, KEY_CAPS_LOCK,
    'q', 'a', 'z'},
    {'w', 's', 'x',
    'e', 'd', 'c'},
    {'r', 'f', 'v',
    't', 'g', 'b'},
    {'y', 'h', 'n',
    'u', 'j', 'm'},
    {'i', 'k', ',',
    'o', 'l', '.'},
    {'p', ';', '/',
    '\\', '\'', KEY_RIGHT_SHIFT},
  },
  {
    {KEY_TAB, KEY_ESC, KEY_CAPS_LOCK,
    KEY_F1, KEY_F13, KEY_INSERT},
    {KEY_F2, KEY_F14, KEY_DELETE,
    KEY_F3, KEY_F15, KEY_F18},
    {KEY_F4, KEY_F16, KEY_F19,
    KEY_F5, KEY_F17, KEY_F20},
    {KEY_F6, KEY_LEFT_ARROW, KEY_HOME,
    KEY_F7, KEY_DOWN_ARROW, KEY_PAGE_DOWN},
    {KEY_F8, KEY_UP_ARROW, KEY_PAGE_UP,
    KEY_F9, KEY_RIGHT_ARROW, KEY_END},
    {KEY_F10, ';', '/',
    KEY_F11, KEY_F12, KEY_PRINT_SCREEN},
  },
};*/

unsigned long shiftLastTime;
bool shiftLock = false;

Key *thumbShiftKey = new KeyWithLock(KEY_LEFT_SHIFT, &shiftLock, &shiftLastTime);

unsigned long functionLayerLastTime;
bool functionLayerOn = false;
bool functionLayerLock = false;

Key *functionLayerKey = new KeyLayerWithLock(&functionLayerOn, &functionLayerLock, &functionLayerLastTime);

Key *keys[LAYER_COUNT][INPUT_COUNT][OUTPUT_COUNT] = {
  {
    {new Key(KEY_TAB), new Key(KEY_ESC), new Key(KEY_CAPS_LOCK),
    new Key('q'), new Key('a'), new Key('z')},
    {new Key('w'), new Key('s'), new Key('x'),
    new Key('e'), new Key('d'), new Key('c')},
    {new Key('r'), new Key('f'), new Key('v'),
    new Key('t'), new Key('g'), new Key('b')},
    {new Key('y'), new Key('h'), new Key('n'),
    new Key('u'), new Key('j'), new Key('m')},
    {new Key('i'), new Key('k'), new Key(','),
    new Key('o'), new Key('l'), new Key('.')},
    {new Key('p'), new Key(';'), new Key('/'),
    new Key('\\'), new Key('\''), new Key(KEY_RIGHT_SHIFT)},
    {thumbShiftKey, functionLayerKey},
  },
  {
    {new Key(KEY_TAB), new Key(KEY_ESC), new Key(KEY_CAPS_LOCK),
    new Key(KEY_F1), new Key(KEY_F13), new Key(KEY_INSERT)},
    {new Key(KEY_F2), new Key(KEY_F14), new Key(KEY_DELETE),
    new Key(KEY_F3), new Key(KEY_F15), new Key(KEY_F18)},
    {new Key(KEY_F4), new Key(KEY_F16), new Key(KEY_F19),
    new Key(KEY_F5), new Key(KEY_F17), new Key(KEY_F20)},
    {new Key(KEY_F6), new Key(KEY_LEFT_ARROW), new Key(KEY_HOME),
    new Key(KEY_F7), new Key(KEY_DOWN_ARROW), new Key(KEY_PAGE_DOWN)},
    {new Key(KEY_F8), new Key(KEY_UP_ARROW), new Key(KEY_PAGE_UP),
    new Key(KEY_F9), new Key(KEY_RIGHT_ARROW), new Key(KEY_END)},
    {new Key(KEY_F10), new KeyScroll(1), new KeyScroll(-1),
    new Key(KEY_F11), new Key(KEY_F12), new Key(KEY_PRINT_SCREEN)},
    {thumbShiftKey, functionLayerKey},
  },
};

bool keyDown[INPUT_COUNT][OUTPUT_COUNT];
bool keyDownPrev[INPUT_COUNT][OUTPUT_COUNT];
byte layer = 0;

void setup() {
  for (byte i = 0; i < OUTPUT_COUNT; i++) {
    pinMode(OUTPUTS[i], OUTPUT);
    digitalWrite(OUTPUTS[i], HIGH);
  }

  for (byte j = 0; j < INPUT_COUNT; j++) {
    pinMode(INPUTS[j], INPUT_PULLUP);
  }

  Keyboard.begin();
  Mouse.begin();

  delay(3000);
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

  handleLayers();
}

void handlePressOrRelease(byte section, byte row, bool press) {
  Key *currentKey = keys[layer][section][row];

  if (currentKey == NULL) return;

  if (press) {
    currentKey->press();
  } else {
    currentKey->release();
  }
}

void handleLayers() {
  if (functionLayerLock || functionLayerOn) {
    layer = FUNCTION_LAYER;
    return;
  }

  layer = BASE_LAYER;
}
