#define MCU attiny85
#define F_CPU 8000000UL;

#include <avr/io.h>
#include <util/delay.h>

#define PIN_FIRE PB0
#define PIN_TRIGGER PB2
#define PIN_MODE PB3
#define PIN_DELAY ADC2

const int triggerRegMask = B00000100;
const int modeRegMask = B00001000;
int shotTime = 0;

int main (void){
  DDRB |= (1 << PIN_FIRE); 

  DDRB &= ~(1 << PIN_TRIGGER);
  PORTB |= (1 << PIN_TRIGGER);

  DDRB &= ~(1 << PIN_MODE);
  PORTB |= (1 << PIN_MODE);

  PORTB &= ~(1 << PIN_FIRE);

  ADMUX = B10100010;   //sets 1.1V IRV, sets ADC2 as input channel, and left adjusts
  ADCSRA = B10000011;  //turn on ADC, keep ADC single conversion mode,

  while (1) {
    if(!(PINB & modeRegMask)){
      //Full auto
      while(!(PINB & triggerRegMask)){
        PORTB |= (1 << PIN_FIRE);
      }
      PORTB &= ~(1 << PIN_FIRE);
    }
    else{
      //Semi auto
      if(!(PINB & triggerRegMask)){
        PORTB |= (1 << PIN_FIRE);
        _delay_ms(shotTime);
        PORTB &= ~(1 << PIN_FIRE);
        while(!(PINB & triggerRegMask));
      }
      ADCSRA |= (1 << ADSC);  //start conversion
      shotTime = (ADCH)*4;
    }
  }
  return 1;
}