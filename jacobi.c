#include "inclusions.h"
/**
\brief FINDS THE EIGENVALUES OF SYMMETRIC MATRICES
m BY THE JACOBI'S ROTATION in a n-dimensional vectorial space
* See matrix.c and inclusions.h files for descriptions of the functions used in this code.
* \fn jacobi(double *M, unsigned int n, double *p)
* \param n {dimension of the square matrix}
* \param p {Matrix reference}
*/
unsigned int jacobi(const double *M, unsigned int n, double *sp)
{
  unsigned int idx[2]={0,1}, i, j, Niteration=0 ;
  //count=0;
  /*
  * idx receives the indexes of the maximal off-diagonal elementn is the size of
  the square matrix we deal with
  idx[1] <=> line
  idx[2] <=> column
  */
  double max_mat ;//threshold
  /*matrixsquare_read(file_name,&n, m);//if we need to read from a file:\
  Make a macro function for that purpose so it be possible to choose from either
  passing a matrix variable or reading from a file stream.*/
  double mR[n][n], mTr[n][n], vP[n][n], mat[n][n];
  //(double *)malloc(n*n*sizeof(double));
  for(i=0; i< n; i++)
  {
    for(j=0; j < n; j++)
    {
      mat[i][j]= M[i*n + j];
    }
  }
  /*mR is the matrix of rotation
  mTr its transposition and
  vP that which will receive the n eigenvalues
  */
  double theta ;
//INITIAL MAX OFF-DIAGONAL ELEMENT CHOICE
/*
  idx[0]=0;
  idx[1]=1;
*/
  do
  {
    //BEGIN SEARCHING FOR THE MAX OUTSIDE THE DIAGONAL ELEMENT INDEXES IN ROW i
    for(i=0 ; i< n ; i++)
    {
      for(j=i+1 ; j<n ; j++)
      {
        if ( fabs(mat[idx[0]][idx[1]]) < fabs(mat[i][j]) )
        {
          idx[0]=i;
          idx[1]=j;
        }
      }
    }
    //END SEACHING FOR THE LARGEST OFF-DIAGONAL ELEMENT
    max_mat=mat[idx[0]][idx[1]];
    //BEGIN SETTING ROTATION ANGLE
    theta = 2.*mat[idx[0]][idx[1]] ;
    theta /= (mat[idx[1]][idx[1]] - mat[idx[0]][idx[0]]) ;
    theta = 0.5*atan( theta ) ;
    //END SETTING ROTATION ANGLE
    //BEGIN CREATION OF THE ROTATION MATRIX
    if(n > 2)
    {//FIRST
      for( i=0 ; i<n ; i++)
      {
        for( j=0 ; j<n ; j++)
        {
          mR[i][j] = (i==j) ? 1.0 : 0. ;
        }
      }
    }
    //SECONDLY
    mR[idx[0]][idx[1]] = sin(theta) ;
    mR[idx[1]][idx[0]] = -1*sin(theta) ;
    mR[idx[1]][idx[1]] = cos(theta);
    mR[idx[0]][idx[0]] = cos(theta) ;
    //BEGIN ROTATION DE JACOBI
    transpose(n, n, &mR[0][0], &mTr[0][0]);//R^{1}->mTr
    Cross(n,n,n,n, &mTr[0][0], &mat[0][0], &vP[0][0] );//mTr*b->vP
    Cross(n, n, n, n, &vP[0][0], &mR[0][0], &mat[0][0]);//vP*mR-> b
    Niteration++ ;
  } while(fabs(max_mat) > TOL);
  //--END JACOBI
  for(i=0; i< n; i++){
    *(sp+i)= mat[i][i] ;
  }
  //Créer plus tard une structure pour cette sortie
  //printf("Tolerance\tIteration\t|Precision|\tmax(Mat)\n%9.3g\t%9.3d\t%9.3g\t%8.3g\n", TOL,Niteration,pcsn, max_mat);
  free(mat);
  return Niteration ;
}
