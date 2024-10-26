// #include "Keyboard.h"
#include "Mouse.h"

const int aButton = 2;
const int bButton = 3;
// const int xLed = 5;

bool _led = false;
bool duty = false;

const unsigned long stepBlinkLong = 3000;
const unsigned long stepBlinkShort = 50;
const unsigned long stepJobMillis = 60000;
unsigned long nextBlinkMillis = 0;
unsigned long nextJobMillis = 0;


void setup() {  // initialize the buttons' inputs:
  pinMode(aButton, INPUT);
  pinMode(bButton, INPUT);
  // pinMode(xLed, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  // initialize mouse control:
  Mouse.begin();
  // Keyboard.begin();

  randomSeed(analogRead(0));
}


void loop() {
  const unsigned long currentMillis = millis();

  if (duty && currentMillis > nextJobMillis) {
    long x_step = random(-5, 5);
    long y_step = random(-5, 5);
    while (0 == x_step && 0 == y_step) {
      x_step = random(-10, 10);
      y_step = random(-10, 10);
    }
    Mouse.move(x_step, y_step, 0);
    nextJobMillis = currentMillis + stepJobMillis;
  }

  if (currentMillis > nextBlinkMillis) {
    digitalWrite(LED_BUILTIN, _led ? HIGH : LOW);
    if (duty) {
      nextBlinkMillis = currentMillis + stepBlinkLong * (nextJobMillis-currentMillis) / stepJobMillis;
    } else {
      if (_led) {
        nextBlinkMillis = currentMillis + stepBlinkShort;
      } else {
        nextBlinkMillis = currentMillis + stepBlinkLong;
      }
    }
    _led = !_led;
  }

  if (digitalRead(aButton) == HIGH) {
    duty = true;
  }

  if (digitalRead(bButton) == HIGH) {
    duty = false;
  }
}
