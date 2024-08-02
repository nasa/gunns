/***************************************************************************************************
PURPOSE:
    (These algorithms comprise the Choleski LDU Decomposition direct method for solving systems of
     linear algebraic equations.  This is a freeware package available from
     http://mymathlib.webtrellis.net.  We modified the return statements for the methods that return
     int to return the failed row numbers, which is useful for debugging.

     Since this is C code and not C++, we're exempting this freeware code from using {} brackets in
     all if & for blocks.
    )

REFERENCE:
    ()

ASSUMPTIONS AND LIMITATIONS:
    ()

PROGRAMMERS:
    ((Jason Harvey) (L-3 Communications) (June 2011) (Initial release))
***************************************************************************************************/

#include "choleski_ldu.h"

////////////////////////////////////////////////////////////////////////////////
// File: choleski_ldu.c                                                       //
// Contents:                                                                  //
//    Choleski_LDU_Decomposition                                              //
//    Choleski_LDU_Solve                                                      //
//    Choleski_LDU_Inverse                                                    //
//                                                                            //
// Required Externally Defined Routines:                                      //
//    Unit_Lower_Triangular_Solve                                             //
//    Unit_Lower_Triangular_Inverse                                           //
//    Unit_Upper_Triangular_Solve                                             //
////////////////////////////////////////////////////////////////////////////////

//                    Required Externally Defined Routines
//void Unit_Lower_Triangular_Solve(double *L, double B[], double x[], int n);
//void Unit_Lower_Triangular_Inverse(double *L, int n);
//void Unit_Upper_Triangular_Solve(double *U, double B[], double x[], int n);

////////////////////////////////////////////////////////////////////////////////
//  int Choleski_LDU_Decomposition(double *A, int n)                          //
//                                                                            //
//  Description:                                                              //
//     This routine uses Choleski's method to decompose the n x n positive    //
//     definite symmetric matrix A into the product of a unit lower triangular//
//     matrix L, a diagonal matrix D, and a unit upper triangular matrix U    //
//     equal to the transpose of L.  A unit triangular matrix is a triangular //
//     matrix with ones along the diagonal.                                   //
//     The matrices L, D, and U replace the matrix A so that the original     //
//     matrix A is destroyed.  The matrix L replaces the lower triangular     //
//     part of A, U replaces the upper triangular part of A, and D replaces   //
//     the diagonal of A.                                                     //
//                                                                            //
//     Choleski's LDU decomposition is performed by evaluating, in order, the //
//     following of expressions for i = 0, ... ,n-1 :                         //
//       L[i][k]*D[k] = (A[i][k] - (L[i][0]*D[0]*L[k][0] + ... +              //
//                                            L[i][k-1]*D[k-1]*L[k][k-1]) )   //
//       D[i] = A[i][i] - ( L[i][0]*D[0]*L[i][0] + ... +                      //
//                                          L[k][k-1]*D[k-1]*L[i][k-1] ) )    //
//     and subsequently setting                                               //
//       U[k][i] = L[i][k], for k = 0, ... , i-1.                             //
//                                                                            //
//     After performing the LDU decomposition for A, call Choleski_LDU_Solve  //
//     to solve the equation Ax = B or call Choleski_LDU_Inverse to calculate //
//     the inverse of the matrix A.                                           //
//                                                                            //
//  Arguments:                                                                //
//     double *A   On input, the pointer to the first element of the matrix   //
//                 A[n][n].  On output, the matrix A is replaced by the lower //
//                 triangular, diagonal, and  upper triangular matrices of the//
//                 Choleski LDL' factorization of A.                          //
//     int     n   The number of rows and/or columns of the matrix A.         //
//                                                                            //
//  Return Values:                                                            //
//     0  Success                                                             //
//    -1  Failure - The matrix A is not positive definite symmetric (within   //
//                  working accuracy).                                        //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double A[N][N];                                                        //
//                                                                            //
//     (your code to initialize the matrix A)                                 //
//     err = Choleski_LDU_Decomposition((double *) A, N);                     //
//     if (err < 0) printf(" Matrix A is singular\n");                        //
//     else { printf(" The LDLt decomposition of A is \n");                   //
//           ...                                                              //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
int Choleski_LDU_Decomposition(double *A, int n)
{
   int i, j, k;
   double *p_i;                   // pointer to L[i][0]
   double *p_j;                   // pointer to L[j][0]
   double *p_k;                   // pointer to L[k][0]
   double ld;                     // dummy storage

   for (i = 1, p_i = A + n; i < n; p_i += n, i++) {

//            Calculate elements given by the product L[i][j]*D[j].

      for (j = 0, p_j = A; j < i; j++, p_j += n) {
         for (k = 0; k < j; k++)
            *(p_i + j) -= *(p_i + k) * *(p_j + k);

         /// - Modified to protect for underflow.
         if (*(p_i + j) > -1.0E-100) *(p_i + j) = 0.0;
      }

//            Calculate the diagonal element D[i] and L[i][j].
//            Store the transpose L[k][i];

      for (k = 0, p_k = A; k < i; p_k += n, k++) {

         ld = *(p_i + k) / *(p_k + k);
         *(p_i + i) -= *(p_i + k) * ld;
         *(p_i + k) = ld;
         *(p_k + i) = ld;
      }
      /// - Modified to return the failing row number to help debug.
      if ( *(p_i + i) <= 0.0 ) return i;
//      if ( *(p_i + i) <= 0.0 ) return -1;
   }
   /// - Modified to support returning failed row numbers (>=0).
   return -1;
//   return 0;
}


