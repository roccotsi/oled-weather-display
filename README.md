# oled-weather-display
Displays weather information with an ESP8266 (NodeMCU)

Used hardware:
* Nodemcu (ESP8266 board)
* OLED 0,96 inch display (connected via I2C)

Used libraries:
* SPI
* Wire
* Adafruit_GFX
* Adafruit_SSD1306
* Arduino
* ArduinoJson
* ESP8266WiFi
* ESP8266HTTPClient

Before compiling, configure:
* In settings.h: WLAN, weather api key and city

Connection of display on NodeMCU (via I2C):
* GND: Pin G
* VCC: Pin 3V
* SDA: Pin D2
* SCL: Pin D1