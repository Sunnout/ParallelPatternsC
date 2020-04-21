#include <string.h>
#include <assert.h>
#include <omp.h>
#include "btree.h"
#include <stdlib.h>
#include <stdio.h>

TreeNode * tree;

void buildTreeBottomUp (void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    tree = (TreeNode *) malloc(sizeof(TreeNode)*(nJob*2-1));
    char * s = (char *) src;

    int pos = nJob - 1; 
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

    pos = (nJob - 1)/2; 
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

void traverseTreeTopDown () {

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