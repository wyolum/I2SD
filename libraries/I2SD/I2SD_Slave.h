/*
  I2SD_Slave.h -- I2C <--> SD interface. 
  (lives on I2SD device i.e. not client code)

  Justin Shaw
  The hardware and software for I2SD have been enabled by the 
  open souce Adafruit data logging shield.  
  Thanks to the Adafruit Team for making them
  available (http://www.ladyada.net/make/logshield/)
  
  LIBRARY DATED 7/5/2011

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
#ifndef I2SD_Slave_H
#define I2SD_Slave_H

// Arduino 1.0 compatibility
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#define WIRE_WRITE(ptr,n_byte) (Wire.write((uint8_t*)(ptr), (n_byte)))
#define WIRE_WRITE1(one_byte) (Wire.write((uint8_t)(one_byte)))
#define WIRE_READ Wire.read()
#else
#include "WProgram.h"
#define WIRE_WRITE(ptr,n_byte) (Wire.send((uint8_t*)(ptr), (n_byte)))
#define WIRE_WRITE1(one_byte) (Wire.send((one_byte)))
#define WIRE_READ Wire.receive()
#endif

#include <inttypes.h>
#include "I2SD.h"
#include "C3SB.h"
#include "SD.h"
#include "Wire.h"

class I2SD_Slave{
 public:
  volatile uint8_t error_code;
  uint8_t buffer[I2C_BUFFER_LEN + 1]; 
  volatile uint8_t data_ready;
  volatile uint8_t buffer_size;
  I2SD_Slave();
  boolean init();
  void err_out(uint8_t err_no);
  void setTX_LED(boolean state);
  void setRX_LED(boolean state);
  boolean open(char* filename, uint8_t mode);
  void close();
  File file;
  uint8_t file_mode;
  unsigned long cursor;
  uint8_t pong_data[I2C_BUFFER_LEN];
  uint8_t next_pong;

 private:
};
void I2SD_Slave_onRequest();
void I2SD_Slave_onReceive(int n_byte);
#endif
