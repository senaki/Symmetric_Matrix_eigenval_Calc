#include "inclusions.h"
#include <omp.h>
//static double array[4096][4096];

  /**
  \brief Writes a nxm matrix into a file \param filename
  \fn int mat_write(size_t n, size_t m, const double *mat, const char *filename, const char *mode)
  \param n
    number of row
  \param m
    number of column
  \param *mat
    matrix reference
  \param *stream
    valid stream pointer where output is written
		\return
		-1 if an error occurred, 0 instead
  */
int mat_write(size_t n, size_t m, const double *mat, FILE* stream )
{
	if ( stream == NULL ) return -1 ;
	fprintf( stream, "# matrix size : %lu x %lu\n", m, n);
   	for( size_t i=0 ; i < n ; i++ ){
			for( size_t j=0 ; j < m ; j++ ) {
				fprintf(stream, "%#-8.4lf\n", mat[i*m + j]) ;
			}
    }
		return 0 ;
}
/**
 \brief Prints a nrow-by-nrow matrix to the screen
 \fn int mat_print(size_t n, size_t m, const double *mat)
 \param n number of row
 \param m number of column
 \param *mat matrix reference
*/
int mat_print(size_t n, size_t m, const double *mat)
{
	size_t i,j;
	if(mat == NULL){
		fputs("You provided a null pointer", stderr);
		return -1;
	}
	fflush(stdout);
	const double (*mm)[n][m] = (const double (*)[n][m])mat;
	puts("[");
	for(i=0; i < n; i++)
	{
		for( j=0 ; j< m ; j++ )
			printf("%-8.4lf|\t", (*mm)[i][j] );//mat[i*m + j]);
		putchar('\n') ;
	}
	puts("]");
	return 0;
}

/**
 \brief Checks if the matrix in is symmetric
 \fn IsSym(int nrow, int ncol, double *in)
 \param nrow, ncol number of row and column
 \return  0 if success, otherwise -1
*/
_Bool IsSym( size_t n, const double *in )
{
  size_t i, j=0;
  if( in==NULL )
  {
    fputs("\033[31mError : NULL pointer\033[0m", stderr);
    return false;
  }
  bool not_sym = false;
  for( i=0 ; i < n; i++ ){
	#pragma omp parallel shared (not_sym)
    {
		#pragma omp for nowait
		for ( j=n-1 ; j>i; j-- )
		{
			if( fabs( (double)fsub(in[j*n + i] , in[ i*n + j ]) ) > TOL )
				#pragma omp critical
				not_sym |= true ; //not symmetric
		}
	}
	if(not_sym)
		break;
  }
  return ! not_sym ;
}
void mat_sum(size_t n, const double *a, const double *b, double *out)
{
  /**
  \brief Sums two square matrices a and b and write the output into out matrix.
  \fn void mat_sum( size_t n, double *a, double *b, double *out )
  \param n matrix dimension
	\param a matrix
	\param b matrix
	\param out output matrix
  */
  size_t i, j;
  for( i=0; i< n; i++)
  {  
  	{
		for(j=0; j<n; j++) 
			{out[ i*n + j ] = a[ i*n + j ] + b[ i*n + j] ;}
	}
  }
  return ;
}
/**
* \fn Transpose(int nrow, int ncol, double *A, double *out)
* \brief Calculates the transpose of a nrow-by-ncol matrix A and put it in the matrix ncol-by-nrow
*
* A and out must be declared agree with the the dimension
*/
inline void transpose(size_t nrow, size_t ncol, const double *A, double *out)
{
	unsigned long j=0;
	int nthread = omp_get_max_threads()-1, sched_chunk = ncol / nthread;
	if(! sched_chunk) sched_chunk = 25;
	//fprintf(stderr, "sched_num : %i\n", sched_chunk);
	ldiv_t Q = {0,0};
	#pragma omp parallel for schedule(dynamic, sched_chunk) private(Q)
	for(j=0 ; j<nrow*ncol; j++)
	{
		Q = ldiv(j, ncol);
		#pragma omp atomic read
		*(out + Q.rem*nrow + Q.quot) = *(A + Q.quot*ncol + Q.rem);
	}
	if(DEBUG & (ncol < 3))
	{
		printf("A :\n");
		mat_print(nrow, ncol, A);
		printf("A Transpose :\n");
		mat_print(nrow, ncol, out);
	}
	return;
}

