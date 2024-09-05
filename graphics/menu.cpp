#include "menu.h"
#include "graphic_handler.h"

void deSelect(MenuMember *mn, bool force) {
  if (mn->selected == false && force == false) return;
  tft.fillRect(0, mn->y_rec, mn->w, 30, ST77XX_BLACK);
  tft.setTextColor(ST77XX_GRAY);
  tft.setCursor(mn->x_cursor, mn->y_cursor);
  tft.print(mn->str);
  mn->selected = false;
}

void select(MenuMember *mn, bool force) {
  if (mn->selected == true && force == false) return;
  tft.fillRect(0, mn->y_rec, mn->w, 30, ST77XX_ORANGE);
  tft.setTextColor(ST77XX_DARK_BLUE);
  tft.setCursor(mn->x_cursor, mn->y_cursor);
  tft.print(mn->str);
  mn->selected = true;
}

void commingSoon() {
  if (subMenuIndex == -1) return;
  graphicPrintTextPosition("Comming Soon...", 43, 130, ST77XX_GREEN);
  tft.fillRect(0, 250, 240, 30, ST77XX_ORANGE);
  tft.setTextColor(ST77XX_DARK_BLUE);
  tft.setCursor(97, 270);
  tft.print("Back");
  subMenuIndex = -1;
}


void About() {
  if (subMenuIndex == -1) return;
  tft.fillScreen(ST77XX_BLACK);
  graphicPrintTextPositionNoClear("Author: TDEV", 60, 100, ST77XX_GREEN);
  graphicPrintTextPositionNoClear("Date: 28/5/24", 60, 125, ST77XX_GREEN);
  graphicPrintTextPositionNoClear("Version: 1.0.0", 60, 150, ST77XX_GREEN);
  tft.fillRect(0, 250, 240, 30, ST77XX_ORANGE);
  tft.setTextColor(ST77XX_DARK_BLUE);
  tft.setCursor(97, 270);
  tft.print("Back");
  subMenuIndex = -1;
}