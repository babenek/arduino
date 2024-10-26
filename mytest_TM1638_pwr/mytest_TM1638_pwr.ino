#include <Arduino.h>
#include <TM1638.h>
#include <Ds1302.h>

TM1638 tm(4, 7, 2);

Ds1302 rtc(8, 13, 12);

Ds1302::DateTime now;
uint8_t current_second;
unsigned long old_millis;
unsigned long delay_millis;

const uint8_t pwr_led_id[6] = { 3, 5, 6, 9, 10, 11 };

// cos func
const uint8_t val_now[10] = { 255, 249, 231, 203, 167, 127, 88, 52, 24, 6 };
const uint8_t val_fut[10] = { 0, 6, 24, 52, 88, 128, 167, 203, 231, 249 };

void setup() {
  for (auto n = 0; n <= 14; ++n) pinMode(n, OUTPUT);
  Serial.begin(9600);
  Serial.println("START");

  tm.reset();
  // tm.test();
  tm.displaySetBrightness(PULSE1_16);

  // initialize the RTC
  rtc.init();

  // test if clock is halted and set a date-time (see example 2) to start it
  // if (rtc.isHalted()) {
  Ds1302::DateTime dt = {
    .year = 24,
    .month = 5,
    .day = 5,
    .hour = 5,
    .minute = 5,
    .second = 5,
    .dow = Ds1302::DOW_SUN
  };

  rtc.setDateTime(&dt);
  // }
  current_second = 99;
  old_millis = millis();
  delay_millis = 0;
}

unsigned led_old = 0;
unsigned olddec = 0;
void loop() {
  const unsigned long now_millis = millis();
  const unsigned long delta_millis = now_millis % 1000;

  delay_millis = 1000 - delta_millis;

  if (2000 < delay_millis)
    delay_millis = 1000;

  old_millis = now_millis;

  rtc.getDateTime(&now);
  if (current_second != now.second) {
    tm.displayVal(0, now.second % 10);
    tm.displayVal(1, now.second / 10);

    tm.displayVal(2, now.minute % 10);
    tm.displayVal(3, now.minute / 10);

    tm.displayVal(4, now.hour % 10);
    tm.displayVal(5, now.hour / 10);

    // tm.displayVal(6, now.day % 10);
    // tm.displayVal(7, now.day / 10);
    current_second = now.second;


    const unsigned led_now = (current_second / 10) % 6;
    const unsigned led_fut = (led_now + 1) % 6;

    tm.displayVal(6, led_now);
    tm.displayVal(7, led_fut);


    for (auto n = 0; n < 6; ++n)
      if (n != led_now and n != led_fut)
        analogWrite(pwr_led_id[n], 0);  // of all unused

    led_old = led_now;
    // double k = 1 - cos((current_second % 10) / (10.0 / PI));
    // // const float k = (current_second % 10) / 10.0;
    // uint8_t val_fut = 128 * k;
    // uint8_t val_now = 255 - val_fut;
    analogWrite(pwr_led_id[led_old], val_now[current_second % 10]);
    analogWrite(pwr_led_id[led_fut], val_fut[current_second % 10]);

    Serial.print("\r\nnow_millis:");
    Serial.print(now_millis);
    Serial.print(" delay_millis:");
    Serial.print(delay_millis);
  }

  delay(delay_millis);
}

// https://blog.wokwi.com/explore-pwm-with-logic-analyzer/
// PWM