#include "inclusions.h"
#include <time.h>
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
	char *inputFName=strdup(argv[1]), *eigValueFName=strdup(argv[2]);
	FILE *flux ;
	//-------------------------------
	if( (flux = fopen(inputFName, "r"))==NULL ){
		fprintf(stderr,"The input file does not open for reading\n");
		exit(EXIT_FAILURE);
	}
	//----------------------------------------------------------------------
	size_t dim;
	int nel=-1;
	nel+=fscanf(flux, "%u", &dim ) ;
	double (*M)[dim]= (double (*)[])malloc(dim*dim*sizeof(double));
	//Nombre d'itérations

	if (M == NULL) {
		fprintf(stderr, "Error : cannot allocate memory\n");
		return EXIT_FAILURE ;
	}
	size_t i, j ;
	for ( i=0 ; i < dim ; i++ ){
		for( j=0 ; j < dim ; j++ ){
			nel+=fscanf( flux, "%lf", &M[i][j] ) ;
		}
	}
	fclose(flux);

	if ( IsSym(dim, M[0]) == false )
	{
		fprintf(stderr,"This matrix is not symmetric\n");
		exit(EXIT_FAILURE) ;
	}
	// Calculate eigenvalues with Jacobi rotation : J(dim, M)
 	// Eigenvalues vector
	double eigVal[dim];
	clock_t start, end;
	start=clock();
	size_t Niteration=jacobi(dim, M) ;
	end=clock();
	//----------------------------------------------------------------------
	for( i=0 ; i < dim ; i++){
		eigVal[i]= M[i][i];
	}
	// Save the eigenvalues
	FILE *fout=fopen(eigValueFName, "w");
	fprintf( fout, "# total iteration : %03lu\n", Niteration ) ;
	mat_write(1, dim, eigVal, fout);
	fclose(fout) ;
	printf("%s -> %s :\t%-u\t%-lu\t%-ld ms\n",
	inputFName,eigValueFName, dim, Niteration, (end-start)*1000/CLOCKS_PER_SEC);
	//free(eigVal);
	free(M);
	free(eigValueFName);free(inputFName);
	return EXIT_SUCCESS;
}
