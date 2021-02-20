rotjacobi : matrix.o jacobi.o main.o
	gcc -O matrix.o jacobi.o main.o -lm -g -o rotjacobi
matrix.o : src/matrix.c src/inclusions.h
	gcc -c -std=gnu11 -O3 -W -Wall -lm src/matrix.c -g
jacobi.o : src/matrix.c src/jacobi.c src/inclusions.h
	gcc -c -std=gnu11 -O3 -W -Wall -lm src/jacobi.c -g
main.o : src/matrix.c src/jacobi.c src/main.c src/inclusions.h
	gcc -c -std=gnu11 -O3 -W -Wall src/main.c -g

clean:
	rm matrix.o jacobi.o main.o rotjacobi
