#ifndef __DEBUG_H
#define __DEBUG_H


extern int debug;


void printDouble (const double *src, size_t n, const char *msg);

void printInt (const int *src, size_t n, const char *msg);

void printLong (const long *src, size_t n, const char *msg);

void printString (char * *src, size_t n, const char *msg);


#endif