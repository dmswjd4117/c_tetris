


main : main.o  
	gcc main.o  -o main  -lncurses
	
	
main.o : main.c tetris.h
	gcc -c main.c  -o main.o 

 