#ifndef choleski_ldu_EXISTS
#define choleski_ldu_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_LINEAR_ALGEBRA_CHOLESKI Cholesky LDU Decomposition (C)
@ingroup   TSM_UTILITIES_MATH_LINEAR_ALGEBRA

@details
PURPOSE:
    (These algorithms comprise the Choleski LDU Decomposition direct method for solving systems of
     linear algebraic equations.  This is a freeware package available from
     http://mymathlib.webtrellis.net.  We modified the return statements for the methods that return
     int to return the failed row numbers, which is useful for debugging.
    )

REFERENCE:
    ()

ASSUMPTIONS AND LIMITATIONS:
    (NOTE - These routines have been replaced by the CholeskyLdu class, please use that class
            instead.)

PROGRAMMERS:
    ((Jason Harvey) (L-3 Communications) (May 2011) (Initial release))

@{
*/

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Decomposes the admittance matrix [A].  Returns -1 on success, or the row number causing
///        method failure.
int  Choleski_LDU_Decomposition(double *A, int n);

/// @brief Uses the decomposed admittance matrix to solve [A]{x} = {b} for {x}.  Returns -1 on
///        success, or the row number causing method failure.
int  Choleski_LDU_Solve(double *LDU, double B[], double x[], int n);

/// @brief Uses the decomposed admittance matrix to find the inverse of [A].  Always returns zero.
int  Choleski_LDU_Inverse(double *LDU, int n);

/// @brief Solves [L]{x} = {b} where [L] is a n x n unit lower triangular matrix.
void Unit_Lower_Triangular_Solve(double *L, double B[], double x[], int n);

/// @brief Calculates the inverse of the unit lower triangular matrix L.
void Unit_Lower_Triangular_Inverse(double *L, int n);

/// @brief Solves [U]{x} = {b} where [U] is a n x n unit upper triangular matrix.
void Unit_Upper_Triangular_Solve(double *U, double B[], double x[], int n);

/// @brief Calculates the inverse of the unit upper triangular matrix L.
void Unit_Upper_Triangular_Inverse(double *U, int n);

#ifdef __cplusplus
}
#endif

/// @}

#endif
