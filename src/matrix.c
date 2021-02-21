#include "inclusions.h"
int mat_write(size_t n, size_t m, const double *mat, FILE* stream )
{
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
	if ( stream == NULL ) return -1 ;
	fprintf( stream, "# matrix size : %lu x %lu\n", n, m);
   	for( size_t i=0 ; i < n ; i++ ){
			for( size_t j=0 ; j < m ; j++ ) {
				fprintf(stream, "%.4lf\t", mat[i*m + j]) ;
			}
			fputc('\n', stream ) ;
    }
		return 0 ;
}
int mat_print(size_t n, size_t m, const double *mat)
{
	/**
	\brief Prints a nrow-by-nrow matrix to the screen
	\fn int mat_print(size_t n, size_t m, const double *mat)
	\param n number of row
	\param m number of column
	\param *mat matrix reference
	*/
	size_t i,j;
	if(mat == NULL){
		fputs("You provided a null pointer", stderr);
		return -1;
	}
	puts("[");
	for(i=0; i < n; i++)
	{
		for( j=0 ; j< m ; j++ )  printf("%.4lf\t", mat[i*m + j]);
		putchar('\n') ;
	}
	puts("]");
	return 0;
}
int IsSym( size_t n, const double *in )
{
  /**
  \brief Checks if the matrix in is symmetric
  \fn IsSym(int nrow, int ncol, double *in)
  \param nrow, ncol number of row and column
  \return  0 if success, otherwise -1
  */
  size_t i, j=0;
  if( in==NULL )
  {
    fputs("\033[31mThe Matrix is not a square or is a NULL pointer\033[0m", stderr);
    return -1;
  }
  for( i=0 ; i < n; i++ ){
    for ( j=n-1 ; j>i; j-- )
    {
      if( in[j*n + i] != in[ i*n + j ] ) return 0 ; //not symmetric
    }
  }
  return 1 ;
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
    for(j=0; j<n; j++) out[ i*n + j ] = a[ i*n + j ] + b[ i*n + j] ;
  }
}
void transpose(size_t nrow, size_t ncol, const double *A, double *out)
{
  /**
  \fn Transpose(int nrow, int ncol, double *A, double *out)
  \brief Calculates the transpose of a nrow-by-ncol matrix A and put it in the matrix ncol-by-nrow
  A and out must be declared agree with the the dimension
  */
  for(size_t i=0; i<nrow; i++)
  {
    for(size_t j=0; j<ncol; j++) *(out + j*nrow +i)=*(A +i*ncol +j);
  }
}
inline int Cross(
	size_t a_nrow,
	size_t b_nrow,
	size_t a_ncol,
	size_t b_ncol,
	const double *A,
	const double *B,
	double *C)
	{
		/**
		\fn void Cross(int a_nrow, int b_nrow, int a_ncol,int b_ncol, const double * restrict A, const double * restrict B, double * restrict C)
		\brief Calculates the cross product of the A by B  A*B=C and put the result into C.
		Their dimension must agree.
		c(i,j)=sum( a(i,k)*b(k,j)), 1<k<a_nrow and 	a_ncol == b_nrow
		C is a_nrow-by-b_ncol matrix
		*/
		if( a_ncol != b_nrow ) {
			fprintf(stderr,"Error in matrix product : dimensions do not match.\n");
			return -1 ;
		}
		for( size_t i=0 ; i < a_nrow ; i++)
		{
			for( size_t j=0 ; j < b_ncol; j++)
			{
				*(C + i*b_ncol + j )=0.0;
				for( size_t k=0 ; k < a_ncol ; k++)
				*(C + i*b_ncol + j ) += *( A + i*a_ncol + k )* (*(B+k*b_ncol + j));
			}
		}
		return 1;
	}
int IsOrtho(size_t n, const double *in)
{
  /**
		\brief Checks if the matrix in is orthogonal
		\fn int IsOrtho(size_t n, const double *in)
		\return 1 if true, 0 instead
  */
  double m1[n][n], mout[n][n];
  transpose(n, n, in, &m1[0][0]);
  Cross(n,n,n,n, &m1[0][0], in, &mout[0][0]);
  size_t i=0;
  do
  {
    ++i;
  }
  while( i < n );
  return (i == n) ? 1 : 0 ;
}
double (*mat_eye(const size_t dim))[]
{
  /**
	\brief Returns the square Identy matrix of dimension dim
  \param dim : the dimension of the matrix
	\return Return a pointer to a dim-by-dim matrix M[dim][dim] dynamically allocated.
	The pointer type is : (double *)[dim]
  */
	static double (*M)[dim]=NULL;
	M=(double (*)[dim]) calloc(dim*dim, sizeof(double));
  for(size_t i=0 ; i < dim ; i++)
   {
     M[i][i]=1.0;
   }
   return M;
}
int mat_zeros(size_t n, size_t m, double *mat)
{
  /**
	\brief Sets to zero all the elements of n-by-m matrix mat
	\return 1 if succeed, 0 instead.
  */
	if(mat==NULL) return -1;
	if((n<1)||(m<1)) return 0;
	memset(mat, 0, n*m*sizeof(double));
  return 1;
}
