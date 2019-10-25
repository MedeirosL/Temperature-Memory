#include "c_types.h"//String
#include <Wire.h>
#include "ScreenGebras.h"
#include "SSD1306Wire.h"
#include "SH1106Wire.h"
#include "WifiGebras.h"
#include "SocketGebras.h"
#include "font.h"

//SSD1306Wire display(0x3c, D6, D5);    //endereço e pinos display
//SH1106Wire display(0x3c, D6, D5);    //endereço e pinos display
SH1106Wire display(0x3c, D4, D5);    //endereço e pinos display
ScreenGebras* ScreenGebras::instance = 0;
long fps;

ScreenGebras* ScreenGebras::getInstance() {
  if (instance == 0) {
    instance = new ScreenGebras();
  }

  return instance;
}

ScreenGebras::ScreenGebras() {}

void ScreenGebras::Setup() {
  simplemsg = "";
  date = "";
  time = "";
  current = -1;
  title = "";
  msg = "";
  
  display.init();
  //display.flipScreenVertically();
  //display.setFont(ArialMT_Plain_10);
}

void ScreenGebras::Reboot() {
  //display = SH1106Wire(0x3c, D6, D5);
}

void ScreenGebras::Display(String simplemsg_) {
  if (simplemsg_ != simplemsg) {
    simplemsg = simplemsg_;
    title = "";
    date = "";
    
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, simplemsg);
    display.display();
  }
}

void ScreenGebras::Clear() {
  display.clear();
  display.display();
}

void ScreenGebras::Current(String date_, String time_, double current_)  { 
  if (date_ != date || time_ != time || current_ != current) {
    date = date_;
    time = time_;
    current = current_;
    title = "";
    simplemsg = "";

    WifiGebras* wifi = WifiGebras::getInstance();
    SocketGebras* socket = SocketGebras::getInstance();
    
    display.clear();
    display.setFont(ArialMT_Plain_10);
    //display.drawString(0, 0, date);
    display.drawString(85, 0, time);
    if (wifi->IsConnected()) {
      display.drawString(0, 0, "W");
    } else {
      display.drawString(0, 0, "W!");
    }
    if (socket->IsConnected()) {
      display.drawString(14, 0, "S");
    } else {
      display.drawString(14, 0, "S!");
    }
    display.setFont(ArialMT_Plain_16);
    display.drawString(30, 10, "Corrente");
    display.setFont(Roboto_Mono_Medium_42);
    display.drawString(6, 21, String(current));
    display.setFont(ArialMT_Plain_24);
    display.drawString(110, 38, "A");   
    display.display();
  }
}

void ScreenGebras::Pulse(String date_, String time_, double pulse_)  { 
  if (date_ != date || time_ != time || pulse_ != pulse) {
    date = date_;
    time = time_;
    pulse = pulse_;
    title = "";
    simplemsg = "";

    WifiGebras* wifi = WifiGebras::getInstance();
    SocketGebras* socket = SocketGebras::getInstance();
    
    display.clear();
    display.setFont(ArialMT_Plain_10);
    //display.drawString(0, 0, date);
    display.drawString(85, 0, time);
    if (wifi->IsConnected()) {
      display.drawString(0, 0, "W");
    } else {
      display.drawString(0, 0, "W!");
    }
    if (socket->IsConnected()) {
      display.drawString(14, 0, "S");
    } else {
      display.drawString(14, 0, "S!");
    }
    display.setFont(ArialMT_Plain_16);
    display.drawString(30, 10, "Pulso");
    display.setFont(Roboto_Mono_Medium_42);
    display.drawString(6, 21, String(pulse));
    display.setFont(ArialMT_Plain_24);
    display.drawString(110, 38, "A");   
    display.display();
  }
}

void ScreenGebras::Pulse2(String date_, String time_, double pulse_, int pulseCount)  { 
  if (date_ != date || time_ != time || pulse_ != pulse) {
    date = date_;
    time = time_;
    pulse = pulse_;
    title = "";
    simplemsg = "";

    WifiGebras* wifi = WifiGebras::getInstance();
    SocketGebras* socket = SocketGebras::getInstance();
    
    display.clear();
    display.setFont(ArialMT_Plain_10);
    //display.drawString(0, 0, date);
    display.drawString(85, 0, time);
    if (wifi->IsConnected()) {
      display.drawString(0, 0, "W");
    } else {
      display.drawString(0, 0, "W!");
    }
    if (socket->IsConnected()) {
      display.drawString(14, 0, "S");
    } else {
      display.drawString(14, 0, "S!");
    }
    display.drawString(32, 0, String(pulseCount));
    display.setFont(ArialMT_Plain_16);
    display.drawString(30, 10, "Pulso");
    display.setFont(Roboto_Mono_Medium_42);
    display.drawString(6, 21, String(pulse));
    display.setFont(ArialMT_Plain_24);
    //display.drawString(110, 38, "A");   
    display.display();
  }
}

