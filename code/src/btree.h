#ifndef _BTREE_H
#define __BTREE_H


void buildTreeBottomUp (
    void *src,
    size_t nJob,
    size_t sizeJob,
    void (*worker)(void *v1, const void *v2, const void *v3)
);

void traverseTreeTopDown (

);

struct treeNode * getLeftChild (
    int parent
);

struct treeNode * getRightChild (
    int parent
);

#endif