#include<stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define WIDTH 10
#define HEIGHT 20
#define BLOCK_SIZE 4
#define NAME_LEN 100

#define QUIT 1
#define KEYB_SPACE 2
#define KEYB_DOWN 3
#define KEYB_LEFT 4
#define KEYB_RIGHT 5
#define KEYB_UP 6
#define NOTHING -1

#define START_PLAY 1
#define PRINT_RANK 2
#define EXIT_GAME 3

struct sigaction act;
struct sigaction old_act;

 
int colors[6] = { 
	COLOR_BLACK, COLOR_RED, COLOR_MAGENTA, COLOR_CYAN, COLOR_BLUE, COLOR_GREEN
};



typedef struct _game_manger{
	int board[HEIGHT+1][WIDTH+1];
	int score;
	int game_over;
	int timer;
} GameM;


typedef struct block{
	int x;
	int y;
	int rotate_state[BLOCK_SIZE][BLOCK_SIZE];
	int color;
	int block_id;
} Block;

GameM gm;

Block cur_block;
Block ghost_block;
Block next_block[4];
int BLOCK_CNT = 5;

int blocks[6][BLOCK_SIZE][BLOCK_SIZE] = {
	{
		
	},
	{
		{0, 0, 0, 0},
		{0, 1, 1, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0},
	},
	{
		{0, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0},
	},
	{
		{1, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	},
	{
		{0, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 0},
	},
	{
		{0, 0, 0, 0},
		{0, 1, 0, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
	}
};


int get_menu();
int control_key();
void process_key(int key);

void paintBlocks();
void initBlock(Block* block);
void copyBlock(Block* dst_block, Block* cpy_block);

void unpaintBlock(Block block);
void downBlock(int signal);
void paintChange();
void fixBlock(Block block);

void play();
void init();

void paintOutLine();

void paintShawdow(Block block);
void paintBoard();
void paintBox();

int isin(Block block , int px, int py, int rotate);
void rotate();
void paintBlock(Block block, char pattern, int is_shawdow, int x, int y);
void removeBlock(int x, int y);
int removeLine();

 

