#include "tetris.h"
#include "redBlackTree.h"

int main(){
	int exit = 0;
	
	initscr();  
	noecho();
	keypad(stdscr, TRUE); 
	
	RBTNode* rankList = NULL;	
	init_rankList(&rankList);
	
	while(!exit){
		clear();
		switch(get_menu()){
			case START_PLAY:
				init();
				play();
				break;
			case PRINT_RANK:
				print_rank(rankList);
				break;
			case EXIT_GAME:
				exit = 1;
				break;
		}
	}


	endwin(); 
	return 0;
}


int get_menu(){
	printw("menu\n");
	printw("1. play game\n");
	printw("2. show rank\n");
	printw("3. exit game\n");
	return wgetch(stdscr)-'0';
}
 
void print_rank(RBTNode* rankList){
	clear();
	RBT_print(rankList);
	wgetch(stdscr);
}

void init_rankList(RBTNode** rankList){
	FILE* fp = fopen("rank.txt", "r");
	if(fp == NULL){
		fp = fopen("rank.txt", "w");
		if(fp == NULL){
			printw("rank file create error!");
			exit(0);
		}
		fprintf(fp, "0\n");
		return;
	}
	
	// init nil node
	element* nil_elem = (element*)  malloc(sizeof(element));
	Nil = RBT_createNode(*nil_elem);
	Nil->color = BLACK;

	int n;
	fscanf(fp, "%d", &n);
	char name[NAME_LEN];
	int score;
	for(int i=0; i<n ;i++){
		fscanf(fp, "%s %d", name, &score);
		element* newElem = (element*) malloc(sizeof(element));
		newElem->name = strdup(name);
		newElem->score = score;
		RBT_insertNode(rankList, RBT_createNode(*newElem));
	}
	
	fclose(fp);
}


void save_rankList(){
	
}

void process_key(int key){
	int flag = 0;
	switch(key){
		case KEYB_DOWN:
			if(flag = (isin(cur_block , 1, 0, 0))){
				cur_block.x += 1;
			}
			break;
		case KEYB_UP:
			if(flag = (isin(cur_block , 0, 0, 1))){
				rotate(&cur_block);
			}
			break;
		case KEYB_LEFT:
			if(flag = (isin(cur_block , 0, -1, 0))){
				cur_block.y -= 1;
			}
			break;
		case KEYB_RIGHT:
			if(flag = (isin(cur_block , 0, 1, 0))){
				cur_block.y += 1;
			}
			break;
		case KEYB_SPACE:
			while(flag = (isin(cur_block , 1, 0, 0))){
				cur_block.x += 1;
			}
			break;
		case NOTHING:
			return;
		default:
			gm.game_over = 1;
			return;
	}
	
	if(flag){
		paintBoard();
		paintShawdow(cur_block);
		paintBlock(cur_block, ' ', 0, cur_block.x, cur_block.y);
	}
	
}

int control_key(){
	int cmd = wgetch(stdscr);
	switch(cmd){
		case KEY_DOWN:
			return KEYB_DOWN;
		case KEY_UP:
			return KEYB_UP;
		case KEY_LEFT:
			return KEYB_LEFT;
		case KEY_RIGHT:
			return KEYB_RIGHT;
		case ' ':
			return KEYB_SPACE;
		case 'q':
			return QUIT;
	}
	
	return NOTHING;
}

void init(){	
	clear();
	// init gm
	gm.timer = 0;
	gm.score = 0;
	gm.game_over = 0;
	for(int i=0; i<=HEIGHT; i++){
		for(int j=0; j<=WIDTH; j++){
			gm.board[i][j] = 0;
		}
	}
	
	// init block's color 
	start_color();
	for(int i=0; i<=5; i++){
		init_pair(i, colors[i], COLOR_YELLOW);
	}
	
	
	// init current tetirs's block 
	initBlock(&cur_block);
	// init next block 
	initBlock(&next_block[0]);
	initBlock(&next_block[1]);

	
	move(0, 0);
	paintBox(0, 0, HEIGHT+2, WIDTH+2);
	
	paintBoard();
	
	// paint next block's box
	int tx = 0;
	int ty = WIDTH+10;
	move(tx, ty);
	paintBox(tx, ty, 7, 9);
	
	tx = 0;
	ty = WIDTH+30;
	move(tx, ty);
	paintBox(tx, ty, 7, 9);
	

}


void initBlock(Block* block){
	block->x = 1;
	block->y = 1;
	block->block_id = rand()%5+1;
	
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			(block->rotate_state)[i][j] = blocks[block->block_id][i][j];
		}
	}
}


void play(){
	act.sa_handler = downBlock;
	sigaction(SIGALRM, &act, &old_act);
	
	paintBlocks();		
	paintShawdow(cur_block);
	refresh();
		
	do{
		if(gm.timer == 0){
			alarm(1);
			gm.timer = 1;
		}
		
		int cmd = control_key();
		process_key(cmd);
	
	}while(!gm.game_over);
	
	clear();
	move(HEIGHT+10, 0);
	printw("GAME OVER!");

	refresh();
}

void copyBlock(Block* dst_block, Block* cpy_block){
	dst_block->x = cpy_block->x;
	dst_block->y = cpy_block->y;
	dst_block->block_id = cpy_block->block_id;
	
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			(dst_block->rotate_state)[i][j] = (cpy_block->rotate_state)[i][j];
		}
	}
}

