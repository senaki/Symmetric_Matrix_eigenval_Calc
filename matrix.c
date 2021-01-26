#include "inclusions.h"
/**
Write an ascii square matrix
*/
///int mat_write(char *file_name, int n, double *a )
int mat_write(unsigned int n,unsigned int m, double *a, char *file_name, char *mode )
{
  /**
  \brief Write a nxm matrix into a file \param filename
  \fn int mat_write(unsigned int n, unsigned int m, double *mat, char *filename, char *mode)
  \param n {number of row}
  \param m {number of column}
  \param *mat {matrix reference}
  \param *mode {string of characters defining output write mode (rwba ...)}
  */
 FILE *fich= fopen(file_name, mode);
  if (fich==NULL){
    return -1;
  }
  else {
    fprintf(fich, "Dim : %02u\t%02u\n", n, m);
    for(unsigned int i=0; i< n; i++)
    {
      for(unsigned int j=0; j<m; j++)
      {
        fprintf(fich, "%.3lf\t", *(a+ i*n + j));
      }
      fprintf(fich, "\n");
    }
    fclose(fich);
    return 0;
  }
}
///int mat_print(int nrow, double *in)

int mat_print(unsigned int n,unsigned int m, double *in)
{
  /**
  \brief Prints a nrow-by-nrow matrix to the screen
  \fn int mat_print(unsigned int n, unsigned int m, double *mat)
  \param n {number of row}
  \param m {number of column}
  \param *mat {matrix reference}
  */
  unsigned int i,j;
  if(in != NULL){
    printf("\033[37m[");
    for(i=0; i< n; i++)
    {
      printf("\n");
      for( j=0;j< m; j++)
      printf("%8.3lf\t", *(in+i*n +j));
    }
    printf("\n]\033[0m\n");
    return 0;
  }
  else return -1;
}
//--
//--
///double *matrixsquare_read(char *path, int *n)
/**
Read a square matrix at path and put the dimension in n and put it in q
the format read is:
n
a11	... a1n
.
.
.
an1   ... ann
*/
/*
double *matrixsquare_read(char *path, unsigned int *n)
{
static double *mat ;
unsigned int i, j, dim;
FILE *flux= fopen( path, "r");
fscanf( flux, "%i", &dim);
*n=dim ;
printf("%i\n",dim);
mat = (double *) calloc(dim*dim, sizeof(double));
printf("MAT(%i,%i)\n",dim,dim );
for(i=0; i< dim; i++)
{
for(j=0; j<dim; j++)
{
fscanf(flux, "%lf\t",(mat + i*dim +j));
printf("%lf\t",*(mat + i*dim +j) );
}
printf("\n");
}
fclose(flux);
return mat;
}
*/
int IsSym( unsigned int nrow, unsigned int ncol, double *in)
{
  /**
  \brief Checks if the matrix in is symmetric
    \fn IsSym(int nrow, int ncol, double *in)
    \param nrow, ncol {number of row and column}
    \return { 0 if success, otherwise -1}
  */
  unsigned int i, j=0, yes_or_no;
  if( (nrow != ncol) || in==NULL )
  {
    printf("\033[31mThe Matrix is not a square or is a NULL pointer\033[0m\n");
    return -1;
  }
  else
  {
    for(i=0; i< nrow; i++)
    {
      for(j=0; j!=i && j< ncol; j++)
      {
        yes_or_no= (*(in+j*ncol +i) != *(in+i*ncol+j));
        if(yes_or_no != 0)
        {
          printf("Rem: Not a symmetric matrix\n");
          return -1;
        }
      }
    }
    return 0;
  }
}

//--
//--
void mat_sum(unsigned int n, double *a, double *b, double *out)
{
  /**
  \brief Sum two square matrices a and b and write the output into out matrix.
  \fn void mat_sum(int n, double *a, double *b, double *out)
  \param n {matrix dimension}
  */
  unsigned int i, j;
  for( i=0; i< n; i++)
  {
    for(j=0; j<n; j++) *(out + i*n + j) = (double) *(a + i*n + j)+*(b + i*n + j);
  }
}
//--
//--
void transpose(unsigned int nrow, unsigned int ncol, double *A, double *out)
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
void Cross(unsigned int a_nrow, unsigned int b_nrow, unsigned int a_ncol,unsigned int b_ncol, double *A, double *B, double *C)
{
  /**
  \fn void Cross(int a_nrow, int b_nrow, int a_ncol,int b_ncol, double *A, double *B, double *C)
  \brief Calculate the cross product of the A by B and put the result into C.
  Their dimension must agree.
  */
  unsigned int i,j,k;
  if(b_nrow != a_ncol)
  {
    printf("\nDimension error!\n");
  }
  else
  {
    for(i=0; i< a_nrow; i++)
    {
      for(j=0; j< b_ncol; j++)
      {
        *(C+ i*b_ncol+j)=0.;
        for(k=0; k< a_ncol; k++)
        *(C+i*b_ncol+j) += (double) *(A+i*a_ncol+k)*(*(B+ k*b_ncol+j));
      }
    }
  }
}
//--
//--
int IsOrtho(unsigned int n, double *in)
{
  /**\fn int IsOrtho(int nrow, int ncol, double *in)
  * \brief Check if the matrix in is orthogonal
  */
  unsigned int i;
  double m1[n][n], mout[n][n];//m1 receive the transpose of in, mout receive the cross matrix product of m1 and in
  transpose(n, n, in, &m1[0][0]);
  Cross(n,n,n,n, &m1[0][0], in, &mout[0][0]);
  //--
  //--
  i=0;
  do
  {
    ++i;
  }
  while( ceil(mout[i][i]) == 1 && i < n);
  if( (unsigned int) abs(i) != n)
  {
    printf("Not an orthogonal matrix");
    return -1;
  }
  else
  {
    printf("Orthogonal matrix");
    return 0;
  }
}
