/*
 * Assignment #6
 *
 * Author: Santiago Pina Ros
 *
 * Size of the code: 4262 bytes
 */


/* CPU frequency */
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>


void a(){
  DDRB = 0xff;
  while(1){
          PORTB = 0xff;
          _delay_ms(300);
          while(PORTB != 0x00){
                PORTB = (PORTB << 1);
                _delay_ms(300);
          }
  }
}

void b(){
  DDRB = 0xff;
  while(1){
          PORTB = 0xff;
          _delay_ms(300);
          while(PORTB != 0x00){
                PORTB = (PORTB >> 1);
                _delay_ms(300);
          }
   }
}


void exec(void (*test) (void)){
  test();
}

int main(){
  exec(a);
  exec(b);
  PORTB = 0xff;
}
