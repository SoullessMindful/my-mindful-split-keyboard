#include "key.h"

const byte OUTPUT_COUNT = 6;
const byte OUTPUTS[OUTPUT_COUNT] = {A0, A1, A2, A3, A4, A5};

const byte INPUT_COUNT = 8;
const byte INPUTS[INPUT_COUNT] = {
  2, 3, 4, 5, 6, 7, 8, 9
};

const byte LAYER_COUNT = 3;
const byte SYMBOL_LAYER = 2;
const byte FUNCTION_LAYER = 1;
const byte BASE_LAYER = 0;

unsigned long shiftLastTime;
bool shiftLock = false;

Key *thumbShiftKey = new KeyWithLock(KEY_LEFT_SHIFT, &shiftLock, &shiftLastTime);

unsigned long functionLayerLastTime;
bool functionLayerOn = false;
bool functionLayerLock = false;

Key *functionLayerKey = new KeyLayerWithLock(&functionLayerOn, &functionLayerLock, &functionLayerLastTime);

unsigned long symbolLayerLastTime;
bool symbolLayerOn = false;
bool symbolLayerLock = false;

Key *symbolLayerKey = new KeyLayerWithLock(&symbolLayerOn, &symbolLayerLock, &symbolLayerLastTime);

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
    {new Key(KEY_BACKSPACE), new Key(' '), new Key(KEY_LEFT_ALT),
    thumbShiftKey, new Key(KEY_LEFT_GUI), new Key(KEY_LEFT_CTRL)},
    {new Key(KEY_RETURN), new Key(' '), new Key(KEY_LEFT_ALT),
    thumbShiftKey, symbolLayerKey, functionLayerKey},
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
    {new Key(KEY_BACKSPACE), new Key(' '), new Key(KEY_LEFT_ALT),
    thumbShiftKey, new Key(KEY_LEFT_GUI), new Key(KEY_LEFT_CTRL)},
    {new Key(KEY_RETURN), new Key(' '), new Key(KEY_LEFT_ALT),
    thumbShiftKey, symbolLayerKey, functionLayerKey},
  },
  {
    {new Key('`'), new Key(KEY_ESC), new Key(KEY_CAPS_LOCK),
    new Key('1'), new Key('<'), new Key('!')},
    {new Key('2'), new Key('{'), new Key('@'),
    new Key('3'), new Key('['), new Key('#')},
    {new Key('4'), new Key('('), new Key('$'),
    new Key('5'), new KeyMacro("=>"), new Key('%')},
    {new Key('6'), new KeyMacro("->"), new Key('^'),
    new Key('7'), new Key(')'), new Key('&')},
    {new Key('8'), new Key(']'), new Key('*'),
    new Key('9'), new Key('}'), new Key('(')},
    {new Key('0'), new Key('>'), new Key(')'),
    new Key('-'), new Key('='), new Key(KEY_RIGHT_SHIFT)},
    {new Key(KEY_BACKSPACE), new Key(' '), new Key(KEY_LEFT_ALT),
    thumbShiftKey, new Key(KEY_LEFT_GUI), new Key(KEY_LEFT_CTRL)},
    {new Key(KEY_RETURN), new Key(' '), new Key(KEY_LEFT_ALT),
    thumbShiftKey, symbolLayerKey, functionLayerKey},
  },
};

bool keyDown[INPUT_COUNT][OUTPUT_COUNT];
bool keyDownPrev[INPUT_COUNT][OUTPUT_COUNT];

byte layer = 0;

void handleLayers() {
  if (symbolLayerLock || symbolLayerOn) {
    layer = SYMBOL_LAYER;
    return;
  }

  if (functionLayerLock || functionLayerOn) {
    layer = FUNCTION_LAYER;
    return;
  }

  layer = BASE_LAYER;
}

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
