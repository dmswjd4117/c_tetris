#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

typedef  struct _element{
	int index;
	int score;
	char* name;
} element;

typedef struct _RBTNode
{
    struct _RBTNode* parent;
    struct _RBTNode* left;
    struct _RBTNode* right;

    enum { RED, BLACK } color;

    element data;

} RBTNode;

RBTNode* Nil;

void init_rankList(RBTNode** rankList);
void print_rank(RBTNode* rankList);
void RBT_print(RBTNode* node);

RBTNode* RBT_createNode(element data);

void RBT_insertNode(RBTNode** tree, RBTNode* newNode);
void RBT_insertHelper(RBTNode** tree, RBTNode* newNode);
void RBT_insertFixUp(RBTNode** tree, RBTNode* newNode);

RBTNode* RBT_searchMinNode(RBTNode* tree);
RBTNode* RBT_searchNode(RBTNode* tree, element data);
RBTNode* RBT_removeNode(RBTNode** tree, element data);
void RBT_removeFixUp(RBTNode** tree, RBTNode* newNode);

void RBT_rotateLeft(RBTNode** Root, RBTNode* parent);
void RBT_rotateRight(RBTNode** Root, RBTNode* parent);

#endif
