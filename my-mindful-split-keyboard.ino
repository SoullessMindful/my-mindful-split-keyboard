#include <Keyboard.h>

const byte OUTPUT_COUNT = 6;
const byte OUTPUTS[OUTPUT_COUNT] = {A0, A1, A2, A3, A4, A5};

const byte INPUT_COUNT = 8;
const byte SPECIAL_SECTION_LEFT = 6;
const byte SPECIAL_SECTION_RIGHT = 7;
const byte INPUTS[INPUT_COUNT] = {
  0, 1, 2, 4, 5, SPECIAL_SECTION_LEFT, SPECIAL_SECTION_RIGHT
};

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
}