////////////////////////////////////////////////////////////////////////////////
//  int Choleski_LDU_Solve(double *LDU, double *B, double *x,  int n)         //
//                                                                            //
//  Description:                                                              //
//     This routine uses Choleski's method to solve the linear equation       //
//     Ax = B.  This routine is called after the matrix A has been decomposed //
//     into the product of a unit lower triangular matrix L, a diagonal matrix//
//     D, and a unit upper triangular matrix U which is the transpose of L.   //
//     The matrix A is the product LDU.                                       //
//     The solution proceeds by solving the linear equation Ly = B for y,     //
//     then solving Dz = y for z and finally solving Ux = z for x.            //
//                                                                            //
//  Arguments:                                                                //
//     double *LDU Pointer to the first element of the matrix whose elements  //
//                 form the unit lower triangular, diagonal, and unit upper   //
//                 triangular matrix factors of A.                            //
//     double *B   Pointer to the column vector, (n x 1) matrix, B            //
//     double *x   Solution to the equation Ax = B.                           //
//     int     n   The number of rows or columns of the matrix LU.            //
//                                                                            //
//  Return Values:                                                            //
//     0  Success                                                             //
//    -1  Failure - The matrix D is singular.                                 //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double A[N][N], B[N], x[N];                                            //
//                                                                            //
//     (your code to create matrix A and column vector B)                     //
//     err = Choleski_LDU_Decomposition(&A[0][0], N);                         //
//     if (err < 0) printf(" Matrix A is singular\n");                        //
//     else {                                                                 //
//        err = Choleski_LDU_Solve(&A[0][0], B, x, n);                        //
//        if (err < 0) printf(" Matrix A is singular\n");                     //
//        else printf(" The solution is \n");                                 //
//           ...                                                              //
//     }                                                                      //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
int Choleski_LDU_Solve(double *LDU, double B[], double x[], int n)
{
   int k;
   double *p_k;

//         Solve the linear equation Ly = B for y, where L is a unit lower
//         triangular matrix.

   Unit_Lower_Triangular_Solve(LDU, B, x, n);

//         Solve the linear equation Dz = y for z, where D is the diagonal
//         matrix.

   for (k = 0, p_k = LDU; k < n; k++, p_k += n) {
       /// - Modified to return the failing row number to help debug.
      if ( *(p_k + k) == 0.0 ) return k;
//      if ( *(p_k + k) == 0.0 ) return -1;
      x[k] /= *(p_k + k);
   }

//         Solve the linear equation Ux = z, where z is the solution
//         obtained above of Ly = B and Dz = y.
//         U is a unit upper triangular matrix.

   Unit_Upper_Triangular_Solve(LDU, x, x, n);

   /// - Modified to support returning failed row numbers (>=0).
   return -1;
//   return 0;
}


