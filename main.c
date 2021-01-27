#include "inclusions.h"
/**
\author Serge Kiki
*\brief Calculate eigenvalues of symmetric square matrices by Jacobi's algorithm
*\fn rotjacobi arg1 arg2
*\param arg1 := input file with dim in the first ROW and the matrix element in the following
*\param arg2 := output file with eigenvalues
*/
const char msg[]="Eigen values of symmetric matrices calculated by the Jacobi's algorithm";
void welcom_msg(const char *msg){
	printf("\033[33m") ;
	for ( unsigned int i=0 ; i < strlen( msg ) ; i++ ) printf("*") ;
	printf("\n%s\n%s\n", msg, author) ;
	for ( unsigned int i=0 ; i < strlen( msg ) ; i++ ) printf("*") ;
	printf("\033[0m\n") ;
}

int main(int argc, char *argv[])
{
	//Verification des paramètres d'entrée
	if ( (argc==1) || (argc==2) ){
		printf("\033[31m %d argument :\n\tArg 1 : input file name;"
		"\n\tArg 2: output file name\nEnter output path name.\033[0m\n", argc-1);
		goto exit_failure ;
	}

	FILE *fidFLAG;
	int status;
	//Pour afficher une seule fois le message d'accueil
	static unsigned int FLAG=0;
	if ( (fidFLAG=fopen(".flag","r")) == NULL ){
		fidFLAG=fopen(".flag", "w");
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
 //-------
	unsigned int i, j, dim=2,//dimension de la matrice
	Niteration=0 ;//Nombre d'itérations
	char *inName=strdup(argv[1]),//duplique argv
	*inFName=basename(inName), //extrait le nom de fichier
	*outName=strdup(argv[2]);
	outName=strcat(outName,"eig_"), outName=strcat(outName,inFName); //nom du fichier de sortie
	printf("\033[31mProcessing : %s\t===>\t%s\033[0m\n", inFName, outName);
	double *M=NULL,	*spc=NULL; //Matrice lire et spectre des valeurs propres
	FILE *fout ;//spectre
	//-------------------------------
	FILE *flux = fopen( argv[1], "r" );
	status=fscanf( flux, "%u\n", &dim ) ;
	M = (double *) malloc ( dim*dim*sizeof(double) ) ;
	for ( i=0 ; i < dim ; i++ ){
		for( j=0 ; j < dim ; j++ ){
			status+=fscanf( flux, "%lf", (M + i*dim + j) ) ;
		}
	}
	if ( IsSym(dim, dim, M) != 1 )
	{
		printf("This matrix is not symmetric\n");
		goto exit_failure ;
	}
	/**
	* spc := vecteur des valeurs propres
	*/
	spc = realloc(spc, dim*sizeof( double ) ) ;
	/**
	* Rotation de Jacobi avec J(M, dim, spc)
	*/
	Niteration=jacobi(M, dim, spc) ;
	fout = fopen(outName, "w");
	/**
	* Write dim and Niteration
	*/
	fprintf( fout, "#%03d\t%03d\n", dim, Niteration ) ;
	for( j=0 ; j < dim ; j++ )
	{
		fprintf( fout, "%010.6lf\n", *( spc + j ) );
	}
	fprintf(fout, "\n") ;
	free(spc) ; free(M) ;
	fclose(fout) ;
	goto exit_success;
	exit_success: return EXIT_SUCCESS;
	exit_failure: return EXIT_FAILURE ;
}
