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
`#include <SolarPosition.h>`

In the setup() function, set the time provider if you are using the real time methods:

`SolarPosition::setTimeProvider(<name of time provider>);`

For example:
`SolarPosition::setTimeProvider(RTC.get);`

