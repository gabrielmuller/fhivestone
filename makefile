all: 
	gcc  -O3 *.c -o fhive.o
	./fhive.o

debug:
	gcc  -g -O0 *.c -o fhive.o
	gdb fhive.o --tui

dbp:
	gcc  -g -O0 *.c -o fhive.o -DBENCHMARK
	gdb fhive.o --tui
	

profile:
	gcc  -pg -O3 *.c -o fhive.o -DBENCHMARK
	./fhive.o 
	gprof fhive.o

mem:
	gcc  -O3 *.c -o fhive.o -DBENCHMARK
	valgrind ./fhive.o

clean:
	rm *.o *.gch *.s *.out
