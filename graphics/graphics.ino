#include "variable.h"
#include "graphic_handler.h"
#include "my_RTC.h"
#include "weather.h"
#include "myBtn.h"
#include "menu.h"
#include <WiFiManager.h>
#include <DNSServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <ArduinoNvs.h>
#include "source_web.h"
DNSServer dnsServer;
AsyncWebServer server1(80);
const IPAddress apIP(192, 168, 1, 1);
const char *apSSID = "Smart Clock";

#define PIN_INPUT 21

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 25200;
const int daylightOffset_sec = 0;

WiFiManager wm;  // global wm instance

MenuMember mn[6] = { { "Config WiFi", 20, 100, 80, 240, false },
                     { "Update GPS", 20, 130, 110, 240, false },
                     { "Set Alarm", 20, 160, 140, 240, false },
                     { "Reset System", 20, 190, 170, 240, false },
                     { "About", 20, 220, 200, 240, false },
                     { "Back", 97, 270, 250, 240, false } };

MenuMember wfmn[2] = { { "Retry", 20, 100, 80, 240, false },
                       { "New WiFi", 20, 130, 110, 240, true } };

MenuMember gpsmn[3] = { { "Update", 20, 100, 80, 240, false },
                        { "GPS Info", 20, 130, 110, 240, false },
                        { "Back", 97, 270, 250, 240, false } };

MenuMember mnconfig[4] = { { "Reconnect", 20, 100, 80, 240, false },
                           { "Reset WiFi", 20, 130, 110, 240, false },
                           { "WiFi Info", 20, 160, 140, 240, false },
                           { "Back", 97, 270, 250, 240, false } };

OWM_Weather owm;
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
Button button(PIN_INPUT);

void setup(void) {
  Serial.begin(9600);
  NVS.begin();
  std::vector<const char *> menu = { "wifi", "info", "param", "sep", "restart", "exit" };
  wm.setMenu(menu);
  wm.setClass("invert");
  g_api = NVS.getString("api");
  g_lat = NVS.getString("lat");
  g_long = NVS.getString("long");
  owm.setApiKey(g_api);
  // wm.resetSettings();
  button.initFunction(&myClickFunction, &myClick2Function);
  initSystem();
}

void loop() {
  button.handle();
  if (state == CLOCK) {
    if (WiFi.status() != WL_CONNECTED) {
      if (connected) {
        graphicPrintTextPosition("WiFi disconnected!", 35, 130, ST77XX_RED);
        connected = false;
      }
    } else {
      connected = true;
      if (b_initClockMode == false) {
        graphicInitClockMode();
        b_initClockMode = true;
      }
      clockMode();
    }
  } else {
    connected = true;
    if (b_initSettingMode == false) {
      initSetting();
      b_initSettingMode = true;
    }
    setting();
  }
  test_Image();
}

void initMenuGPS() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GRAY);
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(2);
  tft.setCursor(80, 40);
  tft.print("GPS");
  tft.setTextSize(1);
  for (int i = 0; i < 3; i++) {
    deSelect(&gpsmn[i], true);
  }
}

void subMenuGPS() {
  switch (menuGPSIndex) {
    case 1:
      deSelect(&gpsmn[2], false);
      select(&gpsmn[0], false);
      break;
    case 2:
      deSelect(&gpsmn[0], false);
      select(&gpsmn[1], false);
      break;
    case 3:
      deSelect(&gpsmn[1], false);
      select(&gpsmn[2], false);
      break;
  }
}

