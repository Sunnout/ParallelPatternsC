#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "patterns.h"


/*------------------------------------ MAP PATTERN ---------------------------------*/


void map (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2)) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);
    char * d = (char *) dest;
    char * s = (char *) src;

    #pragma omp parallel for
    for (int i = 0; i < nJob; i++) {
        worker (&d[i * sizeJob], &s[i * sizeJob]);
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
                    memcpy(&privDest[0], &s[i * sizeJob], sizeJob);
                    first = 0;
                }
                else
                    worker (&privDest[0], &privDest[0], &s[i * sizeJob]);
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
  int min;
  int max;
  char * sum; 
  char * fromLeft;
} TreeNode;


int nextPower_2 (int x) {
   x = x - 1; 
   x = x | (x >> 1); 
   x = x | (x >> 2); 
   x = x | (x >> 4); 
   x = x | (x >> 8); 
   return x + 1; 
}


TreeNode getLeftChild(TreeNode * tree, int parent) {
    return tree[(2*parent+1)];
}


TreeNode getRightChild(TreeNode * tree, int parent) {
    return tree[(2*parent+2)];
}


void createTreeNode (TreeNode * tree, int current, int min, int max, void *src, size_t nJob, size_t nPow, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    char * s = (char *) src;

    if(current >= nPow-1) {
        tree[current].min = current-(nPow-1);
        tree[current].max = current-(nPow-2);
        if(tree[current].min < nJob)
            tree[current].sum = &s[tree[current].min*sizeJob];
        tree[current].fromLeft = calloc(1,sizeJob);      
    } else {
        int split = (max-min)/2 + min;

        //Create left child
        if(split-min <= 10000) {
            createTreeNode(tree, 2*current+1, min, split, src, nJob, nPow, sizeJob, worker);
        } else {
            #pragma omp task
            {
                createTreeNode(tree, 2*current+1, min, split, src, nJob, nPow, sizeJob, worker);
            }
        }
        
        //Create right child
        if(max-split <= 10000) {
            createTreeNode(tree, 2*current+2, split, max, src, nJob, nPow, sizeJob, worker);
        } else {
            #pragma omp task
            {
                createTreeNode(tree, 2*current+2, split, max, src, nJob, nPow, sizeJob, worker);
            }
        }
        #pragma omp taskwait

        TreeNode nodeLeft  = getLeftChild(tree, current);
        TreeNode nodeRight = getRightChild(tree, current);
        tree[current].min = min;
        tree[current].max = max;
        tree[current].sum = malloc(sizeJob);
        worker(tree[current].sum, nodeLeft.sum, nodeRight.sum);
        tree[current].fromLeft = calloc(1, sizeJob);
    }
}


TreeNode * buildTree (void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    size_t nextPow2 = nextPower_2(nJob);
    TreeNode * tree = (TreeNode *) malloc(sizeof(TreeNode)*(nextPow2*2-1));

    #pragma omp parallel 
    {
    #pragma omp single
    createTreeNode(tree, 0, 0, nextPow2, src, nJob, nextPow2, sizeJob, worker);
    }
    return tree;
}


void updateTreeNode (TreeNode * tree, int current, size_t nJob, int splitSize, size_t sizeJob, void* src, void * dest,void (*worker)(void *v1, const void *v2, const void *v3)) {
    TreeNode currentNode = tree[current];
    TreeNode leftChild = getLeftChild(tree, current);
    TreeNode rightChild = getRightChild(tree, current);

    memcpy(leftChild.fromLeft, currentNode.fromLeft, sizeJob);
    worker(rightChild.fromLeft, leftChild.sum, currentNode.fromLeft);

    if(current*2+1 < nJob-1 || current*2+2 < nJob-1){
        int split = splitSize/2;

        if(split <= 10000) {
            #pragma omp task
            {
                updateTreeNode(tree, current*2+1, nJob, split, sizeJob, src, dest, worker);
            }
            #pragma omp task
            {
                updateTreeNode(tree, current*2+2, nJob, split, sizeJob, src, dest, worker);
            }
        } else {
                updateTreeNode(tree, current*2+1, nJob, split, sizeJob, src, dest, worker);
                updateTreeNode(tree, current*2+2, nJob, split, sizeJob, src, dest, worker);
        }
        
    }
}


void traverseTree (TreeNode * tree, size_t nJob, size_t sizeJob, void* src , void * dest, void (*worker)(void *v1, const void *v2, const void *v3)) {
    size_t nextPow2 = nextPower_2(nJob);
    #pragma omp parallel 
    {
    #pragma omp single
    updateTreeNode(tree, 0, nextPow2, nextPow2, sizeJob, src, dest, worker);
    }
}

void freeTree (TreeNode * tree, int nNodes) {

    #pragma omp parallel for
    for(int i = 0; i < nNodes; i++) {
        TreeNode currentNode = tree[i];
        free(currentNode.fromLeft);
        free(currentNode.sum);
    }
    free(tree);
}


void scan (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);

    char * s = (char *) src;
    char * d = (char *) dest;

    TreeNode * tree = buildTree (src, nJob, sizeJob, worker);
    traverseTree (tree, nJob, sizeJob, src, dest, worker);

    size_t nextPow2 = nextPower_2(nJob);
    int nNodes = nextPow2 * 2 -1 - nextPow2;

    #pragma omp parallel for
    for (int i = 0 ; i < nJob; i++){
        int j = nNodes + i;
        worker(&d[i*sizeJob], &s[i*sizeJob], tree[j].fromLeft);
    } 

    //freeTree(tree, nextPow2*2-1);
}


/*------------------------------------ PACK PATTERN ---------------------------------*/


void workerPreffixSum(void* a, const void* b, const void* c) {
    // a = b + c
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
    for (int i=0; i < nJob; i++) {
        if (filter[i]) {
            memcpy (&d[(bitsum[i]-1) * sizeJob], &s[i * sizeJob], sizeJob);
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
    for (int i=0; i < nFilter; i++) {
        assert (filter[i] < nJob);
        memcpy (&d[i * sizeJob], &s[filter[i] * sizeJob], sizeJob);
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
    for (int i=0; i < nJob; i++) {
        assert (filter[i] < nJob);
        memcpy (&d[filter[i] * sizeJob], &s[i * sizeJob], sizeJob);
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
            memcpy (&d[(a-1) * sizeJob], &s[(a-1) * sizeJob], sizeJob); 
            #pragma omp parallel for
            for (int  j = 1;  j <= nPoints; j++) {
                workerList[(j-1)] (&d[(a-j) * sizeJob], &d[(a-j) * sizeJob]);
            }
        } 
        // Part of the max efficiency section and slow down section
        else {
            #pragma omp parallel for
            for (int j = 1; j <= nPoints;  j++) {
                workerList[(j-1 +(a-nJob))] (&d[(nJob-j) * sizeJob], &d[(nJob-j) * sizeJob]);
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

    int * flagWorkers = calloc(1, nWorkers*sizeof(int));
    int finished = 0;

    #pragma omp parallel shared(flagWorkers)
    {
        #pragma omp master
        { 
            while(finished < nJob) {
                for(int j = 0; j < nWorkers && finished < nJob; j++){
                    if (!flagWorkers[j]){
                        flagWorkers[j] = 1; 
                        #pragma omp task
                        {
                            worker (&d[finished * sizeJob], &s[finished * sizeJob]);
                            flagWorkers[j] = 0; 
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