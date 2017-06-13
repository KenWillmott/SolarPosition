// SolarPosition.h

// 2017 Ken Willmott
// Arduino library based on the program "Arduino Uno and Solar Position Calculations"
// (c) David R. Brooks, which can be found at http://www.instesre.org/ArduinoDocuments.htm
// and issued under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License:
// https://creativecommons.org/licenses/by-nc-nd/4.0/

#include <TimeLib.h>            //https://github.com/PaulStoffregen/Time
#include <arduino.h>

const float KM_PER_AU = 149597870.7;  //kilometers per astronomical unit

typedef time_t(*getExternalTime)();

struct SolarPosition_t
{
  float elevation = 0;
  float azimuth = 0;
  float distance = 0;
  time_t time = 0;
};

class SolarPosition
{
  private:

    static getExternalTime getExtTimePtr;  // shared pointer to external sync function
    time_t timeCalculated = 0;

    // current values:
    float Latitude;
    float Longitude;

    // results:
    float Elevation; // in radians
    float Azimuth; // in radians
    float solarDistance;  // in AU

    long JulianDate(int year, int month, int day);
    void calculateSolarPosition(time_t tParam);

  public:

    SolarPosition(float Latitude, float Longitude);

    static void setTimeProvider(getExternalTime getTimeFunction);

    SolarPosition_t getSolarPosition();
    SolarPosition_t getSolarPosition(time_t t);

    float getSolarElevation();
    float getSolarElevation(time_t t);

    float getSolarAzimuth();
    float getSolarAzimuth(time_t t);

    float getSolarDistance();
    float getSolarDistance(time_t t);
};

