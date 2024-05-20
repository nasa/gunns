/*
@file
@brief    Cholesky LDU Decomposition implementation

LIBRARY DEPENDENCY:
     ((software/exceptions/TsNumericalException.o))
*/

#include "CholeskyLdu.hh"
#include "software/exceptions/TsNumericalException.hh"
#include <sstream>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Cholesky LDU Decomposition.
////////////////////////////////////////////////////////////////////////////////////////////////////
CholeskyLdu::CholeskyLdu()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Cholesky LDU Decomposition.
////////////////////////////////////////////////////////////////////////////////////////////////////
CholeskyLdu::~CholeskyLdu()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] A (--) On input, the pointer to the first element of the matrix A[n][n].  On
///                       output, the matrix A is replaced by the lower triangular, diagonal, and
///                       upper triangular matrices of the Cholesky LDL' factorization of A.
/// @param[in]     n (--) The number of rows and/or columns of the matrix A.
///
/// @throws  TsNumericalException
///
/// @details  This routine uses Cholesky's method to decompose the n x n positive definite symmetric
///           matrix A into the product of a unit lower triangular matrix L, a diagonal matrix D,
///           and a unit upper triangular matrix U equal to the transpose of L.  A unit triangular
///           matrix is a triangular matrix with ones along the diagonal.
///
///           The matrices L, D, and U replace the matrix A so that the original matrix A is
///           destroyed.  The matrix L replaces the lower triangular part of A, U replaces the upper
///           triangular part of A, and D replaces the diagonal of A.
///
///           Cholesky's LDU decomposition is performed by evaluating, in order, the following
///           expressions for i = 0, ... ,n-1 :
///               L[i][k]*D[k] = (A[i][k] - (L[i][0]*D[0]*L[k][0] + ... +
///                                          L[i][k-1]*D[k-1]*L[k][k-1]) ),
///               D[i] = A[i][i] - ( L[i][0]*D[0]*L[i][0] + ... +
///                                          L[k][k-1]*D[k-1]*L[i][k-1] ) ),
///           and subsequently setting
///               U[k][i] = L[i][k], for k = 0, ... , i-1.
///
///           After performing the LDU decomposition for A, call Solve to solve the
///           equation Ax = B or call Inverse to calculate the inverse of the matrix A.
////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <cstdio>  //Todo remove - used for that underflow test below
void CholeskyLdu::Decompose(double *A, int n)
{
//    /// - TODO temp underflow test
//    ld = 1.0E-10;
//    for (i = 1; i<10; ++i) {
//        ld *= ld;
//        printf("\nld = %e", ld);
//    }
//    printf("          \n");

    double *p_i = A + n;                   // pointer to L[i][0]
    for (int i = 1; i < n; p_i += n, i++) {

        /// - Calculate elements given by the product L[i][j]*D[j].
        double *p_j = A;                   // pointer to L[j][0]
        for (int j = 0; j < i; j++, p_j += n) {
            const double limit = -1.0E-100;
            for (int k = 0; k < j; k++) {
                /// - Skip operations on zero to save time.
                if (limit > *(p_i + k) and limit > *(p_j + k)) {
                    *(p_i + j) -= *(p_i + k) * *(p_j + k);
                }
            }
            /// - Modified to protect for underflow.  TODO we had to add this because Trick can't
            ///   handle arithmetic underflows, and instead dies with a floating exception.
            ///   Remove this once Trick is fixed, as this wastes CPU time.
            if ( (*(p_i + j) > limit) and (*(p_i + j) < -limit) ) {
                *(p_i + j) = 0.0;
            }
        }

        /// - Calculate the diagonal element D[i] and L[i][j].  Store the transpose L[k][i];
        double *p_k = A;                   // pointer to L[k][0]
        for (int k = 0; k < i; p_k += n, k++) {
            double ld = *(p_i + k) / *(p_k + k);   // temp storage
            *(p_i + i) -= *(p_i + k) * ld;
            *(p_i + k) = ld;
            *(p_k + i) = ld;
        }

        /// - Return the failing row number to aid debugging.
        if ( *(p_i + i) <= 0.0 ) {
            std::ostringstream msg;
            msg << "failed at row " << i;
            throw(TsNumericalException("", "CholeskyLdu::Decompose", msg.str()));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] A (--) On input, the pointer to the first element of the matrix A[n][n].  On
///                       output, the matrix A is replaced by the lower triangular, diagonal, and
///                       upper triangular matrices of the Cholesky LDL' factorization of A.
/// @param[in]     n (--) The number of rows and/or columns of the matrix A.
/// @param[in]     r (--) Vector of row numbers to operate on.  This method only decomposes r rows,
///                       and leaves the other rows of A alone.
///
/// @throws  TsNumericalException
///
/// @details  This overloaded version of Decompose operates only on the row/columns given by the
///           vector r.  The intent was for the caller to break up their matrix into separate
///           partitions, or islands, that can be decomposed separately.  Rather than creating
///           separate matrices for each island, the caller could just pass in the total matrix with
///           just the row numbers to operate on for each island.  So far in practice however, this
///           is slower than if the caller builds separate matrices for each island and decomposes
///           them with the old Decompose method above.  Probably because of all the extra array
///           position counters we have to update to skip between the desired rows, which as you can
///           see below, is quite messy!
/// TODO this is a prototype.  It is tested in the unit test code and works, but could maybe be
///      optimized & cleaned up to run faster.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CholeskyLdu::Decompose(double *A, int n, std::vector<int>& r)
{
    const int rn = static_cast<int>(r.size());
    // create vectors that contain the increments between rows in r.  Tried actual std::vectors,
    // but they're slower to reference than arrays.  This method runs faster with arrays.
    int* vri = new int[rn];
    int* vrk = new int[rn];
    vrk[0] =   r[0];
    vri[0] = n*r[0];
    for (int i=1; i<rn; ++i) {
        vrk[i] = r[i] - r[i-1];
        vri[i] = n * vrk[i];
    }

    double *p_i = A + n*r[1] + r[0];                   // pointer to L[i][0]
    double *p_i_i = A + vri[0] + vrk[0];
    for (int i = 1, ki = vrk[1]; i < rn; i++) {

        /// - Calculate elements given by the product L[i][j]*D[j].
        double *p_j = A + n*vrk[0] + vrk[0];                   // pointer to L[j][0]
        double *p_i_j = p_i;
        for (int j = 0; j < i; j++, p_i_j += vrk[j], p_j += vri[j]) {
            const double limit = -1.0E-100;
            double *p_i_k = p_i;
            double *p_j_k = p_j;
            for (int k = 0; k < j; k++, p_i_k += vrk[k], p_j_k += vrk[k]) {
                /// - Skip operations on zero to save time.
                if (limit > *(p_i_k) and limit > *(p_j_k)) {
                    *(p_i_j) -= *(p_i_k) * *(p_j_k);
                }
            }
            /// - Modified to protect for underflow.  TODO we had to add this because Trick can't
            ///   handle arithmetic underflows, and instead dies with a floating exception.
            ///   Remove this once Trick is fixed, as this wastes CPU time.
            if ( (*(p_i_j) > limit) and (*(p_i_j) < -limit) ) {
                *(p_i_j) = 0.0;
            }
        }

        /// - Calculate the diagonal element D[i] and L[i][j].  Store the transpose L[k][i];
        double *p_k = A + vri[0] + vrk[0];                   // pointer to L[k][0]
        p_i_i += vri[i] + vrk[i];
        double *p_i_k = p_i;
        double *p_k_k = p_k;
        for (int k = 0; k < i; k++, p_k += vri[k], p_i_k += vrk[k], p_k_k += vri[k] + vrk[k] ) {
            double ld = *(p_i_k) / *(p_k_k);   // temp storage
            *(p_i_i)   -= *(p_i_k) * ld;
            *(p_i_k)    = ld;
            *(p_k + ki) = ld;
        }

        /// - Return the failing row number to aid debugging.
        if ( *(p_i_i) <= 0.0 ) {
            delete [] vrk;
            delete [] vri;
            std::ostringstream msg;
            msg << "failed at row " << i;
            throw(TsNumericalException("", "CholeskyLdu::Decompose", msg.str()));
        }

        if (i < rn-1) {    //temp kludge
            p_i += vri[i+1];
            ki  += vrk[i+1];
        }
    }
    delete [] vrk;
    delete [] vri;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] LDU (--) Pointer to the first element of the matrix whose elements form the unit
///                         lower triangular, diagonal, and unit upper triangular matrix factors of
///                         A.
/// @param[in]     B   (--) Pointer to the column vector, (n x 1) matrix, B.
/// @param[out]    x   (--) Solution to the equation Ax = B.
/// @param[in]     n   (--) The number of rows or columns of the matrix LU.
///
/// @throws  TsNumericalException
///
/// @details  This routine uses Cholesky's method to solve the linear equation Ax = B.  This routine
///           is called after the matrix A has been decomposed into the product of a unit lower
///           triangular matrix L, a diagonal matrix D, and a unit upper triangular matrix U which
///           is the transpose of L.
///
///           The matrix A is the product LDU.
///
///           The solution proceeds by solving the linear equation Ly = B for y, then solving Dz = y
///           for z and finally solving Ux = z for x.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CholeskyLdu::Solve(double *LDU, double B[], double x[], int n)
{
    int k;
    double *p_k;

    /// - Solve the linear equation Ly = B for y, where L is a unit lower triangular matrix.
    SolveUnitLowerTriangular(LDU, B, x, n);

    /// - Solve the linear equation Dz = y for z, where D is the diagonal matrix.
    for (k = 0, p_k = LDU; k < n; k++, p_k += n) {
        /// - Return the failing row number to aid debugging.
        if ( *(p_k + k) == 0.0 ) {
            std::ostringstream msg;
            msg << "failed at row " << k;
            throw(TsNumericalException("", "CholeskyLdu::Solve", msg.str()));
        }
        x[k] /= *(p_k + k);
    }

    /// - Solve the linear equation Ux = z, where z is the solution obtained above of Ly = B and
    ///   Dz = y.  U is a unit upper triangular matrix.
    SolveUnitUpperTriangular(LDU, x, x, n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] LDU (--) Pointer to the first element of the matrix whose elements form the unit
///                lower triangular matrix, the diagonal matrix, and the unit upper triangular
///                matrix factors of A.
/// @param[in]     n   (--) The number of rows or columns of the matrix LDU.
///
/// @details  This routine uses Cholesky's method to find the inverse of the matrix A.  This routine
///           is called after the matrix A has been decomposed into a product of a unit lower
///           triangular matrix L, a diagonal matrix D and a unit upper triangular matrix U which is
///           the transpose of L.  The matrix A is the product of L, D, and U.  Upon completion, the
///           inverse of A is stored in LDU so that the matrix LDU is destroyed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CholeskyLdu::Invert(double *LDU, int n)
{
    int i, j, k;
    double *p_i, *p_j, *p_k;

    /// - Invert the unit lower triangular matrix L.
    InvertUnitLowerTriangular(LDU, n);

    /// - Premultiply L inverse by the transpose of L inverse and D inverse.
    for (j = 0, p_j = LDU; j < n; j++, p_j += n) {
        for (i = j, p_i = p_j; i < n; p_i += n, i++) {
            if (j == i) {
                *(p_i + j) = 1.0 / *(p_i + i);
            }
            else {
                *(p_i + j) /= *(p_i + i);
            }
            for (k = i + 1, p_k = p_i + n; k < n; k++, p_k += n) {
                *(p_i + j) += *(p_k + i) * *(p_k + j) / *(p_k + k);
            }
            *(p_j + i) = *(p_i + j);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      L (--) Pointer to the first element of the unit lower triangular matrix.
/// @param[in]      B (--) Pointer to the column vector, (n x 1) matrix, B.
/// @param[in, out] x (--) Pointer to the column vector, (n x 1) matrix, x.
/// @param[in]      n (--) The number of rows or columns of the matrix L.
///
/// @returns void
///
/// @details  This routine solves the linear equation Lx = B, where L is an n x n unit lower
///           triangular matrix.  (Only the subdiagonal part of the matrix is addressed.)  The
///           diagonal is assumed to consist of 1's and is not addressed.
///
///           The algorithm follows:
///                                        x[0] = B[0], and
///                  x[i] = B[i] - (L[i][0] * x[0]  + ... + L[i][i-1] * x[i-1]),
///           for i = 1, ..., n-1.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CholeskyLdu::SolveUnitLowerTriangular(double *L, double B[], double x[], int n)
{
    int i, k;

    /// - Solve the linear equation Lx = B for x, where L is a unit lower triangular matrix.
    x[0] = B[0];
    for (k = 1, L += n; k < n; L += n, k++) {
        for (i = 0, x[k] = B[k]; i < k; i++) {
            x[k] -= x[i] * *(L + i);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] L (--) On input, the pointer to the first element of the matrix whose unit lower
///                       triangular elements form the matrix which is to be inverted. On output,
///                       the lower triangular part is replaced by the inverse.  The diagonal and
///                       superdiagonal elements are not modified.
/// @param[in]     n (--) The number of rows and/or columns of the matrix L.
///
/// @returns void
///
/// @details  This routine calculates the inverse of the unit lower triangular matrix L.  Only the
///           subdiagonal part of the matrix is addressed.  The diagonal is assumed to consist of
///           1's and is not addressed.
///
///           The algorithm follows:
///               Let M be the inverse of L, then L M = I,
///               M[i][j] = -( L[i][j] M[j][j] + ... + L[i][i-1] M[i-1][j] ),
///           for i = 1, ..., n-1, j = 0, ..., i - 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CholeskyLdu::InvertUnitLowerTriangular(double *L, int n)
{
    int i, j, k;
    double *p_i, *p_j, *p_k;

    /// - Invert the subdiagonal part of the matrix L row by row where the diagonal elements are
    ///   assumed to be 1.0.
    for (i = 1, p_i = L + n; i < n; i++, p_i += n) {
        for (j = 0, p_j = L; j < i; p_j += n, j++) {
            *(p_i + j) = -*(p_i + j);
            for (k = j + 1, p_k = p_j + n; k < i; k++, p_k += n) {
                *(p_i + j) -= *(p_i + k) * *(p_k + j);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     U (--) Pointer to the first element of the upper triangular matrix.                                                    //
/// @param[in]     B (--) Pointer to the column vector, (n x 1) matrix, B.
/// @param[in,out] x (--) Pointer to the column vector, (n x 1) matrix, x.
/// @param[in]     n (--) The number of rows or columns of the matrix U.
///
/// @returns void
///
/// @details  This routine solves the linear equation Ux = B, where U is an n x n unit upper
///           triangular matrix.  (Only the superdiagonal part of the matrix is addressed.)  The
///           diagonal is assumed to consist of 1's and is not addressed.
///
///           The algorithm follows:
///                      x[n-1] = B[n-1], and
///               x[i] = B[i] - (U[i][i+1] * x[i+1]  + ... + U[i][n-1] * x[n-1]),
///           for i = n-2, ..., 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CholeskyLdu::SolveUnitUpperTriangular(double *U, double B[], double x[], int n)
{
    int i, k;

    /// - Solve the linear equation Ux = B for x, where U is an upper triangular matrix.
    x[n-1] = B[n-1];
    for (k = n-2, U += n * (n - 2); k >= 0; U -= n, k--) {
        x[k] = B[k];
        for (i = k + 1; i < n; i++) {
            x[k] -= x[i] * *(U + i);
        }
    }
}
