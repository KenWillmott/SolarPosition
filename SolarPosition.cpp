// SolarPosition.cpp

// 2017 Ken Willmott
// Arduino library based on the program "Arduino Uno and Solar Position Calculations"
// (c) David R. Brooks, which can be found at http://www.instesre.org/ArduinoDocuments.htm
// and issued under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License:
// https://creativecommons.org/licenses/by-nc-nd/4.0/

#include "SolarPosition.h"

getExternalTime SolarPosition::getExtTimePtr = NULL;  // pointer to external sync function

// constructor
//
SolarPosition::SolarPosition(const float Lat, const float Lon)
{
  Latitude = Lat * DEG_TO_RAD;
  Longitude = Lon * DEG_TO_RAD;
}

// *** public methods: ***

// assign a Time provider function
//
void SolarPosition::setTimeProvider(getExternalTime getTimeFunction)
{
  getExtTimePtr = getTimeFunction;
}

// Get current Position
//
SolarPosition_t SolarPosition::getSolarPosition()
{
  const SolarPosition_t nullPos;

  if (getExtTimePtr != NULL)
  {
    time_t timeNow = getExtTimePtr();
    SolarPosition_t pos;
    calculateSolarPosition(timeNow);
    pos.elevation = Elevation * RAD_TO_DEG;
    pos.azimuth = Azimuth * RAD_TO_DEG;
    pos.distance = solarDistance * KM_PER_AU;
    pos.time = timeNow;
    return pos;
  }
  else
  {
    return nullPos;
  }
}

// Get Position for specified time
//
SolarPosition_t SolarPosition::getSolarPosition(time_t t)
{
  SolarPosition_t pos;
  calculateSolarPosition(t);
  pos.elevation = Elevation * RAD_TO_DEG;
  pos.azimuth = Azimuth * RAD_TO_DEG;
  pos.distance = solarDistance * KM_PER_AU;
  pos.time = t;
  return pos;
}

// Get current Elevation
//
float SolarPosition::getSolarElevation()
{
  if (getExtTimePtr != NULL)
  {
    calculateSolarPosition(getExtTimePtr());
    return Elevation * RAD_TO_DEG;
  }
  else
  {
    return 0;
  }
}

// Get Elevation for specified time
//
float SolarPosition::getSolarElevation(time_t t)
{
  calculateSolarPosition(t);
  return Elevation * RAD_TO_DEG;
}

// Get current Azimuth
//
float SolarPosition::getSolarAzimuth()
{
  if (getExtTimePtr != NULL)
  {
    calculateSolarPosition(getExtTimePtr());
    return Azimuth * RAD_TO_DEG;
  }
  else
  {
    return 0;
  }
}

// Get Azimuth for specified time
//
float SolarPosition::getSolarAzimuth(time_t t)
{
  calculateSolarPosition(t);
  return Azimuth * RAD_TO_DEG;
}

// Get current Solar distance in AU
//
float SolarPosition::getSolarDistance()
{
  if (getExtTimePtr != NULL)
  {
    calculateSolarPosition(getExtTimePtr());
    return solarDistance * KM_PER_AU;
  }
  else
  {
    return 0;
  }
}

// Get Solar distance in AU for specified time
//
float SolarPosition::getSolarDistance(time_t t)
{
  calculateSolarPosition(t);
  return solarDistance * KM_PER_AU;
}

//
// *** end of public methods: ***


// *** beginning of private functions ***
//

long SolarPosition::JulianDate(int year, int month, int day)
{
  long JD_whole;
  int A, B;
  if (month <= 2) {
    year--; month += 12;
  }
  A = year / 100; B = 2 - A + A / 4;
  JD_whole = (long)(365.25 * (year + 4716))
             + (int)(30.6001 * (month + 1)) + day + B - 1524;
  return JD_whole;
}

void SolarPosition::calculateSolarPosition(time_t tParam)
{

  const float DAYS_PER_JULIAN_CENTURY = 36525.0;
  const long Y2K_JULIAN_DAY = 2451545;

  tmElements_t timeCandidate;

  long JD_whole;
  long JDx;
  float JD_frac;
  float rightAscension;
  float Declination;
  float hourAngle;
  float GreenwichHourAngle;
  float elapsedT;
  float solarLongitude;
  float solarMeanAnomaly;
  float earthOrbitEccentricity;
  float sunCenter;
  float solarTrueLongitude;
  float solarTrueAnomaly;
  float equatorObliquity;

  if (tParam != timeCalculated) // only calculate if time has changed
  {
    breakTime(tParam, timeCandidate);
    JD_whole = JulianDate(tmYearToCalendar(timeCandidate.Year), timeCandidate.Month, timeCandidate.Day);
    JD_frac = (timeCandidate.Hour + timeCandidate.Minute / 60.0 + timeCandidate.Second / 3600.0) / 24.0 - 0.5;

    elapsedT = JD_whole - Y2K_JULIAN_DAY;
    elapsedT = (elapsedT + JD_frac) / DAYS_PER_JULIAN_CENTURY;

    solarLongitude = DEG_TO_RAD * fmod(280.46645 + 36000.76983 * elapsedT, 360);
    solarMeanAnomaly = DEG_TO_RAD * fmod(357.5291 + 35999.0503 * elapsedT, 360);
    earthOrbitEccentricity = 0.016708617 - 0.000042037 * elapsedT;

    sunCenter = DEG_TO_RAD * ((1.9146 - 0.004847 * elapsedT) * sin(solarMeanAnomaly)
                              + (0.019993 - 0.000101 * elapsedT) * sin(2 * solarMeanAnomaly)
                              + 0.00029 * sin(3 * solarMeanAnomaly));

    solarTrueAnomaly = solarMeanAnomaly + sunCenter;
    equatorObliquity = DEG_TO_RAD
                       * (23 + 26 / 60. + 21.448 / 3600. - 46.815 / 3600 * elapsedT);

    JDx = JD_whole - Y2K_JULIAN_DAY;

    GreenwichHourAngle = 280.46061837 + (360 * JDx) % 360 + .98564736629 * JDx
                         + 360.98564736629 * JD_frac;
    GreenwichHourAngle = fmod(GreenwichHourAngle, 360.0);

    solarTrueLongitude = fmod(sunCenter + solarLongitude, TWO_PI);

    rightAscension = atan2(sin(solarTrueLongitude) * cos(equatorObliquity), cos(solarTrueLongitude));

    Declination = asin(sin(equatorObliquity) * sin(solarTrueLongitude));
    hourAngle = DEG_TO_RAD * GreenwichHourAngle + Longitude - rightAscension;

    // results:
    solarDistance = 1.000001018 * (1 - earthOrbitEccentricity * earthOrbitEccentricity)
                    / (1 + earthOrbitEccentricity * cos(solarTrueAnomaly));

    // elevation from the horizon
    Elevation = asin(sin(Latitude) * sin(Declination) + cos(Latitude) * (cos(Declination) * cos(hourAngle)));

    // Azimuth measured eastward from north.
    Azimuth = PI + atan2(sin(hourAngle), cos(hourAngle) * sin(Latitude) - tan(Declination) * cos(Latitude));

    timeCalculated = tParam;
  }
}

// *** end of private functions ***
