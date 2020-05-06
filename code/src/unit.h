#ifndef __UNIT_H
#define __UNIT_H

//TESTING
typedef void (*TESTFUNCTION)(void *, size_t, size_t,int);

extern TESTFUNCTION testFunction0[];
extern char *testNames0[];
extern int nTestFunction0;


extern TESTFUNCTION testFunction1[];
extern char *testNames1[];
extern int nTestFunction1;


extern TESTFUNCTION testFunction2[];
extern char *testNames2[];
extern int nTestFunction2;


extern TESTFUNCTION testFunction3[];
extern char *testNames3[];
extern int nTestFunction3;


extern TESTFUNCTION testFunction4[];
extern char *testNames4[];
extern int nTestFunction4;


extern TESTFUNCTION testFunction5[];
extern char *testNames5[];
extern int nTestFunction5;


extern TESTFUNCTION testFunction6[];
extern char *testNames6[];
extern int nTestFunction6;


//VALIDATING
typedef void (*VALIDATEFUNCTION)(void *, size_t, size_t);

extern VALIDATEFUNCTION validateFunction[];
extern char *validateNames[];
extern int nValidateFunction;

#endif