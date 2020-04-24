#include <string.h>
#include <assert.h>
#include <omp.h>
#include "btree.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

TreeNode * tree;

void buildTreeBottomUp (void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    size_t nextPow2 = nextPower_2(nJob);
    tree = (TreeNode *) malloc(sizeof(TreeNode)*(nextPow2*2-1));
    char * s = (char *) src;

    int pos = nextPow2 - 1; 
    for(int i = 0; i < nJob; i++) {
        TreeNode node;
        node.min = i;
        node.max = i+1;
        node.sum = malloc(sizeJob);
        memcpy(node.sum, &s[i*sizeJob], sizeJob);
        node.fromLeft = malloc(sizeJob);        
        
        memcpy (&tree[pos], &node, sizeof(TreeNode));
        pos++;
    }

    for(int i = nJob; i < nextPow2; i++) {
        TreeNode node;
        node.min = i;
        node.max = i+1;
        node.sum = malloc(sizeJob);
        node.fromLeft = malloc(sizeJob);        
        
        memcpy (&tree[pos], &node, sizeof(TreeNode));
        pos++;
    }

    pos = (nextPow2 - 1)/2; 
    int createdRoot = 0;

    for(int i = pos; i >= 0 && !createdRoot; i = i/2) {
        for(int j = i; j < i + (i + 1); j++) {   

            TreeNode nodeLeft  = getLeftChild(j);
            TreeNode nodeRight = getRightChild(j);
            TreeNode node;
            node.min = nodeLeft.min;
            node.max = nodeRight.max;
            node.sum = malloc(sizeJob);
            worker(node.sum, nodeLeft.sum, nodeRight.sum);
            node.fromLeft = malloc(sizeJob);

            memcpy (&tree[j], &node, sizeof(TreeNode));

            if(i == 0) {
                createdRoot = 1;
            }
        }
    } 
}

void buildTree (void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    tree = (TreeNode *) malloc(sizeof(TreeNode)*(nJob*2-1));
    #pragma omp parallel 
    {
    #pragma omp single
    createTreeNode(0, 0, nJob, src, nJob, sizeJob, worker);
    }
}

void createTreeNode (int current, int min, int max, void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    char * s = (char *)src;

    if(current >= nJob-1) {
        TreeNode node;
        node.min = current-(nJob-1);
        node.max = current-(nJob-2);
        node.sum = malloc(sizeJob);
        memcpy(node.sum, &s[node.min*sizeJob], sizeJob);
        node.fromLeft = calloc(1,sizeJob);      
        memcpy (&tree[current], &node, sizeof(TreeNode));
    } else {
        int split = (max-min)/2 + min;
        //Create left child
        #pragma omp task
        {
            createTreeNode(2*current+1, min, split, src, nJob, sizeJob, worker);
        }
        //Create right child
        #pragma omp task
        {
            createTreeNode(2*current+2, split, max, src, nJob, sizeJob, worker);
        }
        #pragma omp taskwait

        TreeNode node;
        TreeNode nodeLeft  = getLeftChild(current);
        TreeNode nodeRight = getRightChild(current);
        node.min = min;
        node.max = max;
        node.sum = malloc(sizeJob);
        worker(node.sum, nodeLeft.sum, nodeRight.sum);
        node.fromLeft = calloc(1,sizeJob);
        memcpy (&tree[current], &node, sizeof(TreeNode));
    }
}

// Admits there is a tree initialized
void traverseTreeTopDown (size_t nJob, size_t sizeJob, void* src ,void * dest,void (*worker)(void *v1, const void *v2, const void *v3)) {
    // All nodes excepts the leafs
    size_t nextPow2 = nextPower_2(nJob);
    int nNodes = nextPow2 * 2 -1 - nextPow2;
    char *s = (char *) src;
    char *d = (char *) dest;

    for (int i = 0 ; i < nNodes; i++){
        TreeNode current = tree[i];
        TreeNode leftChild = getLeftChild(i);
        TreeNode rightChild = getRightChild(i);

        memcpy(leftChild.fromLeft, current.fromLeft , sizeJob);
        worker(rightChild.fromLeft, leftChild.sum, current.fromLeft);
    }

    for (int i = 0 ; i < nJob ; i++ ){
        int j = nNodes + i;
        worker(&d[i*sizeJob], &s[i*sizeJob], tree[j].fromLeft);
    }

}

void traverseTree (size_t nJob, size_t sizeJob, void* src ,void * dest,void (*worker)(void *v1, const void *v2, const void *v3)) {


    updateTreeNode(nJob, sizeJob, src, dest, worker);

    
}

void updateTreeNode (size_t nJob, size_t sizeJob, void* src ,void * dest,void (*worker)(void *v1, const void *v2, const void *v3)) {
    
}


TreeNode getLeftChild(int parent) {
    return tree[(2*parent+1)];
}

TreeNode getRightChild(int parent) {
    return tree[(2*parent+2)];
}

TreeNode * getTree(){
    return tree;
}

int nextPower_2 (int x) {
   x = x - 1; 
   x = x | (x >> 1); 
   x = x | (x >> 2); 
   x = x | (x >> 4); 
   x = x | (x >> 8); 
   return x + 1; 
}

