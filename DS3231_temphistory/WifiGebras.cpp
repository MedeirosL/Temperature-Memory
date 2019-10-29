#include <ESP8266WiFi.h>
#include "WifiGebras.h"
#include "ScreenGebras.h"

String ssid = "Julio";// Network Name
String pass = "123456789";// Network Password
bool userDisconnected = false;
bool connectionStarted = false;

WifiGebras* WifiGebras::instance = 0;
WifiGebras* WifiGebras::getInstance() {
  if (instance == 0)
    {
        instance = new WifiGebras();
    }

    return instance;
}

WifiGebras::WifiGebras() {}

void WifiGebras::Setup() {
  ScreenGebras* screen = ScreenGebras::getInstance();
  int i = 1;
  userDisconnected = false;
  Serial.println("Initialising connection");
  if (pass != "") {
    Serial.println("Wifi com senha");
    WiFi.begin(ssid, pass);
  } else {
    Serial.println("Wifi sem senha");
    WiFi.begin(ssid);
  }
  String dots = ".";
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    if (i%60==0) {
      Serial.println(".");
    } else {
      Serial.print(".");
    }
    
    screen->Display("Conectando a Wifi" + dots);
    dots += ".";
    if (dots.length() > 3) {
      dots = ".";
    }
    i++;
  }
  Serial.println("");
  screen->Display("Wifi conectada com sucesso");
  Serial.print("WiFi connected to: ");
  Serial.println(ssid);
}

void WifiGebras::Connect() {
  if (!connectionStarted) {
    connectionStarted = true;
    userDisconnected = false;
    if (pass != "") {
      WiFi.begin(ssid, pass);
    } else {
      WiFi.begin(ssid);
    }
  }
}

void WifiGebras::Disconnect() {
  userDisconnected = false;
  connectionStarted = false;
  WiFi.disconnect();
}

bool WifiGebras::IsConnected() {
  return WiFi.status() == WL_CONNECTED;
}

void WifiGebras::SetData(String ssid_, String pass_) {
  WiFi.softAPdisconnect (true);
  ssid = ssid_;
  pass = pass_;
}

void WifiGebras::ChangeTo(String ssid_, String pass_) {
  ssid = ssid_;
  pass = pass_;
  userDisconnected = false;
  Serial.print("Change connection to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected to: ");
  Serial.println(ssid);
}
