#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "SocketGebras.h"
#include "ScreenGebras.h"

WiFiClient socketClient;
HTTPClient http;

SocketGebras* SocketGebras::instance = 0;
SocketGebras* SocketGebras::getInstance() {
  if (instance == 0) {
    instance = new SocketGebras();
  }

  return instance;
}

SocketGebras::SocketGebras() {}

void SocketGebras::Connect() {
  socketClient.connect("10.42.0.1", 8080);
}

String SocketGebras::Read() {
  if (socketClient.connected()) {
    Serial.println("Socket conectado");
    socketClient.print("time\n");
    unsigned long timeout = millis();
    while (socketClient.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Timout pro retorno do socket");
        return "";
      }
    }  
  } else {
    return "";
  }
  String t = "";
  while (socketClient.available()) {
    char ch = static_cast<char>(socketClient.read());
    
    t += ch;
  }
  return t;
}

bool SocketGebras::IsConnected() {
  return socketClient.connected();
}

void SocketGebras::Insert(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, double value, double temp, int type) {
  char Query[100];
  sprintf(Query, "%f&%d-%d-%d-%d-%d-%d&%f&%i\n", value, year, month, day, hour, minute, second, temp, type);
  //Serial.print(Query);
  socketClient.print(Query);
};
