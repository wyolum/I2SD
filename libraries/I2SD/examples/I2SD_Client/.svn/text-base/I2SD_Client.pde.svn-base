/*
  Read and write some data to the I2SD
 */

#include "C3SB.h"
#include "I2SD.h"
#include "SD.h"
#include "Wire.h"

const boolean WRITE_TEST = true;
const int DBG = 13;

I2SD i2sd;

void setup(){
  uint8_t msg_back[100];
  unsigned long n_byte;

  Serial.begin(57600);
  Wire.begin(); // DON'T FORGET THIS!!!!
  
  Serial.println("Clear error...");
  i2sd.clear_error(); // just in case
  Serial.println("Error cleared.");

  // wait for I2SD hardware to start up
  Serial.println("PING...");
  while(!i2sd.ping((uint8_t*)"PING", 4)){
    delay(100);
  }
  Serial.println("PONG received");

  if(WRITE_TEST){
    Serial.println("Open a NEW_FILE.TXT");
    i2sd.open("NEW_FILE.TXT", FILE_WRITE);
    Serial.println("NEW_FILE.TXT Open");
    // Serial.println("STOP"); while(1) delay(100);
    char* msg="Hello from I2SD_Client.pde. "
      "Please note this is longer than 32 chararters.";
    Serial.print("Write '");
    Serial.println(msg);
    i2sd.write((uint8_t*)msg, strlen(msg) + 1);
    Serial.println("Written");
    Serial.println("Open a NEW_FILE.TXT for reading");
    i2sd.open("NEW_FILE.TXT", FILE_READ);
    Serial.println("Open, now read it back");
    n_byte = i2sd.read(msg_back, strlen(msg));
    msg_back[n_byte] = NULL;
    Serial.println((char*)msg_back);
  }
  if(false){
    i2sd.open("NOT_THERE.TXT", FILE_READ);
    i2sd.read(msg_back, 100);
    Serial.print("NOT_THERE n_byte: ");
    Serial.println(n_byte, DEC);
    Serial.print("NOT_THERE msg_back: ");
    Serial.println((char*)msg_back);
    Serial.print("Check LED for error 2");
    Serial.println("Clear error...");
    delay(10000);
    i2sd.clear_error();
    Serial.println("Error cleared.");
  }
  i2sd.open("DEFAULT.TXT", FILE_READ);
  i2sd.seek(0);
}

void loop(){
  const int n_byte = 32 * 7;
  char big_data[n_byte];
  unsigned long now = millis();
  delay(100);
  uint8_t n_byte_back = i2sd.read((uint8_t*)big_data, n_byte);
  for(int i = 0; i < n_byte; i++){
    if(0 < big_data[i] && big_data[i] < 128){
      Serial.print(big_data[i]);
    }
  }
}

