all: 
	gcc  -O3 *.c -o fhive -fopenmp
	./fhive 

debug:
	gcc  -g -O0 *.c -o fhive -fopenmp 
	gdb --tui --args fhive --autoplay --depths 5 5 --complete

dbp:
	gcc  -g -O0 *.c -o fhive 
	gdb fhive --tui
	

profile:
	gcc  -pg -O3 *.c -o fhive 
	./fhive 
	gprof fhive

bench:
	gcc -O3 *.c -o fhive  -fopenmp
	./fhive --autoplay

mem:
	gcc  -g -O0 *.c -o fhive  -fopenmp
	valgrind  --leak-check=full ./fhive --depths 4 4 --autoplay

clean:
	rm *.o *.gch *.s *.out
