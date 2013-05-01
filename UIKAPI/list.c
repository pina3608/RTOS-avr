
#include "list.h"

#include <avr/io.h>//TODO deleteme

/* Define Linked List Node */
struct Node {
  ElementType Element;
  Position    Next;
};

List MakeEmpty(List L) {

  if (L != NULL) {
    DeleteList(L);
  }

  L = malloc( sizeof(struct Node) );

  if (L == NULL) {
    return NULL;
  }

  L->Next = NULL;
  L->Element = 0;
  return L;

}

/* Return "true" if L is empty */
int IsEmpty(List L) {
  return L->Next == NULL;
}

/*
 * Return "true" if P is the last position in list L.
 * (Parameter L is unused in this implementation)
 */
int IsLast(Position P, List L) {
  return P->Next == NULL;
}

/* Return Position of X in L; NULL if not found */
Position Find(ElementType X, List L) {

  Position P;

  P = L->Next;
  while(P != NULL && P->Element != X) {
    P = P->Next;
  }
  return P;
}

/*
 * Delete a node from the passed in list L.
 * Cell pointed to by P->Next is deleted.
 * This implementation assumes that the position
 * is legal.
 * This implementation also assumes the use
 * of a header (dummy) node.
 */
void Delete(ElementType X, List L) {

  Position P, TmpCell;

  P = FindPrevious(X, L);

  /* Assumption use of a header node */
  if (!IsLast(P, L)) {

    /* Position (X) is found; delete it */
    TmpCell = P->Next;

    /* Bypass deleted cell */
    P->Next = TmpCell->Next;
    free(TmpCell);

  }

}

/*
 * If X is not found, then Next field of returned
 * value is NULL
 * This implementation assumes the use of
 * a header node.
 */
Position FindPrevious(ElementType X, List L) {

  Position P;

  P = L;
  while (P->Next != NULL && P->Next->Element != X) {
    P = P->Next;
  }

 return P;

}

/*
 * Insert (after legal position P)
 * This implementation assumes the use of
 * a header node.
 */
void Insert( ElementType X, List L, Position P ) {

  Position TmpCell;

  TmpCell = malloc( sizeof( struct Node ) );
  if(TmpCell == NULL) PORTB= 0x01;

  TmpCell->Element = X;
  TmpCell->Next = P->Next;
  P->Next = TmpCell;
  

}


/* Correct DeleteList algorithm */
void DeleteList(List L) {

  Position P, Tmp;

  /* Header assumed */
  P = L->Next;
  L->Next = NULL;
  while(P != NULL) {
    Tmp = P->Next;
    free(P);
    P = Tmp;
  }

}

Position Header(List L) {
  return L;
}

Position First(List L) {
  return L->Next;
}

Position Advance(Position P) {
  return P->Next;
}

ElementType Retrieve(Position P) {
  return P->Element;
}

