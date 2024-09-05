#include "weather.h"

WeatherInfo getweather() {
  WeatherInfo wt;
  bool weatherReceieved = false;
  OWM_current* current = new OWM_current;

  weatherReceieved = owm.getCurrentWeather(current, g_lat, g_long);
  if (weatherReceieved) {
    if (current) {
      wt.temperature = current->mainTemp;
      wt.humidity = current->humidity;
      wt.status = owm.getStatus(current->weatherCondId);
      city = current->cityName;
    }
  }
  delete current;
  return wt;
}