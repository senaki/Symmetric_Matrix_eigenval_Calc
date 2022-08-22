rotjacobi : matrix.o jacobi.o main.o
	gcc -fdiagnostics-color=always -g -fopenmp matrix.o jacobi.o main.o -lm -o rotjacobi
matrix.o : src/matrix.c src/inclusions.h
	gcc -static -fdiagnostics-color=always -g -DDEBUG=$(_DEBUG) -c -std=gnu11 -W -Wall -lpthread -lm src/matrix.c -fopenmp
jacobi.o : src/matrix.c src/jacobi.c src/inclusions.h
	gcc -static -fdiagnostics-color=always -g -DDEBUG=$(_DEBUG) -c -std=gnu11 -W -Wall -lpthread -lm src/jacobi.c -fopenmp
main.o : src/matrix.c src/jacobi.c src/main.c src/inclusions.h
	gcc -fdiagnostics-color=always -static -fopenmp -g -DDEBUG=$(_DEBUG) -c -std=gnu11 -W -Wall -lpthread src/main.c -fopenmp
clean:
	rm matrix.o jacobi.o main.o rotjacobi
