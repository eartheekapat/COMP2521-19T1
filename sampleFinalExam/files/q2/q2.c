#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "BTree.h"

int nodeCount(BTree t){
    if(t == NULL) {
        return 0;
    }
    return 1 + nodeCount(t->left)+ nodeCount(t->right);
}

int isCompleteHeap(BTree t,int index,int numNodes){
    if(t == NULL) {
        //Empty tree is complete
        return 1;
    }
    // If index is more than numNodes, we know that the tree
    // is not a proper heap
    // left child 2*k, right child 2*k+1
    if (index > numNodes){
        return 0;
    }
    // +1 and +2 because we didnt start from 1 (i think)
    return isCompleteHeap(t->left,2*index+1,numNodes) && isCompleteHeap(t->right,2*index+2,numNodes);
}

int isMaxHeap(BTree t){
    if(t->left == NULL && t->right == NULL){
        return 1;
    }
    
    if(t->right == NULL){
        // check left child only
        return t->data >= t->left->data;
    } else {
        if(t->data >= t->left->data && t->data >= t->right->data){
            // True so we keep recurring
            return isMaxHeap(t->left) && isMaxHeap(t->right);
        } else {
            // False so we return
            return 0;
        }
    }
}

int isHeap(BTree t){
    int count = nodeCount(t);
    int index = 0;
    return isCompleteHeap(t,index,count) && isMaxHeap(t);
}