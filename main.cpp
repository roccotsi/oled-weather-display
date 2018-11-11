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
unsigned long lastWeatherUpdateMillis = 0;
unsigned long updateWeatherIntervalMillis = WEATHER_UPDATE_INTERVAL_MILLIS;
int textSize = 1;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setTextSize(int size) {
  textSize = size;
  display.setTextSize(size);
}

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
  setTextSize(1);
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
  display.setCursor(0, lineIndex * 10 * textSize);
  if (replacedText.length() > 20) {
    replacedText = replacedText.substring(0, 20);
  }
  display.print(replacedText);
}

boolean updateWeatherData() {
  // check if new update from job is needed
  unsigned long currentMillis = millis();
  unsigned long intervalSinceLastUpdate = currentMillis - lastWeatherUpdateMillis;

  if (lastWeatherUpdateMillis == 0 || intervalSinceLastUpdate > updateWeatherIntervalMillis) {
    currentWeather = weather.getCurrentWeather(WEATHER_CITY);
    lastWeatherUpdateMillis = currentMillis;
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  initializeDisplay();
  initializeWlan();
  pinMode(D7,INPUT);
}

void loop() {
  if (updateWeatherData()) {
    display.clearDisplay();
    setTextSize(2);
    printLineCut(0, String(WEATHER_CITY));
    if (currentWeather.iconBitmapSet) {
      printLineCut(1, String(currentWeather.temperatureCelsius) + "°");
      display.drawBitmap(78, 18, currentWeather.iconBitmap, 50, 50, WHITE);
    } else {
      // no bitmap found, so print text and icon name
      printLineCut(1, String(currentWeather.temperatureCelsius) + "° (" + currentWeather.icon + ")");
      printLineCut(2, currentWeather.weatherDescription);
    }
    display.display();
  }
}
