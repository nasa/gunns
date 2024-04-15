#ifndef LambertW_EXISTS
#define LambertW_EXISTS

/**
@file      LambertW.hh
@brief     Lamber W Function declarations

@defgroup  TSM_UTILITIES_MATH_ELEM_FUNCTIONS_LAMBERTW    Lambert W Function
@ingroup   TSM_UTILITIES_MATH_ELEM_FUNCTIONS

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Implements calculations to solve the Lambert W, or Product Logarithm function.)

REFERENCE:
- ((Iacono, R.; Boyd, J. "New approximations to the principal real-valued branch of the Lambert W-function", Advances in Computational Mathematics, Dec. 2017.))

ASSUMPTIONS AND LIMITATIONS:
- ((We don't accept inputs > 1e300 for the principal branch.)
   (we don't accept inputs > -1e-300 for the non-principal branch.)
   (Accuracy is limited and may not converge for inputs near -1/e.))

LIBRARY_DEPENDENCY:
- ((math/approximation/LambertW.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2024-04) (Initial))

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Lambert W (Product Logarithm) solving.
///
/// @details  Implements a calculation of the real branches of the Lambert W function.  There are
///           separate functions for the principal branch, solveW0, and the non-principal branch,
///           solveW1.  Each branch has a valid range of input values, and exceeding this range
///           causes this computation to throw an out of bounds exception.
///
///           This uses the recursive algorithm from Iacono & Boyd (see reference), which for most
///           input ranges converges to zero error within a few iterations.  The user can also
///           provide a non-zero convergence tolerance to reduce the number of iterations and speed
///           up the result.  This implements logic to handle special ranges where the iterative
///           method doesn't perform well.  The worst-case accuracy and number of iterations occurs
///           for inputs near -1/e.
////////////////////////////////////////////////////////////////////////////////////////////////////
class LambertW
{
    public:
        /// @brief  Returns the Lambert W function principal branch solution for the given input.
        static double solveW0(const double input, const double convergence = 0.0);
        /// @brief  Returns the Lambert W function non-principal branch solution for the given input.
        static double solveW1(const double input, const double convergence = 0.0);

    private:
        static const double mEulerNum;            /**< ** (--) Euler's number, e. */
        static const double mEulerNumInv;         /**< ** (--) Inverse of Euler's number. */
        static const double mMaxInputLim0;        /**< ** (--) Maximum input limit for the principal branch. */
        static const double mMaxInputLim1;        /**< ** (--) Maximum input limit for the non-principal branch. */
        static const double mMinRangeBoundary;    /**< ** (--) Lower limit towards -1/e below which we skip solving. */
        static const double mW1InitDiscriminator; /**< ** (--) Input range discriminator for initial values for the non-principal branch. */
        static const unsigned int mIterLimit = 10;
        /// @brief  Checks the given input value for exceeding the minimum range.
        static void checkMinRange(const double input);
        /// @brief  Improves the given initial value to convergence on the Lambert W function.
        static double improve(const double input, const double initial, const double convergence = 0.0);
        /// @brief  Default constructor unavailable since declared private and not implemented.
        LambertW();
        /// @brief  Default destructor unavailable since declared private and not implemented.
        virtual ~LambertW();
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        LambertW(const LambertW&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        LambertW& operator =(const LambertW&);
};

/// @}

#endif
