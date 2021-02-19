#ifndef UtResult_EXISTS
#define UtResult_EXISTS

#include <sstream>

namespace UtResult {

/**
 @file
 @brief    TS21 Utility Unit Test Result Reporting Declaration and Implementation

@defgroup  TSM_UTILITIES_STRINGS_UT_RESULT Unit Test Result Reporting
@ingroup   TSM_UTILITIES_STRINGS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides macros and methods for unit test result reporting.)

@details
REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 CLASS:
- ()

LIBRARY DEPENDENCY:
- ()

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2013-09))

@{
 */

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for Unit Test title line and result line.
///
/// @param[in]  title (--)  The unit test suite title.
///
/// @note     For the first unit test case in a unit test, use UT_RESULT_FIRST instead. \n
///           For all other unit test cases, use UT_RESULT instead.
///
/// @details  This macro creates and sends two lines of text to standard out to start reporting for
///           the initial unit test case in a test suite. \n
///           This message will include:
///           - a title line containing the title truncated to 72 characters followed by a report line with
///           - the unit test class from __FILE__, truncated to 30 characters.
///           - the unit test identification number from static int attribute TEST_ID, 3 digits with 0 padding.
///           - the unit test method from __func)), truncated to 30 characters.
///
/// @par      Example
/// @code
///           #include "strings/UtResult.hh"
///           int UtExample::TEST_ID = 0;
///           void UtExample::setUp() {
///               ...
///               ++TEST_ID;
///           }
///           void UtExample::testConfigAndInput() {
///               UT_RESULT_INITIAL("ISS ECLSS House Rules Example");
///               ...
///               UT_PASS;
///           }
/// @endcode
/// Produces the following two lines of output (the final "." courtesy of cppunit):
/// @code
/// ......................... ISS ECLSS House Rules Example .........................
/// .UtExample ........................ 001: testConfigAndInput ............... Pass.
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define UT_RESULT_INITIAL(title) { \
    std::cout << UtResult::utTitle(title); \
    std::cout << UtResult::utResult(__FILE__, TEST_ID, __func__); \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for Unit Test initial line and result line.
///
/// @note     For the first unit test case in a test suite, use UT_RESULT_INITIAL instead. \n
///           For all other unit test cases, use UT_RESULT instead.
///
/// @details  This macro creates and sends two lines of text to standard out to start reporting for the first test case of a unit test.\n
///           This message will include:
///           - a separator line followed by a report line with
///           - the unit test class from __FILE__, truncated to 30 characters.
///           - the unit test identification number from static int attribute TEST_ID, 3 digits with 0 padding
///           - the unit test method from __func)), truncated to 30 characters
///
/// @par      Example
/// @code
///           #include "strings/UtResult.hh"
///           int UtExample::TEST_ID = 0;
///           void UtExample::setUp() {
///               ...
///               ++TEST_ID;
///           }
///           void UtExample::testConfigAndInput() {
///               UT_RESULT_FIRST;
///               ...
///               UT_PASS;
///           }
/// @endcode
/// Produces the following two lines of output (the final "." courtesy of cppunit):
/// @code
/// .................................................................................
/// .UtExample ........................ 001: testConfigAndInput ............... Pass.
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define UT_RESULT_FIRST { \
    std::cout << "................................................................................"; \
    std::cout << UtResult::utResult(__FILE__, TEST_ID, __func__); \
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for Unit Test result line.
///
/// @note     For the first unit test case in a test suite, use UT_RESULT_INITIAL instead.\n
///           For the first unit test case in a unit test, use UT_RESULT_FIRST instead.
///
/// @details  This macro creates and sends a line of text to standard out for unit test case result reporting.\n
///           This message will include:
///           - the unit test class from __FILE__, truncated to 30 characters.
///           - the unit test identification number from static int attribute TEST_ID, 3 digits with 0 padding
///           - the unit test method from __func)), truncated to 30 characters
///
/// @par      Example
/// @code
///           #include "strings/UtResult.hh"
///           int UtExample::TEST_ID = 0;
///           void UtExample::setUp() {
///               ...
///               ++TEST_ID;
///           }
///           void UtExample::testConfigAndInput() {
///               UT_RESULT;
///               ...
///               UT_PASS;
///           }
/// @endcode
/// Produces the following line of output (the final "." courtesy of cppunit):
/// @code
/// .UtExample ........................ 001: testConfigAndInput ............... Pass.
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define UT_RESULT std::cout << UtResult::utResult(__FILE__, TEST_ID, __func__)

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for Unit Test Pass line.
///
/// @note     For the last unit test case in a test suite, use UT_PASS_FINAL instead. \n
///           For the last unit test case in a unit test, use UT_PASS_LAST instead.
///
/// @details  This macro creates and sends a line of text to standard out (appended to the output
///           of one of the UT_RESULT macros) for unit test case result reporting.
///
/// @par      Example
/// @code
///           #include "strings/UtResult.hh"
///           int UtExample::TEST_ID = 0;
///           void UtExample::setUp() {
///               ...
///               ++TEST_ID;
///           }
///           void UtExample::testConfigAndInput() {
///               UT_RESULT;
///               ...
///               UT_PASS;
///           }
/// @endcode
/// Produces the following line of output (the final "." courtesy of cppunit):
/// @code
/// .UtExample ........................ 001: testConfigAndInput ............... Pass.
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define UT_PASS std::cout << "Pass"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for last Unit Test Pass line.
///
/// @note     For the last unit test case in a test suite, use UT_PASS_FINAL instead. \n
///           For all other unit test cases, use UT_PASS instead.
///
/// @details  This macro creates and sends a line of text to standard out (appended to the output
///           of one of the UT_RESULT macros) for unit test case result reporting on the last test
///           case of a unit test. \n
///
/// @par      Example
/// @code
///           #include "strings/UtResult.hh"
///           int UtExample::TEST_ID = 0;
///           void UtExample::setUp() {
///               ...
///               ++TEST_ID;
///           }
///           void UtExample::testConfigAndInput() {
///               UT_RESULT;
///               ...
///               UT_PASS_LAST;
///           }
/// @endcode
/// Produces the following line of output:
/// @code
/// .UtExample ........................ 001: testConfigAndInput ............... Pass.
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define UT_PASS_LAST std::cout << "Pass." << std::endl;

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Macro for final Unit Test Pass line.
///
/// @note     For the last unit test case in a unit test, use UT_PASS_LAST instead. \n
///           For all other unit test cases, use UT_PASS instead.
///
/// @details  This macro creates and sends two lines of text to standard out (appended to the output
///           of one of the UT_RESULT macros) for unit test case result reporting on the final test
///           case of a test suite.
///
/// @code
/// @par      Example
/// @code
///           #include "strings/UtResult.hh"
///           int UtExample::TEST_ID = 0;
///           void UtExample::setUp() {
///               ...
///               ++TEST_ID;
///           }
///           void UtExample::testConfigAndInput() {
///               UT_RESULT;
///               ...
///               UT_PASS_FINAL;
///           }
/// @endcode
/// Produces the following line of output:
/// @code
/// .UtExample ........................ 001: testConfigAndInput ............... Pass.
/// .................................................................................
/// @endcode
/// @hideinitializer
///////////////////////////////////////////////////////////////////////////////////////////////////
#define UT_PASS_FINAL { \
    std::cout << "Pass.\n"; \
    std::cout << "................................................................................."; \
    std::cout << std::endl; \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  file       (--)  The unit test source file name.
/// @param[in]  testId     (--)  The unit test identification number (0 to 999).
/// @param[in]  function   (--)  The unit test method name.
///
/// @return     string     (--)  The unit test result string with class, method and id.
///
/// @details    Creates a string with the unit test class, id and method for reporting results.
////////////////////////////////////////////////////////////////////////////////////////////////////
static inline std::string utResult(const std::string& file, const int testId, const std::string& function)
{
    /// - Get the class name from the file name by chopping the last 4 characters (.cpp) and then truncate to 30 characters.
    const std::string className    = file.substr(0, std::min(static_cast<int>(file.length()) - 4, 30));

    /// - Truncate the method name to 30 characters.
    const std::string functionName = function.substr(0, std::min(static_cast<int>(function.length()), 30));

    /// - Construct the result line in a string stream.
    std::ostringstream os;
    os << "\n.";
    os << className;
    os << " ";
    os.width(34 - className.length());
    os.fill('.');
    os << " ";
    os.width(3);
    os.fill('0');
    os << testId;
    os << ": ";
    os << functionName;
    os << " ";
    os.width(34 - functionName.length());
    os.fill('.');
    os << " ";

    /// - Convert the result line to a string and return it.
    return os.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  title    (--)  The test suite title.
///
/// @return     string   (--)  The test suite title line.
///
/// @details    Creates a string with the centered test suite title.
////////////////////////////////////////////////////////////////////////////////////////////////////
static inline std::string utTitle(const std::string& title)
{
    /// - Truncate the title to 72 characters.
    const std::string titleName = title.substr(0, std::min(static_cast<int>(title.length()), 72));

    /// - Determine the pad to center the title in the 80 character line.
    const int lpad = (80 - titleName.length()) / 2;
    const int rpad = lpad - (titleName.length() + 1) % 2;

    /// - Construct the title line in a string stream.
    std::ostringstream os;
    os.width(lpad);
    os.fill('.');
    os << " ";
    os << titleName;
    os << " ";
    os.width(rpad);
    os.fill('.');
    os << ".";

    /// - Convert the title line to a string and return it.
    return os.str();
}

};

/// @}

#endif
