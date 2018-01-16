// solarTimeNoClockDemo

// Arduino example sketch for SolarPosition library
//
// Calculate solar position from time and location information
// not using any clock to keep time. Just do calculations.

// 2017 Ken Willmott
// Arduino library based on the program "Arduino Uno and Solar Position Calculations"
// (c) David R. Brooks, which can be found at http://www.instesre.org/ArduinoDocuments.htm
// and issued under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License:
// https://creativecommons.org/licenses/by-nc-nd/4.0/

#include <SolarPosition.h>

// number of decimal digits to print
const uint8_t digits = 3;

// some test positions:
SolarPosition Timbuktu(16.775214, -3.007455); // Timbuktu, Mali, Africa

// create a fixed UNIX time to test fixed time method
int someS = 0;  //second
int someM = 0;  //minute
int someH = 12; //hour
int someD = 15; //day
int someMo = 4; //month
int someY = 1985; //year
tmElements_t someTime = {someS, someM, someH, 0, someD, someMo, CalendarYrToTm(someY) };
time_t someEpochTime = makeTime(someTime);

// program begins

void setup()
{
  Serial.begin(9600);
  Serial.println(F("\tSolar Position Demo"));

  // First test the fixed time methods:
  //
  Serial.print(F("The sun was at an elevation of "));
  Serial.print(Timbuktu.getSolarElevation(someEpochTime), 4);
  Serial.print(F(" and an azimuth of "));
  Serial.println(Timbuktu.getSolarAzimuth(someEpochTime), 4);
  Serial.print(F("in Timbuktu at "));
  printTime(someEpochTime);

  Serial.print(F("The earth was "));
  Serial.print(Timbuktu.getSolarDistance(someEpochTime), 0);
  Serial.println(F(" km from the Sun."));
  Serial.println();

  Serial.println(F("Done..."));
  Serial.println();
}

void loop()
{
}

// Print a solar position to serial
//
void printSolarPosition(SolarPosition_t pos, int numDigits)
{
  Serial.print(F("el: "));
  Serial.print(pos.elevation, numDigits);
  Serial.print(F(" deg\t"));

  Serial.print(F("az: "));
  Serial.print(pos.azimuth, numDigits);
  Serial.println(F(" deg"));
}

// Print a time to serial
//
void printTime(time_t t)
{
  tmElements_t someTime;
  breakTime(t, someTime);

  Serial.print(someTime.Hour);
  Serial.print(F(":"));
  Serial.print(someTime.Minute);
  Serial.print(F(":"));
  Serial.print(someTime.Second);
  Serial.print(F(" UTC on "));
  Serial.print(dayStr(someTime.Wday));
  Serial.print(F(", "));
  Serial.print(monthStr(someTime.Month));
  Serial.print(F(" "));
  Serial.print(someTime.Day);
  Serial.print(F(", "));
  Serial.println(tmYearToCalendar(someTime.Year));
}

