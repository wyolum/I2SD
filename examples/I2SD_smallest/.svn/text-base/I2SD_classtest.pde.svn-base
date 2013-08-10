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
  Wire.begin(I2SD_SLAVE_ID);
  i2sd.init();
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
  Serial.print("error? ");
  Serial.println(i2sd.error_code);
  delay(200);
}
