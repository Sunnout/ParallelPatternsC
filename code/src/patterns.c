#include <string.h>
#include <assert.h>
#include <omp.h>
#include "patterns.h"
#include <stdio.h>

void map (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2)) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);
    char *d = dest;
    char *s = src;

    #pragma omp parallel for
    for (int i = 0;  i < nJob;  i++) {
        worker (&d[i * sizeJob], &s[i * sizeJob]);
    }
}

void reduce (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    /* To be implemented */
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);
    char *d = dest;
    char *s = src;
    if (nJob > 0) {
        memcpy (&d[0], &s[0], sizeJob);
        for (int i = 1;  i < nJob;  i++)
            worker (&d[0], &d[0], &s[i * sizeJob]);
    }
}

void scan (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    /* To be implemented */
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);
    char *d = dest;
    char *s = src;
    if (nJob > 1) {
        memcpy (&d[0], &s[0], sizeJob);
        for (int i = 1;  i < nJob;  i++)
            worker (&d[i * sizeJob], &d[(i-1) * sizeJob], &s[i * sizeJob]);
    }
}

int pack (void *dest, void *src, size_t nJob, size_t sizeJob, const int *filter) {
    /* To be implemented */
    assert (dest != NULL);
    assert (src != NULL);
    assert (filter != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    char *d = dest;
    char *s = src;
    int pos = 0;
    for (int i=0; i < nJob; i++) {
        if (filter[i]) {
            memcpy (&d[pos * sizeJob], &s[i * sizeJob], sizeJob);
            pos++;
        }
    }
    return pos;
}

void gather (void *dest, void *src, size_t nJob, size_t sizeJob, const int *filter, int nFilter) {
    /* To be implemented */
    assert (dest != NULL);
    assert (src != NULL);
    assert (filter != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    assert (nFilter >= 0);
    char *d = dest;
    char *s = src;
    for (int i=0; i < nFilter; i++) {
        assert (filter[i] < nJob);
        memcpy (&d[i * sizeJob], &s[filter[i] * sizeJob], sizeJob);
    }
}

void scatter (void *dest, void *src, size_t nJob, size_t sizeJob, const int *filter) {
    /* To be implemented */
    assert (dest != NULL);
    assert (src != NULL);
    assert (filter != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    char *d = dest;
    char *s = src;
    for (int i=0; i < nJob; i++) {
        assert (filter[i] < nJob);
        memcpy (&d[filter[i] * sizeJob], &s[i * sizeJob], sizeJob);
    }
}

void pipeline (void *dest, void *src, size_t nJob, size_t sizeJob, void (*workerList[])(void *v1, const void *v2), size_t nWorkers) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (workerList != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    char *d = dest;
    char *s = src;

    int nAntiDiagonal = nWorkers + nJob - 1;

    for (int a = 1; a <= nAntiDiagonal; a++) {
        int nPoints = 0;
        if(a < nWorkers && a <= nJob) {
            nPoints = a;
        } 
        else if(nWorkers > nJob){
            if (a - nAntiDiagonal < nJob){
                nPoints = nJob;
            }
            else {
                nPoints = a - nAntiDiagonal +1;
            }
        }
        else {
         if (a > nJob) {
            nPoints = nWorkers - (a - nJob);
         } 
         else {
            nPoints = nWorkers;
            }        
        }
        

        if(a <= nJob) {
            memcpy (&d[(a-1) * sizeJob], &s[(a-1) * sizeJob], sizeJob); 
            #pragma omp parallel for
            for (int  j = 1;  j <= nPoints; j++) {
                workerList[(j-1)] (&d[(a-j) * sizeJob], &d[(a-j) * sizeJob]);
            }
        } else {
            #pragma omp parallel for
            for (int j = 1; j <= nPoints;  j++) {
                workerList[(j-1 +(a-nJob))] (&d[(nJob-j) * sizeJob], &d[(nJob-j) * sizeJob]);
            }
        }
    }
}

void farm (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2), size_t nWorkers) {
    /* To be implemented */
    map (dest, src, nJob, sizeJob, worker);  // it provides the right result, but is a very very vey bad implementation…
}
