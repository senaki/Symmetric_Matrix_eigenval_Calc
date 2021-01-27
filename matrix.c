#include "inclusions.h"
/**
Write an ascii square matrix
*/
///int mat_write(char *file_name, int n, double *a )
int mat_write(unsigned int n,unsigned int m, const double *a, const char *file_name, const char *mode )
{
  /**
  \brief Write a nxm matrix into a file \param filename
  \fn int mat_write(unsigned int n, unsigned int m, const double *mat, const char *filename, const char *mode)
  \param n
    number of row
  \param m
    number of column
  \param *mat
    matrix reference
  \param *mode
    string of characters defining output write mode (rwba ...)}
  */
 FILE *fich= fopen(file_name, mode);
  if (fich==NULL) return -1 ;
  else {
    fprintf(fich, "Dim : %02u\t%02u\n", n, m);
    for(unsigned int i=0 ; i < n ; i++)
    {
      for( unsigned int j=0 ; j < m ; j++) fprintf(fich, "%10.6lf\t", *(a+ i*n + j)) ;
      fprintf( fich, "\n") ;
    }
    fclose(fich) ;
    return 0 ;
  }
}

int mat_print(unsigned int n,unsigned int m, const double *in)
{
  /**
  \brief Prints a nrow-by-nrow matrix to the screen
  \fn int mat_print(unsigned int n, unsigned int m, const double *mat)
  \param n {number of row}
  \param m {number of column}
  \param *mat {matrix reference}
  */
  unsigned int i,j;
  if(in != NULL){
    printf("\033[37m[");
    for(i=0; i < n; i++)
    {
      printf("\n");
      for( j=0 ; j< m ; j++ )
      printf("%8.3lf\t", *(in+i*n +j));
    }
    printf("\n]\033[0m\n");
    return 0;
  }
  else return -1;
}

int IsSym( unsigned int nrow, unsigned int ncol, const double *in)
{
  /**
  \brief Checks if the matrix in is symmetric
  \fn IsSym(int nrow, int ncol, double *in)
  \param nrow, ncol {number of row and column}
  \return { 0 if success, otherwise -1}
  */
  unsigned int i, j=0;
  if( (nrow != ncol) || in==NULL )
  {
    printf("\033[31mThe Matrix is not a square or is a NULL pointer\033[0m\n");
    return -1;
  }
  for(i=0 ; i < nrow; i++){
    for (j=i+1 ; j < ncol; j++)
    {
      if( in[j*ncol +i] != in[i*ncol+j] ) return 0 ; //not symmetric
    }
  }
  return 1 ;
}

//--
//--
void mat_sum(unsigned int n, const double *a, const double *b, double *out)
{
  /**
  \brief Sum two square matrices a and b and write the output into out matrix.
  \fn void mat_sum(int n, double *a, double *b, double *out)
  \param n {matrix dimension}
  */
  unsigned int i, j;
  for( i=0; i< n; i++)
  {
    for(j=0; j<n; j++) out[i*n + j] = a[i*n + j]+b[i*n + j];
  }
}
//--
//--
void transpose(unsigned int nrow, unsigned int ncol, const double *A, double *out)
{
  /**
  \fn Transpose(int nrow, int ncol, double *A, double *out)
  \brief Calculates the transpose of a nrow-by-ncol matrix A and put it in the matrix ncol-by-nrow
  A and out must be declared agree with the the dimension
  */
  unsigned int i, j;
  for(i=0; i<nrow; i++)
  {
    for(j=0; j<ncol; j++) *(out+ j*nrow +i)=*(A +i*ncol +j);
  }
}
//--
//--
void Cross(unsigned int a_nrow,
  unsigned int b_nrow,
  unsigned int a_ncol,
  unsigned int b_ncol,
  const double * restrict A,
  const double * restrict B,
  double * restrict C)
{
  /**
  \fn void Cross(int a_nrow, int b_nrow, int a_ncol,int b_ncol,
  const double * restrict A, const double * restrict B, double * restrict C)
  \brief Calculate the cross product of the A by B and put the result into C.
  Their dimension must agree.
  */
  unsigned int i,j,k;
  if(b_nrow == a_ncol)
  {
    for(i=0; i< a_nrow; i++)
    {
      for(j=0; j< b_ncol; j++)
      {
        C[i*b_ncol+j]=A[i*a_ncol+0]*B[0+j];
        for(k=1; k < a_ncol; k++)
        C[i*b_ncol+j] += A[i*a_ncol+k]*B[k*b_ncol+j];
      }
    }
  }
}
//--
//--
int IsOrtho(unsigned int n, const double *in)
{
  /**\fn int IsOrtho(int nrow, int ncol, const double *in)
  * \brief Check if the matrix in is orthogonal
  */
  unsigned int i=0;
  double m1[n][n], mout[n][n];//m1 receive the transpose of in, mout receive the cross matrix product of m1 and in
  transpose(n, n, in, &m1[0][0]);
  Cross(n,n,n,n, &m1[0][0], in, &mout[0][0]);
  do
  {
    ++i;
  }
  while( fabs(ceil(mout[i][i]))==1 && (i < n) );
  return (i == n) ? 1 : 0 ;
  /*{
    puts("Not an orthogonal matrix");
    return -1;
  }
  else
  {
    puts("Orthogonal matrix");
    return 0;
  }*/
}
double **mat_eye(unsigned int dim)
{
  /** \brief Return the square Identy matrix of dimension dim
  \param dim : the dimension of the matrix
  */
  if (dim<2) return NULL;
  static double **M;
  M=malloc(dim*sizeof(double));
  for(unsigned int i=0 ; i < dim ; i++)
   {
     M[i]=(double *)calloc(dim, sizeof(double));
     M[i][i]=1.0;
   }
   return M;
}
