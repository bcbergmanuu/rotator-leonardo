#include <Arduino.h>
//rotary encoder sketch for arduino leonardo

#define pin_a 2
#define pin_b 3

int16_t position, prevposition;
uint8_t a, b, preva, prevb;

void update () {
  cli();
  
  uint8_t st_a = (PIND & (1<<PIND0)) >> PIND0;
  uint8_t st_b = (PIND & (1<<PIND1)) >> PIND1;
  
  //switch has 11 => 00 illigal action
  if((st_a != preva) && (st_b != prevb)) return;  
  a = st_a;
  b = st_b;

  //noise causes multiple interrupts for same value
  if((a == preva) && (b == prevb)) return;  
  
  //check for previous a and b to skip illigal actions
       if((a == 0) && (b == 1) && (preva == 0) && (prevb == 0)) position++;
  else if((a == 1) && (b == 1) && (preva == 0) && (prevb == 1)) position++;
  else if((a == 1) && (b == 0) && (preva == 0) && (prevb == 0)) position--;
  else if((a == 0) && (b == 0) && (preva == 0) && (prevb == 1)) position--;

  else if((a == 0) && (b == 1) && (preva == 1) && (prevb == 1)) position--;
  else if((a == 1) && (b == 1) && (preva == 1) && (prevb == 0)) position--;
  else if((a == 1) && (b == 0) && (preva == 1) && (prevb == 1)) position++;
  else if((a == 0) && (b == 0) && (preva == 1) && (prevb == 0)) position++;
  
  preva = a;
  prevb = b;  
  sei();
 };


 ISR(INT0_vect) {  
   update();
 }

 ISR(INT1_vect) {  
   update();  
 }


 ISR(PCINT0_vect) {  
   
 }

 
void InitEncoder() {
     // set pin a and b to be input
  pinMode(pin_a, INPUT); 
  pinMode(pin_b, INPUT);
    // and turn on pull-up resistors
  digitalWrite(pin_a, HIGH);
  digitalWrite(pin_b, HIGH);
};

void setup() {
  Serial.begin(112500);
  InitEncoder();

  //interruptenable registers
  EICRA |= (1<<ISC10) | (1<<ISC00);
  //EICRB |= (1<<ISC61);  
  EIMSK |= (1<<INT0) | (1<<INT1);// | (1<<INT6);
  PCMSK0 |= (1<<PCINT0) | (1<<PCINT1);
  PCICR |= (1<<PCIE0); 
  
  //power down registers
  //SMCR |= (1<<SM2) | (1<<SM1) | (1<<SM0);
  PRR0 |= (1<<PRTWI) | (1<<PRSPI) | (1<<PRADC);  
}

void loop()
{     
  if(position != prevposition) {    
    prevposition = position;      
    
    Serial.println(position);
    
  }   
}