#include "inclusions.h"
#include <time.h>
/**
\author Serge Kiki
*\brief Calculate eigenvalues of symmetric square matrices by Jacobi's algorithm
*\fn rotjacobi arg1 arg2
*\param arg1 := input file with dim in the first ROW and the matrix element in the following
*\param arg2 := output file with eigenvalues
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
	//Verification des paramètres d'entrée
	if ( (argc==1) || (argc==2) ){
		fprintf(stderr, "\033[31m %d argument :\n\tArg 1 : input file name;"
		"\n\tArg 2: output file name\nEnter output path name.\033[0m\n", argc-1);
		exit(EXIT_FAILURE);
	}
	FILE *fidFLAG;
	int status;
	unsigned int FLAG=0;;
	//Pour afficher une seule fois le message d'accueil
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
	/*
	*eigVectorFName=strdup(argv[2]);//nom du fichier de sortie
	strcat(strcat(eigVectorFName,"eigvec_"),inFName);
	printf("Output file for eigenvalues :%s\nOutput file for eigenvectors : %s\n",eigValueFName,eigVectorFName );
	*/
	//-------------------------------
	FILE *flux = fopen( inputFName, "r" );
	if( flux == NULL ){
		fprintf(stderr,"The input file does not open for reading\n");
		exit(EXIT_FAILURE);
	}
	unsigned int dim=2;//dimension de la matrice
	status=fscanf( flux, "%u\n", &dim ) ;
	double M[dim][dim];
	// double (*M)[dim];
	// M = (double (*)[dim]) calloc( dim*dim, sizeof(double) ) ;
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
	* Rotation de Jacobi avec J(M, dim, spc)
	*/
	printf("\033[31mProcessing : %s\033[0m\n", inputFName);
	//--------------------------------------------------
	// Eigenvalues vector
	double eigVal[dim];
	// double	*eigVal=(double *)calloc(dim, sizeof( double ));
	// Eigenvector matrix
	// double (*eigVec)[dim];
	//double (*eigVec)[dim]=NULL;
	// Calculate eigenvalues
	clock_t start, end; // Mesurate calculation duration
	start=clock();
	size_t Niteration=jacobi(dim, &M[0][0], eigVal) ;
	end=clock();
	// Save the eigenvalues
	FILE *fout=fopen(eigValueFName, "w");
	fprintf( fout, "# total iteration : %03lu\n", Niteration ) ;
	mat_write(1,dim, eigVal, fout);
	fclose(fout) ;
	/* Save the eigenvectors
	fout=fopen(eigVectorFName, "w");
	mat_write(dim,dim,&eigVec[0][0], fout);
	fclose(fout) ;
	free(eigVec);
	*/
	//free(eigVal);free(M) ;
	printf("Duration : %ld ms | Number of rotation : %lu\n",
	(end-start)*1000/CLOCKS_PER_SEC, Niteration);
	return EXIT_SUCCESS;
}