void menuGPS() {
  bool check;
  switch (subMenuGPSIndex) {
    case 0:
      subMenuGPS();
      break;
    case 1:
      tft.setTextWrap(true);
      graphicPrintTextPosition("The GPS is updating. Connect the Wi-Fi name Smart Clock to the setting. After a successful update, the system will reboot", 30, 30, ST77XX_GREEN);
      tft.fillRect(0, 250, 240, 30, ST77XX_ORANGE);
      tft.setTextColor(ST77XX_DARK_BLUE);
      tft.setCursor(97, 270);
      tft.print("Back");
      changeGPS = true;
      updateGPS();
      WiFi.mode(WIFI_STA);
      graphicPrintTextPosition("Reconnect...", 68, 130, ST77XX_YELLOW);
      wm.setConnectTimeout(5);
      wm.setConfigPortalTimeout(5);
      check = wm.autoConnect("Smart Clock", "");
      if (!check) {
        graphicPrintTextPosition("WiFi disconnected!", 35, 130, ST77XX_RED);
        delay(2000);
      }
      initMenuGPS();
      subMenuGPSIndex = 0;
      break;
    case 2:
      if (subMenuGPSIndex == -1) return;
      tft.fillScreen(ST77XX_BLACK);
      graphicPrintTextPositionNoClear("Lat: ", 0, 100, ST77XX_GREEN);
      graphicPrintStringPosition(g_lat, 60, 100, ST77XX_GREEN);
      graphicPrintTextPositionNoClear("Long: ", 0, 125, ST77XX_GREEN);
      graphicPrintStringPosition(g_long, 60, 125, ST77XX_GREEN);
      graphicPrintTextPositionNoClear("Location: ", 0, 150, ST77XX_GREEN);
      graphicPrintStringPosition(city, 85, 150, ST77XX_GREEN);
      tft.fillRect(0, 250, 240, 30, ST77XX_ORANGE);
      tft.setTextColor(ST77XX_DARK_BLUE);
      tft.setCursor(97, 270);
      tft.print("Back");
      subMenuGPSIndex = -1;
      break;
  }
}

void initMenuWiFi() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GRAY);
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(2);
  tft.setCursor(80, 40);
  tft.print("WIFI");
  tft.setTextSize(1);
  for (int i = 0; i < 4; i++) {
    deSelect(&mnconfig[i], true);
  }
}

void subMenuWiFi() {
  switch (menuWiFiIndex) {
    case 1:
      deSelect(&mnconfig[3], false);
      select(&mnconfig[0], false);
      break;
    case 2:
      deSelect(&mnconfig[0], false);
      select(&mnconfig[1], false);
      break;
    case 3:
      deSelect(&mnconfig[1], false);
      select(&mnconfig[2], false);
      break;
    case 4:
      deSelect(&mnconfig[2], false);
      select(&mnconfig[3], false);
      break;
  }
}

void menuWiFi() {
  switch (subMenuWifiIndex) {
    case 0:
      subMenuWiFi();
      break;
    case 1:
      graphicPrintTextPosition("Reconnect...", 68, 130, ST77XX_YELLOW);
      delay(1000);
      wm.setConfigPortalTimeout(20);
      if (wm.autoConnect("Smart Clock AP", "")) {
        graphicPrintTextPosition("Connected!", 68, 130, ST77XX_GREEN);
        delay(1000);
      } else {
        graphicPrintTextPosition("Failed...", 80, 130, ST77XX_RED);
        delay(1000);
      }
      initMenuWiFi();
      subMenuWifiIndex = 0;
      break;
    case 2:
      graphicPrintTextPosition("Reset successfully", 40, 130, ST77XX_GREEN);
      delay(1000);
      wm.resetSettings();
      initMenuWiFi();
      subMenuWifiIndex = 0;
      break;
    case 3:
      String id = wm.getWiFiSSID(true);
      tft.fillScreen(ST77XX_BLACK);
      graphicPrintStringPosition("Device is connecting wifi:", 0, 80, ST77XX_GREEN);
      graphicPrintStringPosition(id, 0, 110, ST77XX_GREEN);
      delay(2000);
      initMenuWiFi();
      subMenuWifiIndex = 0;
      break;
  }
}

void setting() {
  switch (subMenuIndex) {
    case 0:
      mainMenu();
      break;
    case 1:
      if (b_initSubMenuWifi == false) {
        initMenuWiFi();
        b_initSubMenuWifi = true;
      }
      menuWiFi();
      break;
    case 2:
      if (b_initSubMenuGPS == false) {
        initMenuGPS();
        b_initSubMenuGPS = true;
      }
      menuGPS();
      break;
    case 3:
      commingSoon();
      break;
    case 4:
      graphicPrintTextPosition("System will be restart", 25, 130, ST77XX_YELLOW);
      delay(3000);
      ESP.restart();
      break;
    case 5:
      About();
      break;
  }
}

void initSetting() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GRAY);
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(2);
  tft.setCursor(40, 40);
  tft.print("SETTING");
  tft.setTextSize(1);
  for (int i = 0; i < 6; i++) {
    deSelect(&mn[i], true);
  }
}

