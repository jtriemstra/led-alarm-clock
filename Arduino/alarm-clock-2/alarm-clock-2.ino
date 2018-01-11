#include <Wire.h>
#include <DS3231.h>

DS3231 rtc(SDA, SCL);
Time objAlarmStart;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rtc.begin();

  objAlarmStart = getNullTime();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    char commandChar = Serial.read();
    switch(commandChar)
    {
      case 't':
        Serial.print("success --------");
        break;
      case 'f':
        printTime(rtc.getTime());
        printTime(objAlarmStart);
        break;
      case 's':
        int intExpectedBytes = 16;
        int intReceivedBytes = 0;
        char chrReceivedBytes[intExpectedBytes];
        
        while (intReceivedBytes < intExpectedBytes)
        {
          if (Serial.available() > 0)
          {
            chrReceivedBytes[intReceivedBytes] = Serial.read();
            intReceivedBytes++;
          }
        }

        int intTimeHour, intTimeMin, intTimeSec, intAlarmHour, intAlarmMin, intAlarmSec;
        intTimeHour = ((int)(chrReceivedBytes[0] - '0') * 10) + (int)(chrReceivedBytes[1] - '0');
        intTimeMin = ((int)(chrReceivedBytes[3] - '0') * 10) + (int)(chrReceivedBytes[4] - '0');
        intTimeSec = ((int)(chrReceivedBytes[6] - '0') * 10) + (int)(chrReceivedBytes[7] - '0');
        intAlarmHour = ((int)(chrReceivedBytes[8] - '0') * 10) + (int)(chrReceivedBytes[9] - '0');
        intAlarmMin = ((int)(chrReceivedBytes[11] - '0') * 10) + (int)(chrReceivedBytes[12] - '0');
        intAlarmSec = ((int)(chrReceivedBytes[14] - '0') * 10) + (int)(chrReceivedBytes[15] - '0');

        rtc.setTime(intTimeHour, intTimeMin, intTimeSec);
        objAlarmStart = createTime(intAlarmHour, intAlarmMin, intAlarmSec);
        
        break;
    }
  }

  delay(1000);
}

void printTime(Time in)
{
  if (isNullTime(in)) 
  {
    Serial.print("--------");
    return;
  }
  
  if (in.hour < 10) Serial.print(0);
  Serial.print(in.hour);
  Serial.print(":");
  if (in.min < 10) Serial.print(0);
  Serial.print(in.min);
  Serial.print(":");
  if (in.sec < 10) Serial.print(0);
  Serial.print(in.sec);
}

Time getNullTime()
{
  Time objReturn;
  objReturn.hour = 0;
  objReturn.min = 0;
  objReturn.sec = 0;
  objReturn.date = 1;
  objReturn.mon = 1;
  objReturn.year = 1973;
  objReturn.dow = 1;

  return objReturn;
}

Time createTime(int hour, int min, int sec)
{
  Time objReturn;
  objReturn.hour = hour;
  objReturn.min = min;
  objReturn.sec = sec;
  objReturn.date = 1;
  objReturn.mon = 1;
  objReturn.year = 1973;
  objReturn.dow = 1;

  return objReturn;
}

bool isNullTime(Time in)
{
  return in.hour == 0 && in.min == 0 && in.sec == 0;
}

