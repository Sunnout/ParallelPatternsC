#include <string.h>
#include <assert.h>
#include <omp.h>
#include "btree.h"
#include <stdlib.h>
#include <stdio.h>


struct treeNode  
{ 
  int min;
  int max;
  char * sum; 
  char * fromLeft;
};


struct treeNode * tree;


void buildTreeBottomUp (void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    tree = malloc(sizeof(struct treeNode)*((nJob*2)-1));

    char * s = src;

    int pos = nJob - 1; 
    for(int i = 0; i < nJob; i++) {
        struct treeNode * node = (struct treeNode *) malloc(sizeof(struct treeNode)); 
        node->min = i;
        node->max = i+1;
        node->sum = malloc(sizeJob);
        memcpy (&node->sum, &s[i*sizeJob], sizeJob);
        node->fromLeft = malloc(sizeJob);
        memcpy (&node->fromLeft, NULL, sizeJob);

        memcpy (&tree[pos*sizeof(struct treeNode)], &node, sizeof(struct treeNode));
        pos++;
    }

    pos = (nJob - 1)/2; 
    for(int i = pos; i <= 0; pos = pos/2) {
        for(int j = pos; j < pos + (pos + 1); j++) {
            struct treeNode * node = (struct treeNode *) malloc(sizeof(struct treeNode)); 
            struct treeNode * nodeLeft  = getLeftChild(j);
            struct treeNode * nodeRight = getRightChild(j);

            node->min = nodeLeft->min;
            node->max = nodeRight->max;
            worker(node->sum, nodeLeft->sum, nodeRight->sum);
            node->fromLeft = malloc(sizeJob);
            memcpy (&node->fromLeft, NULL, sizeJob);

            memcpy (&tree[j*sizeof(struct treeNode)], &node, sizeof(struct treeNode));
        }
    }  
}

void traverseTreeTopDown () {

}

struct treeNode * getLeftChild(int parent) {
    return (struct treeNode *) tree[(2*parent+1)*sizeof(struct treeNode)];
}

struct treeNode * getRightChild(int parent) {
    return (struct treeNode *) tree[(2*parent+2)*sizeof(struct treeNode)];
}
