#ifndef Strings_EXISTS
#define Strings_EXISTS

/**
@defgroup  TSM_UTILITIES_STRINGS_STRINGS Strings Utility Functions
@ingroup   TSM_UTILITIES_STRINGS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the class for number to string conversion.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 CLASS:
- ()

LIBRARY DEPENDENCY:
     ((math/UnitConversion.o))

 PROGRAMMERS:
- ((Jose A. Perez) (L3 - Comm) (Initial) (04-17-2013))

@{
 */

#include <string>
#include <sstream>
#include <iomanip>

#include "math/Math.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Provides utility string functions.
///
/// @details
/// All methods are static and there are no attributes, so the default constructor, copy
/// constructor, default destructor and assignment operator are all declared private and not
/// implemented.
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Strings
{
    /// @brief Converts a float to a standard library string.
    static std::string floatToString(
        const double numberToConvert,
        const double decimalPrecision,
        const double minValue,
        const double maxValue);

    /// @brief    Converts an int to a standard library string.
    static std::string intToString(
        const int numberToConvert,
        const int minValue,
        const int maxValue);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] numberToConvert     Floating number to convert.
/// @param[in] decimalPrecision    Decimal precision to round up to.
/// @param[in] minValue            Min value output should be limited to.
/// @param[in] maxValue            Max value output should be limited to.
///
/// @return    std::string         Converted string from float.
///
/// @details   Converts the passed in argument in to a standard library string.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline std::string Strings::floatToString(
    const double numberToConvert,
    const double decimalPrecision,
    const double minValue,
    const double maxValue)
{
    double newNumToCon = Math::limitRange(minValue, Math::quantize(numberToConvert, 1/(pow(10,decimalPrecision))), maxValue);
    std::stringstream convert;
    convert << std::fixed << std::setprecision(decimalPrecision) << newNumToCon;
    std::string cValue(convert.str());

    return cValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] numberToConvert   (--)  Integer number to convert.
/// @param[in] minValue          (--)  Minimum integer value.
/// @param[in] maxValue          (--)  Maximum integer value.
///
/// @return    std::string       (--)  Converted string from int.
///
/// @details   Converts the passed in argument in to a standard library string.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string Strings::intToString(
    const int numberToConvert,
    const int minValue,
    const int maxValue)
{
    int newValue = std::max(std::min(numberToConvert, maxValue), minValue);
    std::stringstream convert;
    convert << newValue;
    std::string cValue(convert.str());

    return cValue;
}

#endif