void mainMenu() {
  switch (menuIndex) {
    case 1:
      deSelect(&mn[5], false);
      select(&mn[0], false);
      break;
    case 2:
      deSelect(&mn[0], false);
      select(&mn[1], false);
      break;
    case 3:
      deSelect(&mn[1], false);
      select(&mn[2], false);
      break;
    case 4:
      deSelect(&mn[2], false);
      select(&mn[3], false);
      break;
    case 5:
      deSelect(&mn[3], false);
      select(&mn[4], false);
      break;
    case 6:
      deSelect(&mn[4], false);
      select(&mn[5], false);
      break;
  }
}

void clockMode() {
  if (millis() - getTimeInterval > 1000) {
    getLocalTime(&localHour, &localMin, &local_dofw, &local_dofm, &local_mon, &local_year);
    graphicTimeUpdate(localHour, localMin, false);
    graphicDayUpdate(local_dofw, local_dofm, local_mon, local_year, false);
    getTimeInterval = millis();
  }

  if (millis() - getWeatherInterval > (3 * 60 * 1000) || firtGet == false) {
    WeatherInfo wt = getweather();
    firtGet = true;
    getWeatherInterval = millis();
    graphicUpdateTemAndHum(wt.temperature, wt.humidity, false);
    graphicWeatherUpdate(wt.status, localHour);
  }
  // graphicWeatherUpdate(weather, h);
}
void myClick2Function() {
  if (connection == false) {
    if (reConnect)
      connectSelection = 1;
    else
      connectSelection = 2;
    return;
  }
  if (changeGPS) {
    changeGPS = false;
    return;
  }
  if (subMenuIndex == -1) {
    b_initSettingMode = false;
    subMenuIndex = 0;
    return;
  }
  if (subMenuGPSIndex == -1) {
    b_initSubMenuGPS = false;
    subMenuGPSIndex = 0;
    return;
  }
  if (b_initSubMenuWifi) {
    switch (menuWiFiIndex) {
      case 1:
        subMenuWifiIndex = 1;
        return;
      case 2:
        subMenuWifiIndex = 2;
        return;
      case 3:
        subMenuWifiIndex = 3;
        return;
      case 4:
        b_initSettingMode = false;
        b_initSubMenuWifi = false;
        subMenuIndex = 0;
        subMenuWifiIndex = 0;
        menuWiFiIndex = 0;
        return;
    }
  }
  if (b_initSubMenuGPS) {
    switch (menuGPSIndex) {
      case 1:
        subMenuGPSIndex = 1;
        return;
      case 2:
        subMenuGPSIndex = 2;
        return;
      case 3:
        b_initSettingMode = false;
        b_initSubMenuGPS = false;
        subMenuIndex = 0;
        subMenuGPSIndex = 0;
        menuGPSIndex = 0;
        return;
    }
  }
  switch (menuIndex) {
    case 1:
      subMenuIndex = 1;
      break;
    case 2:
      subMenuIndex = 2;
      break;
    case 3:
      subMenuIndex = 3;
      break;
    case 4:
      subMenuIndex = 4;
      break;
    case 5:
      subMenuIndex = 5;
      break;
    case 6:
      state = CLOCK;
      b_initClockMode = false;
      menuIndex = -1;
      break;
  }
}

void myClickFunction() {
  if (connection == false) {
    reConnect = !reConnect;
    return;
  }
  if (state == CLOCK) {
    state = SETTING;
  }
  if (state == SETTING && subMenuIndex == 0) {
    if (menuIndex == -1) {
      state = SETTING;
      b_initSettingMode = false;
    }
    menuIndex += 1;
    if (menuIndex == 7)
      menuIndex = 1;
  }
  if (state == SETTING && b_initSubMenuWifi == true) {
    if (menuWiFiIndex == -1) {
      b_initSubMenuWifi = false;
    }
    menuWiFiIndex += 1;
    if (menuWiFiIndex == 5)
      menuWiFiIndex = 1;
  }
  if (state == SETTING && b_initSubMenuGPS == true) {
    if (menuGPSIndex == -1) {
      b_initSubMenuGPS = false;
    }
    menuGPSIndex += 1;
    if (menuGPSIndex == 4)
      menuGPSIndex = 1;
  }
}

