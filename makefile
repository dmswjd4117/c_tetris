
OBJECTS = main.o redBlackTree.o 


main : $(OBJECTS)  
	gcc $(OBJECTS) -o main  -lncurses
	
	
main.o : main.c 
	gcc -c main.c  -o main.o 

redBlackTree.o : redBlackTree.c
	gcc -c redBlackTree.c  -o redBlackTree.o 
	
	
clean:
	rm *.o