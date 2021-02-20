#include "inclusions.h"
/**
\brief Finds the eigenvalues of symmetric matrix M by Jacobi's method.
See matrix.c and inclusions.h files for descriptions of the functions used in this code.
\fn jacobi(size_t n, double (*M)[n])
\param n dimension of the square matrix
\param M symmetric matrix
\return Niteration the number of iterations
*/
size_t jacobi(size_t n, double (*M)[n])
{
  size_t idx[2]={0,1}, i, j, Niteration=0 ;
  /*
  idx receives the indexes of the maximal off-diagonal elementn is the size of
  the square matrix we deal with
  idx[1] <=> line
  idx[2] <=> column
  */
  double theta=.0, max_mat,
  (*mR)[n]=(double (*)[n]) calloc(n, sizeof(double[n])),
  (*mTr)[n]=(double (*)[n]) calloc(n, sizeof(double[n])),
  (*vP)[n]=(double (*)[n]) calloc(n, sizeof(double[n]));
  for( i=0 ; i < n; i++ )
  {
    mR[i][i]=1.0;
    for( j=i+1 ; j < n ; j++ )
    {
      mR[i][j] = mR[j][i] = 0.;
    }
  }
  /*
  mR is the matrix of rotation
  mTr its transposition and
  vP that which will receive the n eigenvalues
  */
  do
  {
    for( i=0 ; i < n ; i++ )
    {
      for( j=i+1 ; j < n ; j++ )
      {
        if ( fabs( M[idx[0]][idx[1]] ) < fabs( M[i][j] ) )
        {
          idx[0]=i;
          idx[1]=j;
        }
      }
    }
    max_mat=fabs( M[idx[0] ][ idx[1] ]);
    // Determinaton of rotation angle theta
    theta = 2.*M[idx[0]][idx[1]] ;
    theta /= ( M[idx[1]][idx[1]] - M[idx[0]][idx[0]] ) ;
    theta = 0.5*atan( theta ) ;
    //---------------------------------
    mR[idx[0]][idx[0]] = cos(theta) ;
    mR[idx[1]][idx[1]] = cos(theta);
    mR[idx[0]][idx[1]] = sin(theta) ;
    mR[idx[1]][idx[0]] = -1*sin(theta) ;
    /*
    Transpose mR -> mTr
    */
    transpose(n, n, &mR[0][0], &mTr[0][0]);
    /*
     Left Multiply mTr by M -> vP
   */
    Cross(n, n, n, n, &mTr[0][0], &M[0][0], &vP[0][0] );
    /*
    Right Multply vP by mR -> M
    */
    Cross(n, n, n, n, &vP[0][0], &mR[0][0], &M[0][0]);
    Niteration++ ;
    /*
     Re-initialize rotation matrix
    */
    mR[ idx[0] ][ idx[0] ] = 1;
    mR[ idx[1] ][ idx[1] ] = 1;
    mR[ idx[0] ][ idx[1] ] = 0;
    mR[ idx[1] ][ idx[0] ] = 0;
    if (Niteration > 1e4 ) {
      fprintf(stderr, "Warninng : too much iteration. The method is not adapted to your need !\n");
      break;
    }
  } while( max_mat > TOL);
  free(mR), free(mTr), free(vP);
  return Niteration ;
}
