# SolarPosition
Arduino Library to calculate the position of the sun relative to geographic coordinates

based on the program "Arduino Uno and Solar Position Calculations"
(c) David R. Brooks, which can be found at http://www.instesre.org/ArduinoDocuments.htm
and issued under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License:
https://creativecommons.org/licenses/by-nc-nd/4.0/

Dr. Brooks program uses equations are from the book,
"Jean Meeus, Astronomical Algorithms, Willmann-Bell, Inc., Richmond, VA".
A very important aspect is that straightforward floating point calculations for those equations
are likely to introduce significant errors with the 32 bit float type used on the Arduino.
So a special method of splitting Julian days into an integer and fractional part is used
to overcome that problem.

This adaptation introduces an object oriented interface, and a method of synchronizing the time
to a time source in a standard UNIX epoch format. Thus it is easily usable with any Arduino user
program.

It relies on the Time library, https://github.com/PaulStoffregen/Time
which provides convenient constants definitions and functions
which are not only useful in simplifying the library code, but can be used in the user program
to manipulate and display the time values associated with the solar locations.

# Usage

First ensure that the Time library is correctly installed.

Include this library:

    #include <SolarPosition.h>

In the setup() function, set the time provider if you are using the real time methods:

    SolarPosition::setTimeProvider(<name of time provider>);

For example:

    SolarPosition::setTimeProvider(RTC.get);

# Data Storage

If you want to store solar positions as aggregate data (i.e. as a whole), you should use
the struct datatype that is provided for that purpose:

```
struct SolarPosition_t
{
  float elevation = 0;
  float azimuth = 0;
  float distance = 0;
  time_t time = 0;
};
```

Once you declare a SolarPosition_t object, like this:

    SolarPosition_t somePosition;
    
you can assign sun position data to it, including the time associated with the position,
by using the method that returns an aggregate position like this:

    somePosition = someLocation.getSolarPosition();

Subsequently, you can access a member variable like this:

    azimuthVariable = somePosition.azimuth;
    
This is a convenient way to ensure that all the variables were created at the same time, if that
is important for the application.

# Accessing Data Directly

The above data structure is only necessary if you wish to store the results - if not, you can access members
directly, like this:

    azimuthVariable = someLocation.getSolarPosition().azimuth;

However, direct methods are provided for those members so the above can be stated more simply:

    azimuthVariable = someLocation.getSolarAzimuth();

All parameters of a position object are calculated at the same time. 
Before any solar calculation is performed internally, a check is made to see if the time has changed from the
previous calculation. If the results are available from the previous calculation, they will be used instead
of performing the calculations again. This avoids wasting time in redundant calculations.

# Utility Function

In case the geographical locations will be assigned dynamically or radians are preferred as an angular unit, or for any other reason the SolarPosition class is not preferred, direct access to the utility function calculateSolarPosition() is permitted. See the example sketch solarTimeFunctionDemo for details.
    calculateSolarPosition()

# Time parameter

The mathematical formulas in the library base all calculations on UTC time (otherwise known as GMT).
Therefore, all times must be in UTC, and any time provider sources such as RTC ICs must be configured
to provide UTC time. If local time is needed, it is recommended to perform a conversion separately from
the workings of this library.

All methods in this library use a time_t type time for calculations. If the parameter is not passed, then
the time source that has been specified by the setTimeProvider() method will be used. This makes it
easy to work with either real time, or arbitrary past or future times.

If the real time methods are called, a time provider function must first be selected once with the
setTimeProvider() method. The time provider is shared between all objects. Failing to do so will not
crash the program, but invalid results will be returned. Once it is set, methods that take no
time value as a parameter, will automatically get the time from the time provider. In this case,
the methods return real time values. It is not necessary to use a time provider if the fixed
time methods are called, since the time parameter is passed to them explicitly.

# Constructor

<H2>SolarPosition(Latitude, Longitude)</H2>

    SolarPosition(float Latitude, float Longitude);
create a location given Latitude and Longitude in decimal degrees

# Public Methods

<H2>SolarPosition::setTimeProvider(<time provider function pointer>)</H2>

    static void setTimeProvider(getExternalTime getTimeFunction);

<H2>getSolarPosition()</H2>

    SolarPosition_t getSolarPosition();
    SolarPosition_t getSolarPosition(time_t t);
return a complete solar position structure
See the description of struct SolarPosition_t for information about the data members

<H2>getSolarElevation()</H2>

    float getSolarElevation();
    float getSolarElevation(time_t t);
return solar elevation in decimal degrees

<H2>getSolarAzimuth()</H2>

    float getSolarAzimuth();
    float getSolarAzimuth(time_t t);
return solar azimuth in decimal degrees

<H2>getSolarDistance()</H2>

    float getSolarDistance();
    float getSolarDistance(time_t t);
return solar distance (from center of the earth to the sun) in kilometers
