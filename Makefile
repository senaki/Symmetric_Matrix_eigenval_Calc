rotjacobi : matrix.o jacobi.o main.o
	gcc -O3 matrix.o jacobi.o main.o -lpthread -lm -o rotjacobi
matrix.o : src/matrix.c src/inclusions.h
	gcc -O3 -c -std=gnu11 -W -Wall -lpthread -lm src/matrix.c
jacobi.o : src/matrix.c src/jacobi.c src/inclusions.h
	gcc -O3 -c -std=gnu11 -W -Wall -lpthread -lm src/jacobi.c
main.o : src/matrix.c src/jacobi.c src/main.c src/inclusions.h
	gcc -O3 -c -std=gnu11 -W -Wall -lpthread src/main.c
clean:
	rm matrix.o jacobi.o main.o rotjacobi
