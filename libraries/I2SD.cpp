/*
  I2SD.cpp -- I2C <--> SD interface.

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
Address_t Address;

boolean I2SD::ping(uint8_t* ping_data, uint8_t len){
  uint8_t buffer[32];
  uint8_t i;
  boolean out = true;

  buffer[0] = I2SD_PING_MSG;
  for(i = 0; i < len; i++){
    // copy ping_data to buffer
    buffer[i + 1] = ping_data[i + 0];
  }
  Wire.beginTransmission(I2SD_SLAVE_ID);
#if defined(ARDUINO) && ARDUINO >= 100
  WIRE_WRITE(buffer, len + 1);
#else
  Wire.send(buffer, len + 1); // seek(0) message
#endif
  Wire.endTransmission();
  
  delay(100);
  
  Wire.requestFrom(I2SD_SLAVE_ID, len);
  if(Wire.available() == len){
    for(i = 0; i < len && Wire.available(); i++){
      char c = WIRE_READ; // receive a byte as character
      Serial.print(c);
      if(c != ping_data[i]){
	out = false;
	break;
      }
    }
    Serial.println("");
  }
  else{
    out = false;
  }
  return out;
}

void I2SD::seek(unsigned long addr){
  uint8_t buffer[5];

  Address.dat32 = addr;
  buffer[0] = I2SD_SEEK_MSG;
  for(uint8_t i = 0; i < 4; i++){
    buffer[i + 1] = Address.char4[i + 0];
  }
  Wire.beginTransmission(I2SD_SLAVE_ID);
#if defined(ARDUINO) && ARDUINO >= 100
  WIRE_WRITE(buffer, 5);
#else:
  Wire.send(buffer, 5); // seek(0) message
#endif
  Wire.endTransmission();
  cursor = addr;
}

void I2SD::open(char* filename, uint8_t mode){
  uint8_t buffer[I2C_BUFFER_LEN];
  buffer[0] = I2SD_OPEN_MSG;
  buffer[1] = mode;
  strcpy((char*)(buffer + 2), filename);
  Wire.beginTransmission(I2SD_SLAVE_ID);
#if defined(ARDUINO) && ARDUINO >= 100
  WIRE_WRITE(buffer, strlen(filename) + 3);
#else
  Wire.send(buffer, strlen(filename) + 3); // open "TEST.TXT" message
#endif
  Wire.endTransmission();
  cursor = 0;
  delay(10);
}

uint8_t I2SD::read(uint8_t *data, uint8_t n_byte){
  uint8_t out = c3sb.read_from(I2SD_SLAVE_ID, data, n_byte);
  delay(0);
  cursor += out;
  if(out % I2C_BUFFER_LEN){
    seek(cursor);
  }
  return out;
}

void I2SD::write(uint8_t *data, uint8_t n_byte){
  uint8_t buffer[I2C_BUFFER_LEN];
  c3sb.write_to(I2SD_SLAVE_ID, data, n_byte);
  cursor += n_byte;
}

void I2SD::clear_error(){
  uint8_t msg[1];
  msg[0] = I2SD_CLEAR_ERROR_MSG;
  // c3sb.raw_send(msg, 1, false);
  // return;
  Wire.beginTransmission(I2SD_SLAVE_ID);
#if defined(ARDUINO) && ARDUINO >= 100
  WIRE_WRITE(msg, 1);
#else
  Wire.send(msg, 1); // seek(0) message
#endif
  Wire.endTransmission();
}
