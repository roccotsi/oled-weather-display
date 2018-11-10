/*
  Weather.h - Library for getting weather information from openweathermap.org.
  Created by Patrick Lenger, 2018
*/
#ifndef Weather_h
#define Weather_h

#include "Arduino.h"

typedef struct {
  int temperatureCelsius;
  String weatherDescription;
  String icon;
  String error;
  const unsigned char* iconBitmap;
  boolean iconBitmapSet;
} WeatherData;

class Weather
{
  public:
    Weather(String key);
    WeatherData getCurrentWeather(String city);
};

#endif
