#include "pins_arduino.h"
/*
 * an extension to the interrupt support for arduino.
 * add pin change interrupts to the external interrupts, giving a way
 * for users to have interrupts drive off of any pin.
 * Refer to avr-gcc header files, arduino source and atmega datasheet.
 */

/*
 * Theory: all IO pins on Atmega168 are covered by Pin Change Interrupts.
 * The PCINT corresponding to the pin must be enabled and masked, and
 * an ISR routine provided.  Since PCINTs are per port, not per pin, the ISR
 * must use some logic to actually implement a per-pin interrupt service.
 */

/* Pin to interrupt map:
 * D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
 * D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
 * A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
 */

volatile uint8_t *port_to_pcmask[] = {
  &PCMSK0,
  &PCMSK1,
  &PCMSK2
};

static int PCintMode[24];

typedef void (*voidFuncPtr)(void);

volatile static voidFuncPtr PCintFunc[24] = { 
  NULL };

volatile static uint8_t PCintLast[3];

/*
 * attach an interrupt to a specific pin using pin change interrupts.
 */
 void PCattachInterrupt(uint8_t pin, void (*userFunc)(void), int mode) {
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  uint8_t slot;
  volatile uint8_t *pcmask;

  // map pin to PCIR register
  if (port == NOT_A_PORT) {
    return;
  } 
  else {
    port -= 2;
    pcmask = port_to_pcmask[port];
  }

// -- Fix by Baziki. In the original sources it was a little bug, which cause analog ports to work incorrectly.
  if (port == 1) {
     slot = port * 8 + (pin - 14);
  }
  else {
     slot = port * 8 + (pin % 8);
  }
// --Fix end
  PCintMode[slot] = mode;
  PCintFunc[slot] = userFunc;
  // set the mask
  *pcmask |= bit;
  // enable the interrupt
  PCICR |= 0x01 << port;
}

void PCdetachInterrupt(uint8_t pin) {
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *pcmask;

  // map pin to PCIR register
  if (port == NOT_A_PORT) {
    return;
  } 
  else {
    port -= 2;
    pcmask = port_to_pcmask[port];
  }

  // disable the mask.
  *pcmask &= ~bit;
  // if that's the last one, disable the interrupt.
  if (*pcmask == 0) {
    PCICR &= ~(0x01 << port);
  }
}

// common code for isr handler. "port" is the PCINT number.
// there isn't really a good way to back-map ports and masks to pins.
static void PCint(uint8_t port) {
  uint8_t bit;
  uint8_t curr;
  uint8_t mask;
  uint8_t pin;

  // get the pin states for the indicated port.
  curr = *portInputRegister(port+2);
  mask = curr ^ PCintLast[port];
  PCintLast[port] = curr;
  // mask is pins that have changed. screen out non pcint pins.
  if ((mask &= *port_to_pcmask[port]) == 0) {
    return;
  }
  // mask is pcint pins that have changed.
  for (uint8_t i=0; i < 8; i++) {
    bit = 0x01 << i;
    if (bit & mask) {
      pin = port * 8 + i;
      // Trigger interrupt if mode is CHANGE, or if mode is RISING and
      // the bit is currently high, or if mode is FALLING and bit is low.
      if ((PCintMode[pin] == CHANGE
          || ((PCintMode[pin] == RISING) && (curr & bit))
          || ((PCintMode[pin] == FALLING) && !(curr & bit)))
          && (PCintFunc[pin] != NULL)) {
        PCintFunc[pin]();
      }
    }
  }
}


SIGNAL(PCINT0_vect) {
  PCint(0);
}
SIGNAL(PCINT1_vect) {
  PCint(1);
}
SIGNAL(PCINT2_vect) {
  PCint(2);
}

volatile uint16_t valid_msg = 0;
volatile unsigned long pulse_start = 0;
const int BIT_BUFFER_LEN = 16;
volatile boolean bits[BIT_BUFFER_LEN];
volatile unsigned long pws[BIT_BUFFER_LEN];
volatile uint16_t buffer = 1;
volatile uint16_t msg = 1;
volatile int bit_i = 0;
const short pw_thresh = 1000;
const uint16_t START_SEQ = 0x00ff;
const uint16_t STOP_SEQ = 0x005f;
const uint16_t STOP_SEQ_LEN = 8;
const uint16_t MSG_LEN = 10;
const uint16_t STARTING = 1;
const uint16_t MSG_STATE = 2;
const uint16_t STOPPING = 3;
volatile uint16_t state = STARTING;

long i = 1;

void tock(void) {
  boolean bit;
  unsigned long pw;

  if (digitalRead(A3)){
    digitalWrite(3, LOW);
    pulse_start = micros();
  }
  else{ // ## on pulse complete
    digitalWrite(3, HIGH);
    pw = micros() - pulse_start;
    bit = pw > pw_thresh;
    buffer = ((buffer << 1) + bit);
    bit_i++;
    if(state == STARTING){ // 00000000111111111
      if(buffer == START_SEQ){
	state = MSG_STATE;
	buffer = 0;
	bit_i = 0;
      }
    }
    else if(state == MSG_STATE){
      if(bit_i == MSG_LEN){
	msg = buffer;
	state = STOPPING;
	buffer = 1;
	bit_i = 0;
      }
    }
    else if(state == STOPPING){
      if(bit_i == STOP_SEQ_LEN){
	if(buffer & STOP_SEQ == STOP_SEQ){
	  valid_msg = msg;
	}
	else{
	  valid_msg = 0;
	}
	msg = 0;
	state = STARTING;
	buffer = 1;
	bit_i = 0;
      }
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("IR2");
  pinMode(A3, INPUT);
  PCattachInterrupt(A3, tock, CHANGE);
  digitalWrite(A3, HIGH);
}

void loop() {
  i++;
  delay(1);
  if(valid_msg){
    Serial.print(valid_msg, DEC);
    Serial.println("");
    valid_msg = 0;
  }
}
