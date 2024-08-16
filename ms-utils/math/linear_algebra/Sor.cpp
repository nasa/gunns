/**************************** TRICK HEADER *******************
PURPOSE:
    This routine implements a Successive OverRelaxation method to
    solve linear systems  A]{x} = {B}.  SOR is an interative
    method.  It may be advantageous to use if matrix A is
    sparse but of no particular pattern.  If a particular pattern
    is known, other  methods may converge quicker.  If the system
    is too large then a Choleski method may be better.

REQUIREMENTS:
- (TBD)

REFERENCE:
- (Numerical Analysis by Burden and Faires)

ASSUMPTIONS AND LIMITATIONS:
  (matrices are allocated as a pointers to doubles.  So A is
  pointer to an array of doubles.  )

LIBRARY DEPENDENCY:
  ()

PROGRAMMERS:
  ((Mark Denson) (L-3 Communications) (5/2013) (for GUNNS study))

*/

#include "Sor.hh"
#include <cmath>
//#include <stdio.h>

/// @details  This particular value of DBL_EPSILON * DBL_EPSILON is chosen because Gunns
///           conditions its matrix with a minimum value of DBL_EPSILON * 1.0E-15.
const double Sor::condition = DBL_EPSILON * DBL_EPSILON;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     A      (--) The admittance matrix [A] of [A]{x}={B} for this call.  If A cannot
///                            be referred to as a doubly indexed matrix then if you have a declared
///                            matrix, dm[n][n], set A according to:
///                                double *A;
///                                A = (double **)malloc( (unsigned) n*sizeof(double));
///                                for (i=0; i< n; i++) {A[i] = dm[i];}
/// @param[in]     n      (--) The number of rows in the system, and the size of [A] array is n*n.
///
/// @returns  bool (--)  True if the matrix is positive-definite, false otherwise.
///
/// @details  Compares each diagonal of the matrix to be above a pre-defined condition tolerance.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Sor::isPositiveDefinite(      double* A,
                             const int     n)
{
    //TODO this is not actually the correct definition of positive definite.  For instance
    //     A = {1.0, -1.0, -1.0, 1.0} is not positive-definite but would pass this test.
    //     The proper definition is z(T)Az > 0 for all real vectors z.
    //the solve method really doesn't care about positive-definite, but we do need to avoid divide
    //by zero.  This version does protect for that.
    for (int i=0; i<n; ++i, A += n) {
        if (*(A + i) < condition) {
            return (false);
        }
    }
    return (true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] x      (--) Both the initial starting point for this solution (in) and the
///                            solution (out) {x} of [A]{x}={B} for this call.
/// @param[in]     A      (--) The admittance matrix [A] of [A]{x}={B} for this call.  If A cannot
///                            be referred to as a doubly indexed matrix then if you have a declared
///                            matrix, dm[n][n], set A according to:
///                                double *A;
///                                A = (double **)malloc( (unsigned) n*sizeof(double));
///                                for (i=0; i< n; i++) {A[i] = dm[i];}
/// @param[in]     B      (--) The resultant vector {B} in [A]{x}={B}.
/// @param[in]     n      (--) The number of rows in the system, and the size of the {x} and {B}
///                            arrays, and [A] is size n*n.
/// @param[in]     Wt     (--) Omega term (0 < Wt <= 1) to accelerate convergence, if Wt = 1 then
///                            this method is equivalent to the Gauss-Seidel method.
/// @param[in]     maxitr (--) The limit on the number of SOR iterations this call may perform.
/// @param[in]     convg  (--) The delta in {x} between iterations below which the solution is
///                            considered converged.
///
/// @returns       int    (--) The number of iterations this solution took if it converged, or -1 if
///                            it did not converge.
///
/// @details  Uses the SOR method to solve the system [A]{x} = {B} for {x}, given an initial
///           estimate for {x} from the caller.  The caller can specify
///
/// @note     If this method fails to converge, the array x{} will be returned with the results of
///           the last, unconverged SOR iteration.  The caller must restore a previous valid
///           solution or take other steps as appropriate.
///
/// @note     The arrays x{}, A[] and B{} must be created outside this routine.
////////////////////////////////////////////////////////////////////////////////////////////////////

int Sor::solve(      double* x,
                     double* A,
               const double* B,
               const int     n,
               const float   Wt,
               const int     maxitr,
               const double  convg)
{
    const double Wt_dbl = static_cast<double>(Wt);
    int k=1;

//    printf("----------------- in SOR --------------\n");

    /* k is the number of iterations, pop out if max iterations is exceeded */
    while (k < maxitr) {         /* compute trial solutions  */

//        printf("---- Iteration %d ----\n", k+1);

        /* init p_i again, in general it will be a pointer to A[i,0]  */
        double* p_i = A;
        double esum = 0.0;
        for (int i=0; i<n; i++, p_i += n) {    /* set pointer to beginning of matrix row */
            double lterm = 0.0;
            double hterm = 0.0;

            /* this part consists of solns already computed */
            if (i > 0) {
                for(int j=0; j<i; ++j) {
                    lterm += *(p_i + j) * x[j];  /* point to jth element in row i but only first i-1 elements*/
                }
            }
//            printf("for %d term, lterm = %e\n", i, lterm);

            /* this part consists of solns computed last iteration */
            if (i < n-1) {
                for(int j=i+1; j<n; ++j) {
                    hterm += *(p_i + j) * x[j];  /* point to jth element in row i but only last i+1 elements */
                }
            }
//            printf("for %d term, hterm = %e\n", i, hterm);

            /* now compute the new trial solution term */
            double eterm = (1.0-Wt_dbl)*(x[i]) + (Wt_dbl/(*(p_i + i)))*(-lterm - hterm + B[i]);  /* extra parens to stop parsing error */
            esum += std::fabs(eterm - x[i]);
            x[i] = eterm;
//            printf("x[%d] = %e\n", i, x[i]);
        }

        /* to see error iteration by iteration */
//        printf("error = %e\n", esum);

        /* test if finished or revise the trial solution and continue */
        if (esum < convg*n) {
//            printf("-------- finished SOR, error = %f--------------\n", esum);
            return (k);
        }

        k++;  /* another iteration */
    }
//    printf("------------- too many iterations in SOR --------------\n");
    return (-1);
}
