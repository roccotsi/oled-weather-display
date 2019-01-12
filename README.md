# oled-weather-display
Displays weather information with an ESP8266 (NodeMCU) on an OLED display

Used hardware:
* Nodemcu (ESP8266 board)
* OLED 0,96 inch display (connected via I2C)

Used libraries:
* SPI
* Wire
* Adafruit_GFX (Adafruit GFX Library by Adafruit)
* Adafruit_SSD1306 (Adafruit SSD1306 by Adafruit)
* Arduino
* ArduinoJson (ArduinoJson by Benoit Blanchon)
* ESP8266WiFi
* ESP8266HTTPClient

Before compiling, configure:
* In settings.h: WLAN, weather api key and city

Connection of display on NodeMCU (via I2C):
* GND: Pin G
* VCC: Pin 3V
* SDA: Pin D2
* SCL: Pin D1

Connect button to PIN D7 with pulldown resistor (10kOhm) to GND.
