#include "my_RTC.h"

bool getLocalTime(int *hour, int *min, int *dow, int *dom, int *mon, int *year) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return false;
  }
  *hour = timeinfo.tm_hour;
  *min = timeinfo.tm_min;
  *dow = timeinfo.tm_wday;
  *dom = timeinfo.tm_mday;
  *mon = timeinfo.tm_mon + 1;
  *year = timeinfo.tm_year + 1900;
  return true;
}

void setUpTime(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}