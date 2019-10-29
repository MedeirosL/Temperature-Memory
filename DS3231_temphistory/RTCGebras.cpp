#include <ESP8266WiFi.h>
#include "RTCGebras.h"
#include "uRTCLib.h"//Biblioteca RTC

RTCGebras* RTCGebras::instance = 0;
uRTCLib RTCGebras::rtc = uRTCLib(0x68);

RTCGebras* RTCGebras::getInstance() {
  if (instance == 0) {
    instance = new RTCGebras();
  }

  return instance;
}

RTCGebras::RTCGebras() {}

void RTCGebras::Setup() {}

String RTCGebras::DateToString(){
  char DataStr[8];
  sprintf(DataStr,"%02d/%02d/20%d", rtc.day(), rtc.month(), rtc.year());
  return DataStr;
}

String RTCGebras::HourToString(){
  char HoraStr[8];
  sprintf(HoraStr,"%d:%02d:%02d", rtc.hour(), rtc.minute(), rtc.second());
  //Serial.println(HoraStr);
  return HoraStr; 
}

uint8_t RTCGebras::Day() {
  return rtc.day();
}

uint8_t RTCGebras::Month() {
  return rtc.month();
}

uint8_t RTCGebras::Year() {
  return rtc.year();
}

uint8_t RTCGebras::Hour() {
  return rtc.hour();
}

uint8_t RTCGebras::Minute() {
  return rtc.minute();
}

uint8_t RTCGebras::Second() {
  return rtc.second(); 
}

void RTCGebras::SetTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint8_t year) {
  rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
}

void RTCGebras::SetTimestamp(int time) {
  time_t tt = time;
  tm* dt = gmtime(&tt);
  int year = dt->tm_year - 100;//aqui só pega o ano com 2 numeros
  rtc.set(dt->tm_sec, dt->tm_min, dt->tm_hour, 0, dt->tm_mday, dt->tm_mon + 1, year);
}

void RTCGebras::Refresh() {
  rtc.refresh();
}
