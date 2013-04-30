

#include "UIKAPI.h"

#define saveContext()                           \
  asm volatile ( "push  r0               \n\t"  \
                 "in    r0, __SREG__     \n\t"  \
                 "cli                    \n\t"  \
                 "push  r0               \n\t"  \
                 "push  r1               \n\t"  \
                 "clr   r1               \n\t"  \
                 "push  r2               \n\t"  \
                 "push  r3               \n\t"  \
                 "push  r4               \n\t"  \
                 "push  r5               \n\t"  \
                 "push  r6               \n\t"  \
                 "push  r7               \n\t"  \
                 "push  r8               \n\t"  \
                 "push  r9               \n\t"  \
                 "push  r10              \n\t"  \
                 "push  r11              \n\t"  \
                 "push  r12              \n\t"  \
                 "push  r13              \n\t"  \
                 "push  r14              \n\t"  \
                 "push  r15              \n\t"  \
                 "push  r16              \n\t"  \
                 "push  r17              \n\t"  \
                 "push  r18              \n\t"  \
                 "push  r19              \n\t"  \
                 "push  r20              \n\t"  \
                 "push  r21              \n\t"  \
                 "push  r22              \n\t"  \
                 "push  r23              \n\t"  \
                 "push  r24              \n\t"  \
                 "push  r25              \n\t"  \
                 "push  r26              \n\t"  \
                 "push  r27              \n\t"  \
                 "push  r28              \n\t"  \
                 "push  r29              \n\t"  \
                 "push  r30              \n\t"  \
                 "push  r31              \n\t"  \
                 "lds  r26, currentTCB   \n\t"  \
                 "lds  r27, currentTCB+1 \n\t"  \
                 "in    r0, 0x3d         \n\t"  \
                 "st    x+, r0           \n\t"  \
                 "in    r0, 0x3e         \n\t"  \
                 "st    x+, r0           \n\t"  \
        );

/* 
 * Opposite to savecontext().  Interrupts will have been disabled during
 * the context save so we can write to the stack pointer. 
 */

#define restoreContext()                                        \
  asm volatile (    "lds    r26, currentTCB               \n\t"    \
                    "lds    r27, currentTCB+1             \n\t"    \
                    "ld     r28, x+                    \n\t"    \
                    "out    __SP_L__, r28              \n\t"    \
                    "ld        r29, x+                 \n\t"    \
                    "out    __SP_H__, r29              \n\t"    \
                    "pop    r31                        \n\t"    \
                    "pop    r30                        \n\t"    \
                    "pop    r29                        \n\t"    \
                    "pop    r28                        \n\t"    \
                    "pop    r27                        \n\t"    \
                    "pop    r26                        \n\t"    \
                    "pop    r25                        \n\t"    \
                    "pop    r24                        \n\t"    \
                    "pop    r23                        \n\t"    \
                    "pop    r22                        \n\t"    \
                    "pop    r21                        \n\t"    \
                    "pop    r20                        \n\t"    \
                    "pop    r19                        \n\t"    \
                    "pop    r18                        \n\t"    \
                    "pop    r17                        \n\t"    \
                    "pop    r16                        \n\t"    \
                    "pop    r15                        \n\t"    \
                    "pop    r14                        \n\t"    \
                    "pop    r13                        \n\t"    \
                    "pop    r12                        \n\t"    \
                    "pop    r11                        \n\t"    \
                    "pop    r10                        \n\t"    \
                    "pop    r9                         \n\t"    \
                    "pop    r8                         \n\t"    \
                    "pop    r7                         \n\t"    \
                    "pop    r6                         \n\t"    \
                    "pop    r5                         \n\t"    \
                    "pop    r4                         \n\t"    \
                    "pop    r3                         \n\t"    \
                    "pop    r2                         \n\t"    \
                    "pop    r1                         \n\t"    \
                    "pop    r0                         \n\t"    \
                    "out    __SREG__, r0               \n\t"    \
                    "pop    r0                         \n\t"    \
                );
                
               
//TODO: delete void SIG_OUTPUT_COMPARE1A( void ) __attribute__ ( ( signal, naked ) ); 
volatile uint32_t UIKTickNum = 0;
volatile uint16_t UIKTickLen = 0;
volatile TcbElement *tcb;//TODO: Change to array or create list
volatile uint8_t* currentTCB; // Pointer to the current task
uint8_t currentTask;

volatile uint8_t maxTaskNumber = 0;
volatile uint8_t taskNum= 0;


