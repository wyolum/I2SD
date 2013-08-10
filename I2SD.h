/*
  I2SD.h -- I2C <--> SD interface.
  (This is the interface for the client code see I2SD_Slave.h for 
   the interface to the device code)

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
#ifndef I2SD_H
#define I2SD_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#define WIRE_WRITE(ptr,n_byte) (Wire.write((uint8_t*)(ptr), (n_byte)))
#define WIRE_WRITE1(one_byte) (Wire.write((uint8_t)(one_byte)))
#define WIRE_READ Wire.read()
#else
#include "WProgram.h"
#define WIRE_WRITE(ptr,n_byte) (Wire.send((uint8_t*)(ptr), (n_byte)));
#define WIRE_WRITE1(one_byte) (Wire.send((one_byte)))
#define WIRE_READ Wire.receive()
#endif

#include <inttypes.h>
#include "C3SB.h"
#include "SD.h"
#include "Wire.h"

const uint8_t I2SD_TX_LED_PIN = 2;
const uint8_t I2SD_RX_LED_PIN = 3;
const uint8_t I2SD_SLAVE_SELECT = 10;

const uint8_t I2SD_NO_ERROR = 0;
const uint8_t I2SD_INIT_ERROR = 1;
const uint8_t I2SD_OPEN_ERROR = 2;
const uint8_t I2SD_MODE_ERROR = 3;
const uint8_t I2SD_EOF_ERROR = 4;


const uint8_t I2SD_SLAVE_ID = 81;

// message types
enum MsgTypes {
  I2SD_PING_MSG = 1,
  I2SD_WRITE_MSG = C3SB_WRITE_MSG, // 2
  I2SD_SEEK_MSG,                   // 3
  I2SD_OPEN_MSG,                   // 4
  I2SD_N_BYTE_MSG,                 // 5
  I2SD_CLEAR_ERROR_MSG,            // 6
};
/*const uint8_t I2SD_PING_MSG = 1;
  const uint8_t I2SD_WRITE_MSG = C3SB_WRITE_MSG;
  const uint8_t I2SD_SEEK_MSG = C3SB_WRITE_MSG + 1;
  const uint8_t I2SD_OPEN_MSG = C3SB_WRITE_MSG + 2;
*/
union Address_t {
  uint32_t dat32; 
  char char4[4];
};
extern Address_t Address;


class I2SD{
 public:
  unsigned long cursor;

  C3SB c3sb;

  /* Transmit n_byte bytes of data to I2SD card.
   * return true if identical bytes are returned from I2SD card
   */
  boolean ping(uint8_t* ping_data, uint8_t n_byte);

  /* Move file cursor to addr bytes from the beginning of the file
   */
  void seek(unsigned long addr);

  /* Open a file with the specified name and mode
   * May induce I2SD_OPEN_ERROR on I2SD card
   */
  void open(char* filename, uint8_t mode);

  /* Read n_byte bytes of the current file.
   * File must be open for reading
   */
  uint8_t read(uint8_t *data, uint8_t n_byte);

  /* Write n_byte bytes of data to current file
   * File must be open for writing
   * May induce I2SD_MODE_ERROR on I2SD card
   */
  void write(uint8_t *data, uint8_t n_byte);

  /* Clear any error on I2SD card.
   * May induce I2SD_MODE_ERROR on I2SD card
   */
  void clear_error();
};
// extern I2SD i2sd; // does not work??!
#endif
