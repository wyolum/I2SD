/*
  I2SD_Slave.cpp -- I2C <--> SD interface.

  Justin Shaw
  The hardware and software for I2SD have been enabled by the 
  open souce Adafruit data logging shield.  
  Thanks to the Adafruit Team for making them
  available (http://www.ladyada.net/make/logshield/)
  
  LIBRARY VERSION: 0.01, DATED 7/5/2011

Licenced under Creative Commons Attribution.
Attribution 3.0 Unported
This license is acceptable for Free Cultural Works.
You are free:

    * to Share — to copy, distribute and transmit the work
    * to Remix — to adapt the work
    *

Under the following conditions:

    *

      Attribution — You must attribute the work in the manner specified by 
the author or licensor (but not in any way that suggests that they endorse
you or your use of the work).

      Attribute this work:
      Information
      What does "Attribute this work" mean?
      The page you came from contained embedded licensing metadata, including
how the creator wishes to be attributed for re-use. You can use the HTML here 
to cite the work. Doing so will also include metadata on your page so that 
others can find the original work as well.

With the understanding that:
    * Waiver — Any of the above conditions can be waived if you get permission 
      from the copyright holder.
    * Public Domain — Where the work or any of its elements is in the public 
      domain under applicable law, that status is in no way affected by the 
      license.
    * Other Rights — In no way are any of the following rights affected by the
      license:
          o Your fair dealing or fair use rights, or other applicable copyright
	    exceptions and limitations;
          o The author's moral rights;
          o Rights other persons may have either in the work itself or in how 
            the work is used, such as publicity or privacy rights.
    * Notice — For any reuse or distribution, you must make clear to others 
     the license terms of this work. The best way to do this is with a link 
     to this web page.
*/
#include "I2SD.h"
#include "I2SD_Slave.h"
#include "SD.h"

I2SD_Slave* i2sd_p;

I2SD_Slave::I2SD_Slave(){
}

boolean I2SD_Slave::init(){
  error_code = I2SD_NO_ERROR;
  pinMode(I2SD_TX_LED_PIN, OUTPUT);
  pinMode(I2SD_RX_LED_PIN, OUTPUT);
  pinMode(I2SD_SLAVE_SELECT, OUTPUT);
  i2sd_p = this;
  Wire.begin(I2SD_SLAVE_ID);
  Wire.onReceive(I2SD_Slave_onReceive);
  Wire.onRequest(I2SD_Slave_onRequest);
  buffer[I2C_BUFFER_LEN] = NULL; // assure no run-away prints
  if(!SD.begin(I2SD_SLAVE_SELECT)){
    err_out(I2SD_INIT_ERROR);//, "Cannot initialize SD card");
  }
  else{
    if(open("DEFAULT.TXT", FILE_READ)){
    }
    else{
      err_out(I2SD_OPEN_ERROR);
    }
    // open("TEST.TXT", FILE_READ);
    next_pong = false;
  }
  return error_code == I2SD_NO_ERROR;
}

boolean I2SD_Slave::open(char* filename, uint8_t mode){
  if(mode == FILE_WRITE){
    SD.remove(filename);
  }
  file_mode = mode;
  file = SD.open(filename, mode);
  if(!file){
    err_out(I2SD_OPEN_ERROR);//, "Cannot open file");
  }
  return error_code == I2SD_NO_ERROR;
}

void I2SD_Slave::close(){
  file.close();
}

void I2SD_Slave::err_out(uint8_t err_no){
  error_code = err_no;
}

void I2SD_Slave::setTX_LED(boolean state){
  digitalWrite(I2SD_TX_LED_PIN, state);
}
void I2SD_Slave::setRX_LED(boolean state){
  digitalWrite(I2SD_RX_LED_PIN, state);
}

