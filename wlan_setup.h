/*
  wlan_setup.h - Library for setting up wlan
  Supports static wlan ssid/password and provides also HTML configuration page with ssid/password in flash memory
  Created by Patrick Lenger, 2018
*/
#ifndef wlan_setup_h
#define wlan_setup_h

#include "Arduino.h"

class WlanSetup
{
  public:
    WlanSetup();
    bool connect(String ssid, String password, unsigned int timeoutSec);
};

#endif
