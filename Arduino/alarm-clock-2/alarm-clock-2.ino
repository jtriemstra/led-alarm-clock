#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <DS3231.h>

const int LED_PIN = 12;
const int STRING_LENGTH = 1;
const int ALARM_DURATION_MIN = 20;

DS3231 rtc(SDA, SCL);
Time objAlarmStart;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRING_LENGTH, LED_PIN, NEO_GRB + NEO_KHZ800);

bool blnAlarmOn;
int intCurrentRed, intCurrentGreen, intCurrentBlue;
int intAlarmLoop;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rtc.begin();

  objAlarmStart = getNullTime();
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // put your main code here, to run repeatedly:
  handleCommand();
  doAlarm();
  
  delay(1000);
}

void doAlarm()
{
  if (isAlarmOn())
  {
    blnAlarmOn = true;
  }
  else {
    blnAlarmOn = false;
    intCurrentRed = intCurrentGreen = intCurrentBlue = 0;
    intAlarmLoop = 0;
  }
  
  if (blnAlarmOn){
    intAlarmLoop++;
    if (intAlarmLoop >= 768) {}
    else if (intAlarmLoop >= 512){ intCurrentBlue = intAlarmLoop - 512; }
    else if (intAlarmLoop >= 256){ intCurrentGreen = intAlarmLoop - 256; }
    else{ intCurrentRed = intAlarmLoop; }
  }

  strip.setPixelColor(0, strip.Color(intCurrentRed, intCurrentGreen, intCurrentBlue));
  strip.show();
}

void handleCommand()
{
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

bool isAlarmOn()
{
  Time objAlarmEnd = addMinutes(objAlarmStart, ALARM_DURATION_MIN);
  return isTimeBetween(objAlarmStart, objAlarmEnd, rtc.getTime());
}

Time addMinutes(Time objTime, int intMinutes)
{
  Time objReturn;
  objReturn.hour = objTime.hour;
  objReturn.min = objTime.min;
  objReturn.sec = objTime.sec;

  objReturn.min = objReturn.min + intMinutes;
  while (objReturn.min >= 60)
  {
    objReturn.min = objReturn.min - 60;
    objReturn.hour = objReturn.hour + 1;
  }

  while (objReturn.hour >= 24)
  {
    objReturn.hour = objReturn.hour - 24;
  }
  
  return objReturn;
}

bool isTimeBetween(Time objStart, Time objEnd, Time objCheck)
{
  if (objStart.hour < objCheck.hour && objCheck.hour < objEnd.hour) return true;
  if (objStart.hour == objCheck.hour && objCheck.hour < objEnd.hour && objStart.min < objCheck.min) return true;
  if (objStart.hour < objCheck.hour && objCheck.hour == objEnd.hour && objCheck.min < objEnd.min) return true;
  if (objStart.hour == objCheck.hour && objCheck.hour == objEnd.hour && objStart.min < objCheck.min && objCheck.min < objEnd.min) return true;
  if (objStart.hour == objCheck.hour && objCheck.hour == objEnd.hour && objStart.min == objCheck.min && objCheck.min < objEnd.min) return true;
  
  return false;
}

