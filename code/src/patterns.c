#include <string.h>
#include <assert.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include "patterns.h"
#include <stdlib.h>
#include <stdio.h>

void map (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2)) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);
    char *d = (char *) dest;
    char *s = (char *) src;

    #pragma omp parallel for
    for (int i = 0;  i < nJob;  i++) {
        worker (&d[i * sizeJob], &s[i * sizeJob]);
    }
}

void reduce (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);
    char *d = (char *) dest;
    char *s = (char *) src;
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
  void * sum; 
  void * fromLeft;
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

void createTreeNode (TreeNode * tree, int current, int min, int max, void *src, size_t nJob,size_t nPow, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    char * s = (char *)src;

    if(current >= nPow-1) {
        TreeNode node;
        node.min = current-(nPow-1);
        node.max = current-(nPow-2);
        node.sum = malloc(sizeJob);
        if(node.min < nJob)
            memcpy(node.sum, &s[node.min*sizeJob], sizeJob);
        node.fromLeft = calloc(1,sizeJob);      
        memcpy (&tree[current], &node, sizeof(TreeNode));
    } else {
        int split = (max-min)/2 + min;
        //Create left child
        #pragma omp task
        {
            createTreeNode(tree, 2*current+1, min, split, src, nJob,nPow, sizeJob, worker);
        }
        //Create right child
        #pragma omp task
        {
            createTreeNode(tree, 2*current+2, split, max, src, nJob,nPow, sizeJob, worker);
        }
        #pragma omp taskwait

        TreeNode node;
        TreeNode nodeLeft  = getLeftChild(tree, current);
        TreeNode nodeRight = getRightChild(tree, current);
        node.min = min;
        node.max = max;
        node.sum = malloc(sizeJob);
        worker(node.sum, nodeLeft.sum, nodeRight.sum);
        node.fromLeft = calloc(1, sizeJob);
        memcpy (&tree[current], &node, sizeof(TreeNode));
    }
}

TreeNode * buildTree (void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    size_t nextPow2 = nextPower_2(nJob);
    TreeNode * tree = (TreeNode *) malloc(sizeof(TreeNode)*(nextPow2*2-1));

    #pragma omp parallel 
    {
    #pragma omp single
    createTreeNode(tree, 0, 0, nextPow2, src,nJob, nextPow2, sizeJob, worker);
    }
    return tree;
}

void updateTreeNode (TreeNode * tree, int current,size_t nJob, size_t sizeJob, void* src ,void * dest,void (*worker)(void *v1, const void *v2, const void *v3)) {
    TreeNode currentNode = tree[current];
    TreeNode leftChild = getLeftChild(tree, current);
    TreeNode rightChild = getRightChild(tree, current);

    memcpy(leftChild.fromLeft, currentNode.fromLeft, sizeJob);
    worker(rightChild.fromLeft, leftChild.sum, currentNode.fromLeft);

    if( current*2+1 < nJob-1 || current*2+2 < nJob-1 ){
        #pragma omp task
        {
            updateTreeNode(tree, current*2+1, nJob, sizeJob, src, dest, worker);
        }
        #pragma omp task
        {
            updateTreeNode(tree, current*2+2, nJob, sizeJob, src, dest, worker);
        }
    }
}

void traverseTree (TreeNode * tree, size_t nJob, size_t sizeJob, void* src ,void * dest,void (*worker)(void *v1, const void *v2, const void *v3)) {
    size_t nextPow2 = nextPower_2(nJob);
 #pragma omp parallel 
    {
    #pragma omp single
    updateTreeNode(tree, 0, nextPow2, sizeJob, src, dest, worker);
    }
}


void scan (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (worker != NULL);

    char *s = (char *) src;
    char *d = (char *) dest;

    TreeNode * tree = buildTree (src, nJob, sizeJob, worker);
    traverseTree (tree, nJob, sizeJob, src, dest, worker);

    size_t nextPow2 = nextPower_2(nJob);
    int nNodes = nextPow2 * 2 -1 - nextPow2;

    #pragma omp parallel for
    for (int i = 0 ; i < nJob; i++){
        int j = nNodes + i;
        worker(&d[i*sizeJob], &s[i*sizeJob], tree[j].fromLeft);
    } 

    free(tree);
}

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
    char * d = (char *)dest;
    char * s = (char *)src;
    void * f = (void *)filter;

    void * parallel_preffix = calloc(1, nJob * sizeof(int));
    scan(parallel_preffix, f, nJob, sizeof(int), workerPreffixSum);
    char * bitsum = (char *) parallel_preffix;
    int res = bitsum[(nJob-1)*sizeof(int)];

    #pragma omp parallel for
    for (int i=0; i < nJob; i++) {
        if (filter[i]) {
            memcpy (&d[(bitsum[i*sizeof(int)]-1) * sizeJob], &s[i * sizeJob], sizeJob);
        }
    }
    free(parallel_preffix);
    return res;
}

void gather (void *dest, void *src, size_t nJob, size_t sizeJob, const int *filter, int nFilter) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (filter != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    assert (nFilter >= 0);
    char *d = (char *) dest;
    char *s = (char *) src;

    #pragma omp parallel for
    for (int i=0; i < nFilter; i++) {
        assert (filter[i] < nJob);
        memcpy (&d[i * sizeJob], &s[filter[i] * sizeJob], sizeJob);
    }
}

/**
 * WARNING - This implementation of scatter is non deterministic when the filter array contains repeatead elements
 **/
void scatter (void *dest, void *src, size_t nJob, size_t sizeJob, const int *filter) {
    assert (dest != NULL);
    assert (src != NULL);
    assert (filter != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    char *d = (char *) dest;
    char *s = (char *) src;

    #pragma omp parallel for
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
    char *d =(char *) dest;
    char *s =(char *) src;

    // Number of Antidiagonals
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
            // if the antidiagonal is in the slow down section
            else {
                nPoints = nAntiDiagonal - a + 1;
            }
        }
        // Else , if Nworkers <= nJob and its not in the ramp up section
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
        //Initial ramp up and part of max efficiency sections
        if(a <= nJob) {
            memcpy (&d[(a-1) * sizeJob], &s[(a-1) * sizeJob], sizeJob); 
            #pragma omp parallel for
            for (int  j = 1;  j <= nPoints; j++) {
                workerList[(j-1)] (&d[(a-j) * sizeJob], &d[(a-j) * sizeJob]);
            }
        } 
        // Part of max efficiency secion and slow down section
        else {
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

