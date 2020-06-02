#ifndef CholeskyLdu_EXISTS
#define CholeskyLdu_EXISTS

/**
@file
@brief    Cholesky LDU Decomposition declarations

@defgroup  TSM_UTILITIES_MATH_LINEAR_ALGEBRA_CHOLESKY_LDU Cholesky LDU Decomposition
@ingroup   TSM_UTILITIES_MATH_LINEAR_ALGEBRA

@details
PURPOSE:
- (These algorithms comprise the Cholesky LDU Decomposition direct method for solving systems of
   linear algebraic equations.  These use algorithms translated to C++ from a freeware package
   available from http://www.mymathlib.com, file choleski_ldu.c.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((CholeskyLdu.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-03) (Initial Delivery))

@{
*/

#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Cholesky LDU Matrix Decomposition Utilities
///
/// @details  These algorithms comprise the Cholesky LDU Decomposition direct method for solving
///           systems of linear algebraic equations.  These use algorithms translated to C++ from a
///           freeware package available from http://www.mymathlib.com, file choleski_ldu.c.
////////////////////////////////////////////////////////////////////////////////////////////////////
class CholeskyLdu
{
    public:
        /// @brief Default constructor.
        CholeskyLdu();

        /// @brief Default destructor.
        virtual ~CholeskyLdu();

        /// @brief Decomposes the admittance matrix [A].
        virtual void Decompose(double *A, int n);

        /// @brief Decomposes only the rows of the admittance matrix [A] given by the vector
        ///        argument r.
        virtual void Decompose(double *A, int n, std::vector<int>& r);

        /// @brief Uses the decomposed admittance matrix to solve [A]{x} = {b} for {x}.
        virtual void Solve(double *LDU, double B[], double x[], int n);

        /// @brief Uses the decomposed admittance matrix to find the inverse of [A].
        virtual void Invert(double *LDU, int n);

        /// @brief Solves [L]{x} = {b} where [L] is a n x n unit lower triangular matrix.
        virtual void SolveUnitLowerTriangular(double *L, double B[], double x[], int n);

        /// @brief Calculates the inverse of the unit lower triangular matrix L.
        virtual void InvertUnitLowerTriangular(double *L, int n);

        /// @brief Solves [U]{x} = {b} where [U] is a n x n unit upper triangular matrix.
        virtual void SolveUnitUpperTriangular(double *U, double B[], double x[], int n);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        CholeskyLdu(const CholeskyLdu& that);

        /// @details  Assignment operator unavailable since declared private and not implemented.
        CholeskyLdu& operator =(const CholeskyLdu&);

};

/// @}

#endif
