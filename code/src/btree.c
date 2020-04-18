#include <string.h>
#include <assert.h>
#include <omp.h>
#include "btree.h"
#include <stdlib.h>
#include <stdio.h>





void buildTreeBottomUp (void *src, size_t nJob, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) {
    tree = malloc(sizeof(struct treeNode)*((nJob*2)-1));

    char * s = src;


    int pos = nJob - 1; 
    for(int i = 0; i < nJob; i++) {
        struct treeNode * node = (struct treeNode *) malloc(sizeof(struct treeNode)); 
        node->min = i;
        node->max = i+1;
        void * sum = malloc(sizeJob);
        memcpy (&sum, &s[i*sizeJob], sizeJob);
        node-> sum =sum;
        node->fromLeft = malloc(sizeJob);
        
        printf("Vou meter na tree com min %d , max %d NA posição %d\n" , node->min,node->max,pos);
        memcpy (&tree[pos*sizeof(struct treeNode)], node, sizeof(struct treeNode));
        pos++;
    }

    pos = (nJob - 1)/2; 
    printf("Im first pos = %d \n",pos);
    for(int i = pos; i >= 0; pos = pos/2) {
        printf("Eu sou o i = %d \n",i);
        for(int j = pos; j < pos + (pos + 1); j++) {
            printf("eu sou o j %d \n",j);
            struct treeNode * node = (struct treeNode *) malloc(sizeof(struct treeNode)); 
            struct treeNode * nodeLeft  = getLeftChild(j);
            struct treeNode * nodeRight = getRightChild(j);

            node->min = nodeLeft->min;
            node->max = nodeRight->max;
            printf("quase no worker \n");
           // worker(node->sum, nodeLeft->sum, nodeRight->sum);
            node->fromLeft = malloc(sizeJob);
            printf("Vou meter na tree com min %d , max %d NA posição %d\n" , node->min,node->max,j);
            memcpy (&tree[j*sizeof(struct treeNode)], node, sizeof(struct treeNode));
        }
    }  
}

void traverseTreeTopDown () {

}

struct treeNode * getLeftChild(int parent) {
    return  &tree[(2*parent+1)*sizeof(struct treeNode)];
}

struct treeNode * getRightChild(int parent) {
    return &tree[(2*parent+2)*sizeof(struct treeNode)];
}

struct treeNode * getTree(){
    return tree;
}