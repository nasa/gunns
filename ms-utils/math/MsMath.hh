#ifndef MSMath_EXISTS
#define MSMath_EXISTS

/**
 @file
 @brief    Utility Function declarations and implementations.

 @defgroup  TSM_UTILITIES_MATH_MATH Utility Functions
 @ingroup   TSM_UTILITIES_MATH

 @copyright Copyright 2019 United States Government as represented by the Administrator of the
            National Aeronautics and Space Administration.  All Rights Reserved.

  PURPOSE: (Provides the class for mathematical utility functions.)

 @details
 REQUIREMENTS:
 - ()

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 ((math/UnitConversion.o))

 PROGRAMMERS:
 - ((Kenneth McMurtrie) (Tietronix Software) (Additional methods) (2012-05))
    (Jose A. Perez) (L3 - Comm) (Added roundUp) (04-17-2013))
    (Mark Wiederholt) (Lockheed) (Added asinh) (07-24-2013))

 @{
 */

#include <cmath>
#include <cfloat>
#include <algorithm>

#include "math/UnitConversion.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Mathematical Utility Functions
///
/// @details  All methods are static and there are no attributes, so the default constructor,
///           copy constructor, default destructor and assignment operator are all declared private
///           and not implemented.
////////////////////////////////////////////////////////////////////////////////////////////////////
class MsMath
{
    public:
        /// @brief Resolves an angle to range [-pi,+PI]
        static double anglePi(const double angle);
        /// @brief Resolves an angle to range [0,+PI]
        static double angle2Pi(const double angle);
        /// @brief Resolves an angle to range [-180,+180]
        static double angle180(const double angle);
        /// @brief Resolves an angle to range [0,+360]
        static double angle360(const double angle);
        /// @brief Protected (against range error) asin function
        static double protectedAsin(const double x);
        /// @brief Protected (against range error) acos function
        static double protectedAcos(const double x);
        /// @brief Protected (against range error) sqrt function
        static double protectedSqrt(const double x);
        /// @brief Protected (against range error) log10 function
        static double protectedLog10(const double x);
        /// @brief Protected (against range error) log function
        static double protectedLog(const double x);
        /// @brief Protected (against zero divisor) division function
        static double protectedDiv(const double num, const double den, const double threshold = DBL_EPSILON, const double retval = 0.0);
        /// @brief Limits the value of a double argument to a specified range (lower <= x <= upper).
        static double limitRange(const double lower, const double x, const double upper);
        /// @brief Limits the value of an integer argument to a specified range (lower <= x <= upper).
        static int limitRange(const int lower, const int x, const int upper);
        /// @brief Is the double input within specified range (lower <= x <= upper)?
        static bool isInRange(const double lower, const double x, const double upper);
        /// @brief Is the integer input within specified range (lower <= i <= upper)?
        static bool isInRange(const int lower, const int i, const int upper);
        /// @brief  Limits the value of a double argument outside a specified range (x <= lower || upper <= x).
        static double innerLimit(const double lower, const double x, const double upper);
        /// @brief  Limits the value of an integer argument outside a specified range (x <= lower || upper <= x).
        static int innerLimit(const int lower, const int x, const int upper);
        /// @brief Rounds a double to the nearest integer value
        static int round(const double x);
        /// @brief Inverse hyperbolic sine
        static double asinh(const double value);
        /// @brief Alternate power function implementation
        static double fastPow(const double base, const double exp);

