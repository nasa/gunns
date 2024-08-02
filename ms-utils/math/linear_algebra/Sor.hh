#ifndef _SOR_
#define _SOR_

/**
@defgroup  TSM_UTILITIES_MATH_LINEAR_ALGEBRA_SOR Successive Over-Relaxation
@ingroup   TSM_UTILITIES_MATH_LINEAR_ALGEBRA

@details
PURPOSE:
- (An alternate solver routine)

REQUIREMENTS:
- (TBD)

REFERENCE:
- (Numerical Analysis by Burden and Faires)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((math/linear_algebra/Sor.o))

PROGRAMMERS:
- ((Mark Denson) (L-3 Communications) (5/2013) (test))

@{
*/

#include <cfloat>

class Sor
{
    public:
        /// @brief Although the class is never instantiated, Trick 10 requires a public destructor.
        virtual ~Sor();

        /// @brief  Determines if the matrix is positive definite.
        static bool isPositiveDefinite(      double* A,
                                       const int     n);

        /// @brief  Solves [A]{x} = {B} for {x} iteratively.
        static int solve(      double* x,
                               double* A,
                         const double* B,
                         const int     n,
                         const float   Wt,
                         const int     maxitr,
                         const double  convg);

    private:
        static const double condition; /**< (--) Minimum matrix diagonal for positive definite check */
        /// @brief  Default constructor unavailable since declared private and not implemented.
        Sor();
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        Sor(const Sor& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        Sor& operator =(const Sor& that);
};

/// @}

#endif
