

/**
 *
 * Author: Santiago Pina
 *
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


void UIKInitialize(uint16_t ticklen, uint8_t maxTask);

uint8_t UIKAddTask(void(* task) (void), uint8_t priority);

void UIKRun(uint8_t taskid);

void UIKSchedule(void);


/*
**	End of Header File Information
*/
#endif
