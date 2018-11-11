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

typedef struct {
  // 9:00, 15:00, 21:00, 3:00
  WeatherData weather[4];
  String time[4]; // store the time by array index (corresponds to index of weather)
  String error;
} ForecastWeatherData;

class Weather
{
  public:
    Weather(String key);
    WeatherData getCurrentWeather(String city);
    ForecastWeatherData getForecastWeather(String city);
};

#endif
