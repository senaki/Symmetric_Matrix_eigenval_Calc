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
const char msg[]="Eigen values of symmetric matrices calculated by the Jacobi's algorithm";
void welcom_msg(const char *msg){
	size_t msgLen = strlen(msg), i ;
	printf("\033[33m") ;
	for ( i=0 ; i < msgLen ; i++ ) printf("*") ;
	printf("\n%s\n%s\n", msg, author) ;
	for ( i=0 ; i < msgLen ; i++ ) printf("*") ;
	printf("\033[0m\n") ;
}

int main(int argc, char *argv[])
{
		if ( (argc==1) || (argc==2) ){
		fprintf(stderr, "\033[31m %d argument :\n\tArg 1 : input file name;"
		"\n\tArg 2: output file name\nEnter output path name.\033[0m\n", argc-1);
		exit(EXIT_FAILURE);
	}
	FILE *fidFLAG;
	int status;
	unsigned int FLAG=0;;

	if ( (fidFLAG=fopen(".flag","r+")) == NULL ){
		fidFLAG=fopen(".flag", "w+");
		welcom_msg( msg );//Message d'accueil
		FLAG++;
		fprintf(fidFLAG,"%u", FLAG);
		fclose(fidFLAG);
	}
	else {
		status=fscanf(fidFLAG,"%u", &FLAG);
		FLAG++;
		fprintf(fidFLAG,"%u", FLAG);
		fclose(fidFLAG);
	}
	size_t i, j ;//Nombre d'itérations
	char *inputFName=strdup(argv[1]),//duplique argv
	*inFName=basename(inputFName), //extrait le nom de fichier
	*eigValueFName=strdup(argv[2]);
	strcat(strcat(eigValueFName,"eigval_"), inFName);
	//-------------------------------
	FILE *flux = fopen( inputFName, "r" );
	if( flux == NULL ){
		fprintf(stderr,"The input file does not open for reading\n");
		exit(EXIT_FAILURE);
	}
	/**
	\var dim
	\brief Matrix dimension
	\var M
	\brief Matrix
	*/
	unsigned int dim = 2;
	status = fscanf( flux, "%u", &dim ) ;
	double (*M)[dim] = (double (*)[dim]) calloc(dim, sizeof(double[dim]) );
	if (M == NULL) {
		fprintf(stderr, "Error : cannot allocate memory\n");
		return EXIT_FAILURE ;
	}
	for ( i=0 ; i < dim ; i++ ){
		for( j=0 ; j < dim ; j++ ){
			status+=fscanf( flux, "%lf", &M[i][j] ) ;
		}
	}
	if ( IsSym(dim, &M[0][0]) != 1 )
	{
		fprintf(stderr,"This matrix is not symmetric\n");
		exit(EXIT_FAILURE) ;
	}
	/*
		Calculate eigenvalues with Jacobi rotation : J(dim, M)
	*/
	double *eigVal=(double *)calloc(dim, sizeof(double)); // Eigenvalues vector
	clock_t start, end; // Measurement of duration
	start=clock();
	size_t Niteration=jacobi(dim, M) ;
	end=clock();
	for( i=0 ; i < dim ; i++){
		eigVal[i]= M[i][i] ;
	}
	free(M);
	/*
	 Save the eigenvalues
	*/
	FILE *fout=fopen(eigValueFName, "w");
	fprintf( fout, "# total iteration : %03lu\n", Niteration ) ;
	mat_write(1, dim, eigVal, fout);
	fclose(fout) ;
	printf("\033[31mProcessing %s took : ", inputFName);
	printf("%ld ms | %lu rotation%c\033[0m\n",
	(end-start)*1000/CLOCKS_PER_SEC, Niteration, (Niteration>1)?'s':' ');

	return EXIT_SUCCESS;
}
