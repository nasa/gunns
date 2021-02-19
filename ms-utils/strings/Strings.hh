#ifndef Strings_EXISTS
#define Strings_EXISTS

/**
@defgroup  TSM_UTILITIES_STRINGS_STRINGS Strings Utility Functions
@ingroup   TSM_UTILITIES_STRINGS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides generic string manipulation functions.)

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
#include <vector>

#include "math/MsMath.hh"

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

    /// @brief Returns a vector of substrings of the given string split by the given delimiter.
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter);

    /// @brief Returns a copy of the given string stripped of leading and trailing whitespace.
    static std::string trim(const std::string& str);

    /// @brief Returns true if theString ends with the given ending.
    static bool endsWith(const std::string& theString, const std::string& ending);
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
    double newNumToCon = MsMath::limitRange(minValue, MsMath::quantize(numberToConvert, 1/(pow(10,decimalPrecision))), maxValue);
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
inline std::string Strings::intToString(
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] str (--) The string to parse.
/// @param[in] delimiter (--) The delimiter to parse from the string.
///
/// @returns  std::vector<std::string> (--) Vector of strings split from the given string.
///
/// @details  Splits the given string by the given delimiter and returns the result as a vector of
///           sub-strings.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline std::vector<std::string> Strings::split(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> result;
    std::string s = str;
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        result.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    result.push_back(s);
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] str (--) The string to trim.
///
/// @returns  std::string (--) The trimmed string.
///
/// @details  Copies the given string, trims the copy by removing characters of the set
///           ( \n\r\t\f\v) (including space) from the beginning and end, and returns the trimmed
///           copy.
///           credit: https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
////////////////////////////////////////////////////////////////////////////////////////////////////
inline std::string Strings::trim(const std::string& str)
{
    const std::string WHITESPACE = " \n\r\t\f\v";
    size_t start = str.find_first_not_of(WHITESPACE);
    const std::string ltrim = (start == std::string::npos) ? "" : str.substr(start);
    size_t end = ltrim.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : ltrim.substr(0, end + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] str (--) The string to check the ending of.
/// @param[in] end (--) The suffix to check at the end of the str string.
///
/// @returns  bool (--) True if str ends with end, otherwise false.
///
/// @details  Compares the last n characters of str with end, n being the length of end.  If end is
///           longer than str, always returns false.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Strings::endsWith(const std::string& str, const std::string& end)
{
    if (str.length() >= end.length()) {
        return (0 == str.compare (str.length() - end.length(), end.length(), end));
    }
    return false;
}

#endif
