#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "patterns.h"


/*----------------------------------- POSITION MACRO --------------------------------*/

#define POS(INDEX, SIZE) calculatePosition(INDEX, SIZE)

int calculatePosition (int i, size_t size) {
    return i * size;
}

/*----------------------------------- CACHE PADDING --------------------------------*/

//Because in 64bit arch we need 16 ints worth of padding
#define PADDING 17


//=======================================================
// PATTERNS
//=======================================================

/*------------------------------------ MAP PATTERN ---------------------------------*/


void map (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2)) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);
    char * d = (char *) dest;
    char * s = (char *) src;

    #pragma omp parallel for
    for (int i = 0; i < nJob; i++) {
        worker (&d[POS(i, sizeJob)], &s[POS(i, sizeJob)]);
    }
}


/*------------------------------------ REDUCE PATTERN ---------------------------------*/


void reduce (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);
    char * d = (char *) dest;
    char * s = (char *) src;
    int threadNum;
    char * privDest;

    if (nJob > 0) {        
       #pragma omp parallel private(privDest)
       {
            privDest = malloc(sizeJob);
            int first = 1;

            #pragma omp for
            for (int i = 0; i < nJob; i++) {
                if(first){
                    memcpy(&privDest[0], &s[POS(i, sizeJob)], sizeJob);
                    first = 0;
                }
                else
                    worker (&privDest[0], &privDest[0], &s[POS(i, sizeJob)]);
            }

            #pragma omp single 
            {
                memcpy(&d[0], &privDest[0], sizeJob);
                threadNum = omp_get_thread_num();
            }

            #pragma omp critical 
            {
                if(omp_get_thread_num() != threadNum) {
                    worker(&d[0], &d[0], &privDest[0]);
                }
            }
            free(privDest);
        }
    }
}


/*------------------------------------ SCAN PATTERN ---------------------------------*/


typedef struct   
{ 
  char * sum; 
  char * fromLeft;
} TreeNode;


size_t nextPower_2 (int x) {
   size_t res = 1;

    while(res < x){
        res = res * 2;
    }

   return res; 
}


void createTreeNode (TreeNode * tree, int current, int min, int max, void *src, size_t nJob, size_t nPow, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    char * s = (char *) src;

    if(current >= nPow-1) {
        if((current-(nPow-1)) < nJob){
            tree[current].sum = &s[POS((current-(nPow-1)), sizeJob)];
        }
        else{
            tree[current].sum = &s[0];
        }   
        tree[current].fromLeft = calloc(1, sizeJob); 
           
    } else {
        int split = (max-min)/2 + min;
        int left = 2*current+1;
        int right = 2*current+2;

        //Create left child
        if(split-min <= 20000) {
            createTreeNode(tree, left, min, split, src, nJob, nPow, sizeJob, worker);
        } else {
            #pragma omp task untied
            {
                createTreeNode(tree, left, min, split, src, nJob, nPow, sizeJob, worker);
            }
        }
        
        //Create right child
        if(max-split <= 20000) {
            createTreeNode(tree, right, split, max, src, nJob, nPow, sizeJob, worker);
        } else {
            #pragma omp task untied
            {
                createTreeNode(tree, right, split, max, src, nJob, nPow, sizeJob, worker);
            }
        }
        #pragma omp taskwait

        tree[current].sum = malloc(sizeJob);
        worker(tree[current].sum, tree[left].sum, tree[right].sum);
        tree[current].fromLeft = calloc(1, sizeJob);
    }
}


TreeNode * buildTree (void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    size_t nextPow2 = nextPower_2(nJob);
    TreeNode * tree = (TreeNode *) malloc(sizeof(TreeNode) * (nextPow2*2-1));
    createTreeNode(tree, 0, 0, nextPow2, src, nJob, nextPow2, sizeJob, worker);
    return tree;
}


void updateTreeNode (TreeNode * tree, int current, size_t nJob, int splitSize, size_t sizeJob, void* src, void * dest,void (*worker)(void *v1, const void *v2, const void *v3)) {
    int left = 2*current+1;
    int right = 2*current+2;

    memcpy(tree[left].fromLeft, tree[current].fromLeft, sizeJob);
    worker(tree[right].fromLeft, tree[left].sum, tree[current].fromLeft);

    if(left < nJob-1 || right < nJob-1){
        int split = splitSize/2;

        if(split <= 20000) {
            #pragma omp task untied
            {
                updateTreeNode(tree, left, nJob, split, sizeJob, src, dest, worker);
            }
            #pragma omp task untied
            {
                updateTreeNode(tree, right, nJob, split, sizeJob, src, dest, worker);
            }
        } else {
                updateTreeNode(tree, left, nJob, split, sizeJob, src, dest, worker);
                updateTreeNode(tree, right, nJob, split, sizeJob, src, dest, worker);
        }
        
    }
}


void traverseTree (TreeNode * tree, size_t nJob, size_t sizeJob, void* src , void * dest, void (*worker)(void *v1, const void *v2, const void *v3)) {
    size_t nextPow2 = nextPower_2(nJob);
    updateTreeNode(tree, 0, nextPow2, nextPow2, sizeJob, src, dest, worker);
}

void freeTree (TreeNode * tree, int nNodes, int nPow, int nJob) {

    #pragma omp for
    for(int i = 0; i < nNodes; i++) {
        free(tree[i].fromLeft);
        if(!(i >= (nPow-1)))
            free(tree[i].sum);
    }
    #pragma omp single
    free(tree);
}


