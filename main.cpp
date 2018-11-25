#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <weather.h>
#include <settings.h>
#include <wlan_setup.h>

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET); // SCL: D1, SDA: D2

WlanSetup wlanSetup = WlanSetup();
Weather weather(OPEN_WEATHER_MAP_APP_ID);
WeatherData currentWeather;
ForecastWeatherData forecastWeather;
unsigned long lastWeatherUpdateMillis = 0;
unsigned long updateWeatherIntervalMillis = WEATHER_UPDATE_INTERVAL_MILLIS;
int displayWaitTime = 5000; // 5 seconds per page
int textSize = 1;
bool displayOn = true;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setTextSize(int size) {
  textSize = size;
  display.setTextSize(size);
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

bool initializeWlan() {
  setTextSize(1);
  Serial.println("Connecting to Wi-Fi");
  printLineCut(0, "Verbinde mit WLAN...");
  display.display();

  // WiFi.mode(WIFI_STA);
  // WiFi.begin (WLAN_SSID, WLAN_PASSWORD);
  // while (WiFi.status() != WL_CONNECTED) {
  //   Serial.print(".");
  //   delay(500);
  // }
  bool successful = wlanSetup.connect(WLAN_SSID, WLAN_PASSWORD, 10);
  if (successful) {
    Serial.println("WiFi connected");
    return true;
  } else {
    Serial.println("WiFi not connected");
    display.clearDisplay();
    printLineCut(0, "Verbindung fehlgeschlagen");
    display.display();
    delay(displayWaitTime);
    return false;
  }
}

void reconnectWlan() {
  initializeWlan();
  if ((WiFi.status() != WL_CONNECTED)) {
    display.clearDisplay();
    setTextSize(2);
    printLineCut(0, "Kein WLAN");
    display.display();
    delay(displayWaitTime);
  }
}

boolean updateWeatherData() {
  // check if new update from job is needed
  unsigned long currentMillis = millis();
  unsigned long intervalSinceLastUpdate = currentMillis - lastWeatherUpdateMillis;

  if (lastWeatherUpdateMillis == 0 || intervalSinceLastUpdate > updateWeatherIntervalMillis) {
    currentWeather = weather.getCurrentWeather(WEATHER_CITY);
    forecastWeather = weather.getForecastWeather(WEATHER_CITY);
    Serial.println("Weather updated");
    lastWeatherUpdateMillis = currentMillis;
    return true;
  }
  return false;
}

void displayCurrentWeather() {
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

void displayForecastWeather() {
  int index = 0;
  while (index < 4) {
    display.clearDisplay();
    setTextSize(2);
    printLineCut(0, forecastWeather.time[index]);
    if (forecastWeather.weather[index].iconBitmapSet) {
      printLineCut(1, String(forecastWeather.weather[index].temperatureCelsius) + "°");
      display.drawBitmap(78, 18, forecastWeather.weather[index].iconBitmap, 50, 50, WHITE);
    } else {
      // no bitmap found, so print text and icon name
      printLineCut(1, String(forecastWeather.weather[index].temperatureCelsius) + "° (" + forecastWeather.weather[index].icon + ")");
      printLineCut(2, forecastWeather.weather[index].weatherDescription);
    }
    display.display();
    delay(displayWaitTime);
    index++;
  }
}

void turnDisplayOnOff(bool on) {
  displayOn = on;
  if (on) {
    display.ssd1306_command(SSD1306_DISPLAYON);
  } else {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }
}

void setup() {
  Serial.begin(115200);
  initializeDisplay();
  initializeWlan();
  pinMode(D7,INPUT);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    updateWeatherData();
    if (displayOn) {
      displayCurrentWeather();
      delay(displayWaitTime);
      displayForecastWeather();
      turnDisplayOnOff(false);
    }
    int val = digitalRead(D7);
    if (val == 1) {
      turnDisplayOnOff(true);
    }
  } else {
    reconnectWlan();
  }
}
