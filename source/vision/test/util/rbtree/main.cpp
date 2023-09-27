#include "rbtree.h"
#include "lbtstd.h"

vint32 cmp(void *a, void *b) { 
    lbt_assert(a);
    lbt_assert(b);
    return (*(vint32*)a) - (*(vint32*)b);
}

int 
main(int argc, char** argv) {
    RBtree* rbt = rbtree_create(cmp);
}