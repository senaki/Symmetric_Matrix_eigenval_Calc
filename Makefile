rotjacobi : matrix.o jacobi.o main.o
	gcc -O matrix.o jacobi.o main.o -lm -o rotjacobi
matrix.o : src/matrix.c src/inclusions.h
	gcc -c -std=gnu11 -O3 -W -Wall -lm src/matrix.c
jacobi.o : src/matrix.c src/jacobi.c src/inclusions.h
	gcc -c -std=gnu11 -O3 -W -Wall -lm src/jacobi.c
main.o : src/matrix.c src/jacobi.c src/main.c src/inclusions.h
	gcc -c -std=gnu11 -O3 -W -Wall src/main.c

clean:
	rm matrix.o jacobi.o main.o
