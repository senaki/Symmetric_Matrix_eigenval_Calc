rotjacobi : matrix.o jacobi.o main.o
	gcc matrix.o jacobi.o main.o -lm -o rotjacobi
matrix.o : matrix.c inclusions.h
	gcc -c -std=gnu11 -O3 -W -Wall -lm matrix.c
jacobi.o : matrix.c jacobi.c inclusions.h
	gcc -c -std=gnu11 -O3 -W -Wall -lm jacobi.c
main.o : matrix.c jacobi.c main.c inclusions.h
	gcc -c -std=gnu11 -O3 -W -Wall main.c

clean:
	rm matrix.o jacobi.o main.o
