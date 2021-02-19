#ifndef LaguerreMethod_EXISTS
#define LaguerreMethod_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_ROOT_FINDING_LAGUERRE_METHOD Laguerre's Polynomial Root-Finding Method
@ingroup   TSM_UTILITIES_MATH_ROOT_FINDING

@details
PURPOSE:
- (Implements Laguerre's method for finding zeroes of polynomial functions.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((LaguerreMethod.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-08) (Initial Delivery))

@{
*/

#include <complex>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Laguerre's polynomial root-finding method.
///
/// @details  Implements Laguerre's method for finding zeroes of polynomial functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class LaguerreMethod
{
    public:
        /// @brief  Uses Laguerre's method to improve a complex root of a given polynomial with
        ///         complex coefficients.
        static int solve(std::complex<double>& x, std::complex<double>* coeffs, int size);

    private:
        /// @brief  Default constructor unavailable since declared private and not implemented.
        LaguerreMethod();

        /// @brief  Default destructor unavailable since declared private and not implemented.
        virtual ~LaguerreMethod();

        /// @brief  Copy constructor unavailable since declared private and not implemented.
        LaguerreMethod(const LaguerreMethod&);

        /// @brief  Assignment operator unavailable since declared private and not implemented.
        LaguerreMethod& operator =(const LaguerreMethod&);
};

/// @}

#endif
