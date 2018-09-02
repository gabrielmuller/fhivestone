all: 
	gcc -O3 *.c -o fhive.o
	./fhive.o

debug:
	gcc -g -O0 *.c -o fhive.o
	gdb fhive.o --tui