/**
* \fn void Cross(int a_nrow, int b_nrow, int a_ncol,int b_ncol, const double * restrict A, const double * restrict B, double * restrict C)
* \brief Calculates the cross product of the A by B  qA*B=C and put the result into C.
*
* Their dimension must agree.
* c(i,j)=sum( a(i,k)*b(k,j)), 1<k<a_nrow and 	a_ncol == b_nrow
* C is a_nrow-by-b_ncol matrix
*/
inline int Cross(
	size_t a_nrow,
	size_t b_nrow,
	size_t a_ncol,
	size_t b_ncol,
	const double *A,
	const double *B,
	double *C)
	{
		if( a_ncol != b_nrow ) {
			fprintf(stderr,"Error in matrix product : dimensions do not match.\n");
			return -1 ;
		}
		unsigned long i = 0, k = 0;
		int nthread = omp_get_max_threads(), sched_chunk = a_ncol / nthread;
		if(! sched_chunk) sched_chunk = 25;
		//double (*cc)[b_ncol] = (double (*)[b_ncol])C;
		/*
		bool cc_set = false ;
		if( a_nrow * b_ncol <= 4096*4096){
			cc = (double (*)[b_ncol])array ;
		}
		else
		{ 
			cc = (double (*)[b_ncol])malloc(a_nrow*b_ncol*sizeof(double));
			cc_set=true;
		};
		*/
		double cc[a_nrow][b_ncol] ;
		memset(cc, 0, a_nrow*b_ncol*sizeof(double));
		for( k = 0; k < a_ncol; k++){
			#pragma omp parallel for schedule(dynamic, sched_chunk)
			for (i = 0 ; i < a_nrow * b_ncol; i++){
				ldiv_t Q = ldiv(i, b_ncol);
				/*if(DEBUG)
					fprintf(stderr, "k = %lu ; i = %lu : Q.quot : %li; Q.rem : %li\n", k, i, Q.quot, Q.rem);*/
				#pragma omp critical
				cc[Q.quot][Q.rem] += *(A + Q.quot*a_ncol + k) * (*(B + k*b_ncol + Q.rem));
				cc[Q.quot][Q.rem] = (int)(cc[Q.quot][Q.rem]*1E5 + 0.5);
				cc[Q.quot][Q.rem] /= 1E5;
			}
		}
		#pragma omp parallel for schedule(dynamic, sched_chunk)
		for (i = 0 ; i < a_nrow * b_ncol; i++){
			ldiv_t Q = ldiv(i, b_ncol);
			#pragma omp critical
			*(C + Q.quot*b_ncol + Q.rem ) = cc[Q.quot][Q.rem] ;
		}
		if(DEBUG & (a_nrow <= 10))
			mat_print(a_nrow, b_ncol, (const double *)cc);
		/*
		if(cc_set)
			free(cc);
		*/
		return 1;
	}
/**
	\brief Checks if the matrix in is orthogonal
	\fn int IsOrtho(size_t n, const double *in)
	\return 1 if true, 0 instead
*/
int IsOrtho(size_t n, const double *in)
{
  double m1[n][n], mout[n][n];
  memset(mout, 0, n*n*sizeof(double));
  transpose(n, n, in, &m1[0][0]);
  Cross(n,n,n,n, &m1[0][0], in, &mout[0][0]);
  double sum = .0;
  unsigned int i = 0;
  do
  {
    sum += mout[i][i];
	i++;
  }
  while( i < n );
  fprintf(stderr, "IsOrth returns : %lf\n", sum);
  return ( sum == (double)n || (isless(fabs(sum - n), TOL))) ? 1 : 0 ;
}
/**
\brief Returns the square Identy matrix of dimension dim
\param dim : the dimension of the matrix
\return Return a pointer to a dim-by-dim matrix M[dim][dim] dynamically allocated.
The pointer type is : (double *)[dim]
*/
double (*mat_eye(const size_t dim) )[]
{
	double (*M)[dim]=NULL;
	M=(double (*)[dim]) calloc(dim*dim, sizeof(double));
	#pragma omp parallel for
	for(size_t i=0 ; i < dim ; i++)
	{
		M[i][i]=1.0;
	}
   return M;
}
/**
\brief Sets to zero all the elements of n-by-m matrix mat
\return 1 if succeed, 0 instead.
*/
int mat_zeros(size_t n, size_t m, double *mat)
{
	if(mat==NULL) return -1;
	if((n<1)||(m<1)) return 0;
	memset(mat, 0, n*m*sizeof(double));
  return 1;
}
//----
/**
	\function welcom_msg(const char *msg)
	\brief Display msg content as a banner
	\param msg character string to display

	Displays the content of the msg character string into a banner.
	Serves as welcome message
*/
void welcom_msg(const char *msg){
	FILE *fidFLAG;
	unsigned int FLAG=0;;
	if ( (fidFLAG=fopen(".flag","r+")) == NULL ){
		fidFLAG=fopen(".flag", "w+");
		size_t msgLen = strlen(msg), i ;
		printf("\033[33m") ;
		for ( i=0 ; i < msgLen ; i++ ) printf("*") ;
		printf("\n%s\n", msg) ;
		for ( i=0 ; i < msgLen ; i++ ) printf("*") ;
		printf("\033[0m\n") ;		FLAG++;
		fprintf(fidFLAG,"%u", FLAG);
		fclose(fidFLAG);
	}
	else {
		if( fscanf(fidFLAG,"%u", &FLAG) == EOF )
			return ;
		FLAG++;
		fprintf(fidFLAG,"%u", FLAG);
		fclose(fidFLAG);
	}
}
