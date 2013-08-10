/*
  Modified from SD card read/write by 
 by David A. Mellis
 updated 2 Dec 2010
 by Tom Igoe
 modified by Justin Shaw July 2011 for I2SD
 
 This example code is in the public domain.
 	 
 */
 
#include <SD.h>
const int chipSelect = 10;
const int D1_PIN = 4;
const int D2_PIN = 7;
File myFile;

void setup()
{
  pinMode(D1_PIN, OUTPUT);
  pinMode(D2_PIN, OUTPUT);
  digitalWrite(D1_PIN, HIGH);
  digitalWrite(D2_PIN, HIGH);

  delay(100);
  digitalWrite(D1_PIN, LOW);
  digitalWrite(D2_PIN, LOW);

  Serial.begin(115200);
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   digitalWrite(D1_PIN, HIGH); 
  if (!SD.begin(chipSelect)) {
    digitalWrite(D1_PIN, LOW); 
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
}

void err_out(int err_code){
  digitalWrite(D1_PIN, HIGH); 
  digitalWrite(D2_PIN, LOW); 
  while(true){
    delay(1000);
    for(int i=0; i < err_code; i++){
      digitalWrite(D2_PIN, HIGH); 
      delay(200);
      digitalWrite(D2_PIN, LOW); 
      delay(200);
    }
  }
}
unsigned long count = 0;
void loop()
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  SD.remove("test.txt");
  myFile = SD.open("test.txt", FILE_WRITE);
  
  // if the file opened okay, write to it:
  if (myFile) {
    digitalWrite(D1_PIN, HIGH); 
    digitalWrite(D2_PIN, HIGH); 
    delay(300);
    Serial.println("Writing to test.txt...");
    myFile.print("test: ");
    myFile.println(count++, DEC);
    // close the file:
    myFile.close();
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
    err_out(1);
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    
    digitalWrite(D1_PIN, HIGH); 
    digitalWrite(D2_PIN, HIGH); 
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening test.txt");
    err_out(3);
  }
  Serial.println("Delete old DEFAULT.TXT");
  SD.remove("DEFAULT.TXT");

  Serial.println("Write DEFAULT.TXT");

  myFile = SD.open("DEFAULT.TXT", FILE_WRITE);
  for(int i=0; i < 100; i++){
    myFile.print(i);
    myFile.println(" All work and no play makes Jack a dull boy");
  }
  myFile.close();
  while(1)delay(1); // energizer bunny
}


