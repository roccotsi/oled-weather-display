/*
  Weather.h - Library for getting weather information from openweathermap.org.
  Created by Patrick Lenger, 2018
*/
#include "Arduino.h"
#include "Weather.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

String _key;
const char* WEATHER_URL_TEMPLATE = "http://api.openweathermap.org/data/2.5/weather?units=metric&lang=de&q=%1,de&appid=%2";
const char* FORECAST_URL_TEMPLATE = "http://api.openweathermap.org/data/2.5/forecast?units=metric&lang=de&q=%1,de&appid=%2&cnt=10";

Weather::Weather(String key)
{
  _key = key;
}

String callUrlAndGetResponse(String url) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    Serial.println("URL: " + url);
    http.begin(url);
    int httpCode = http.GET();
    String payload;
    if (httpCode > 0) {
      payload = http.getString();
    }

    http.end();   //Close connection
    Serial.println("HTTP connection closed");
    return payload;
  }
  Serial.println("Not connected to WLAN");
  return "";
}

WeatherData Weather::getCurrentWeather(String city)
{
  WeatherData weatherData;

  if (city.length() == 0) {
    weatherData.error = "no city specified";
    return weatherData;
  }
  String url = WEATHER_URL_TEMPLATE;
  url.replace("%1", city);
  url.replace("%2", _key);

  String payload = callUrlAndGetResponse(url);
  if (payload.length() == 0) {
    weatherData.error = "No weather data received";
    return weatherData;
  }
  StaticJsonBuffer<2000> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(payload);
  if (!root.success()) {
    weatherData.error = "Failed to read JSON";
    return weatherData;
  }
  Serial.println("JSON parsed");
  String degree = root["main"]["temp"];
  int degreeInt = degree.toInt();
  Serial.println("degree: " + String(degreeInt));
  String weather = root["weather"][0]["description"];
  Serial.println("weather: " + weather);
  String icon = root["weather"][0]["icon"];
  Serial.println("weather: " + icon);
  weatherData.error = "";
  weatherData.temperatureCelsius = degreeInt;
  weatherData.weatherDescription = weather;
  weatherData.icon = icon;
  return weatherData;
}