uint8_t idleStack[MIN_STACK_SIZE] __attribute__((weak));



List assocEvents[MAX_EVENTS];


void UIKIdle(){

  while(1);

}


void UIKTick(){
  int i;
  UIKTickNum++;
  for(i = 0; i < taskNum; i++) {
    //decrement delays where necessary
    if(tcb[i].delay > 0) {
	  tcb[i].delay--;
	  if(tcb[i].delay == 0) {
	    //the state should have been waiting
	    tcb[i].state = eReady;
	  }	
    }
  }
}


void UIKDispatcher() {

  int high = IDLE_ID;
  int i;

  for(i = 0; i < taskNum; i++) {
	//find highest priority ready task
    if(tcb[i].state == eReady && tcb[i].priority < tcb[high].priority) {
	  high = i;
	}
  }

  currentTask = high;
  currentTCB = tcb[high].contextPtr;
  
}

/*
 * UIKTickHandler
 */

ISR(TIMER1_COMPA_vect, ISR_NAKED){

  saveContext();
  UIKTick();
  UIKDispatcher();
  restoreContext();
  asm volatile ("reti");

}

uint8_t UIKInitialize(uint16_t ticklen, uint8_t maxTasks){

  int i;
  if(UIKTickLen != 0 || ticklen <= 0){
    return 0;
  }
  
  tcb = (TcbElement *) calloc(maxTasks, sizeof(TcbElement));
  if(tcb == NULL){
  	return 0;
  }
  
  maxTaskNumber = maxTasks;
  UIKTickLen = ticklen;
  
  // Enable interrupts
  TCCR1B |= (1 << WGM12);
  TIMSK |= (1 << OCIE1A); 
  OCR1A = ticklen;
  TCCR1B |= (1 << CS10);
  
  if (UIKAddTask(&UIKIdle, MIN_PRIORITY, idleStack, MIN_STACK_SIZE) > maxTaskNumber)
  	return 0;

  for(i = 0; i < MAX_EVENTS; i++) {
	assocEvents[i] = MakeEmpty(NULL);
  }
  
  return 1;

}

uint8_t UIKAddTask(void* task, uint8_t priority, uint8_t* stack, uint16_t stackSize){

    
  if (taskNum > maxTaskNumber) {
    return taskNum;
  }
  
  
  tcb[taskNum].state = eSuspended;
  tcb[taskNum].priority = priority;
  tcb[taskNum].delay = 0;
  


  // The stack grows down in memory 
  uint8_t* contextPtr = stack + stackSize - 1;

 
  //TODO: delete tcb[taskNum].stack_lptr = contextPtr;
  tcb[taskNum].contextPtr = contextPtr - 1;

  // place a few known bytes on the bottom - useful for debugging 
  *contextPtr = 0x11; 
  contextPtr--;
  *contextPtr = 0x22; 
  contextPtr--;
  *contextPtr = 0x33;
  contextPtr--;
  
  // Address of the task
  *contextPtr = ((unsigned int) task) & 0x00ff;
  contextPtr--;
  *contextPtr = (((unsigned int) task) >> 8) & 0x00ff;
  contextPtr--;
  
  //simulate stack after a call to savecontext
  *contextPtr = 0x00;  //r0
  contextPtr--;
  *contextPtr = 0x00;  // necessary for retiTODO: tray 0x08
  contextPtr--;
  *contextPtr = 0x00;  //r1 wants to always be 0
  contextPtr--;
  *contextPtr = 0x02;  //r2
  contextPtr--;
  *contextPtr = 0x03;  //r3
  contextPtr--;
  *contextPtr = 0x04;  //r4
  contextPtr--;
  *contextPtr = 0x05;  //r5
  contextPtr--;
  *contextPtr = 0x06;  //r6
  contextPtr--;
  *contextPtr = 0x07;  //r7
  contextPtr--;
  *contextPtr = 0x08;  //r8
  contextPtr--;
  *contextPtr = 0x09;  //r9
  contextPtr--;
  *contextPtr = 0x10;  //r10
  contextPtr--;
  *contextPtr = 0x11;  //r11
  contextPtr--;
  *contextPtr = 0x12;  //r12
  contextPtr--;
  *contextPtr = 0x13;  //r13
  contextPtr--;
  *contextPtr = 0x14;  //r14
  contextPtr--;
  *contextPtr = 0x15;  //r15
  contextPtr--;
  *contextPtr = 0x16;  //r16
  contextPtr--;
  *contextPtr = 0x17;  //r17
  contextPtr--;
  *contextPtr = 0x18;  //r18
  contextPtr--;
  *contextPtr = 0x19;  //r19
  contextPtr--;
  *contextPtr = 0x20;  //r20
  contextPtr--;
  *contextPtr = 0x21;  //r21
  contextPtr--;
  *contextPtr = 0x22;  //r22
  contextPtr--;
  *contextPtr = 0x23;  //r23
  contextPtr--;
  *contextPtr = 0x24;  //r24
  contextPtr--;
  *contextPtr = 0x25;  //r25
  contextPtr--;
  *contextPtr = 0x26;  //r26
  contextPtr--;
  *contextPtr = 0x27;  //r27
  contextPtr--;
  *contextPtr = 0x28;  //r28
  contextPtr--;
  *contextPtr = 0x29;  //r29
  contextPtr--;
  *contextPtr = 0x30;  //r30
  contextPtr--;
  *contextPtr = 0x31;  //r31
  contextPtr--;
  
   
  //store the address of the stack 
  *(tcb[taskNum].contextPtr) = (((unsigned int) contextPtr) & 0xff);
  *(tcb[taskNum].contextPtr + 1) = ((((unsigned int) contextPtr) >> 8) & 0xff);

  taskNum++; 
  
  //return the task id 
  return(taskNum - 1);
	
}


