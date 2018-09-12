all: 
	gcc  -O3 *.c -o fhive -fopenmp

single:
	gcc  -O3 *.c -o fhive