void scan (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);

    char * s = (char *) src;
    char * d = (char *) dest;

    size_t nextPow2 = nextPower_2(nJob);
    int nNodes = nextPow2 * 2 - 1 - nextPow2;
    TreeNode * tree;

    #pragma omp parallel 
    {   
        #pragma omp single    
        {
            tree = buildTree (src, nJob, sizeJob, worker);
            traverseTree (tree, nJob, sizeJob, src, dest, worker);
        }

        #pragma omp for
        for (int i = 0 ; i < nJob; i++){
            int j = nNodes + i;
            worker(&d[POS(i, sizeJob)], &s[POS(i, sizeJob)], tree[j].fromLeft);
        } 

        freeTree(tree, nextPow2*2-1, nextPow2, nJob);
    }
}


/*------------------------------------ PACK PATTERN ---------------------------------*/


void workerPreffixSum(void* a, const void* b, const void* c) {
    *(int *)a = *(int *)b + *(int *)c;
}


int pack (void *dest, void *src, size_t nJob, size_t sizeJob, const int *filter) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (filter != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    char * d = (char *) dest;
    char * s = (char *) src;
    void * f = (void *) filter;

    void * parallel_preffix = calloc(1, nJob * sizeof(int));
    scan(parallel_preffix, f, nJob, sizeof(int), workerPreffixSum);
    int * bitsum = (int *) parallel_preffix;
    int res = bitsum[(nJob-1)];
    
    #pragma omp parallel for
    for (int i = 0; i < nJob; i++) {
        if (filter[i]) {
            memcpy (&d[POS(bitsum[i]-1, sizeJob)], &s[POS(i, sizeJob)], sizeJob);
        }
    }
    free(parallel_preffix);
    return res;
}


/*------------------------------------ GATHER PATTERN ---------------------------------*/


void gather (void *dest, void *src, size_t nJob, size_t sizeJob, const int *filter, int nFilter) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (filter != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    assert (nFilter >= 0);
    char * d = (char *) dest;
    char * s = (char *) src;

    #pragma omp parallel for
    for (int i = 0; i < nFilter; i++) {
        assert (filter[i] < nJob);
        memcpy (&d[POS(i, sizeJob)], &s[POS(filter[i], sizeJob)], sizeJob);
    }
}


/*------------------------------------ SCATTER PATTERN ---------------------------------*/


/**
 * WARNING - This implementation of scatter is non deterministic when the filter array contains repeatead elements
 **/
void scatter (void *dest, void *src, size_t nJob, size_t sizeJob, const int *filter) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (filter != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    char * d = (char *) dest;
    char * s = (char *) src;

    #pragma omp parallel for
    for (int i = 0; i < nJob; i++) {
        assert (filter[i] < nJob);
        memcpy (&d[POS(filter[i], sizeJob)], &s[POS(i, sizeJob)], sizeJob);
    }
}


/*------------------------------------ PIPELINE PATTERN ---------------------------------*/


void pipeline (void *dest, void *src, size_t nJob, size_t sizeJob, void (*workerList[])(void *v1, const void *v2), size_t nWorkers) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (workerList != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    char * d =(char *) dest;
    char * s =(char *) src;

    // Number of antidiagonals
    int nAntiDiagonal = nWorkers + nJob - 1;

    // Iterate over the antidiagonals
    for (int a = 1; a <= nAntiDiagonal; a++) {
        int nPoints = 0;
        // If the antidiagonal is in the ramp up section
        if(a <= nWorkers && a <= nJob) {
            nPoints = a;
        } 
        else if(nWorkers > nJob){
            // If the antidiagonal is in the max efficiency section
            if (a <= nAntiDiagonal - (nJob-1)){
                nPoints = nJob;
            }
            // If the antidiagonal is in the slow down section
            else {
                nPoints = nAntiDiagonal - a + 1;
            }
        }
        // Else, if nWorkers <= nJob and its not in the ramp up section
        else {
           // If the antidiagonal is in the slow down section 
         if (a > nJob) {
            nPoints = nWorkers - (a - nJob);
         } 
        // If the antidiagonal is in the max efficiency section
         else {
            nPoints = nWorkers;
            }        
        }
        // Initial ramp up and part of the max efficiency sections
        if(a <= nJob) {
            memcpy (&d[POS(a-1, sizeJob)], &s[POS(a-1, sizeJob)], sizeJob); 
            #pragma omp parallel for
            for (int j = 1; j <= nPoints; j++) {
                workerList[(j-1)] (&d[POS(a-j, sizeJob)], &d[POS(a-j, sizeJob)]);
            }
        } 
        // Part of the max efficiency section and slow down section
        else {
            #pragma omp parallel for
            for (int j = 1; j <= nPoints;  j++) {
                workerList[(j-1 +(a-nJob))] (&d[POS(nJob-j, sizeJob)], &d[POS(nJob-j, sizeJob)]);
            }
        }
    }
}


/*------------------------------------ FARM PATTERN ---------------------------------*/


void farm (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2), size_t nWorkers) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);
    assert (nWorkers >= 0);
    char * d = (char *) dest;
    char * s = (char *) src;

    int * flagWorkers = calloc(1, (nWorkers + PADDING) * sizeof(int));
    int finished = 0;

    #pragma omp parallel
    {
        assert(omp_get_num_threads() >= 2);

        #pragma omp master
        { 
            while(finished < nJob) {
                for(int j = 0; j < nWorkers && finished < nJob; j++){
                    if (!flagWorkers[j+PADDING]){
                        flagWorkers[j+PADDING] = 1; 
                        #pragma omp task untied
                        {
                            worker (&d[POS(finished, sizeJob)], &s[POS(finished, sizeJob)]);
                            flagWorkers[j+PADDING] = 0; 
                        }
                        finished++;
                    }
                }
            }
            #pragma omp taskwait
        }
    }
    free(flagWorkers);
}