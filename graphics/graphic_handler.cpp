#include "graphic_handler.h"

void Paint_DrawImage(const unsigned char* image, uint16_t xStart, uint16_t yStart, uint16_t W_Image, uint16_t H_Image) {
  int i, j;
  for (j = 0; j < H_Image; j++) {
    for (i = 0; i < W_Image; i++) {
      if (xStart + i < 240 && yStart + j < 280)
        tft.drawPixel(xStart + i, yStart + j, (pgm_read_byte(image + j * W_Image * 2 + i * 2 + 1)) << 8 | (pgm_read_byte(image + j * W_Image * 2 + i * 2)));
    }
  }
}

void graphicTimeUpdate(int hour, int min, bool update) {
  static int preHour, preMin = 0;
  if (hour != preHour || min != preMin || update == true) {
    tft.setFont(&Dialog_bold_20);
    tft.setTextSize(3);

    tft.setTextColor(ST77XX_BLACK);
    tft.setCursor(140, 115);
    tft.print((preHour < 10) ? ("0" + static_cast<String>(preHour)) : static_cast<String>(preHour));
    tft.setCursor(140, 175);
    tft.print((preMin < 10) ? ("0" + static_cast<String>(preMin)) : static_cast<String>(preMin));

    tft.setTextColor(ST77XX_GRAY);
    tft.setCursor(140, 115);
    tft.print((hour < 10) ? ("0" + static_cast<String>(hour)) : static_cast<String>(hour));
    tft.setCursor(140, 175);
    tft.print((min < 10) ? ("0" + static_cast<String>(min)) : static_cast<String>(min));

    preHour = hour;
    preMin = min;
  }
}

void graphicDayUpdate(int dow, int dom, int mon, int year, bool update) {
  static int preDow, preDom, preMon, preYear = 0;
  int16_t x1, y1, x2, y2;
  uint16_t w2, h2;
  uint16_t w1, h1;
  int startX1, startX2;
  if (preDow != dow || preDom != dom || preMon != mon || preYear != year || update == true) {
    String preTime = ((preDom < 10) ? ("0" + (String)preDom) : (String)preDom) + " - " + ((preMon < 10) ? ("0" + (String)preMon) : (String)preMon) + " - " + (String)year;
    String nowTime = ((dom < 10) ? ("0" + (String)dom) : (String)dom) + " - " + ((mon < 10) ? ("0" + (String)mon) : (String)mon) + " - " + (String)year;
    tft.setFont(&FreeSansBold9pt7b);
    tft.setTextSize(1);

    tft.setTextColor(ST77XX_BLACK);

    tft.getTextBounds(dayOfWeek[preDow], 0, 0, &x1, &y1, &w1, &h1);
    startX1 = (240 - w1) / 2;
    tft.setCursor(startX1, 20);
    tft.print(dayOfWeek[preDow]);

    tft.getTextBounds(preTime, 0, 0, &x2, &y2, &w2, &h2);
    startX2 = (240 - w2) / 2;
    tft.setCursor(startX2, 50);
    tft.print(preTime);

    tft.setTextColor(ST77XX_LIGHT_YELLOW);
    tft.getTextBounds(dayOfWeek[dow], 0, 0, &x1, &y1, &w1, &h1);
    startX1 = (240 - w1) / 2;
    tft.setCursor(startX1, 20);
    tft.print(dayOfWeek[dow]);

    tft.getTextBounds(nowTime, 0, 0, &x2, &y2, &w2, &h2);
    startX2 = (240 - w2) / 2;
    tft.setCursor(startX2, 50);
    tft.print(nowTime);

    preDow = dow;
    preDom = dom;
    preMon = mon;
    preYear = year;
  }
}

void graphicWeatherUpdate(String status, int hour) {
  status.toLowerCase();
  if (status == "clear sky") {
    if (hour < 18 && hour >= 6)
      Paint_DrawImage(gImage_sun, 20, 70, 100, 100);
    else
      Paint_DrawImage(gImage_moon, 20, 70, 100, 100);
  } else if (status == "few clouds") {
    if (hour < 18 && hour >= 6)
      Paint_DrawImage(gImage_few_cloud, 20, 70, 100, 100);
    else
      Paint_DrawImage(gImage_moon_few_cloud, 20, 70, 100, 100);
  } else if (status == "scattered clouds") {
    Paint_DrawImage(gImage_scattered_cloud, 20, 70, 100, 100);
  } else if (status == "broken clouds" || status == "overcast clouds") {
    Paint_DrawImage(gImage_broken_cloud, 20, 70, 100, 100);
  } else if (status.indexOf("shower rain") != -1 || status.indexOf("thunderstorm") != -1 || status.indexOf("drizzle") != -1) {
    Paint_DrawImage(gImage_shower_rain, 20, 70, 100, 100);
  } else if (status.indexOf("rain") != -1) {
    Paint_DrawImage(gImage_rain, 20, 70, 100, 100);
  }
}

void graphicUpdateTemAndHum(int tem, int hum, bool update) {
  static int preTem, preHum = 0;
  if (tem != preTem || hum != preHum || update == true) {
    tft.setFont(&Dialog_bold_20);
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_BLACK);

    tft.setCursor(90, 220);
    tft.print((String)preTem);
    tft.drawCircle(188, 190, 4, ST77XX_BLACK);
    tft.setCursor(195, 220);
    tft.print("C");

    tft.setCursor(90, 265);
    tft.print((String)preHum);
    tft.setCursor(180, 265);
    tft.print("%");

    tft.setTextColor(ST77XX_ORANGE);
    tft.setCursor(90, 220);
    tft.print((String)tem);
    tft.drawCircle(188, 190, 4, ST77XX_ORANGE);
    tft.setCursor(195, 220);
    tft.print("C");

    tft.setTextColor(ST77XX_BLUE);
    tft.setCursor(90, 265);
    tft.print((String)hum);
    tft.setCursor(180, 265);
    tft.print("%");

    preTem = tem;
    preHum = hum;
  }
}

void graphicLoading() {
  tft.init(240, 280);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GRAY);
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(2);
  tft.setCursor(55, 120);
  tft.print("SMART");
  tft.setCursor(55, 160);
  tft.print("CLOCK");
}

void graphicPrintTextPosition(const char* text, int x, int y, uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(1);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(text);
}

void graphicPrintStringPosition(String text, int x, int y, uint16_t color){
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(1);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(text);
}

void graphicPrintTextPositionNoClear(const char* text, int x, int y, uint16_t color) {
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(1);
  tft.setTextColor(color);
  tft.setCursor(x, y);
  tft.print(text);
}