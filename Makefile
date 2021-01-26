rotjacobi : matrix.o jacobi.o main.o
	gcc matrix.o jacobi.o main.o -lm -o rotjacobi -O3
matrix.o : matrix.c inclusions.h
	gcc -c -std=gnu11 -W -Wall -lm matrix.c -O3 
jacobi.o : matrix.c jacobi.c inclusions.h
	gcc -c -std=gnu11 -W -Wall -lm jacobi.c -O3 
main.o : matrix.c jacobi.c main.c inclusions.h
	gcc -c -std=gnu11 -W -Wall -O3 main.c -O3 

clean:
	rm matrix.o jacobi.o main.o
