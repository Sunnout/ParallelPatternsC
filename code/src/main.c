#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <sys/time.h>

#include "unit.h"
#include "debug.h"


//Define the data type: 1 -> double; 2 -> long; 3 -> int; 4 -> String (doesn't fully work);
#define TYPE_SET 1


#if TYPE_SET == 1
//=======================================================
// TYPE DOUBLE
//=======================================================

#define TYPE double
#define RAND() drand48()
#define PRINT(SRC, N, STRING) printDouble(SRC, N, STRING)
#endif

#if TYPE_SET == 2
//=======================================================
// TYPE LONG
//=======================================================

#define TYPE long
#define RAND() lrand48()
#define PRINT(SRC, N, STRING) printLong(SRC, N, STRING)
#endif

#if TYPE_SET == 3
//=======================================================
// TYPE LONG
//=======================================================

#define TYPE int
#define RAND() rand()
#define PRINT(SRC, N, STRING) printInt(SRC, N, STRING)
#endif


#if TYPE_SET == 4
//=======================================================
// TYPE STRING
//=======================================================

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
#endif



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
    int seq = 0;
    int t = 1;
    

    int c;
    while ((c = getopt (argc, argv, "dvst:")) != -1)
    switch (c) {
        case 'd':
            debug = 1; break;
        case 'v':
            validate = 1; break;
        case 's':
            seq = 1; break;
        case 't':
            t = atoi(optarg); break;
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

    printf ("Initializing SRC array\n");
        TYPE *src = malloc (sizeof(*src) * N);
        for (i = 0; i < N; i++)
            src[i] = RAND();
        printf ("Done!\n");
        
        PRINT(src, N, "SRC");

    if(validate) {
        printf ("\n\n");
        printf("Doing %d Validations\n", nValidateFunction);

        for (int i = 0;  i < nValidateFunction;  i++) {
            printf ("%s: ", validateNames[i]);
            validateFunction[i] (src, N, sizeof(*src));
            printf ("\n");
        }
    } 

    if (debug)
        printf ("\n\n");

    
   


    if(seq)
        printf("Doing  Tests Sequential\n");
    else
        printf("Doing Tests Parallel\n");

    double TEST_TIME_START = wctime();
        double start,end ;
         switch(t){
            case 1:
                for (int i = 0;  i < nTestFunction1;  i++) {
                    start = wctime();
                    testFunction1[i] (src, N, sizeof(*src),seq);
                    end = wctime();
                    printf ("%s:\t%8.3lf seconds\n", testNames1[i], end-start);
                    if (debug)
                        printf ("\n\n");
                }
            break;
            case 2:
                for (int i = 0;  i < nTestFunction2;  i++) {
                    start = wctime();
                    testFunction2[i] (src, N, sizeof(*src),seq);
                    end = wctime();
                    printf ("%s:\t%8.3lf seconds\n", testNames2[i], end-start);
                    if (debug)
                        printf ("\n\n");
                }
            break;
            case 3:
               for (int i = 0;  i < nTestFunction3;  i++) {
                    start = wctime();
                    testFunction3[i] (src, N, sizeof(*src),seq);
                    end = wctime();
                    printf ("%s:\t%8.3lf seconds\n", testNames3[i], end-start);
                    if (debug)
                        printf ("\n\n");
                }
            break;
            case 4:
                for (int i = 0;  i < nTestFunction4;  i++) {
                    start = wctime();
                    testFunction4[i] (src, N, sizeof(*src),seq);
                    end = wctime();
                    printf ("%s:\t%8.3lf seconds\n", testNames4[i], end-start);
                    if (debug)
                        printf ("\n\n");
                }
            break;
            case 5:
                for (int i = 0;  i < nTestFunction5;  i++) {
                    start = wctime();
                    testFunction5[i] (src, N, sizeof(*src),seq);
                    end = wctime();
                    printf ("%s:\t%8.3lf seconds\n", testNames5[i], end-start);
                    if (debug)
                        printf ("\n\n");
                }
            break;
            case 6:
                for (int i = 0;  i < nTestFunction6;  i++) {
                    start = wctime();
                    testFunction6[i] (src, N, sizeof(*src),seq);
                    end = wctime();
                    printf ("%s:\t%8.3lf seconds\n", testNames6[i], end-start);
                    if (debug)
                        printf ("\n\n");
                }
            break;
            default:
                printf("Invalid Test option\n");
                abort ();
         }
      
    
    double TEST_TIME_END = wctime();
    printf ("%s:\t%8.3lf seconds\n", "Test Time", TEST_TIME_END-TEST_TIME_START);

    
    

    free (src);
    
    return 0;
}
