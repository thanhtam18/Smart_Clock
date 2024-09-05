#ifndef MENU_H
#define MENU_H

#include "Arduino.h"
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include <Fonts/FreeSansBold9pt7b.h> 
#include <Fonts/AnumberFont.h>


typedef struct{
  const char *str;
  int x_cursor;
  int y_cursor;
  int y_rec;
  int w;
  bool selected;
}MenuMember;

extern int menuIndex;
extern int subMenuIndex;
extern Adafruit_ST7789 tft;

void select(MenuMember *mn, bool force);
void deSelect(MenuMember *mn, bool force);
void commingSoon();
void About();
#endif