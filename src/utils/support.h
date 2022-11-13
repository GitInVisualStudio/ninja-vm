/*
 * support.h -- object representation and support functions
 */


#ifndef _SUPPORT_H_
#define _SUPPORT_H_

#include "heap.h"

// Obj ref is acutally an alloc pointer
typedef struct Alloc* ObjRef;

void fatalError(char *msg);		/* print a message and exit */
void * newPrimObject(int dataSize);	/* create a new primitive object */
void * getPrimObjectDataPointer(void * primObject);
int getPrimSize(void* primObject);

#endif /* _SUPPORT_H_ */