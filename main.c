#include "inclusions.h"
/**
*\brief Calculate eigenvalues of symmetric square matrices by Jacobi's algorithm
*\fn rotjacobi arg1 arg2
*\param arg1 := input file with dim in the first ROW and the matrix element in the following
*\param arg2 := output file with eigenvalues
*/
const char msg[]="Eigen values of symmetric matrices calculated by the Jacobi's algorithm";
void welcom_msg(const char *msg){
	/*!
	* \author Serge Kiki
	* \brief Affiche un message d'accueil.
	*/
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

	int status;
	FILE *fidFLAG;//Pour afficher une seule fois le message d'accueil
	static unsigned int FLAG=0;
	if ( (fidFLAG=fopen("flag","r")) == NULL ){
		
		welcom_msg( msg );//Message d'accueil
		fidFLAG=fopen("flag", "w");
		fprintf(fidFLAG,"%u",1);
		fclose(fidFLAG);
	}
	else {
		status=fscanf(fidFLAG,"%u",&FLAG);
		FLAG++;
		fprintf(fidFLAG,"%u", FLAG);
		fclose(fidFLAG);
	}

 //-------
	unsigned int i, j, dim=2,//dimension de la matrice
	Niteration=0 ;//Nombre d'itérations
	char //*inPath=dirname(argv[1]),
	*inName=strdup(argv[1]),//duplique argv
	*inFName=basename(inName),//extrait le nom de fichier
	*outName=strdup(argv[2]);
	outName=strcat(outName,"eig_"), outName=strcat(outName,inFName); //nom du fichier de sortie
	printf("\033[31mProcessing: %s\t===>%s\033[0m\n", inFName, outName);
	//goto exit_success ;
	double *M=NULL,//Matrice lire
	*spc=NULL; //Matrice lire et spectre des valeurs propres
	FILE *fout,//spectre
	*fNiteration ;//nombre d'itérations
	//Lecture de la matrice

	//-------------------------------
	FILE *flux = fopen( argv[1], "r" );
	status=fscanf( flux, "%u\n", &dim ) ;
	M = (double *) malloc ( dim*dim*sizeof(double) ) ;
	for ( i=0 ; i < dim ; i++ ){
		for( j=0 ; j < dim ; j++ ){
			status+=fscanf( flux, "%lf", (M + i*dim + j) ) ;
		}
	}
//	printf( "\nRead data : %2i\tDim : %2u\n",status, dim ) ;
	if (IsSym(dim, dim, M) != 0)
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
	//printf("\033[1;32mEigen values spectra : \033[0m");
	//mat_print(1,dim,spc);
	fout = fopen(outName, "w");
	//fNiteration = fopen("../output/Niteration", "w");
	fNiteration=fout;
	/**
	* Write dim and Niteration
	*/
	fprintf( fNiteration, "%03d\t%03d\n", dim, Niteration ) ;
	for( j=0 ; j < dim ; j++ )
	{
		fprintf( fout, "%08.6lf\n", *( spc + j ) );
	}
	fprintf(fout, "\n") ;
	free(spc) ; free(M) ;
	fclose(fout) ;
	goto exit_success;
	//fclose(fNiteration) ;
	exit_success: return EXIT_SUCCESS;
	exit_failure: return EXIT_FAILURE ;
}
