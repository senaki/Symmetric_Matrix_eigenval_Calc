#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#ifndef INCLUSIONS_H_INCLUDED
#define INCLUSIONS_H_INCLUDED
#define author "SERGE KIKI"
#define TOL 1E-6
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <libgen.h>
extern char *strdup(const char *);
//from an array
extern int mat_print(unsigned int n,unsigned int m, const double *in);
//from a file
extern int mat_write( unsigned int n,unsigned int m, const  double *a, FILE *stream );
//extern unsigned int jacobi(unsigned int n, const double *m, double *outEigVal, double *outEigVec); Next release
extern unsigned int jacobi(unsigned int n, const double *m, double *outEigVal);
extern int IsSym( unsigned int n, const double *in);//Check if the matrix is symmetric
extern int IsOrtho(unsigned int n, const double *in);
extern void transpose(unsigned int nrow, unsigned int ncol, const double *A, double *out);
extern void Cross(unsigned int a_nrow, unsigned int b_nrow, unsigned int a_ncol, unsigned int b_ncol, const double* restrict A, const double* restrict B, double* restrict C);//matricial product
extern void mat_sum(unsigned int n, const double *a, const double *b, double *out); //matrix sum
extern int mat_zeros(unsigned int n, unsigned int m, double *mat) ;
extern double (*mat_eye(const unsigned int dim))[] ;
#endif //INCLUSIONS_H_INCLUDED
