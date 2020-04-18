#ifndef _BTREE_H
#define __BTREE_H

struct treeNode  
{ 
  int min;
  int max;
  void * sum; 
  void * fromLeft;
} ;




struct treeNode * tree;

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

struct treeNode * getTree();

#endif