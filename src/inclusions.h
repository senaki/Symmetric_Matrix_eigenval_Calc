#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#ifndef INCLUSIONS_H_INCLUDED
#define INCLUSIONS_H_INCLUDED
#define author "Serge NKS"
#define TOL 1e-5
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <libgen.h>
#include <stddef.h>
extern char *strdup(const char *);
extern void *alloca(size_t size);
//from an array
extern int mat_print(size_t n, size_t m, const double *in);
//from a file
extern int mat_write( size_t n, size_t m, const  double *a, FILE *stream );
//extern size_t jacobi(size_t n, const double *m, double *outEigVal, double *outEigVec); Next release
extern size_t jacobi(size_t n, double (*m)[n]);
extern int IsSym( size_t n, const double *in);//Check if the matrix is symmetric
extern int IsOrtho(size_t n, const double *in);
extern void transpose(size_t nrow, size_t ncol, const double *A, double *out);
extern int Cross(size_t Anrow, size_t Bnrow, size_t Ancol, size_t Nncol,
  const double *A, const double *B, double *C);//matricial product
extern void mat_sum(size_t n, const double *a, const double *b, double *out); //matrix sum
extern int mat_zeros(size_t n, size_t m, double *mat) ;
extern double (*mat_eye(const size_t dim))[] ;
#endif //INCLUSIONS_H_INCLUDED