int removeBottom(){
	for(int r=HEIGHT; r>=1; r--){
		int flag = 1;
		for(int i=1; i<=WIDTH; i++){
			 if(gm.board[r][i] == 0){
				 flag = 0;
				 break;
			 }
		}
		if(!flag){
			continue;
		}
		// r번째 행 깨트림..
		for(int i=r; i>=2; i--){
			for(int j=1; j<=WIDTH; j++){
				gm.board[i][j] = gm.board[i-1][j];
			}
		}
		for(int i=1; i<=WIDTH; i++){
			gm.board[1][i] = 0;
		}
		r -= 1;
	}
}

void downBlock(int signal){
	if(isin(cur_block, 1, 0, 0)){
		cur_block.x += 1;	
		//
		paintBoard();
		paintShawdow(cur_block);
		paintBlock(cur_block, ' ', 0, cur_block.x, cur_block.y);
		// 
	}else{
		if(cur_block.x == 1){
			gm.game_over = 1;
			return;
		}

		fixBlock(cur_block);
		removeBottom();
		paintBoard();
		
		copyBlock(&cur_block, &next_block[0]);
		copyBlock(&next_block[0], &next_block[1]);
		initBlock(&next_block[1]);
		paintBlocks();
	}
	
	gm.timer = 0;
}

void fixBlock(Block block){
	
	int x = block.x;
	int y = block.y;
 
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			if(block.rotate_state[i][j] == 0) continue;
			int tx = x + i;
			int ty = y + j;
 
			gm.board[tx][ty] = block.block_id;
		}
	}
}

 


void paintBoard(){
	for(int i=1; i<=HEIGHT; i++){
		move(i, 1);
		for(int j=1; j<=WIDTH; j++){
			if(gm.board[i][j]){
				attron(COLOR_PAIR(gm.board[i][j]) | A_REVERSE);
				printw(" ");
				attroff(COLOR_PAIR(gm.board[i][j])  | A_REVERSE);
			}else{
				attron(COLOR_PAIR(0));
				printw(".");
				attroff(COLOR_PAIR(0));
			}
		}
	}

}

void paintShawdow(Block block){
	while(isin(block, 1, 0, 0)){
		block.x += 1;
	}
	
	paintBlock(block,'/', 1, block.x, block.y);
}

void paintBlock(Block block, char pattern, int isShawdow, int x, int y){
	int tx = 0;
	int ty = 0;
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			if(block.rotate_state[i][j] == 0){
				continue;
			}
			tx = x + i;
			ty = y + j;
			move(tx, ty);
			if(isShawdow){
				attron(COLOR_PAIR(block.block_id) | A_REVERSE);
				printw("%c", pattern);
				attroff(COLOR_PAIR(block.block_id) | A_REVERSE);
			}else{
				attron(COLOR_PAIR(block.block_id) | A_REVERSE);
				printw("%c", pattern);
				attroff(COLOR_PAIR(block.block_id) | A_REVERSE);
			}
		}
	}
}

void paintBlocks(){
	// paint cur block
	paintBlock(cur_block , ' ', 0, cur_block.x, cur_block.y);
	
	// paint next block
	removeBlock(1, WIDTH+13);
	paintBlock(next_block[0] , ' ', 0, 1, WIDTH+13);
	
	removeBlock(1, WIDTH+33);
	paintBlock(next_block[1] , ' ', 0, 1, WIDTH+33);
}


void removeBlock(int x, int y){
	int tx = 0;
	int ty = 0;
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			tx = x + i;
			ty = y + j;
			move(tx, ty);
			printw(" ");
		}
	}
}

 

int isin(Block block , int px, int py, int rotate){
	int tx = 0;
	int ty = 0;
	int x = block.x + px;
	int y = block.y + py;
	int temp[BLOCK_SIZE][BLOCK_SIZE];
	

	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			if(rotate){
			    temp[i][j] = block.rotate_state[BLOCK_SIZE-j-1][i];	
			}else{
				temp[i][j] = block.rotate_state[i][j];
			}
		}
	}
	
	
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			if(temp[i][j] == 0){
				continue;
			}
			tx = x + i;
			ty = y + j;
			if(tx <= 0 || tx > HEIGHT){
				return 0;
			}
			if(ty <= 0 || ty > WIDTH){
				return 0;
			}
			if(gm.board[tx][ty] != 0){
				return 0;
			}
		}
	}
	
	
	return 1;
}

void rotate(Block* block){
	int temp[BLOCK_SIZE][BLOCK_SIZE];
	
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			temp[i][j] = block->rotate_state[BLOCK_SIZE-j-1][i];
		}
	}
	
	for(int i=0; i<BLOCK_SIZE; i++){
		for(int j=0; j<BLOCK_SIZE; j++){
			block->rotate_state[i][j] = temp[i][j];
		}
	}
} 

void paintBox(int y, int x, int height, int width){

	for(int i=0; i<width-1; i++){
		move(y, x);
		addch(ACS_HLINE);
		x = x+1;
	}
	for(int i=0; i<height-1; i++){
		move(y, x);
		addch(ACS_HLINE);
		y = y+1;
	}
	for(int i=0; i<width-1; i++){
		move(y, x);
		addch(ACS_HLINE);
		x = x -1;
	}
	for(int i=0; i<height-1; i++){
		move(y, x);
		addch(ACS_HLINE);
		y = y -1;
	}
}

