#ifndef SocketGebras_h
#define SocketGebras_h

#include <Dns.h>

class SocketGebras {
  private:
    static SocketGebras* instance;
    SocketGebras();
    int hardwareType;
  public:
    static SocketGebras* getInstance();
    bool IsConnected();
    void Connect();
    String Read();
    void Insert(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, double value, double temp, int type);
};
#endif
