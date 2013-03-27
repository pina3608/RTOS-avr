

#include "UIKAPI.h"


volatile uint32_t UIKTickNum = 0;

volatile uint16_t UIKTickLen = 0;

/*
 * UIKTickHandler
 */
ISR(TIMER1_COMPA_vect){

  UIKTickNum++;

}

void UIKIdle (void){

  while(1);

}

uint8_t UIKInitialize(uint16_t ticklen, uint8_t maxTasks){


  if(UIKTickLen != 0 || ticklen <= 0){
    return 0;
  }

  UIKTickLen = ticklen;
  OCR1A = ticklen;


  return 1;

}
