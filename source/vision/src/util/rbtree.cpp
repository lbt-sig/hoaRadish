// MIT License

// Copyright (c) 2023 LBT_sig

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <assert.h>

#include "hlog.h"
#include "lbtret.h"
#include "util/rbtree.h"
#include "comm/lbtstd.h"

RBtree * 
rbtree_create(RBtreeCmpFn cmpfn) 
{
    RBtree* ptree = NULL;
    ptree = (RBtree*)lbt_malloc(sizeof(RBtree));
    if(!ptree) {
        return NULL;
    }
    rbtree_init(ptree);
    ptree->cmp_ = cmpfn;
    return ptree;
}

void 
rbtree_init(RBtree* ptree) 
{
    ptree->cmp_ = NULL;
}

static void 
rbtree_rotate_left(RBtree *ptree, RBtreeNode *node) {
    RBtreeNode *right = node->right_;
    node->right_ = right->left_;
    if (right->left_ != NULL) {
        right->left_->parent_ = node;
    }

    right->parent_ = node->parent_;

    if (node->parent_ != NULL) {
        if (node == node->parent_->left_) {
            node->parent_->left_ = right;
        } else {
            node->parent_->right_ = right;
        }
    } else {
        ptree->root_ = right;
    }
    right->left_ = node;
    node->parent_ = right;
}

static void 
rbtree_rotate_right(RBtree* ptree, RBtreeNode *node) {
    RBtreeNode *left = node->left_;
    node->left_ = left->right_;

    if (left->right_ != NULL) {
        if (node == node->parent_->right_) {
            node->parent_->right_ = left;
        } else {
            node->parent_->left_ = left;
        }
    } else {
        ptree->root_ = left;
    }
    left->right_ = node;
    node->parent_ = left;
}

static void 
rbtree_insert_fix(RBtree *ptree, RBtreeNode *node) {
    RBtreeNode *uncle = NULL;
    while(node != ptree->root_ && node->parent_->color_ == RED) {
        if (node->parent_ == node->parent_->parent_->left_) {
            //  Grandfather's right child node is the uncle of the current node.
            uncle = node->parent_->parent_->right_;

            if (uncle->color_ == RED) {
                node->parent_->color_ = BLACK;
                uncle->color_ = BLACK;
                node->parent_->parent_->color_ = RED;
                
                //  Grandfather node as current node.
                node = node->parent_->parent_;
            } else {
                if (node == node->parent_->right_) {
                    node = node->parent_;
                    rbtree_rotate_left(ptree, node);
                }
                node->parent_->color_ = BLACK;
                node->parent_->parent_->color_ = RED;
                rbtree_rotate_right(ptree, node->parent_->parent_);
            }
        } else {
            uncle = node->parent_->parent_->left_;

            if (uncle->color_ == RED) {
                node->parent_->color_ = BLACK;
                uncle->color_ = BLACK;
                node->parent_->parent_->color_ = RED;
                node = node->parent_->parent_;
            } else {
                if (node == node->parent_->left_) {
                    node = node->parent_;
                    rbtree_rotate_right(ptree, node);
                }
                node->parent_->color_ = BLACK;
                node->parent_->parent_->color_ = RED;
                rbtree_rotate_left(ptree, node->parent_);
            }
        }
    }
    ptree->root_->color_ = BLACK;
}

RBtreeNode *
rbtree_insert(RBtree* ptree, RBtreeNode* node)
{
    lbt_assert(ptree);
    lbt_assert(node);

    int res = 0;
    RBtreeNode *root = ptree->root_;
    RBtreeNode *parent = NULL;

    while(node) {
        if((res = ptree->cmp_(node, root)) == 0) {
            return NULL;
        }
        parent = root;

        if(res < 0) {
            root = root->left_;
        } else {
            node = node->right_;
        }
    }

    node->parent_ = parent;
    node->left_ = node->right_ = NULL;
    
    // initialize is red.
    node->color_ = RED;
    ptree->n_++;
    
    if (parent != NULL) {
        if (res < 0) {
            parent->left_ = node;
        } else {
            parent->right_ = node;    
        }
    } else {
        ptree->root_ = node;
    }

    rbtree_insert_fix(ptree, node);
    return node;
}