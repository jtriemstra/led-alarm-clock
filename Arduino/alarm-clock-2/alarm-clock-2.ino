#include <Wire.h>
#include <DS3231.h>

DS3231 rtc(SDA, SCL);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rtc.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    char commandChar = Serial.read();
    switch(commandChar)
    {
      case 't':
        Serial.print("success ");
        break;
      case 'f':
        printTime(rtc.getTime());
        break;
    }
  }

  delay(1000);
}

void printTime(Time in)
{
  if (in.hour < 10) Serial.print(0);
  Serial.print(in.hour);
  Serial.print(":");
  if (in.min < 10) Serial.print(0);
  Serial.print(in.min);
  Serial.print(":");
  if (in.sec < 10) Serial.print(0);
  Serial.print(in.sec);
}
