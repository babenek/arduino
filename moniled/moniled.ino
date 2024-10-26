#include <SoftwareSerial.h>

String inData;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PINB2, OUTPUT);
  pinMode(PINB3, OUTPUT);
  pinMode(PINB4, OUTPUT);
  pinMode(PINB5, OUTPUT);
  pinMode(PINB6, OUTPUT);
  Serial.begin(115200);
  //  Serial.println(">>>");
}

const unsigned long nextStep = 500;
unsigned long currentMillis = 0;

bool led_ = false;
bool led2 = false;
bool led3 = false;
bool led4 = false;
bool led5 = false;
bool led6 = false;

unsigned long next_ = 0;
unsigned long next2 = 0;
unsigned long next3 = 0;
unsigned long next4 = 0;
unsigned long next5 = 0;
unsigned long next6 = 0;

void loop() {
  currentMillis = millis();
  //  if (0 == currentMillis % 3147)
  //  {
  //    Serial.println (String(currentMillis, DEC));
  //    delay(1);
  //  }

  // blink always
  if (currentMillis > next_)
  {
    digitalWrite(LED_BUILTIN, led_ ?  HIGH : LOW);
    led_ = ! led_;
    next_ = currentMillis + nextStep * 3;
  }

  if (currentMillis > next2)
  {
    digitalWrite(PINB2, led2 ?  HIGH : LOW);
    led2 = ! led2;
    next2 = currentMillis + nextStep;
  }

  if (currentMillis > next3)
  {
    digitalWrite(PINB3, led3 ?  HIGH : LOW);
    led3 = ! led3;
    next3 = currentMillis + nextStep;
  }

  if (currentMillis > next4)
  {
    digitalWrite(PINB4, led4 ?  HIGH : LOW);
    led4 = ! led4;
    next4 = currentMillis + nextStep;
  }

  if (currentMillis > next5)
  {
    digitalWrite(PINB5, led5 ?  HIGH : LOW);
    led5 = ! led5;
    next5 = currentMillis + nextStep;
  }

  // blink when no data received or error
  if (currentMillis > next6)
  {
    digitalWrite(PINB6, led6 ?  HIGH : LOW);
    led6 = ! led6;
    next6 = currentMillis + nextStep;
  }

  if (Serial.available())
  {
    digitalWrite(PINB6, HIGH);
    led6 = true;

    while (Serial.available())
    {
      char c = Serial.read();
      if ( '\n' == c )
      {
        ;
      }
      else if ('w' == c)
      {
        digitalWrite(PINB2, HIGH);
        next2 = currentMillis + nextStep;
        led2 = true;
      }
      else if ('2' == c)
      {
        digitalWrite(PINB2, HIGH);
        next2 = next2 + nextStep * 2;
        led2 = true;
      }

      else if ('e' == c)
      {
        digitalWrite(PINB3, HIGH);
        next3 = currentMillis + nextStep;
        led3 = true;
      }
      else if ('3' == c)
      {
        digitalWrite(PINB3, HIGH);
        next3 = next3 + nextStep * 2 ;
        led3 = true;
      }

      else if ('r' == c)
      {
        digitalWrite(PINB4, HIGH);
        next4 = currentMillis + nextStep ;
        led4 = true;
      }
      else if ('4' == c)
      {
        digitalWrite(PINB4, HIGH);
        next4 = next4 + nextStep * 2 ;
        led4 = true;
      }

      else if ('t' == c)
      {
        digitalWrite(PINB5, HIGH);
        next5 = currentMillis + nextStep;
        led5 = true;
      }
      else if ('5' == c)
      {
        digitalWrite(PINB5, HIGH);
        next5 = next5 + nextStep * 2;
        led5 = true;
      }
      else
      {
        Serial.println (String(int(c), DEC));
        next6 = currentMillis;
      }
    }

    if (next6 < next2) next6 = next2;
    if (next6 < next3) next6 = next3;
    if (next6 < next4) next6 = next4;
    if (next6 < next5) next6 = next5;

  }




}
