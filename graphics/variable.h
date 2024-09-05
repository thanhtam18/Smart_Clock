#ifndef VARIABLE_H
#define VARIABLE_H

typedef enum {
  CLOCK,
  SETTING
} State;

String g_api = "";
String g_lat = "";
String g_long = "";

String city = "";

const char* dayOfWeek[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

unsigned long getTimeInterval = 0;
unsigned long getWeatherInterval = 0;

bool firtGet = false;

int localHour, localMin = 0;
int local_dofw, local_dofm, local_mon, local_year = 0;

State state = CLOCK;
String weather = "";

bool connection = true;
bool connected = false;
bool reConnect = true;
int connectSelection = 0;

int menuIndex = -1;
bool b_initClockMode = true;
bool b_initSettingMode = true;

int subMenuIndex = 0;

int menuWiFiIndex = 0;
int subMenuWifiIndex = 0;
bool b_initSubMenuWifi = false;


bool changeGPS = false;
int menuGPSIndex = 0;
int subMenuGPSIndex = 0;
bool b_initSubMenuGPS = false;

#endif