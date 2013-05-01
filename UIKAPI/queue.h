
#include <inttypes.h>
#include <stdlib.h>

typedef uint8_t ElementQueueType;

#ifndef _Queue_h

  struct QueueRecord;
  typedef struct QueueRecord *Queue;

  int         IsEmptyQueue(Queue Q);
  int         IsFull(Queue Q);
  Queue       CreateQueue(int MaxElements);
  void        DisposeQueue(Queue Q);
  void        MakeEmptyQueue(Queue Q);
  void        Enqueue(ElementQueueType X, Queue Q);
  ElementQueueType Front(Queue Q);
  void        Dequeue(Queue Q);
  ElementQueueType FrontAndDequeue(Queue Q);

#endif  /* _Queue_h */

