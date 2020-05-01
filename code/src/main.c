#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <sys/time.h>

#include "unit.h"
#include "debug.h"


/*------------------------------------ TYPE DOUBLE---------------------------------*/


/*
#define TYPE double
#define RAND() drand48()
#define PRINT(SRC, N, STRING) printDouble(SRC, N, STRING)

*/

/*------------------------------------ TYPE  LONG ---------------------------------*/


/*
#define TYPE long
#define RAND() lrand48()
#define PRINT(SRC, N, STRING) printLong(SRC, N, STRING)
*/

/*------------------------------------ TYPE INT ---------------------------------*/

/*
#define TYPE int
#define RAND() rand()
#define PRINT(SRC, N, STRING) printInt(SRC, N, STRING)
*/

/*------------------------------------ TYPE String ---------------------------------*/



#define TYPE char *
#define PRINT(SRC, N, STRING) printString(SRC, N, STRING)
#define RAND() randString()


#define STRING_SIZE 20
char * randString (void) {
    char * p = malloc(STRING_SIZE+1);
    for (int i=0; i < STRING_SIZE; i++) {
        p[i] = lrand48()%('z'-'a'+1)+'a';
    }
    p[STRING_SIZE] = '\0';
    return p;
}






////////////////////////////////////////////////////////////////////////////////////////
/// Get wall clock time as a double
/// You may replace this with opm_get_wtime()
double wctime () {
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return tv.tv_sec + 1E-6 * tv.tv_usec;
}



int main(int argc, char* argv[]) {
    int i, N;
    int validate = 0;
    

    int c;
    while ((c = getopt (argc, argv, "dv")) != -1)
    switch (c) {
        case 'd':
            debug = 1; break;
        case 'v':
            validate = 1; break;
        default:
            printf("Invalid option\n");
            abort ();
    }
    argc -= optind;
    argv += optind;
    
    if (argc != 1) {
        printf("Usage: ./example N\n");
        return -1;
    }

    srand48(time(NULL));
    srand48(time(NULL));

    N = atol(argv[0]);

    if(validate) {
        printf ("Initializing SRC array\n");
        TYPE *src = malloc (sizeof(*src) * N);
        for (i = 0; i < N; i++)
            src[i] = RAND();
        printf ("Done!\n");
        
        PRINT(src, N, "SRC");
        
        for (int i = 0;  i < nValidateFunction;  i++) {
            validateFunction[i] (src, N, sizeof(*src));
            printf ("\n");
        }
        free (src);
    } else {
        printf ("Initializing SRC array\n");
        TYPE *src = malloc (sizeof(*src) * N);
        for (i = 0; i < N; i++)
            src[i] = RAND();
        printf ("Done!\n");
        
        PRINT(src, N, "SRC");
        if (debug)
            printf ("\n\n");

        for (int i = 0;  i < nTestFunction;  i++) {
            double start = wctime();
            testFunction[i] (src, N, sizeof(*src));
            double end = wctime();
            printf ("%s:\t%6.3lf seconds\n", testNames[i], end-start);
            if (debug)
                printf ("\n\n");
        }

        free (src);
    }
    
    return 0;
}
