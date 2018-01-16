// solarTimeRTC

// Arduino example sketch for SolarPosition library
//
// Calculate solar position from time and location information
// using an RTC as a time source.

// 2017 Ken Willmott
// Arduino library based on the program "Arduino Uno and Solar Position Calculations"
// (c) David R. Brooks, which can be found at http://www.instesre.org/ArduinoDocuments.htm
// and issued under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License:
// https://creativecommons.org/licenses/by-nc-nd/4.0/

#include <SolarPosition.h>

// choose your RTC library (DS1307 lib can also read from the DS3231 IC):

//#include <DS3232RTC.h>
#include <DS1307RTC.h>

// number of decimal digits to print
const uint8_t digits = 3;

// some test positions:
SolarPosition Toronto(43.653109, -79.386304);  // Toronto, Canada
SolarPosition Timbuktu(16.775214, -3.007455); // Timbuktu, Mali, Africa
SolarPosition Melbourne(-37.668987, 144.841006); //Melbourne Airport (MEL)
SolarPosition Ulaanbaatar(47.847410, 106.769004); //Ulaanbaatar Airport (ULN)

// program begins

void setup()
{
  Serial.begin(9600);
  Serial.println(F("\tSolar Position Demo"));

  // set the Time service as the time provider
  SolarPosition::setTimeProvider(RTC.get);
}

void loop()
{
  // now test the real time methods:
  //
  printTime(RTC.get());

  Serial.print(F("Toronto:\t"));
  printSolarPosition(Toronto.getSolarPosition(), digits);
  Serial.print(F("Melbourne:\t"));
  printSolarPosition(Melbourne.getSolarPosition(), digits);
  Serial.print(F("Timbuktu:\t"));
  printSolarPosition(Timbuktu.getSolarPosition(), digits);
  Serial.print(F("Ulaanbaatar:\t"));
  printSolarPosition(Ulaanbaatar.getSolarPosition(), digits);
  Serial.println();

  delay(15000);
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