void UIKRun(uint8_t taskid) {
  //the scheduler will run the task if it has the maximum priority
  tcb[taskid].state = eReady;
}

void UIKStart() {
  //start with the idle process
  tcb[0].state = eReady;
  currentTask = IDLE_ID;
  currentTCB = tcb[IDLE_ID].contextPtr;
  restoreContext();
  asm volatile ("reti");
}

//TODO: delete ? void UIKSchedule() __attribute__ ( ( naked ) );
void UIKSchedule() {
  saveContext();
  UIKDispatcher();
  restoreContext();
  asm volatile ("reti");
}

void UIKDelay(uint16_t count) {
  tcb[currentTask].state = eDelayBlocked;
  tcb[currentTask].delay = count;
  UIKSchedule(); 
}

UIKSem* UIKSemCreate(uint8_t value){
	UIKSem* sem;
	sem = (UIKSem *)malloc(sizeof(UIKSem));
	sem->value = value;
	sem->taskQueue = CreateQueue(maxTaskNumber);
	return sem;
}

void UIKSemPend(UIKSem* sem) {
  cli();
  if(sem->value > 0) {
  	sem->value--;
    sei();
  }
  else{
    Enqueue(currentTask, sem->taskQueue);
    tcb[currentTask].state = eSemBlocked;
    UIKSchedule();	  
  }
}

void UIKSemPost(UIKSem* sem) {
  cli();
  if(!IsEmptyQueue(sem->taskQueue)){
  	tcb[Front(sem->taskQueue)].state = eReady;
  	Dequeue(sem->taskQueue);
  	UIKSchedule();
  }
  else{
  	sem->value++;
    sei(); 
  }
}

uint8_t UIKSemValue(UIKSem* sem) {
  return sem->value;
}

void UIKAssocEvent(uint8_t Event) {

  if(Event == 0)
  	return;

  cli();
  int i = 0;
  while(Event != 0){
  	if((Event & 0x01) != 0){
  		List L = assocEvents[i];
  		Position P = Header(L);
  		Insert(currentTask, L, P);
  	}
  	Event = Event >> 1;
  	i++;
  }
 
  tcb[currentTask].state = eEventBlocked;
  UIKSchedule();
}


void UIKDisassocEvent(uint8_t Event) {
  cli();
  int i = 0;
  while(Event != 0){
  	if((Event & 0x01) != 0){
  		Delete(currentTask, assocEvents[i]);
  	}
  	Event = Event >> 1;
  	i++;
  }
  sei();
}

void UIKRaiseEvent(uint8_t Event){
	
  cli();
  int i = 0;
  while(Event != 0){
  	if((Event & 0x01) != 0){
  		List L = assocEvents[i];
  		Position P = Header(L);
  		while(!IsLast(P,L)){
  			P = Advance(P);
  			if(tcb[Retrieve(P)].state == eEventBlocked)
  			 tcb[Retrieve(P)].state= eReady;
  		}
  	}
  	Event = Event >> 1;
  	i++;
  }
  UIKSchedule();
}