rotjacobi : matrix.o jacobi.o main.o
	gcc matrix.o jacobi.o main.o -lm -o rotjacobi
matrix.o : matrix.c inclusions.h
	gcc -std=gnu11 -W -Wall -lm -c matrix.c
jacobi.o : matrix.c jacobi.c inclusions.h
	gcc -std=gnu11 -W -Wall -lm -c jacobi.c
main.o : matrix.c jacobi.c main.c inclusions.h
	gcc -std=gnu11 -W -Wall -lm -c main.c
	
clean:
	rm matrix.o jacobi.o main.o