void initSystem() {
  graphicLoading();
  delay(3000);
  graphicPrintTextPosition("Connecting WiFi", 46, 130, ST77XX_GRAY);
  wm.setConnectTimeout(10);
  wm.setConfigPortalTimeout(10);
  connection = wm.autoConnect("Smart Clock AP", "");
  while (1) {
    if (connection) {
      graphicPrintTextPosition("WiFi Connected!", 46, 130, ST77XX_GREEN);
      delay(2000);
      graphicPrintTextPosition("Waiting to setup time", 30, 130, ST77XX_GREEN);
      delay(2000);
      setUpTime();
      unsigned long now = millis();
      while (!getLocalTime(&localHour, &localMin, &local_dofw, &local_dofm, &local_mon, &local_year)) {
        if (millis() - now > 30000) {
          ESP.restart();
        }
      }
      break;
    } else {
      button.handle();
      graphicPrintTextPositionNoClear("WiFi Connect Failed!", 30, 30, ST77XX_RED);
      if (reConnect) {
        if (connectSelection == 1) {
          ESP.restart();
        }
        select(&wfmn[0], false);
        deSelect(&wfmn[1], false);
      } else {
        if (connectSelection == 2) {
          wm.setConfigPortalTimeout(30);
          graphicPrintTextPosition("Reconnect...", 68, 130, ST77XX_YELLOW);
          connection = wm.startConfigPortal("Smart Clock AP", "");
        }
        select(&wfmn[1], false);
        deSelect(&wfmn[0], false);
      }
    }
  }
  tft.fillScreen(ST77XX_BLACK);
  Paint_DrawImage(gImage_tem, 20, 180, 50, 50);
  Paint_DrawImage(gImage_hum, 20, 225, 50, 50);
}

void test_Image() {
  if (Serial.available() > 0) {
    weather = Serial.readStringUntil('\n');
    Serial.println(weather);
  }
}
void graphicInitClockMode() {
  tft.fillScreen(ST77XX_BLACK);
  getLocalTime(&localHour, &localMin, &local_dofw, &local_dofm, &local_mon, &local_year);
  WeatherInfo wt = getweather();
  graphicTimeUpdate(localHour, localMin, true);
  graphicDayUpdate(local_dofw, local_dofm, local_mon, local_year, true);
  graphicUpdateTemAndHum(wt.temperature, wt.humidity, true);
  graphicWeatherUpdate(wt.status, localHour);
  Paint_DrawImage(gImage_tem, 20, 180, 50, 50);
  Paint_DrawImage(gImage_hum, 20, 225, 50, 50);
}

void notFound(AsyncWebServerRequest *request) {
  String s = form + "<input type=\"text\" placeholder=\"API\" name=\"api\" value=\"" + g_api + "\"/>" + "<input type=\"text\" placeholder=\"LATITUDE\" name=\"lat\" value=\"" + g_lat + "\"/>" + "<input type=\"text\" placeholder=\"LONGITUDE\" name=\"long\" value=\"" + g_long + "\"/>" + end_form;
  request->send(200, "text/html", s);
}

void setwifi(AsyncWebServerRequest *request) {
  String apiKey = request->arg("api");
  String lat_ = request->arg("lat");
  String long_ = request->arg("long");
  String s;
  Serial.print("API: ");
  Serial.println(apiKey);
  NVS.setString("api", apiKey);
  Serial.print("LAT: ");
  Serial.println(lat_);
  NVS.setString("lat", lat_);
  Serial.print("LONG: ");
  Serial.println(long_);
  NVS.setString("long", long_);
  s = set_complete;
  request->send(200, "text/html", s);
}

void updateGPS() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID, "");
  dnsServer.start(53, "*", apIP);
  Serial.print("Starting Web Server at ");
  Serial.println(WiFi.softAPIP());
  server1.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", form);
    ESP.restart();
  });
  server1.on("/setgps", HTTP_GET, setwifi);
  server1.onNotFound(notFound);
  Serial.print("Starting Access Point at \"");
  Serial.print(apSSID);
  Serial.println("\"");
  server1.begin();
  while (1) {
    button.handle();
    dnsServer.processNextRequest();
    server1.begin();
    if (!changeGPS) {
      return;
    }
  }
}
