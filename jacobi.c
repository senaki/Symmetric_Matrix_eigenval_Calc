#include "inclusions.h"
/**
\brief FINDS THE EIGENVALUES OF SYMMETRIC MATRICES
m BY THE JACOBI'S ROTATION in a n-dimensional vectorial space
* See matrix.c and inclusions.h files for descriptions of the functions used in this code.
* \fn jacobi(double *M, sizet_ n, double *p)
* \param n dimension of the square matrix
* \param p Matrix reference
*/
size_t jacobi(size_t n, const double *M, double *sp)
{
  size_t idx[2]={0,1}, i, j, Niteration=0 ;
  /*
  idx receives the indexes of the maximal off-diagonal elementn is the size of
  the square matrix we deal with
  idx[1] <=> line
  idx[2] <=> column
  */
  double theta=.0, max_mat, (*mR)[n]=mat_eye(n), mTr[n][n], vP[n][n], mat[n][n];
  for( i=0 ; i < n; i++ )
  {
    mat[i][i] = M[i*n+i] ;
    for( j=i+1 ; j < n ; j++ )
    {
      mat[i][j] = mat[j][i] = M[i*n + j] ;
    }
  }
  /*
  mR is the matrix of rotation
  mTr its transposition and
  vP that which will receive the n eigenvalues
  */
  do
  {
    // Re-initialize rotation matrix
    mR[idx[0]][idx[0]]=1;
    mR[idx[1]][idx[1]]=1;
    mR[idx[0]][idx[1]]=0;
    mR[idx[1]][idx[0]]=0;
    for(i=0 ; i < n ; i++)
    {
      for(j=i+1 ; j <n ; j++)
      {
        if ( fabs(mat[idx[0]][idx[1]]) < fabs(mat[i][j]) )
        {
          idx[0]=i;
          idx[1]=j;
        }
      }
    }
    max_mat=fabs(mat[idx[0]][idx[1]]);
    // Determinaton of rotation angle theta
    theta = 2.*mat[idx[0]][idx[1]] ;
    theta /= (mat[idx[1]][idx[1]] - mat[idx[0]][idx[0]]) ;
    theta = 0.5*atan( theta ) ;
    //----------------------------------------------------
    mR[idx[0]][idx[0]] = cos(theta) ;
    mR[idx[1]][idx[1]] = cos(theta);
    mR[idx[0]][idx[1]] = sin(theta) ;
    mR[idx[1]][idx[0]] = -1*sin(theta) ;
    // Transpose mR -> mTr
    transpose(n, n, &mR[0][0], &mTr[0][0]);
    // Left Multiply mTr by mat -> vP
    Cross(n, n, n, n, &mTr[0][0], &mat[0][0], &vP[0][0] );
    // Right Multply vP by mR -> mat
    Cross(n, n, n, n, &vP[0][0], &mR[0][0], &mat[0][0] );
    Niteration++ ;
  } while( max_mat > TOL);
  //--END JACOBI
  for(i=0; i < n; i++){
    sp[i]= mat[i][i] ;
  }
  free(mR);
  return Niteration ;
}