void ScreenGebras::Output(String date_, String time_, float output_, float outputFrequency_)  { 
  if (date_ != date || time_ != time || output_ != output || outputFrequency_ != outputFrequency) {
    date = date_;
    time = time_;
    output = output_;
    outputFrequency = outputFrequency_;
    title = "";
    simplemsg = "";

    WifiGebras* wifi = WifiGebras::getInstance();
    SocketGebras* socket = SocketGebras::getInstance();
    
    display.clear();
    display.setFont(ArialMT_Plain_10);
    //display.drawString(0, 0, date);
    display.drawString(85, 0, time);
    if (wifi->IsConnected()) {
      display.drawString(0, 0, "W");
    } else {
      display.drawString(0, 0, "W!");
    }
    if (socket->IsConnected()) {
      display.drawString(14, 0, "S");
    } else {
      display.drawString(14, 0, "S!");
    }
    display.setFont(ArialMT_Plain_16);
    display.drawString(30, 10, "Vazão");
    display.setFont(Roboto_Mono_Medium_42);
    if(isinf(outputFrequency)) {
      display.drawString(6, 21, "0.00");
    } else {
      display.drawString(6, 21, String(output));
    }
    display.setFont(ArialMT_Plain_24);
    //display.drawString(110, 38, "A");   
    display.display();
  }
}

void ScreenGebras::Slider(String date_, String time_, int slider_)  { 
  if (date_ != date || time_ != time || slider_ != slider) {
    date = date_;
    time = time_;
    slider = slider_;
    title = "";
    simplemsg = "";

    WifiGebras* wifi = WifiGebras::getInstance();
    SocketGebras* socket = SocketGebras::getInstance();
    
    display.clear();
    display.setFont(ArialMT_Plain_10);
    //display.drawString(0, 0, date);
    display.drawString(85, 0, time);
    if (wifi->IsConnected()) {
      display.drawString(0, 0, "W");
    } else {
      display.drawString(0, 0, "W!");
    }
    if (socket->IsConnected()) {
      display.drawString(14, 0, "S");
    } else {
      display.drawString(14, 0, "S!");
    }
    display.setFont(ArialMT_Plain_16);
    display.drawString(30, 10, "Slider");
    display.setFont(Roboto_Mono_Medium_42);
    display.drawString(6, 21, String(slider));
    display.setFont(ArialMT_Plain_24);
    display.drawString(110, 38, "A");   
    display.display();
  }
}

void ScreenGebras::Menu(String title_, String bottom_, String middle_, String top_)  { 
  if (title_ != title || middle_ != middle) {
    title = title_;
    bottom = bottom_;
    middle = middle_;
    top = top_;
    date = "";
    simplemsg = "";
    
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, title);
    display.drawString(0, 32, middle);
    
    display.setFont(ArialMT_Plain_10);
    display.drawString(3, 22, top);
    display.drawString(3, 47, bottom);
    /*display.drawString(0, 38, "["+String(currMenu)+"]");
    display.drawString(40, 38, String(menus[0]));
    display.drawString(60, 38, String(menus[1]));
    display.drawString(80, 38, String(menus[2]));*/
    display.display();  
  }
}

int p2 = 22;
int bx = 62;
int by = 30;
int speedx = 3;
int speedy = 3;
int score1 = 0;
int score2 = 0;
bool isRunning = false;
int prevP1 = 0;

void ScreenGebras::P(int p1, bool isStart) {
  if (isStart) {
    p2 = 22;
    bx = 62;
    by = 30;
    speedx = 3;
    speedy = 3;
    score1 = 0;
    score2 = 0;
    isRunning = false;
  }
  if (millis() - fps < 33) {
    //controle de fps
    return;
  }
  fps = millis();
  if (prevP1 != p1) {
    isRunning = true;
  }
  prevP1 = p1;
  //delay(16.66);//60fps
  //delay(33.33);//30fps
  //delay(66.66);//15fps
  //128x64
  if (isRunning) {
    bx += speedx;
    by += speedy;
    if (bx < 8) {
      if (by <= p1 + 15 && by >= p1-4) {
        bx = 8;
        speedx = 2;
        while (speedy == 0) {
          speedy = random(-4, 4);
        }
      } else {
        score2++;
        p2 = 22;
        bx = 62;
        by = 30;
        speedx = random(3, 4);
        speedy = random(-4, 4);
        isRunning = false;
      }
    } else if (bx > 116) {
      if (by <= p2 + 15 && by >= p2-4) {
        bx = 116;
        speedx = -2;
        while (speedy == 0) {
          speedy = random(-4, 4);
        }
      } else {
        score1++;
        p2 = 22;
        bx = 62;
        by = 30;
        speedx = random(3, 4);
        speedy = random(-4, 4);
        isRunning = false;
      }
    }

    //A.I.
    if (bx > 64 && speedx > 0) {
      int diff = random(0, 3);
      if (p2 < by) {
        p2 += diff;
      } else if (p2 > by) {
        p2 -= diff;
      }
    }
  
    if (p2 < 0) {
      p2 = 0;
    } else if (p2 > 49) {
      p2 = 49;
    }
  
    if (by < 0) {
      by = 0;
      speedy = random(1, 4);
    } else if (by > 60) {
      by = 60;
      speedy = -random(1, 4);
    }
  }
  
  //128x64
  display.clear();
  display.drawRect(4, p1, 4, 15);
  display.drawRect(120, p2, 4, 15);
  display.drawRect(64, 0, 1, 128);
  display.setFont(ArialMT_Plain_10);
  display.drawString(48, 4, String(score1));
  display.drawString(74, 4, String(score2));
  display.drawRect(bx, by, 4, 4);
  display.display();
}
