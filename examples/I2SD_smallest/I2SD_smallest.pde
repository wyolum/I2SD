/*
  I2SD_Slave class handles all of the I2C events.  Load this code
  directly onto the I2SD device.  


  This is slave code.
  For client code, see
  
 */

#include <inttypes.h>
#include "I2SD_Slave.h"
#include "SD.h"
#include "Wire.h"
#include "C3SB.h"

I2SD i2sd = I2SD();
C3SB c3sb = C3SB();
// File file;

void setup(){
  Wire.begin(I2SD_SLAVE_ID);
  // uint8_t dest[32];
  // c3sb.read_from(1, dest, 1);
}
void loop(){
  uint8_t dat[32];
  Wire.available();
  Wire.receive();
  Wire.beginTransmission(1);
  Wire.send(dat, 32);
  Wire.endTransmission();
#ifdef NOTDEF
  i2sd.ping(dat, 32);
  i2sd.seek(0);
  i2sd.open("test", FILE_READ);
  i2sd.read(dat, 32);
  i2sd.write(dat, 32);
  i2sd.clear_error();
#endif
}