////////////////////////////////////////////////////////////////////////////////
//  int Choleski_LDU_Inverse(double *LDU,  int n)                             //
//                                                                            //
//  Description:                                                              //
//     This routine uses Choleski's method to find the inverse of the matrix  //
//     A.  This routine is called after the matrix A has been decomposed      //
//     into a product of a unit lower triangular matrix L, a diagonal matrix  //
//     D and a unit upper triangular matrix U which is the transpose of L.    //
//     The matrix A is the product of L, D, and U.  Upon completion, the      //
//     inverse of A is stored in LDU so that the matrix LDU is destroyed.     //
//                                                                            //
//  Arguments:                                                                //
//     double *LDU Pointer to the first element of the matrix whose elements  //
//                 form the unit lower triangular matrix, the diagonal matrix,//
//                 and the unit upper triangular matrix factors of A.         //
//     int     n   The number of rows or columns of the matrix LDU.           //
//                                                                            //
//  Return Values:                                                            //
//     0  Success                                                             //
//    -1  Failure - The matrix A is singular.                                 //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double A[N][N], B[N], x[N];                                            //
//                                                                            //
//     (your code to create matrix A and column vector B)                     //
//     err = Choleski_LDU_Decomposition(&A[0][0], N);                         //
//     if (err < 0) printf(" Matrix A is singular\n");                        //
//     else {                                                                 //
//        err = Choleski_LDU_Inverse(&A[0][0], n);                            //
//        if (err < 0) printf(" Matrix A is singular\n");                     //
//        else printf(" The inverse is \n");                                  //
//           ...                                                              //
//     }                                                                      //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
int Choleski_LDU_Inverse(double *LDU, int n)
{
   int i, j, k;
   double *p_i, *p_j, *p_k;

//         Invert the unit lower triangular matrix L.

   Unit_Lower_Triangular_Inverse(LDU, n);

//     Premultiply L inverse by the transpose of L inverse and D inverse.

   for (j = 0, p_j = LDU; j < n; j++, p_j += n) {
      for (i = j, p_i = p_j; i < n; p_i += n, i++) {
         if (j == i) *(p_i + j) = 1.0 / *(p_i + i);
         else *(p_i + j) /= *(p_i + i);
         for (k = i + 1, p_k = p_i + n; k < n; k++, p_k += n)
            *(p_i + j) += *(p_k + i) * *(p_k + j) / *(p_k + k);
         *(p_j + i) = *(p_i + j);
      }
   }

   return 0;
}


////////////////////////////////////////////////////////////////////////////////
// File: unit_lower_triangular.c                                              //
// Routines:                                                                  //
//    Unit_Lower_Triangular_Solve                                             //
//    Unit_Lower_Triangular_Inverse                                           //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  void Unit_Lower_Triangular_Solve(double *L, double *B, double x[], int n) //
//                                                                            //
//  Description:                                                              //
//     This routine solves the linear equation Lx = B, where L is an n x n    //
//     unit lower triangular matrix.  (Only the subdiagonal part of the matrix//
//     is addressed.)  The diagonal is assumed to consist of 1's and is not   //
//     addressed.                                                             //
//     The algorithm follows:                                                 //
//                          x[0] = B[0], and                                  //
//            x[i] = B[i] - (L[i][0] * x[0]  + ... + L[i][i-1] * x[i-1]),     //
//     for i = 1, ..., n-1.                                                   //
//                                                                            //
//  Arguments:                                                                //
//     double *L   Pointer to the first element of the unit lower triangular  //
//                 matrix.                                                    //
//     double *B   Pointer to the column vector, (n x 1) matrix, B.           //
//     double *x   Pointer to the column vector, (n x 1) matrix, x.           //
//     int     n   The number of rows or columns of the matrix L.             //
//                                                                            //
//  Return Values:                                                            //
//     void                                                                   //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double A[N][N], B[N], x[N];                                            //
//                                                                            //
//     (your code to create matrix A and column vector B)                     //
//     Unit_Lower_Triangular_Solve(&A[0][0], B, x, n);                        //
//     printf(" The solution is \n");                                         //
//           ...                                                              //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
void Unit_Lower_Triangular_Solve(double *L, double B[], double x[], int n)
{
   int i, k;

//         Solve the linear equation Lx = B for x, where L is a unit lower
//         triangular matrix.

   x[0] = B[0];
   for (k = 1, L += n; k < n; L += n, k++)
      for (i = 0, x[k] = B[k]; i < k; i++) x[k] -= x[i] * *(L + i);
}


////////////////////////////////////////////////////////////////////////////////
//  void Unit_Lower_Triangular_Inverse(double *L,  int n)                     //
//                                                                            //
//  Description:                                                              //
//     This routine calculates the inverse of the unit lower triangular       //
//     matrix L.  Only the subdiagonal part of the matrix is addressed.       //
//     The diagonal is assumed to consist of 1's and is not addressed.        //
//     The algorithm follows:                                                 //
//        Let M be the inverse of L, then L M = I,                            //
//          M[i][j] = -( L[i][j] M[j][j] + ... + L[i][i-1] M[i-1][j] ),       //
//     for i = 1, ..., n-1, j = 0, ..., i - 1.                                //
//                                                                            //
//  Arguments:                                                                //
//     double *L   On input, the pointer to the first element of the matrix   //
//                 whose unit lower triangular elements form the matrix which //
//                 is to be inverted. On output, the lower triangular part is //
//                 replaced by the inverse.  The diagonal and superdiagonal   //
//                 elements are not modified.                                 //
//     int     n   The number of rows and/or columns of the matrix L.         //
//                                                                            //
//  Return Values:                                                            //
//     void                                                                   //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double L[N][N];                                                        //
//                                                                            //
//     (your code to create the matrix L)                                     //
//     Unit_Lower_Triangular_Inverse(&L[0][0], N);                            //
//     printf(" The inverse is \n");                                          //
//           ...                                                              //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
void Unit_Lower_Triangular_Inverse(double *L, int n)
{
   int i, j, k;
   double *p_i, *p_j, *p_k;

//         Invert the subdiagonal part of the matrix L row by row where
//         the diagonal elements are assumed to be 1.0.

   for (i = 1, p_i = L + n; i < n; i++, p_i += n) {
      for (j = 0, p_j = L; j < i; p_j += n, j++) {
         *(p_i + j) = -*(p_i + j);
         for (k = j + 1, p_k = p_j + n; k < i; k++, p_k += n)
            *(p_i + j) -= *(p_i + k) * *(p_k + j);
      }
   }
}


