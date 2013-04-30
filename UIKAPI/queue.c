
#include "queue.h"


struct QueueRecord {
  int Capacity;
  int Front;
  int Rear;
  int Size;
  ElementType *Array;
};

int IsEmptyQueue(Queue Q) {
  return Q->Size == 0;
}

int IsFull(Queue Q) {
  return Q->Size == Q->Capacity;
}

Queue CreateQueue(int MaxElements) {
  Queue Q;


  Q = malloc (sizeof(struct QueueRecord));
  if (Q == NULL) {
    return NULL;
  }

  Q->Array = malloc( sizeof(ElementType) * MaxElements );
  if (Q->Array == NULL) {
    free(Q);
    return NULL;
  }

  Q->Capacity = MaxElements;
  MakeEmptyQueue(Q);

  return Q;
}

void MakeEmptyQueue(Queue Q) {

  Q->Size = 0;
  Q->Front = 1;
  Q->Rear = 0;

}

void DisposeQueue(Queue Q) {
  if (Q != NULL) {
    free(Q->Array);
    free(Q);
  }
}

static int Succ(int Value, Queue Q) {
  if (++Value == Q->Capacity) {
    Value = 0;
  }
  return Value;
}

void Enqueue(ElementType X, Queue Q) {

  if (IsFull(Q)) {
    return;
  } else {
    Q->Size++;
    Q->Rear = Succ(Q->Rear, Q);
    Q->Array[Q->Rear] = X;
  }

}

ElementType Front(Queue Q) {

  if (!IsEmptyQueue(Q)) {
    return Q->Array[Q->Front];
  }

  /* Return value to avoid warnings from the compiler */
  return 0;

}

void Dequeue(Queue Q) {

  if (IsEmptyQueue(Q)) {
    return;
  } else {
    Q->Size--;
    Q->Front = Succ(Q->Front, Q);
  }

}

ElementType FrontAndDequeue(Queue Q) {

  ElementType X = 0;

  if (IsEmptyQueue(Q)) {
    return X;
  } else {
    Q->Size--;
    X = Q->Array[Q->Front];
    Q->Front = Succ(Q->Front, Q);
  }
  return X;

}
