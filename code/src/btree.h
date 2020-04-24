#ifndef _BTREE_H
#define __BTREE_H


typedef struct   
{ 
  int min;
  int max;
  void * sum; 
  void * fromLeft;
} TreeNode;


void buildTree (
    void *src,
    size_t nJob,
    size_t sizeJob,
    void (*worker)(void *v1, const void *v2, const void *v3)
);
void createTreeNode (int current, int min, int max, void *src, size_t nJob,size_t nPow, size_t sizeJob, void (*worker)(void *v1, const void *v2, const void *v3)) ;

void traverseTreeTopDown (

);
void updateTreeNode (int current,size_t nJob, size_t sizeJob, void* src ,void * dest,void (*worker)(void *v1, const void *v2, const void *v3)) ;

void traverseTree (size_t nJob, size_t sizeJob, void* src ,void * dest,void (*worker)(void *v1, const void *v2, const void *v3)) ;

TreeNode getLeftChild (
    int parent
);

TreeNode getRightChild (
    int parent
);

TreeNode * getTree(
);

int nextPower_2(
    int x
); 

#endif