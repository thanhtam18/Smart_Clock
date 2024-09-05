#ifndef _RTC_H
#define _RTC_H
#include <WiFi.h>
#include "time.h"
#include "Arduino.h"

extern const char* ntpServer;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;

void setUpTime();
bool getLocalTime(int *hour, int *min, int *dow,  int *dom,  int *mon,  int *year);
#endif