#include "comm/v_type.h"

typedef vint32 (*RBtreeCmpFn)(void* a, void* b);

// Node Color.
enum Color {
    RED   = 0,
    BLACK = 1,
};

// define rbtree node.
struct RBtreeNode
{
    RBtreeNode* left_;
    RBtreeNode* right_;
    RBtreeNode* parent_;
    Color       color_;
};

struct RBtree 
{
    RBtreeCmpFn cmp_;
    RBtreeNode *root_;
    vint64         n_; 
};

// create a rbtree pointer.
RBtree* rbtree_create(RBtreeCmpFn);
void rbtree_init(RBtree*);