        /// @brief Quantize the input value.
        static double quantize(const double input, const double resolution);

    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Default constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        MsMath();
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        MsMath(const MsMath&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Default destructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ~MsMath();
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        const MsMath& operator =(const MsMath&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor required by Trick10 although it should never be used.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline MsMath::~MsMath()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  angle  (r)  Input angle in range [-3pi, +3pi).
///
/// @return     double (r)   Angle resolved to the range [-pi,+pi).
///
/// @warning    If the input angle is not known to lie in the range [-3pi, +3pi), a mod function
///             should be used instead of this method.
///
/// @details    Returns the specified angle resolved to the range [-pi,+pi).
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::anglePi(const double angle)
{
    double result = angle;

    if (result < -UnitConversion::PI_UTIL) {
        result += UnitConversion::TWO_PI;
    } else if (result >= +UnitConversion::PI_UTIL) {
        result -= UnitConversion::TWO_PI;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  angle  (r)   Input angle in radians in range [-4pi,+4pi).
///
/// @return     double (r)   Angle resolved to the range [0,+2pi).
///
/// @warning    If the input angle is not known to lie in the range [-4pi,+4pi), a mod function
///             should be used instead of this method.
///
/// @details    Returns the specified angle resolved to the range [0,+2pi).
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::angle2Pi(const double angle)
{
    double result = angle;

    if (result < 0.0) {
        result += UnitConversion::TWO_PI;
    }
    if (result < 0.0) {
        result += UnitConversion::TWO_PI;
    }
    if (result >= UnitConversion::TWO_PI) {
        result -= UnitConversion::TWO_PI;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  angle  (d)   Input angle in degrees in range [-540, +540).
///
/// @return     double (d)   Angle resolved to the range [-180,+180).
///
/// @warning    If the input angle is not known to lie in the range [-540, +540), a mod function
///             should be used instead of this method.
///
/// @details    Returns the specified angle resolved to the range [-180,+180).
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::angle180(const double angle)
{
    double result = angle;

    if (result < -180.0) {
        result += 360.0;
    } else if (result >= +180.0) {
        result -= 360.0;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  angle  (d)   Input angle in degrees in range [-720,+720).
///
/// @return     double (d)   Angle resolved to the range [0,+360).
///
/// @warning    If the input angle is not known to lie in the range [-720,+720), a mod function
///             should be used instead of this method.
///
/// @details    Returns the specified angle resolved to the range [0,+360).
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::angle360(const double angle)
{
    double result = angle;

    if (result < 0.0) {
        result += 360.0;
    }
    if (result < 0.0) {
        result += 360.0;
    }
    if (result >= 360.0) {
        result -= 360.0;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  x      (--)  Input argument.
///
/// @return     double (--)  Arcsin of argument.
///
/// @details    Returns the arcsine of the argument with protection against range error.
///             \verbatim
///                       _
///                      |  +pi/2,   x >= 1.0
///             result = |  asin(x), -1.0 < x < 1.0
///                      |_ -pi/2,   x <= -1.0
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::protectedAsin(const double x)
{
    double result;

    if (x > 1.0 - DBL_EPSILON) {
        result = +UnitConversion::PI_OVER_2;
    } else if (x < -1.0 + DBL_EPSILON) {
        result = -UnitConversion::PI_OVER_2;
    } else {
        result = asin(x);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  x      (--)  Input argument.
///
/// @return     double (--)  Arccos of argument.
///
/// @details    Returns the arccosine of the argument with protection against range error.
///             \verbatim
///                       _
///                      |  0.0,     x >= 1.0
///             result = |  acos(x), -1.0 < x < 1.0
///                      |_ pi,      x <= -1.0
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::protectedAcos(const double x)
{
    double result;

    if (x > 1.0 - DBL_EPSILON) {
        result = 0.0;
    } else if (x < -1.0 + DBL_EPSILON) {
        result = UnitConversion::PI_UTIL;
    } else {
        result = acos(x);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  x      (--)  Input argument.
///
/// @return     double (--)  Sqrt of argument.
///
/// @details    Returns the square root of the argument with protection against range error.
///             \verbatim
///                       _
///                      |  0.0,     x <= 0.0
///             result = |_ sqrt(x), x >  0.0
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::protectedSqrt(const double x)
{
    double result;

    if (x < DBL_EPSILON) {
        result = 0.0;
    } else {
        result = sqrt(x);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  x      (--)  Input argument.
///
/// @return     double (--)  Log10 of argument.
///
/// @details    Returns the base 10 logarithm of the argument with protection against range error.
///             \verbatim
///                       _
///                      |  0.0,      x <= 0.0
///             result = |_ log10(x), x >  0.0
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::protectedLog10(const double x)
{
    double result;

    if (x < DBL_EPSILON) {
        result = 0.0;
    } else {
        result = log10(x);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  x      (--)  Input argument.
///
/// @return     double (--)  Log of argument.
///
/// @details    Returns the natural logarithm of the argument with protection against range error.
///             \verbatim
///                       _
///                      |  0.0,    x <= 0.0
///             result = |_ log(x), x >  0.0
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::protectedLog(const double x)
{
    double result;

    if (x < DBL_EPSILON) {
        result = 0.0;
    } else {
        result = log(x);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  num        (--)  Numerator (dividend).
/// @param[in]  den        (--)  Denominator (divisor).
/// @param[in]  threshold  (--)  Zero divisor threshold.
/// @param[in]  retval     (--)  Zero divisor return value.
///
/// @return     double     (--)  The quotient of the arguments.
///
/// @warning    If the threshold is negative, then division by zero may be attempted, so don't do that.
///
/// @details    Returns the quotient of the arguments with protection against divide by zero. The zero
///             divisor threshold and return value are optional and default to DBL_EPSILON and 0.0.
///             \verbatim
///                       _
///                      |  num / den    den > +threshold
///             result = |  retval,     -threshold <= den <= +threshold
///                      |_ num / den    den < -threshold
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::protectedDiv(const double num, const double den, const double threshold, const double retval)
{
    double result = retval;

    if (!MsMath::isInRange(-threshold, den, +threshold)) {
        result = num / den;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  lower  (--)  Lower limit of range.
/// @param[in]  x      (--)  Input argument to be limited.
/// @param[in]  upper  (--)  Upper limit of range.
///
/// @return     double (--)  The value limited to the specified range.
///
/// @warning    If lower > upper, then lower is returned without complaint, so don't do that.
///
/// @details    Limits the value of a double argument to the specified range (lower <= x <= upper).
///             \verbatim
///                       _
///                      |  lower,     x < lower
///             result = |  x,         lower <= x <= upper
///                      |_ upper,     x > upper
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::limitRange(const double lower, const double x, const double upper)
{
    return std::max(std::min(x, upper), lower);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  lower  (--)  Lower limit of range.
/// @param[in]  x      (--)  Input argument to be limited.
/// @param[in]  upper  (--)  Upper limit of range.
///
/// @return     int    (--)  The value limited to the specified range.
///
/// @warning    If lower > upper, then lower is returned without complaint, so don't do that.
///
/// @details    Limits the value of an integer argument to the specified range (lower <= x <= upper).
///             \verbatim
///                       _
///                      |  lower,     x < lower
///             result = |  x,         lower <= x <= upper
///                      |_ upper,     x > upper
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int MsMath::limitRange(const int lower, const int x, const int upper)
{
    return std::max(std::min(x, upper), lower);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  lower  (--)  Lower limit of range.
/// @param[in]  x      (--)  Input argument to be limited.
/// @param[in]  upper  (--)  Upper limit of range.
///
/// @return     bool   (--)  True if the input argument is within the specified range, otherwise false.
///
/// @warning    If lower > upper, then false is returned without complaint, so don't do that.
///
/// @details    Determines if the double argument value is within the specified range (lower <= x <= upper).
///             \verbatim
///                       _
///                      |  false,     x < lower
///             result = |  true,         lower <= x <= upper
///                      |_ false,     x > upper
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool MsMath::isInRange(const double lower, const double x, const double upper)
{
    return lower <= x && x <= upper;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  lower  (--)  Lower limit of range.
/// @param[in]  i      (--)  Input argument to be limited.
/// @param[in]  upper  (--)  Upper limit of range.
///
/// @return     bool  (--)  True if the input argument is within the specified range, otherwise false.
///
/// @warning    If lower > upper, then false is returned without complaint, so don't do that.
///
/// @details    Determines if the integer argument value is within the specified range (lower <= i <= upper).
///             \verbatim
///                       _
///                      |  false,     i < lower
///             result = |  true,         lower <= i <= upper
///                      |_ false,     i > upper
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool MsMath::isInRange(const int lower, const int i, const int upper)
{
    return lower <= i && i <= upper;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  lower  (--)  Lower limit of range.
/// @param[in]  x      (--)  Input argument to be limited.
/// @param[in]  upper  (--)  Upper limit of range.
///
/// @return     double (--)  The value limited to outside the specified range.
///
/// @warning    If lower > upper, then x is returned without complaint, so don't do that.
///
/// @details    Limits the value of a double argument to lie outside the specified range
///             (x <= lower || upper <= x).
///             \verbatim
///                       _
///                      |  x,         x <= lower
///                      |  lower,     lower <= x < (upper - lower) / 2
///             result = |  upper,     (upper - lower) / 2 <= x < upper
///                      |_ x,         x >= upper
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::innerLimit(const double lower, const double x, const double upper)
{
    double result = x;
    if (lower < x && x < upper) {
        const double middle = 0.5 * (upper + lower);
        if (x < middle) {
            result = lower;
        } else {
            result = upper;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  lower  (--)  Lower limit of range.
/// @param[in]  x      (--)  Input argument to be limited.
/// @param[in]  upper  (--)  Upper limit of range.
///
/// @return     int (--)  The value limited to outside the specified range.
///
/// @warning    If lower > upper, then x is returned without complaint, so don't do that.
///
/// @details    Limits the value of an integer argument to lie outside the specified range
///             (x <= lower || upper <= x).
///             \verbatim
///                       _
///                      |  x,         x <= lower
///                      |  lower,     lower <= x < (upper - lower) / 2
///             result = |  upper,     (upper - lower) / 2 <= x < upper
///                      |_ x,         x >= upper
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int MsMath::innerLimit(const int lower, const int x, const int upper)
{
    int result = x;
    if (lower < x && x < upper) {
        const double middle = 0.5 * (upper + lower);
        if (x < middle) {
            result = lower;
        } else {
            result = upper;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  x      (--)  Input argument to be rounded.
///
/// @return     int    (--)  The value rounded to the nearest integer value.
///
/// @details    Rounds the value of a double to the nearest integer value.
///             (floor(x) is defined to be equal to x minus its fractional component).
///             \verbatim
///                       _
///             result = |  floor(x),           x - floor(x) < 0.5
///                      |_ floor(x) + 1,       x - floor(x) >= 0.5
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int MsMath::round(const double x)
{
    // Casting to an integer removes the fractional part of x
    return static_cast<int>(std::floor(x + 0.5));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  input      (--)  Input argument to be quantized.
/// @param[in]  resolution (--)  Resolution to quantize value to.
///
/// @return     double     (--)  The quantized value.
///
/// @note       Resolution arguments <= ±DBL_EPSILON will return the input.
///             Resolution sign does not affects the output.
///
/// @details    Quantize the value by the resolution provided as an argument.
///             \verbatim
///             result example = quantize(3.14159, 1.0)    = 3.0;
///             result example = quantize(3.14159, 0.1)    = 3.1;
///             result example = quantize(3.14159, 0.01)   = 3.14;
///             result example = quantize(3.14159, 0.0)    = 3.14159;
///             result example = quantize(123456.7, 10.0)  = 123460.0;
///             result example = quantize(123456.7, 100.0) = 123500.0;
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::quantize(const double input, const double resolution)
{
    double result = input;
    if (DBL_EPSILON < resolution) {
        result = resolution * round(input / resolution);
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  value     (--)  Input argument
///
/// @return     double    (--)  The inverse hyperbolic sine of the value input
//
/// @details   Finds the inverse hyperbolic sine of the input value.
///
///             \verbatim
///                       _
///             result = |  asinh(x)
///                      |_
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::asinh(const double value)
{
    return  MsMath::protectedLog(value + sqrt(value * value + 1.0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  base     (--)  Base of the power calculation
/// @param[in]  exp      (--)  Exponent of the power calculation
///
/// @return     double   (--)  Result of the power function
///
/// @details   Computes the power in a reliable time frame. std::pow calculations have been observed
///            to spike up to 2 milliseconds for 1 calculation.
///            This is a 30th order series approx for b^x=pow(b,x)
///            based on the identity base^x = e^xlog(base).
///            It uses multiplications for the integer powers of the base.
///            Example: b^7.342 = b^7 * b^0.342 = b*b*b*b*b*b*b*e^(0.342log(b))
///            For any base and exponent it gives single precision results.
///            Gives perfect results for any integer exponent.
///            Like any series solution it has problems when base is close to 0.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MsMath::fastPow(const double base, const double exp)
{
    int num;
    double a;
    double b;
    double pwr;

//  Reduce the size of the exponent by truncation
//  i.e. b^7.342 = b^7 * b^0.342 = b^num * b^frac_exp

    num= int(std::fabs(exp));

//  make series for (base)^(frac_exp) = e^(frac_exp*ln(base))
//  use the least amount of divides as possible
    pwr= 1.0;
    b= std::fabs(exp)-num;

//  don't calc series if exponent is whole number
    if(b > 0.000000001){
//      set the log of the base
        a= std::log(std::fabs(base));

        b= std::fabs(b*a);
        pwr=1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(
            1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(
            1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b*(1+b
           *0.03333333333333333)/29)/28)/27)/26)*0.04)
           *0.04166666666666667)/23)/22)/21)*0.05)/19)
           *0.05555555555555556)/17)*0.0625)
           *0.06666666666666667)/14)/13)
           *0.08333333333333333)*0.09090909090909091)*0.1)
           *0.11111111111111111)*0.125)/7)*0.1666666666666667)
           *0.2)*0.25)*0.3333333333333333)*0.5);

//      check for negative log(base) if base < 1.0
        if(a < 0) {pwr=1./pwr;}
    }

//  get the integer powers of base if needed
    if(num > 0) {
//      put together the parts of b^x: b*b*b*b*...*b^(frac_exp)
        for(int k=1;k<num+1;k++) {
            pwr= pwr*std::fabs(base);
        }
    }

    //check for negative exponent
    if( exp < 0 ) { pwr=1.0/pwr;}

    //check for negative base with odd exponent
    if((base < 0)  &&  (std::fabs(static_cast<int>(round(exp)) % 2) == 1) ) {
        pwr=-pwr;
    }

   return pwr;
}

#endif
