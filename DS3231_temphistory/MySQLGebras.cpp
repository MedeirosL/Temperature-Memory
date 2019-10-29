#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <Dns.h>
#include "MySQLGebras.h"

char user[] = "gebras";// MySQL user
char password[] = "1234";// MySQL password
bool useIp = false;
WiFiClient client;
MySQL_Connection conn((Client *)&client);

void MySQLGebras::Setup() {
  //WiFi.hostByName("hw.gebras.com", server_addr);
  //WiFi.hostByName("DESKTOP-8D3J3HQ", server_addr);
  //WiFi.hostByName("raspberrypi", server_addr);
  WiFi.hostByName("192.168.4.2", server_addr);
  Serial.print("Connecting to database: ");
  Serial.println(server_addr);

  while (conn.connect(server_addr, 3306, user, password) != true) {
    delay(200);
    //Serial.print ( "." );
    WiFi.hostByName("192.168.4.2", server_addr);
    Serial.print("Connecting to database: ");
    Serial.println(server_addr);
  }
  Serial.println("Database connected!");
};

bool MySQLGebras::IsConnected() {
  return conn.connected();
}

void MySQLGebras::Insert(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, double value) {
  if (!conn.connected()) {
    Serial.println("Database not connected.");
  } else {
    Serial.print("Insert to DB...");
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

    char Query[100];
    sprintf(Query, "INSERT INTO gebras.hardware (Valor, Hora) VALUES (%f, '%d/%d/%d %d:%d:%d')", value, year, month, day, hour, minute, second);
    if (!cur_mem->execute(Query)) {
      //error in mysql connection, action required
      Serial.println(" Error");
    } else {
      Serial.println(" Done");
    }
    delete cur_mem;
  }
};

void MySQLGebras::Read() {
  if (!conn.connected()) {
    Serial.println("Database not connected.");
  } else {
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    if (cur_mem->execute("SELECT * FROM gebras.hardware ORDER BY ID DESC LIMIT 1")) {
      Serial.println("Fetch ok");
      //Fetch the columns (required) but we don't use them.
      column_names *columns = cur_mem->get_columns();
    
      row_values *row = NULL;
      do {
        row = cur_mem->get_next_row();
        if (row != NULL) {
          Serial.print("LastId = ");
          Serial.println(row->values[0]);
          Serial.print("Value = ");
          Serial.println(row->values[1]);
          Serial.print("Date = ");
          Serial.println(row->values[2]);
        }
      } while (row != NULL);
    } else {
      Serial.println("Fetch ok");
      //error in mysql connection, action required
    }
    
    delete cur_mem;
  }
};

void MySQLGebras::Test() {
  Serial.println("MySQLGebras::Test");
};
