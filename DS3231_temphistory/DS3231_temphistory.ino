#include <user_interface.h>;//Biblioteca necessaria para acessar os Timers
#include <Wire.h>
#include <time.h>
#include <OneWire.h>
#include <time.h>
#include <NTPClient.h>//Biblioteca do NTP - Editada com "getFullFormattedTime
#include <WiFiUdp.h>//Biblioteca do UDP
#include <EEPROM.h>
#include "WifiGebras.h"
#include "ScreenGebras.h"

WifiGebras* wifi = WifiGebras::getInstance();

WiFiUDP udp;//Cria um objeto "UDP".
NTPClient ntp(udp, "south-america.pool.ntp.org", -3 * 3600, 60000);//Cria um objeto "NTP" com as configurações.
OneWire  ds(5);  // on pin D1 (a 4.7K resistor is necessary)

os_timer_t tmr0;//Cria o Timer. Maximo de 7 Timers.

unsigned char timerFlag=0,timerCont=0,timerData=0,flag2=0;
float celsius[24],avgTemp;
unsigned int internalHour=0,internalMinute=00,internalSecond=0,idx=0;
unsigned long internalEpoch=-1,temp=0,t1=0,t2=0;
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
String internalDay,internalDate,iSimpleDate,iSimpleTime;
int whole,remain;

void setup(void) {
  Serial.begin(115500);
  EEPROM.begin(4096);
  //Serial.println("OK");
  os_timer_setfn(&tmr0, timer, NULL); //timer, subrotina, null
  os_timer_arm(&tmr0, 1000, true); //timer, tempo em ms (entre 1ms e 2s), loop=true

  for (char j=0;j<=23;j++){
      celsius[j]=0;
  }
  t1=millis();
  wifi->SetData("Arno", "abcde12345");
  wifi->Connect();
  while ( !wifi->IsConnected() && temp+60000>millis()) { // Tenta conectar por um minuto
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println("Conectado!!");
  ntp.begin();//Inicia o NTP.
  ntp.forceUpdate();//Força o Update.
  SetupClock(); //atualiza a hora ao ligar

  for (int i = 0; i < 10; i++)
  {
    Serial.print(EEPROM.read(i));
    Serial.print(" ");
  }
  Serial.println();

  idx=((int(EEPROM.read(0))<<8)+int(EEPROM.read(1))); //0
  if (idx<2)
    idx+=2;
  EEPROM.write(idx, 0xff); //1
  EEPROM.write(idx+1, 0xff); //2
  EEPROM.write(idx+2, internalHour); //3
  EEPROM.write(idx+3, internalMinute); //4
  idx+=4;
  EEPROM.write(0,idx>>8);
  EEPROM.write(1,idx);
  EEPROM.commit();
  for (char j=0;j<=10;j++){
      Serial.print(EEPROM.read(j));
      Serial.print(" ");
  }
  Serial.println();

  pinMode(LED_BUILTIN,OUTPUT);
}
 
void loop(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float fahrenheit;

  /*if (!wifi->IsConnected())
    digitalWrite(LED_BUILTIN,LOW);
  else
    digitalWrite(LED_BUILTIN,HIGH);*/

  if (internalMinute%4==0 && flag2==0){
    flag2=1;
    EEPROM.write(idx,whole);
    EEPROM.write(idx+1,remain);
    idx+=2;
    EEPROM.write(0,idx>>8);
    EEPROM.write(1,idx);
    EEPROM.commit();
    timerData=0;
    Serial.print(internalHour);
    Serial.print(":");
    Serial.print(internalMinute);
    Serial.print(" ; ");
    Serial.print(whole);
    Serial.print(".");
    Serial.print(remain);
    Serial.println(" ºC");
    for(int i=0;i<100;i++){
      Serial.print(EEPROM.read(i));
      Serial.print(" ");
      if(i%10==0 && i!=0)
        Serial.println();
    }
     Serial.println();
  }
  
  if ((internalMinute+1)%4==0 && flag2==1){
    flag2=0;
    
  }

  if ((internalSecond)%5!=0){
    timerFlag=0;
  }
  if ((internalSecond%5)==0 && timerFlag==0){
    timerFlag=1;
    Serial.print(internalHour);
    Serial.print(":");
    Serial.print(internalMinute);
    Serial.print(":");
    Serial.print(internalSecond);
    //Serial.print("\t;\t");
    if ( !ds.search(addr)) {
      ds.reset_search();
      delay(250);
      //Serial.print(" TESTE AAA "); //
      return;
    }
    //Serial.print("ROM =");
    for( i = 0; i < 8; i++) {
      //Serial.write(' ');
      //Serial.print(addr[i], HEX);
    }
    //Serial.print(" TESTE 1 ");
    if (OneWire::crc8(addr, 7) != addr[7]) {
        Serial.println("CRC is not valid!");
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
        type_s = 0;
        break;
      case 0x22:
        type_s = 0;
        break;
      default:
      
        type_s = 0;
        return;
    } 
    
    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end

    delay(750);
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
    Serial.print(avgTemp);
    Serial.print(" ");
    whole = avgTemp;
    remain = abs((avgTemp - whole) * 100);
    Serial.print(whole);
    Serial.print(".");
    Serial.println(remain);
    //Serial.println(" Celsius ");
    //Serial.print(fahrenheit);
    //Serial.println(" Fahrenheit");
    
    timerData++;

    Serial.print(timerData);
    Serial.print(" ");
  }
}

void timer(void*z){ //software timer de 1ms - Conta o relógio internalo
  timerCont++;
  if (timerCont>=10){
    //timerFlag=1;
    timerCont=0;
  }
  if(internalHour>23){
    internalHour=0;
  }
  if(internalMinute>59){
    internalHour++;
    internalMinute=0;
  }
  if(internalSecond>59){
    internalMinute++;
    internalSecond=0;
  }
    internalSecond++;
    internalEpoch++;
}


void SetupClock(){ //função que atualiza o relógio interno, se estiver conectado
  if(wifi->IsConnected()){
    ntp.update();
    internalDay = daysOfTheWeek[ntp.getDay()];
    internalHour = ntp.getHours();
    internalMinute = ntp.getMinutes();
    internalSecond=ntp.getSeconds();
    internalEpoch = ntp.getEpochTime();
    //internalDate = ntp.getFullFormattedTime(internalEpoch);
    //iSimpleDate = ntp.getFormattedDate(internalEpoch);
    //iSimpleTime = ntp.getFormattedTime2(internalEpoch);
    delay(10);
  }
}
