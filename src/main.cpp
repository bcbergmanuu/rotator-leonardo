#include <Arduino.h>

#include <util/atomic.h>
//rotary encoder sketch for arduino leonardo

#define pin_a 2
#define pin_b 3

static volatile int16_t position, prevposition;
static volatile uint8_t preva, prevb;

#define ca 0 //current a
#define cb 1 //current b
#define pa 2 //past a
#define pb 3 //past b

void update () {  
  uint8_t a, b;
  a = (PIND & (1<<PIND0)) >> PIND0;
  b = (PIND & (1<<PIND1)) >> PIND1;
  //noise causes multiple interrupts for same value
  if((a == preva) && (b == prevb)) return;  
  uint8_t r = 0;
  if(a) r|= (1<<ca);
  if(b) r|= (1<<cb);
  if(preva) r |= (1<<pa);
  if(prevb) r |= (1<<pb);
  if((r == 2) || (r == 11) || (r == 13) || (r == 4)) position++;   
  else if((r==14) || (r==7) || (r ==1) || (r == 8)) position--;
  else return;
  preva = a;
  prevb = b;  
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