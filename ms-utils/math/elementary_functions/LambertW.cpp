/**
@file      LambertW.cpp
@brief     Lamber W Function implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((software/exceptions/TsOutOfBoundsException.o))

*/

#include "LambertW.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include <cfloat>

/// @details Euler's number, e, as a constant to speed computation.
const double LambertW::mEulerNum = std::exp(1.0);

/// @details Inverse of Euler's number as a constant to speed computation.
const double LambertW::mEulerNumInv = 1.0 / std::exp(1.0);

/// @details Maximum input range for the principal branch chosen to avoid floating-point error from the log function.
const double LambertW::mMaxInputLim0 = 1.0e300;

/// @details Maximum input range for the non-principal branch chosen to avoid floating-point error from the log function.
const double LambertW::mMaxInputLim1 = -1.0e-300;

/// @details Lower limit towards -1/e below which we skip solving and just return -1.
const double LambertW::mMinRangeBoundary = -LambertW::mEulerNumInv + 1.0e-12;

/// @details Input range discriminator for initial values for the non-principal branch.  This
///          deviates from the reference and we chose this value for best performance.
const double LambertW::mW1InitDiscriminator = -0.008;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] input       (--) Given input to the W function.
/// @param[in] convergence (--) The minimum convergence delta between iterations at which to stop iterating.
///
/// @returns  double (--) The Lambert W function principal branch result for the given input.
///
/// @throws   TsOutOfBoundsException, for invalid input ranges.
///
/// @details  Checks for valid range of input, computes the initial value for the iteration method,
///           calls the improve function to iterate on the initial value until convergence, and
///           returns its result.
////////////////////////////////////////////////////////////////////////////////////////////////////
double LambertW::solveW0(const double input, const double convergence)
{
    /// - Check for exceeding the min or max ranges.
    if (input > mMaxInputLim0) {
        //  - For inputs > 1e300, throw exception because it can fail in the log function.
        throw TsOutOfBoundsException("Input range exceeded", "LambertW::solveW0",
                                     "Input exceeds the max limit for the principal branch.");
    }
    checkMinRange(input);

    double result = 0.0;
    if (input < mMinRangeBoundary) {
        /// - For the special input range near -1/e, return -1.
        result = -1.0;
    } else if (MsMath::isInRange(-DBL_EPSILON, input, DBL_EPSILON)) {
        /// - For the special input range near zero, return the input value.
        result = input;
    } else {
        /// - Compute initial value based on the given input.
        double initial = 0.0;
        if (input > mEulerNum) {
            const double logInput = std::log(input);
            initial = logInput - std::log(logInput);
        } else if (input > 0) {
            initial = input / mEulerNum;
        } else {
            const double eulerX = mEulerNum * input;
            const double sqrtEulerX = 1.0 + std::sqrt(1.0 + eulerX);
            initial = eulerX * std::log(sqrtEulerX) / (eulerX + sqrtEulerX);
        }

        /// - Call the improve function and return its result.
        result = improve(input, initial, convergence);
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] input       (--) Given input to the W function.
/// @param[in] convergence (--) The minimum convergence delta between iterations at which to stop iterating.
///
/// @returns  double (--) The Lambert W function non-principal branch result for the given input.
///
/// @throws   TsOutOfBoundsException, for invalid input ranges.
///
/// @details  Checks for valid range of input, computes the initial value for the iteration method,
///           calls the improve function to iterate on the initial value until convergence, and
///           returns its result.
////////////////////////////////////////////////////////////////////////////////////////////////////
double LambertW::solveW1(const double input, const double convergence)
{
    /// - Check for exceeding the min or max ranges.
    if (input > mMaxInputLim1) {
        //  - For inputs > -1e-300, throw exception because it can fail in the log function.
        throw TsOutOfBoundsException("Input range exceeded", "LambertW::solveW1",
                                     "Input exceeds the max limit for the non-principal branch.");
    }
    checkMinRange(input);

    double result = input;
    if (input < mMinRangeBoundary) {
        /// - For the special input range near -1/e, return -1.
        result = -1.0;
    } else {
        /// - Compute initial value based on the given input.
        double initial = 0.0;
        if (input > mW1InitDiscriminator) {
            const double logNegInput = std::log(-input);
            initial = logNegInput - std::log(-logNegInput);
        } else {
            initial = -1.0 - std::sqrt(2.0 + 2.0 * mEulerNum * input);
        }

        /// - Call the improve function and return its result.
        result = improve(input, initial, convergence);
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] input (--) Given input to the W function.
///
/// @throws   TsOutOfBoundsException, for invalid input ranges.
///
/// @details  Throws exception if the given input value is less then -1/e.
////////////////////////////////////////////////////////////////////////////////////////////////////
void LambertW::checkMinRange(const double input)
{
    if (input < -mEulerNumInv) {
        /// - Lambert W has no solution for inputs less than -1/e.
        throw TsOutOfBoundsException("Input range exceeded", "LambertW::checkMinRange",
                                     "Input exceeds the minimum limit.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     input       (--) Given input to the W function.
/// @param[in]     initial     (--) Initial value on the desired branch.
/// @param[in]     convergence (--) The minimum convergence delta between iterations at which to stop iterating.
///
/// @returns  double (--) The converged Lambert W function result.
///
/// @details  Iterates on the initial value until the desired convergence is reached or the max
///           iteration limit is reached.  This uses the quadratic-rate recursive formula of Iacono
///           and Boyd (see reference), which converges to the W function on either branch, provided
///           an initial value on the desired branch.  This assumes the caller avoids troublesome
///           input ranges: near zero for either branch, very large values for the principal branch,
///           and invalid range (< -1/e) for either branch.  Convergence is assured everywhere else,
///           except near -1/e.  For inputs near -1/e for both branches, the method doesn't converge
///           so once the max iterations has been reached, stop and return the last best value.
////////////////////////////////////////////////////////////////////////////////////////////////////
double LambertW::improve(const double input, const double initial, const double convergence)
{
    double result = initial;
    for (unsigned int i=0; i<mIterLimit; ++i) {
        const double previous = result;
        result = result * (1.0 + std::log(input / result)) / (1.0 + result);
        /// - Check convergence and break out early if converged.
        if (std::fabs(result - previous) <= convergence) {
            break;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] input (--) Given input to the W function.
///
/// @returns  double (--) The Lambert W function principal branch result for the given input.
///
/// @throws   TsOutOfBoundsException, for invalid input ranges.
///
/// @details  Checks for valid range of input, computes and returns approximate principal branch
///           solution for the given input.  Approximations are accurate to within 3% error or
///           better (see comments below for accuracy in each region).  For the input < -0.01, we
///           don't have an approximation and instead fall back on the exact solution from solveW0.
///
///           These approximations, when used, should use less compute than the solveW0 function.
///
/// @note  We have a lot of magic numbers and violate the coding standard of no magic numbers here,
///        because these are specific curve fits to a specific function, and defining constant class
///        attributes for these would be silly.  Risk is mitigated by the unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
double LambertW::fastSolveW0(const double input)
{
    if (input > mMaxInputLim0) {
        //  - For inputs > 1e300, throw exception because it can fail in the log function.
        throw TsOutOfBoundsException("Input range exceeded", "LambertW::fastSolveW0",
                                     "Input exceeds the max limit for the principal branch.");
    }
    checkMinRange(input);

    double result = 0.0;
    if (input <= -1.0e-2) {
        result = solveW0(input);
    } else if (input < 1.0e-2) {
        /// - Accuracy within 1% error:
        result = input;
    } else {
        const double lnx = std::log(input);
        if (input < 1.0e-1) {
            /// - This is from Eqn. 24 of Reference "Lambert W-function simplified expressions..."
            /// - Accuracy within 2% error:
            result = input - std::exp((4.123e-6 * lnx + 2.0) * lnx + 1.64e-4);
        } else if (input < 1.0e2) {
            /// - Accuracy within 0.5% error:
            result = (((-8.3436e-4 * lnx + 5.1352e-4) * lnx + 7.0871e-2) * lnx + 0.35642) * lnx + 0.56635;
        } else if (input <= 1.0e10) {
            /// - Accuracy within 0.3% error:
            result = ((-1.9947e-4 * lnx + 1.2102e-2) * lnx + 0.70037) * lnx - 8.5476e-2;
        } else {
            /// - Accuracy within 3% error:
            result = 0.996 * lnx - 3.47;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] input (--) Given input to the W function.
///
/// @returns  double (--) The Lambert W function non-principal branch result for the given input.
///
/// @throws   TsOutOfBoundsException, for invalid input ranges.
///
/// @details  Checks for valid range of input, computes and returns approximate principal branch
///           solution for the given input.  Approximations are accurate to within 1% error or
///           better (see comments below for accuracy in each region).  For the input > -1e-20, we
///           don't have an approximation and instead fall back on the exact solution from solveW1.
///
///           These approximations, when used, should use less compute than the solveW1 function.
///
/// @note  We have a lot of magic numbers and violate the coding standard of no magic numbers here,
///        because these are specific curve fits to a specific function, and defining constant class
///        attributes for these would be silly.  Risk is mitigated by the unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
double LambertW::fastSolveW1(const double input)
{
    /// - Check for exceeding the min or max ranges.
    if (input > mMaxInputLim1) {
        //  - For inputs > -1e-300, throw exception because it can fail in the log function.
        throw TsOutOfBoundsException("Input range exceeded", "LambertW::fastSolveW1",
                                     "Input exceeds the max limit for the non-principal branch.");
    }
    checkMinRange(input);

    double result = 0.0;
    if (input > -1.0e-20) {
        result = solveW1(input);
    } else if (input > -1.0e-3) {
        /// - This is from Eqn. 26 of Reference "Lambert W-function simplified expressions..."
        /// - Accuracy within 0.4% error:
        const double lnx = std::log(-input);
        result = ((2.4978e-5 * lnx + 2.8111e-3) * lnx + 1.1299) * lnx - 1.4733;
    } else if (input > -0.1) {
        /// - Accuracy within 0.4% error:
        const double lnx = std::log(-input);
        result = (2.292e-2 * lnx + 1.411) * lnx - 0.461;
    } else if (input >= -0.364) {
        /// - This is from Eqn. 28 of Reference "Lambert W-function simplified expressions..."
        /// - Accuracy within 1.6% error:
        result = (((248.42 * input + 134.24) * input + 4.4258) * input - 14.629) * input - 4.9631;
    } else {  // input > -1/e
        result = solveW1(input);
    }
    return result;
}
