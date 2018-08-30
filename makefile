all: 
	gcc -O3 *.c -o five.o
	./five.o

debug:
	gcc -g -O0 *.c -o five.o
	gdb five.o --tui
    
