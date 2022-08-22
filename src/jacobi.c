#include "inclusions.h"
#include <omp.h>
#include <time.h>
time_t j_start = 0, j_end = 0;
double duration = 0.0;
/**
\brief Finds the eigenvalues of symmetric matrix M by Jacobi's method.
See matrix.c and inclusions.h files for descriptions of the functions used in this code.
\fn jacobi(size_t n, double (*M)[n])
\param n dimension of the square matrix
\param M symmetric matrix
\return Niteration the number of iterations
*/

size_t jacobi(size_t n, double M[][n])
{
  int nthread = omp_get_max_threads()-1, sched_chunk = n / nthread;
  if(! sched_chunk) sched_chunk = 25;
  size_t idx[2]={0,1}, i, j, Niteration=0 ;
  double val = 0., vals = 0.0, vald = 0.;
  int xpn = 0, xpns, xpnd;
  /*
  idx receives the indexes of the maximal off-diagonal elementn is the size of
  the square matrix we deal with
  idx[1] <=> line
  idx[2] <=> column
  */
  double theta=.0, tsub =0., max_mat, (*mR)[n]=(double (*)[n])malloc(n*sizeof(double[n])),
  (*mTr)[n]=(double (*)[n])malloc(n*sizeof(double[n])),
  (*vP)[n]=(double (*)[n])malloc(n*sizeof(double[n]));
  memset(mR, 0, n*n*sizeof(double));
  memset(mTr, 0, n*n*sizeof(double));
  memset(vP, 0, n*n*sizeof(double));
  #pragma omp parallel
  { 
  #pragma omp for schedule(dynamic, sched_chunk) 
  for( i=0 ; i < n; i++ )
  {
    #pragma omp atomic write
    mR[i][i]=1.0;
  }
  }
  /*
  mR is the matrix of rotation
  mTr its transposition and
  vP that which will receive the n eigenvalues
  */
  j_start = time(NULL);
  do
  {
    Niteration++ ;
    if( DEBUG )
      fprintf(stderr, "Iteration : %lu\n", Niteration);
    
    size_t k = 0;
    max_mat=fabs(M[idx[0]][idx[1]]);
    for( i=0 ; i < n ; i++ )
    {
      k = i+1;
      #pragma omp parallel shared(max_mat, idx)
      {
      #pragma omp for schedule(dynamic,sched_chunk)
      for( j = k ; j < n ; j++ )
      {
        if ( max_mat < fabs( M[i][j] ) )
        {
          #pragma omp atomic write
          max_mat = fabs(M[i][j]);
          #pragma omp atomic read
          idx[0]=i;
          #pragma omp atomic read
          idx[1]=j;
        }
      }
    }
    }
    //theta = atan( 2.*(max_mat/((M[idx[1]][idx[1]] - M[idx[0]][idx[0]]))) )*.5
    //fprintf(stderr, "%lu,%lu : %lf\n", idx[0], idx[1], fabs( M[idx[0] ][ idx[1] ]));
    max_mat = fabs( M[idx[0] ][ idx[1] ]);
    // max_mat = val * pow(2, xpnt)
    val = frexp(max_mat, &xpn);
    //theta = 2*max_max*max_mat
    xpn++ ; // := 2*pow(2, xpnt)

    //theta = 2.*M[idx[0]][idx[1]] ; // := xpnt + 1;

    // Determinaton of rotation angle theta
    tsub = fsub(M[idx[1]][idx[1]], M[idx[0]][idx[0]]);
    vals = frexp(tsub, &xpns);
    //theta /=(M[idx[1]][idx[1]] - M[idx[0]][idx[0]]) ;
    // theta  = fdiv(max_mat, tsub);
    xpn -= xpns;
    vald = fdiv(val, vals);
    val = frexp(vald, &xpnd);
    xpn += xpnd;
    //theta = dmull(0.5L, atanl( (long double)theta )) ;
    theta = ldexp(val, xpn);
    theta = atan( theta ) ;
    theta *= .5;
    //fprintf(stderr, "%lf ; %lf \n", theta, atan( 2.*(max_mat/((M[idx[1]][idx[1]] - M[idx[0]][idx[0]]))) )*.5);    
    
    //---------------------------------
    mR[idx[0]][idx[0]] = cos(theta);
    mR[idx[1]][idx[1]] = cos(theta);
    mR[idx[0]][idx[1]] = sin(theta) ;
    mR[idx[1]][idx[0]] = -1.*sin(theta) ;
    /*
    1-Transpose mR -> mTr
    2-Left Multiply mTr by M -> vP
    3-Right Multply vP by mR -> M
    */
    transpose(n, n, &mR[0][0], &mTr[0][0]);
    Cross(n, n, n, n, &mTr[0][0], &M[0][0], &vP[0][0] );
    Cross(n, n, n, n, &vP[0][0], &mR[0][0], &M[0][0]);
    //mat_print(n,n, (const double *)M);
    /*
     Re-initialize rotation matrix
    */
    mR[ idx[0] ][ idx[0] ] = 1.0;
    mR[ idx[1] ][ idx[1] ] = 1.0;
    mR[ idx[0] ][ idx[1] ] = 0.0;
    mR[ idx[1] ][ idx[0] ] = 0.0;
    j_end = time(NULL);
    duration =  difftime(j_end, j_start) ;
    if( duration > 600.)
    {
        fprintf(stderr, "Convergence : too long (N, Abs. max, duration) = (%lu, %#-8.4lf, %.2lf s)\n", Niteration, max_mat, duration);
        break;
    }
    if (Niteration > 5e4) {
        fprintf(stderr, "Too much iterations : (N, Abs. max, duration) = (%lu, %#-8.4lf, %.2lf s)\n", Niteration, max_mat, duration);
      break;
    }
  } while( isgreater(max_mat, TOL) );
  free(mR);  free(mTr);  free(vP);
  return Niteration ;
}
