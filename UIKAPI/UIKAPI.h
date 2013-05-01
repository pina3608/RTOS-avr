
/**
 *
 * Final RTOS
 *
 * Author: Santiago Pina
 *
 * */

/*
**	Header File Information
*/
#ifndef __UIKAPI_h__
#define __UIKAPI_h__


#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "queue.h"

#define MIN_PRIORITY 255
#define MIN_STACK_SIZE 85
#define IDLE_ID 0
#define MAX_EVENTS 8

/* Task states */
typedef enum
{
	eRunning = 0,
	eReady,
	eSemBlocked,
	eDelayBlocked,
	eEventBlocked,
	eSuspended
} eTaskState;

typedef struct TCB_ELEMENT {
  
	volatile eTaskState state;
	uint8_t priority;
	uint8_t* contextPtr;
  	uint32_t delay;

} TcbElement;


void UIKIdle();

uint8_t UIKInitialize(uint16_t ticklen, uint8_t maxTask);

uint8_t UIKAddTask(void* task, uint8_t priority, uint8_t* stack, uint16_t stackSize);

void UIKRun(uint8_t taskid);

void UIKStart();

void UIKSchedule() __attribute__ ( ( naked ) );


/* Semaphores */

typedef struct UIK_SEM {
  uint8_t value;
  Queue taskQueue;
} UIKSem;

void UIKDelay(uint16_t count);

UIKSem* UIKSemCreate(uint8_t value);

void UIKSemPend(UIKSem* sem);

void UIKSemPost(UIKSem* sem);

uint8_t UIKSemValue(UIKSem* sem);


/* Events */


void UIKAssocEvent(uint8_t Event);

void UIKDisassocEvent(uint8_t Event);

void UIKRaiseEvent(uint8_t Event);


/*
**	End of Header File Information
*/
#endif
