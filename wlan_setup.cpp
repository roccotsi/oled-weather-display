/*
  wlan_setup.h - Library for setting up wlan
  Supports static wlan ssid/password and provides also HTML configuration page with ssid/password in flash memory
  Created by Patrick Lenger, 2018
*/
#include "Arduino.h"
#include "wlan_setup.h"
#include <ESP8266WiFi.h>

WlanSetup::WlanSetup()
{
}

bool WlanSetup::connect(String ssid, String password, unsigned int timeoutSec) {
  unsigned long startMillis = millis();
  unsigned long currentMillis = millis();
  WiFi.mode(WIFI_STA);
  WiFi.begin (ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if ((currentMillis - startMillis) > timeoutSec * 1000) {
      return false;
    }
    delay(200);
  }
  return true;
}
