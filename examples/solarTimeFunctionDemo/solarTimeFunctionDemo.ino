// solarTimeFunctionDemo

// Arduino example sketch for SolarPosition library
//
// This sketch demonstrates how to directly access the utility function that the object oriented interface uses
// to calculate solar positions. Note that the function units are radians and AU (astronomical units),
// unlike the SolarPosition class which automatically converts those to degrees and kilometers.

// Calculate solar position from time and location information
// using Time library functions (relying on CPU clock based timer) to keep time.

// 2017 Ken Willmott
// Arduino library based on the program "Arduino Uno and Solar Position Calculations"
// (c) David R. Brooks, which can be found at http://www.instesre.org/ArduinoDocuments.htm
// and issued under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License:
// https://creativecommons.org/licenses/by-nc-nd/4.0/

#include <SolarPosition.h>

// number of decimal digits to print
const uint8_t digits = 3;

// A solar position structure to demonstrate storing complete positions
SolarPosition_t savedPosition;

// a test position:
// (43.6777, -79.6248) Toronto, Canada airport (YYZ)

const float Latitude = 43.6777;
const float Longitude = -79.6248;

// program begins

void setup()
{
  Serial.begin(9600);
  Serial.println(F("\tSolar Position Utility Function Demo"));

  // obtain a complete current position
  savedPosition = calculateSolarPosition(SECS_YR_2000, Latitude * DEG_TO_RAD, Longitude * DEG_TO_RAD);

  Serial.print(F("The sun was at an elevation of "));
  Serial.print(savedPosition.elevation * RAD_TO_DEG, 4);
  Serial.print(F(" and an azimuth of "));
  Serial.println(savedPosition.azimuth * RAD_TO_DEG, 4);
  Serial.print(F("in YYZ airport at "));
  printTime(savedPosition.time);

  Serial.print(F("The earth was "));
  Serial.print(savedPosition.distance * KM_PER_AU, 0);
  Serial.println(F(" km from the Sun."));
  Serial.println();

  Serial.println(F("Finished..."));
  Serial.println();
}

void loop()
{
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
