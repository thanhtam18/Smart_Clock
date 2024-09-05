#ifndef _MAIN_H
#define _MAIN_H
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h> 
#include <Fonts/AnumberFont.h>
#include "image.h"
#include <string>
#define TFT_CS        5
#define TFT_RST        19 
#define TFT_DC         4

#define ST77XX_GRAY 0xC616
#define ST77XX_LIGHT_YELLOW 0xFFE6
#define ST77XX_DARK_BLUE 0x000C
extern Adafruit_ST7789 tft;

extern const char* dayOfWeek[];

void Paint_DrawImage(const unsigned char *image, uint16_t xStart, uint16_t yStart, uint16_t W_Image, uint16_t H_Image);
void graphicTimeUpdate(int hour, int min, bool update);
void graphicDayUpdate(int dow, int dom, int mon, int year, bool update);
void graphicWeatherUpdate(String status, int hour);
void graphicUpdateTemAndHum(int tem, int hum, bool update);
void graphicLoading();
void graphicPrintTextPosition(const char* text, int x, int y, uint16_t color);
void graphicPrintStringPosition(String text, int x, int y, uint16_t color);
void graphicPrintTextPositionNoClear(const char* text, int x, int y, uint16_t color);
#endif