////////////////////////////////////////////////////////////////////////////////
// File: unit_upper_triangular.c                                              //
// Routines:                                                                  //
//    Unit_Upper_Triangular_Solve                                             //
//    Unit_Upper_Triangular_Inverse                                           //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  int Unit_Upper_Triangular_Solve(double *U, double *B, double x[], int n)  //
//                                                                            //
//  Description:                                                              //
//     This routine solves the linear equation Ux = B, where U is an n x n    //
//     unit upper triangular matrix.  (Only the superdiagonal part of the     //
//     matrix is addressed.)  The diagonal is assumed to consist of 1's and   //
//     is not addressed.                                                      //
//     The algorithm follows:                                                 //
//                  x[n-1] = B[n-1], and                                      //
//       x[i] = B[i] - (U[i][i+1] * x[i+1]  + ... + U[i][n-1] * x[n-1]),      //
//     for i = n-2, ..., 0.                                                   //
//                                                                            //
//  Arguments:                                                                //
//     double *U   Pointer to the first element of the upper triangular       //
//                 matrix.                                                    //
//     double *B   Pointer to the column vector, (n x 1) matrix, B.           //
//     double *x   Pointer to the column vector, (n x 1) matrix, x.           //
//     int     n   The number of rows or columns of the matrix U.             //
//                                                                            //
//  Return Values:                                                            //
//     void                                                                   //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double A[N][N], B[N], x[N];                                            //
//                                                                            //
//     (your code to create matrix A and column vector B)                     //
//     Unit_Upper_Triangular_Solve(&A[0][0], B, x, n);                        //
//     printf(" The solution is \n");                                         //
//           ...                                                              //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
void Unit_Upper_Triangular_Solve(double *U, double B[], double x[], int n)
{
   int i, k;

//         Solve the linear equation Ux = B for x, where U is an upper
//         triangular matrix.
   x[n-1] = B[n-1];
   for (k = n-2, U += n * (n - 2); k >= 0; U -= n, k--) {
      x[k] = B[k];
      for (i = k + 1; i < n; i++) x[k] -= x[i] * *(U + i);
   }
}


////////////////////////////////////////////////////////////////////////////////
//  int Unit_Upper_Triangular_Inverse(double *U,  int n)                      //
//                                                                            //
//  Description:                                                              //
//     This routine calculates the inverse of the unit upper triangular matrix//
//     U.  The subdiagonal part of the matrix is not addressed.               //
//     The diagonal is assumed to consist of 1's and is not addressed.        //
//     The algorithm follows:                                                 //
//        Let M be the inverse of U, then U M = I,                            //
//          M[i][j] = -( U[i][i+1] M[i+1][j] + ... + U[i][j] M[j][j] ),       //
//     for i = n-2, ... , 0,  j = n-1, ..., i+1.                              //
//                                                                            //
//                                                                            //
//  Arguments:                                                                //
//     double *U   On input, the pointer to the first element of the matrix   //
//                 whose unit upper triangular elements form the matrix which //
//                 is to be inverted. On output, the upper triangular part is //
//                 replaced by the inverse.  The subdiagonal elements are     //
//                 not modified.                                              //
//     int     n   The number of rows and/or columns of the matrix U.         //
//                                                                            //
//  Return Values:                                                            //
//     void                                                                   //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double U[N][N];                                                        //
//                                                                            //
//     (your code to create the matrix U)                                     //
//     Unit_Upper_Triangular_Inverse(&U[0][0], N);                            //
//     printf(" The inverse is \n");                                          //
//           ...                                                              //
//     }                                                                      //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
void Unit_Upper_Triangular_Inverse(double *U, int n)
{
   int i, j, k;
   double *p_i, *p_j, *p_k;
   double sum;

//         Invert the superdiagonal part of the matrix U row by row where
//         the diagonal elements are assumed to be 1.0.

   for (i = n - 2, p_i = U + n * (n - 2); i >=0; p_i -= n, i-- ) {
      for (j = n - 1; j > i; j--) {
         *(p_i + j) = -*(p_i + j);
         for (k = i + 1, p_k = p_i + n; k < j; p_k += n, k++ )
            *(p_i + j) -= *(p_i + k) * *(p_k + j);
      }
   }
}
