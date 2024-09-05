#ifndef WEATHER_H
#define WEATHER_H
#include <JSON_Decoder.h>
#include <OWM_for_ESP.h>

// #define API_KEY "660201a60c86d61f7f89cfcd93d029be"
// #define LATITUDE "10.84" 
// #define LONGITUDE "106.67"

extern String g_api;
extern String g_lat;
extern String g_long;
extern String city;
extern OWM_Weather owm;

typedef struct {
  int temperature;
  int humidity;
  String status;
} WeatherInfo;

WeatherInfo getweather();

#endif