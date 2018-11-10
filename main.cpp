#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <Weather.h>
#include <Settings.h>

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET); // SCL: D1, SDA: D2

Weather weather(OPEN_WEATHER_MAP_APP_ID);
WeatherData currentWeather;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// convert images to byte array: http://javl.github.io/image2cpp/
const unsigned char PROGMEM sun[] = {
  // size: 16x16
  0xfe, 0x7f, 0xfe, 0x7f, 0xce, 0x73, 0xc6, 0x63, 0xe8, 0x17, 0xf0, 0x0f, 0xf3, 0xcf, 0x03, 0xc0,
  0x03, 0xc0, 0xf3, 0xcf, 0xf0, 0x0f, 0xe8, 0x17, 0xc6, 0x63, 0xce, 0x73, 0xfe, 0x7f, 0xfe, 0x7f
};

void initializeWlan() {
  Serial.println("Connecting to Wi-Fi");
  display.println("Verbinde mit WLAN...");
  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.begin (WLAN_SSID, WLAN_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
}

void initializeDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
}

String replaceSpecialCharactersForLcd(String text) {
  // text is encoded in UTF-8, codes are for SSD1306 Adafruit library
  text.replace("\303\204", "\216"); // Ä
  text.replace("\303\226", "\231"); // Ö
  text.replace("\303\234", "\232"); // Ü
  text.replace("\303\244", "\204"); // ä
  text.replace("\303\266", "\224"); // ö
  text.replace("\303\274", "\201"); // ü
  text.replace("\303\237", "\340"); // ß
  text.replace("\302\260", "\367"); // °
  return text;
}

// prints the line and cut it to length of line if needed
void printLineCut(byte lineIndex, String text) {
  String replacedText = replaceSpecialCharactersForLcd(text);
  display.setCursor(0, lineIndex * 10);
  if (replacedText.length() > 20) {
    replacedText = replacedText.substring(0, 20);
  }
  display.print(replacedText);
}

void setup() {
  Serial.begin(115200);
  initializeDisplay();
  initializeWlan();
  pinMode(D7,INPUT);
}

int i = 0;
void loop() {
  if (i == 0) {
    // load only once
    currentWeather = weather.getCurrentWeather(WEATHER_CITY);
    i++;
  }

  display.clearDisplay();
  printLineCut(0, String(WEATHER_CITY) + ": " + String(currentWeather.temperatureCelsius) + "°");
  printLineCut(1, currentWeather.weatherDescription);
  display.display();
}
