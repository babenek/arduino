#include <Arduino.h>
#include <TM1638.h>
#include <Ds1302.h>

TM1638 tm(4, 7, 2);

Ds1302 rtc(8, 13, 12);

pulse_t pulse = PULSE1_16;
uint8_t text[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
Ds1302::DateTime now;
uint8_t old_second = 0;
uint8_t led_state = 0;
unsigned long led_time[6] = { 0, 0, 0, 0, 0, 0 };
unsigned long old_millis = 0;
unsigned n = 0;

const uint8_t led_mask[6] = {
  0x03,
  0x06,
  0x0C,
  0x18,
  0x30,
  0x21,
};


void setup() {
  Serial.begin(9600);
  tm.reset();
  // tm.test();
  tm.displaySetBrightness(PULSE1_16);

  // initialize the RTC
  rtc.init();

  // test if clock is halted and set a date-time (see example 2) to start it
  if (rtc.isHalted()) {
    Serial.println("RTC is halted. Setting time...");

    Ds1302::DateTime dt = {
      .year = 24,
      .month = Ds1302::MONTH_JAN,
      .day = 1,
      .hour = 0,
      .minute = 0,
      .second = 0,
      .dow = Ds1302::DOW_TUE
    };

    rtc.setDateTime(&dt);
  }

  unsigned long t = millis();
  for (n = 0; n < 8; ++n) led_time[n] = t;
  led_state = 0;
}

unsigned led_old = 0;
unsigned olddec = 0;
void loop() {
  const unsigned long now_millis = millis();

  rtc.getDateTime(&now);
  if (old_second != now.second) {
    tm.displayVal(0, now.second % 10);
    tm.displayVal(1, now.second / 10);

    // tm.displayVal(2, now.minute % 10);
    // tm.displayVal(3, now.minute / 10);

    // tm.displayVal(4, now.hour % 10);
    // tm.displayVal(5, now.hour / 10);

    // tm.displayVal(6, now.day % 10);
    // tm.displayVal(7, now.day / 10);
    old_second = now.second;
  }

  const unsigned led_now = (old_second / 10) % 6;
  const unsigned led_fut = (led_now + 1) % 6;


  tm.displayVal(2, led_now);
  tm.displayVal(3, led_fut);  

  const unsigned secdec = old_second % 10;
  if (led_old != led_now) {
    led_old = led_now;
    old_millis = now_millis;
  }

  const unsigned long step = 128;
  unsigned long x = (now_millis - old_millis);
  float y = step * x / 10000.0;

  if (led_time[led_now] < now_millis) {
    uint8_t mask_now = static_cast<uint8_t>(1) << led_now;
    if (led_state & mask_now) {
      // currently is on - will be off and when on again
      led_time[led_now] = now_millis + y;
      led_state &= ~mask_now;
    } else {
      led_time[led_now] = now_millis + step - y;
      led_state |= mask_now;
    }
    
  }
  
  if (led_time[led_fut] < now_millis) {
    uint8_t mask_fut = static_cast<uint8_t>(1) << led_fut;
    if (led_state & mask_fut) {
      led_time[led_fut] = now_millis + step - y;
      led_state &= ~mask_fut;
    } else {
      led_time[led_fut] = now_millis + y;
      led_state |= mask_fut;
    }
  }

  led_state &= led_mask[led_now];

  tm.writeLeds(led_state);
}

// https://blog.wokwi.com/explore-pwm-with-logic-analyzer/
// PWM