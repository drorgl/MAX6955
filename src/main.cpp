#include <Arduino.h>

#include <Wire.h>
#include <MAX6955.h>

#define SCL A5
#define SDA A4

MAX6955 maxdisp(0);

void setup()
{
  maxdisp.init();

  maxdisp.test(true);
  delay(300);
  maxdisp.test(false);

  //test global min / max intensity
  maxdisp.write(PLANE_0, "########");

  for (int i = 0; i < 5; i++)
  {
    maxdisp.setGlobalIntensity(0);
    delay(1000);
    maxdisp.setGlobalIntensity(15);
    delay(1000);
  }

  //test individual intensity
  for (int i = 0; i < 10; i++)
  {
    maxdisp.setIntensity(0, 0);
    maxdisp.setIntensity(1, 0x0F);
    maxdisp.setIntensity(2, 0);
    maxdisp.setIntensity(3, 0x0F);
    maxdisp.setIntensity(4, 0);
    maxdisp.setIntensity(5, 0x0F);
    maxdisp.setIntensity(6, 0);
    maxdisp.setIntensity(7, 0x0F);

    delay(100);

    maxdisp.setIntensity(0, 0x0F);
    maxdisp.setIntensity(1, 0);
    maxdisp.setIntensity(2, 0x0F);
    maxdisp.setIntensity(3, 0);
    maxdisp.setIntensity(4, 0x0F);
    maxdisp.setIntensity(5, 0);
    maxdisp.setIntensity(6, 0x0F);
    maxdisp.setIntensity(7, 0);

    delay(100);
  }

  maxdisp.setGlobalIntensity(15);

  //test decimal point parsing
  maxdisp.clear();
  maxdisp.write(PLANE_0, ".1");
  delay(1000);
  maxdisp.write(PLANE_0, "1");
  delay(1000);
  maxdisp.write(PLANE_0, "2");
  delay(1000);
  maxdisp.write(PLANE_0, "1.2");
  delay(1000);
  maxdisp.write(PLANE_0, "12.3");
  delay(1000);
  maxdisp.write(PLANE_0, "123.4");
  delay(1000);
  maxdisp.write(PLANE_0, "1234.5");
  delay(1000);
  maxdisp.write(PLANE_0, "12345.6");
  delay(1000);
  maxdisp.write(PLANE_0, "123456.7");
  delay(1000);
  maxdisp.write(PLANE_0, "1234567.8");
  delay(1000);
  maxdisp.write(PLANE_0, "12345678");
  delay(1000);
  maxdisp.clear();
  delay(1000);

  //test blank blinking
  maxdisp.write(PLANE_0, "########");

  maxdisp.blink(true, false);
  delay(5000);
  maxdisp.blink(true, true);
  delay(5000);
  maxdisp.blink(false, false);
  delay(5000);

  //test partial blinking
  maxdisp.write(PLANE_1, "###--###");
  maxdisp.write(PLANE_0, "########");

  maxdisp.blink(true, false);
  delay(5000);
  maxdisp.blink(true, true);
  delay(5000);
  maxdisp.blink(false, false);
  delay(5000);

  //shutdown
  maxdisp.powerup(false);

  while (true){}
}

void loop()
{
}
