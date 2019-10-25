#include <user_interface.h>;//Biblioteca necessaria para acessar os Timers
#include <Wire.h>
#include <time.h>
#include <OneWire.h>
#include "RTCGebras.h"
//#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA

//RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231 
OneWire  ds(5);  // on pin D1 (a 4.7K resistor is necessary)

os_timer_t tmr0;//Cria o Timer. Maximo de 7 Timers.
RTCGebras* rtc = RTCGebras::getInstance();
unsigned char timerFlag=0,timerCont=0;
float celsius[24],avgTemp;
String hour;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
void timer(void*z){ //software timer de 1ms - Conta o relógio internalo
  timerCont++;
  if (timerCont>=10){
    timerFlag=1;
    timerCont=0;
  }
}
void setup(void) {
  Serial.begin(115500);
  //Serial.println("OK");
  os_timer_setfn(&tmr0, timer, NULL); //timer, subrotina, null
  os_timer_arm(&tmr0, 100, true); //timer, tempo em ms (entre 1ms e 2s), loop=true

  for (char j=0;j>23;j++){
      celsius[j]=0;
  }
  rtc->Setup();
  rtc->SetTimestamp(1572034610);
  //rtc.adjust(DateTime(2019, 10, 25, 17, 09, 30));
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
 
void loop(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float fahrenheit;
  if (timerFlag==1){
    //DateTime now = rtc.now();
    //Serial.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    //Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    //Serial.println(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    rtc->Refresh();
    hour=rtc->HourToString();
    Serial.println(hour);
    if ( !ds.search(addr)) {
      //Serial.println("No more addresses.");
     // Serial.println();
      ds.reset_search();
      delay(250);
      return;
    }
    //Serial.print("ROM =");
    for( i = 0; i < 8; i++) {
      //Serial.write(' ');
      //Serial.print(addr[i], HEX);
    }
   
    if (OneWire::crc8(addr, 7) != addr[7]) {
        //Serial.println("CRC is not valid!");
        return;
    }
    //Serial.println();
   
    // the first ROM byte indicates which chip
    switch (addr[0]) {
      case 0x10:
        //Serial.println("  Chip = DS18S20");  // or old DS1820
        type_s = 1;
        break;
      case 0x28:
        //Serial.println("  Chip = DS18B20");
        type_s = 0;
        break;
      case 0x22:
        //Serial.println("  Chip = DS1822");
        type_s = 0;
        break;
      default:
        //Serial.println("Device is not a DS18x20 family device.");
        type_s = 0;
        return;
    } 
   
    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end
    
    delay(1000);     // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.
    
    present = ds.reset();
    ds.select(addr);    
    ds.write(0xBE);         // Read Scratchpad
   
    //Serial.print("  Data = ");
    //Serial.print(present, HEX);
    //Serial.print(" ");
    for ( i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = ds.read();
      //Serial.print(data[i], HEX);
      //Serial.print(" ");
    }
    //Serial.print(" CRC=");
    //Serial.print(OneWire::crc8(data, 8), HEX);
    OneWire::crc8(data, 8);
    //Serial.println();
   
    // Convert the data to actual temperature
    // because the result is a 16 bit signed integer, it should
    // be stored to an "int16_t" type, which is always 16 bits
    // even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];
    if (type_s) {
      raw = raw << 3; // 9 bit resolution default
      if (data[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data[6];
      }
    } else {
      byte cfg = (data[4] & 0x60);
      // at lower res, the low bits are undefined, so let's zero them
      if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
      else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
      else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
      //// default is 12 bit resolution, 750 ms conversion time
    }
    for (char j=0;j<23;j++){
      celsius[j]=celsius[j+1];
    }

    celsius[23] = (float)raw / 16.0;
 
    //fahrenheit = celsius * 1.8 + 32.0;
    avgTemp=0;
    Serial.print("Temperature Vector= [ ");
    for (char j=0;j<24;j++){
        Serial.print(celsius[j]);
        Serial.print(" ");
        avgTemp+=celsius[j];
    }
    avgTemp/=24.;
    Serial.println(" ]");
    Serial.print("Média: ");
    Serial.println(avgTemp);
    //Serial.println(" Celsius ");
    //Serial.print(fahrenheit);
    //Serial.println(" Fahrenheit");
    timerFlag=0;
  }
}