// Slave event handler
void I2SD_Slave_onRequest(){
  uint8_t i;
  if(i2sd_p->file.size() >= i2sd_p->file.position() - 1){
    // i2sd_p->err_out(I2SD_EOF_ERROR);
  }
  if(i2sd_p->error_code != I2SD_NO_ERROR){
    // Serial.print("Error :");
    // Serial.println(i2sd_p->error_code, DEC);
    
    i2sd_p->buffer[0] = i2sd_p->error_code;
    i2sd_p->buffer_size = 1;
    i2sd_p->data_ready = true;
  }
  if(!i2sd_p->data_ready){
    if(i2sd_p->file_mode){
      for(i = 0; 
	  i < I2C_BUFFER_LEN && 
	    i2sd_p->file.position() < i2sd_p->file.size() - 1; 
	  i++){
	i2sd_p->buffer[i] = i2sd_p->file.read();
      }
      if(i < I2C_BUFFER_LEN){
	for(uint8_t j=i; j<I2C_BUFFER_LEN; j++){
	  i2sd_p->buffer[j] = NULL;
	}
      }
      i2sd_p->buffer_size=i;
    }
  }
  i2sd_p->setTX_LED(HIGH);
  WIRE_WRITE(i2sd_p->buffer, i2sd_p->buffer_size);
  i2sd_p->setTX_LED(LOW);
  i2sd_p->data_ready = false;
  i2sd_p->buffer_size = 0;
}

// Slave event handler
void I2SD_Slave_onReceive(int n_byte){
#if defined(ARDUINO) && ARDUINO >= 100
  uint8_t msg_type = Wire.read(); // WIRE_READ;
#else
  uint8_t msg_type = Wire.receive(); // WIRE_READ;
#endif
  uint8_t i;
  i2sd_p->setRX_LED(HIGH);

  if(msg_type == I2SD_CLEAR_ERROR_MSG){
    i2sd_p->error_code = I2SD_NO_ERROR;
    Serial.println("Error Cleared");
    return;
  }
  else if(i2sd_p->error_code == I2SD_NO_ERROR){
    // Serial.print("I2SD_Slave_onReceive(), n_byte:");
    // Serial.println(n_byte, DEC);
    // delayMicroseconds(100);
    Serial.print("MSG_TYPE: ");
    Serial.println(msg_type, DEC);
    if(msg_type == I2SD_SEEK_MSG){
      // grab address
      Address_t Address;
      for(i = 0; 
	  i < sizeof(i2sd_p->cursor) && Wire.available(); 
	  i++){
	Address.char4[i] = WIRE_READ;
      }
      if(i == 4){ // make sure full address received
	i2sd_p->file.seek(Address.dat32);
	Serial.print("SEEK: ");
	Serial.println(Address.dat32);
      }
      else{
	Serial.print("SEEK address garbled");
      }
    }
    else if(msg_type == I2SD_PING_MSG){
      for(i = 0; Wire.available(); i++){
	i2sd_p->buffer[i] = WIRE_READ;
      }
      Serial.println("PONG");
      // reply PING data in next request
      i2sd_p->buffer_size = i;
      i2sd_p->data_ready = true;
    }
    else if(msg_type == I2SD_WRITE_MSG){
      if(i2sd_p->file_mode == FILE_WRITE){
	while(Wire.available()){
	  i2sd_p->file.write(WIRE_READ);
	}
      }
      else{
	i2sd_p->err_out(I2SD_MODE_ERROR);//, "File not in FILE_WRITE mode");
      }
    }
    else if(msg_type == I2SD_OPEN_MSG){
      uint8_t mode, i;
      mode = WIRE_READ;
      
      char filename[I2C_BUFFER_LEN - 1]; // one extra char reserved for 
      // null terminator
      for(i = 0; Wire.available() && i < I2C_BUFFER_LEN - 2; i++){
	filename[i] = WIRE_READ;
      }
      if(i > 0){
	filename[i] = NULL; // terminate string just in case
	Serial.print("Open:");
	Serial.print(filename);
	Serial.print(", mode:");
	Serial.println(mode, DEC);
	i2sd_p->close();
	i2sd_p->open(filename, mode);
	Serial.println("File is now open!");
      }
    }
    else{
      Serial.print("unknown message type: ");
      Serial.println(msg_type, DEC);
    }
  }
  i2sd_p->setRX_LED(LOW);
}

