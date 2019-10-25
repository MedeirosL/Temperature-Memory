#ifndef MySQLGebras_h
#define MySQLGebras_h

#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <Dns.h>

class MySQLGebras {
  private:
    DNSClient dns_client;
    IPAddress server_addr;
  public:
    bool IsConnected();
    void Setup();
    void Test();
    void Insert(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, double value);
    void Read();
};
#endif
