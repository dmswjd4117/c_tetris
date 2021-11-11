#include "redBlackTree.h"


void RBT_print(RBTNode* node) {
    if (node == Nil) {
        return;
    }

    
    RBT_print(node->left);
    printw("  score : %d\n  name : %s\n\n",
		   node->data.score,  node->data.name);
    RBT_print(node->right);
}


RBTNode* RBT_createNode(element data) {
    RBTNode* newNode = (RBTNode*)malloc(sizeof(RBTNode));
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->data = data;
    newNode->color = BLACK;
    return newNode;
}

void RBT_insertHelper(RBTNode** tree, RBTNode* newNode) {
    if ((*tree) == NULL) {
        (*tree) = newNode;
        return;
    }

    RBTNode* parent = (*tree);
    if ((parent->data).score > (newNode->data).score) {
        if (parent->right == Nil) {
            parent->right = newNode;
            newNode->parent = parent;
        }
        else {
            RBT_insertHelper(&parent->right, newNode);
        }

        return;
    }


    if ((parent->data).score < (newNode->data).score) {
        if (parent->left == Nil) {
            parent->left = newNode;
            newNode->parent = parent;
        }
        else {
            RBT_insertHelper(&parent->left, newNode);
        }

        return;
    }
}


void RBT_insertNode(RBTNode** tree, RBTNode* newNode) {
   
    RBT_insertHelper(tree, newNode);

    newNode->color = RED;
    newNode->left = Nil;
    newNode->right = Nil;
    

    RBT_insertFixUp(tree, newNode);

}


void RBT_insertFixUp(RBTNode** root, RBTNode* node) {
    while (node != *(root) && node->parent->color == RED) { 
        if (node->parent == node->parent->parent->left) {
            RBTNode* uncle = node->parent->parent->right;
            if (uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right) {
                    node = node->parent;
                    RBT_rotateLeft(root, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                RBT_rotateRight(root, node->parent->parent);

            }
        }
        else {
            RBTNode* uncle = node->parent->parent->left;
            if (uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left) {
                    node = node->parent;
                    RBT_rotateRight(root, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                RBT_rotateLeft(root, node->parent->parent);
            }
        }
    }

    (*root)->color = BLACK;
}

RBTNode* RBT_searchNode(RBTNode* tree, element data) {
    if (tree == Nil || tree == NULL) {
        return NULL;
    }
    if ((tree->data).score > data.score) {
        return RBT_searchNode(tree->right, data);
    }
    if ((tree->data).score < data.score) {
        return RBT_searchNode(tree->left, data);
    }
    return tree;
}

RBTNode* RBT_searchMinNode(RBTNode* tree) {
    if (tree == Nil) {
        return NULL;
    }
    if (tree->left == Nil) {
        return tree;
    }
    return RBT_searchMinNode(tree->left);
}
 
RBTNode* RBT_removeNode(RBTNode** tree, element data) {
    RBTNode* removed = NULL;
    RBTNode* child = NULL;
    RBTNode* target = RBT_searchNode((*tree), data);
    if (target == NULL) {
        return NULL;
    }
    if (target->left == Nil || target->right == Nil) {
        removed = target;
    }
    else {
        removed = RBT_searchMinNode(target->right);
        target->data = removed->data;
    }

    // removed'child <-> removed's parent

    if (removed->left != Nil) {
        child = removed->left;
    }
    else {
        child = removed->right;
    }
    if (child != Nil) {
        child->parent = removed->parent;
    }



    if (removed->parent == NULL) {
        (*tree) = child;
    }
    else if (removed->parent->right == removed) {
        removed->parent->right = child;
    }
    else {
        removed->parent->left = child;
    }


    // fix up 

    if (removed->color == BLACK) {
        RBT_removeFixUp(tree, child);
    }

    return removed;
}

void RBT_removeFixUp(RBTNode** tree, RBTNode* node) {
    RBTNode* sibling = NULL;

    while (node->parent != NULL && node->color == BLACK) {
        if (node == node->parent->left) {
            sibling = node->parent->right;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                RBT_rotateLeft(tree, node->parent);
            }
            else {
                if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->left->color == RED) {
                        sibling->color = RED;
                        sibling->left->color = BLACK;
                        RBT_rotateRight(tree, sibling);
                    }
                    else {
                        sibling->color = node->parent->color;
                        node->parent->color = BLACK;
                        sibling->right->color = BLACK;
                        RBT_rotateLeft(tree, node->parent);
                        node = (*tree);
                    }
                }
            }
        }
        else {
            sibling = node->parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                RBT_rotateRight(tree, node->parent);
            }
            else {
                if (sibling->left->color == BLACK && sibling->left->color == BLACK) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->right->color == RED) {
                        sibling->color = RED;
                        sibling->right->color = BLACK;
                        RBT_rotateLeft(tree, sibling);
                    }
                    else {
                        sibling->color = node->parent->color;
                        node->parent->color = BLACK;
                        sibling->left->color = BLACK;
                        RBT_rotateRight(tree, node->parent);
                        node = (*tree);
                    }
                }
            }
        }
    }
}


void RBT_rotateLeft(RBTNode** root, RBTNode* parent) {
  
    // 1. rightchild's left <-> parent 
    RBTNode* rightChild = parent->right;
    parent->right = rightChild->left;
    if (rightChild->left != NULL) {
        rightChild->left->parent = parent;
    }

    // 2. rightchild <-> parent's parent
    rightChild->parent = parent->parent;
    if (parent->parent == NULL) {
        (*root) = rightChild;
    }
    else {
        if (parent == parent->parent->right) {
            parent->parent->right = rightChild;
        }
        else {
            parent->parent->left = rightChild;
        }
    }

    // 3. parent <-> child
    rightChild->left = parent;
    parent->parent = rightChild;
}





void RBT_rotateRight(RBTNode** root, RBTNode* parent) {

    // 1. leftChild's left <-> parent 
    RBTNode* leftChild = parent->left;
    parent->left = leftChild->right;  
    if (leftChild->right != NULL) {
        leftChild->right->parent = parent;
    }

    // 2. leftChild <-> parent's parent
    leftChild->parent = parent->parent;
    if (parent->parent == NULL) {
        (*root) = leftChild;
    }
    else {
        if (parent == parent->parent->right) {
            parent->parent->right = leftChild;
        }
        else {
            parent->parent->left = leftChild;
        }
    }

    // 3. parent <-> child
    leftChild->right = parent;
    parent->parent = leftChild;

}