#include <stdio.h>
#include <unistd.h>
#include "debug.h"

#define STRING_SIZE 20

int debug = 0;


void printDouble (const double *src, size_t n, const char *msg) {
    if (debug) {
        printf ("%s %s: ", msg, "double");
        for (int i = 0;  i < n;  i++)
            printf ("[%d]=%lf ", i, src[i]);
        printf ("\n-------------------------------\n");
    }
}

void printInt (const int *src, size_t n, const char *msg) {
    if (debug) {
        printf ("%s int: ", msg);
        for (int i = 0;  i < n;  i++)
            printf ("[%d]=%d ", i, src[i]);
        printf ("\n-------------------------------\n");
    }
}

void printLong (const long *src, size_t n, const char *msg) {
    if (debug) {
        printf ("%s long: ", msg);
        for (int i = 0;  i < n;  i++)
            printf ("[%d]=%ld ", i, src[i]);
        printf ("\n-------------------------------\n");
    }
}

void printString (char * *src, size_t n, const char *msg) {
    if (debug) {
        printf ("%s string: ", msg);
        for (int i = 0;  i < n;  i++) {
            printf ("[%d]=%s ", i, src[i]);
        }   
        printf ("\n-------------------------------\n");
    }
}
