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

I2SD_Slave i2sd = I2SD_Slave();
// File file;

void setup(){
  Serial.begin(57600);
  Serial.println("I2SD Slave v1.0");
  Serial.println("Copyright WyoLum, LLC 2012");
  Wire.begin(I2SD_SLAVE_ID);
  i2sd.init();

  
  // From MaceTech.com
  // set 1Hz reference square wave
  Wire.beginTransmission(0x68); // address DS3231
  Wire.write(0x0E); // select register
  // Wire.write(0b00000000, 1); // write register bitmap, bit 7 is /EOSC
  Wire.write(0b00011100); 
  Wire.endTransmission();
}
void loop(){
  if(i2sd.error_code){
    Serial.print("ERROR OUT.  err_no: ");
    Serial.println(i2sd.error_code, DEC);
    digitalWrite(I2SD_TX_LED_PIN, HIGH); 
    digitalWrite(I2SD_RX_LED_PIN, LOW); 
    while(i2sd.error_code){
      delay(1000);
      for(int i=0; i < i2sd.error_code; i++){
	digitalWrite(I2SD_RX_LED_PIN, HIGH); 
	delay(200);
	digitalWrite(I2SD_RX_LED_PIN, LOW); 
	delay(200);
      }
    }
  }
  //Serial.print("error? ");
  //Serial.println(i2sd.error_code);
  delay(200);
}
