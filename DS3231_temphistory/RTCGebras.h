#ifndef RTCGebras_h
#define RTCGebras_h
#include "uRTCLib.h"

class RTCGebras {
  private:
    static RTCGebras* instance;
    static uRTCLib rtc;
    RTCGebras();
  public:
    static RTCGebras* getInstance();
    void Setup();
    String DateToString();
    String HourToString();
    uint8_t Day();
    uint8_t Month();
    uint8_t Year();
    uint8_t Hour();
    uint8_t Minute();
    uint8_t Second();
    void SetTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint8_t year);
    void SetTimestamp(int time);
    void Refresh();
};
#endif
