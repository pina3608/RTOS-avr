#include "UIKAPI.h"
#include <avr/io.h>
#include <avr/interrupt.h>




/*count the lower bytes of PORTB up from 0 to 8 */
uint8_t taskcount_stack[MIN_STACK_SIZE] __attribute__((weak));



/*this task counts to 8 (highest priority) */
void taskcount() {
  uint8_t cnt;
  while(1) {
    for (cnt=0; cnt < 0x08; cnt++) {
//	  UIKSemPend(portbsem);
      PORTB = (PORTB & 0xf0) | (0x0f - cnt);
//	  UIKSemPost(portbsem);
//	  UIKDelay(100);
	  }
  }
}





/* As per the specs, TICKLEN is the number of 10^-9 seconds on a 1MHz chip */
/* as set below it 1 TICKLEN == 1/100 seconds */
const int TICKLEN = 10000;

int main(void) {
  int id;

  /*Initialize the ports */
  DDRB = 0xff; 
  DDRD = 0x00; 

  /*semaphore to protect portb */
 // portbsem = UIKSemCreate();
  PORTB = 0xff;
 
  /*Initialize the RTOS - this sets up the timer */
  UIKInitialize(TICKLEN, 5); 

  /*Tell the RTOS about our tasks */
  id = UIKAddTask(&taskcount, 30, taskcount_stack, MIN_STACK_SIZE); 
  UIKRun(id);

  /*this enables interupts, so doesn't need to be done in main */
  UIKStart();
  return 0;
}


