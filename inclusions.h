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
extern int mat_print(unsigned int n,unsigned int m, double *in);//from an array
extern int mat_write( unsigned int n,unsigned int m, double *a, char *file_name, char *mode );//from a file
//--
extern unsigned int jacobi(double *m, unsigned int n, double *spectre);
extern int IsSym( unsigned int nrow, unsigned int ncol, double *in);//Check if the matrix is symmetric
extern int IsOrtho(unsigned int n, double *in);
extern void transpose(unsigned int nrow, unsigned int ncol, double *A, double *out);
extern void Cross(unsigned int a_nrow, unsigned int b_nrow, unsigned int a_ncol,unsigned int b_ncol, double *A, double *B, double *C);//matricial product
extern void mat_sum(unsigned int n, double *a, double *b, double *out); //matrix sum
#endif //INCLUSIONS_H_INCLUDED
