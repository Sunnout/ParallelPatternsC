#ifndef __UNIT_H
#define __UNIT_H

typedef void (*TESTFUNCTION)(void *, size_t, size_t);

extern TESTFUNCTION testFunction[];

extern char *testNames[];

extern int nTestFunction;


typedef void (*VALIDATEFUNCTION)(void *, size_t, size_t);

extern VALIDATEFUNCTION validateFunction[];

extern char *validateNames[];

extern int nValidateFunction;

#endif