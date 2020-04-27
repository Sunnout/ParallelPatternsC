#include <string.h>
#include <assert.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include "patterns.h"

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
    /* To be implemented */
    assert (dest != NULL);
    assert (src != NULL);
    assert (workerList != NULL);
    assert (nJob >= 0);
    assert (sizeJob > 0);
    char *d = dest;
    char *s = src;
    for (int i=0; i < nJob; i++) {
        memcpy (&d[i * sizeJob], &s[i * sizeJob], sizeJob);
        for (int j = 0;  j < nWorkers;  j++) {
            assert (workerList[j] != NULL);
            workerList[j] (&d[i * sizeJob], &d[i * sizeJob]);
        }
    }
}

void farm (void *dest, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2), size_t nWorkers) {
    /* To be implemented */
    map (dest, src, nJob, sizeJob, worker);  // it provides the right result, but is a very very vey bad implementation…
}

