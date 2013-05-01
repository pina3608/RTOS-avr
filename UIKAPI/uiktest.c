#include "UIKAPI.h"
#include <avr/io.h>

/* CPU frequency */
#define F_CPU 1000000UL

#include <util/delay.h>
#include <avr/interrupt.h>

UIKSem* sem;


void a(){
  while(1){
          PORTB = 0xff;
          _delay_ms(300);
          UIKSemPend(sem);
          while(PORTB != 0x00){
                PORTB = (PORTB << 1);
                _delay_ms(300);
          }
          UIKSemPost(sem);
          UIKDelay(1000);
  }
}

void b(){
  while(1){
          PORTB = 0xff;
          _delay_ms(300);
          UIKSemPend(sem);
          while(PORTB != 0x00){
                PORTB = (PORTB >> 1);
                _delay_ms(300);
          }
          UIKSemPost(sem);
          UIKDelay(2000);
   }
}

void c(){
  int j;
  while(1){
          UIKAssocEvent(1);
          uint8_t aux = PORTB;
          for(j = 0; j < 5; j++){
                PORTB = 0x00;
                _delay_ms(300);
                PORTB = 0xff;
                _delay_ms(300);
          }
          PORTB = aux;
   }
}
void d(){
        while(1){
                UIKRaiseEvent(1);
                UIKDelay(1000);
        
        }

}


uint8_t a_stack[MIN_STACK_SIZE] __attribute__((weak));
uint8_t b_stack[MIN_STACK_SIZE] __attribute__((weak));
uint8_t c_stack[MIN_STACK_SIZE] __attribute__((weak));
uint8_t d_stack[MIN_STACK_SIZE] __attribute__((weak));



const int TICKLEN = 10000;

int main(void) {
  int id;


  DDRB = 0xff;
  sem = UIKSemCreate(1);
  
  UIKInitialize(TICKLEN, 5); 

  id = UIKAddTask(&a, 30, a_stack, MIN_STACK_SIZE); 
  UIKRun(id);
  
  id = UIKAddTask(&b, 35, b_stack, MIN_STACK_SIZE); 
  UIKRun(id);
  
  id = UIKAddTask(&c, 20, c_stack, MIN_STACK_SIZE); 
  UIKRun(id);
  
  id = UIKAddTask(&d, 25, d_stack, MIN_STACK_SIZE); 
  UIKRun(id);

  /*this enables interupts, so doesn't need to be done in main */
  UIKStart();
  return 0;
}

//derecha
