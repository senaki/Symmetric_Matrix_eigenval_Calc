#include "inclusions.h"
#include <time.h>
#include <omp.h>
extern double duration ;
int cmp_fun(const void *a, const void *b){
	const double *va = (const double *)a, *vb = (const double *)b;
	if( isless(*va, *vb) )
		return -1;
	if( isgreater(*va, *vb))
		return 1;
	return 0;
}
/**
\author Serge Kiki
\brief This code calculates eigenvalues of symmetric square matrices by Jacobi's algorithm.

This code calculates eigenvalues of symmetric square matrices by Jacobi's algorithm.

Usage : rotjacobi arg1 arg2

\param arg1 : input file with dim in the first ROW and the matrix element in the following
\param arg2 : output path where the eigenvalues file will be created
\note Do not use it for matrix wih dimension greater than 500 : possible memory issue !
*/
int main(int argc, char *argv[])
{
	if (argc<2){
		fprintf(stderr, "\033[31m %d argument :\n\tArg 1 : input file name;"
		"\n\tArg 2: output file name\nEnter output path name.\033[0m\n", argc-1);
		exit(EXIT_FAILURE);
	}
	const char msg[]="Eigen values of symmetric matrices calculated by the Jacobi's algorithm";
	welcom_msg(msg);
	int nthread = omp_get_max_threads(), sched_chunk;
	char *inputFName=strdup(argv[1]), *eigValueFName=strdup(argv[2]);
	FILE *flux ;
	//-------------------------------
	if( (flux = fopen(inputFName, "r"))==NULL ){
		fprintf(stderr,"The input file does not open for reading\n");
		exit(EXIT_FAILURE);
	}
	//----------------------------------------------------------------------
	unsigned long dim;
	long nel=0;
	nel+=fscanf(flux, "%lu", &dim ) ;
	if(nel == EOF)
		return 1;
	
	sched_chunk = (dim > (long unsigned)nthread ) ? dim / nthread : 10;
	//Pointer vers un tableau de dim x dim (matrice carrée) élements
	double (*M)[dim]= (double (*)[])malloc(dim*dim*sizeof(double));
	//Nombre d'itérations

	if (M == NULL) {
		fprintf(stderr, "Error : cannot allocate memory\n");
		return EXIT_FAILURE ;
	}
	unsigned long i, j ;
	double mmax = 0.0;
	for ( i=0 ; i < dim ; i++ ){
		for( j=0 ; j < dim ; j++ ){
			nel+=fscanf( flux, "%lf", &M[i][j] ) ;
			if( isgreaterequal(fabs(M[i][j]), mmax) )
				mmax = fabs(M[i][j]);
		}
	}
	fprintf(stderr, "Abs. max value : %lf\n", mmax);
	fprintf(stderr, "nel = %li\n", --nel);
	fclose(flux);
	if ( IsOrtho(dim, M[0]) == 0 )
	{
		fprintf(stderr,"This matrix is not orthogonal\n");
	}
	if ( IsSym(dim, M[0]) == false )
	{
		fprintf(stderr,"This matrix is not symmetric\n");
		exit(EXIT_FAILURE) ;
	}
	else
	{
		fprintf(stderr,"This matrix is symmetric\n");
	}
	void *p = (void *)M;
	#pragma omp parallel for schedule(dynamic, sched_chunk)
	for (i =0; i < dim*dim ; i++)
	{
		 *((double *)p) = (double) fdiv( *((double *)p), mmax) ;
		 p += sizeof(double);
	}
	// mat_print(dim, dim, (const double *)&M[0][0]);
	// Calculate eigenvalues with Jacobi rotation : J(dim, M)
 	// Eigenvalues vector
	double eigVal[dim];
	clock_t start, end;
	start=clock();
	unsigned long Niteration=(unsigned long)jacobi(dim, M) ;
	end=clock();
	//----------------------------------------------------------------------
	#pragma omp parallel
	{
	#pragma omp for schedule(dynamic, sched_chunk)
	for( i=0 ; i < dim ; i++){
		eigVal[i]= (double)fmul(mmax, M[i][i]);
	}
	}
	qsort(eigVal, dim, sizeof(double), cmp_fun);

	// Save the eigenvalues
	FILE *fout=fopen(eigValueFName, "w");
	fprintf( fout, "# total iteration : %03lu\n", Niteration ) ;
	mat_write(1, dim, eigVal, fout);
	fclose(fout) ;
	printf("Convergene duration : %.2f s\n", duration);
	printf("%s -> %s :(%-lu; %-lu; %-.2lf s)\n",
	inputFName,eigValueFName, dim, Niteration, (((double)(end-start))/CLOCKS_PER_SEC));
	//free(eigVal);
	free(M);
	free(eigValueFName);free(inputFName);
	return EXIT_SUCCESS;
}
