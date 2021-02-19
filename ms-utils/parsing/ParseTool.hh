#ifndef ParseTool_EXISTS
#define ParseTool_EXISTS
/**
@defgroup TSM_UTILITIES_PARSING_ParseTool Parse tool
@ingroup  TSM_UTILITIES_PARSING

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (Provides the class for parsing utility functions. The main function of the ParseToll class is
    the parseLines() method, which stores lines from a text file into the members of a referenced
    string vector. Other utilities of the ParseTool including trim(), which removes single/double
    quotes and preceding/trailing whitespace from a string, and splitLineCharacter(), which
    divides an input string up by a specified character.

    The class constructor is not implemented (although the destructor is, to avoid a Trick error),
    so the ParseTool should never be directly instantiated. It contains no attributes, and all of
    its method are static.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (parsing/ParseTool.o)

PROGRAMMERS:
   ((Joe Valerioti) L3) (Initial) (Apr 2012))

@{
*/

#include <vector>
#include <string>
#include <fstream>  //needed for std::ifstream
#include "simulation/hs/TsHsMsg.hh" //needed for TS_HS_UNKNOWN
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Provides utility parsing functions.
///
/// @details
/// All methods are static and there are no attributes, so the default constructor, copy
/// constructor, default destructor and assignment operator are all declared private and not
/// implemented.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ParseTool
{
    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Default destructor. Necessary even though class never instantiated.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ~ParseTool();
        /// @brief  Trims whitespace and quotes from a string.
        static const std::string trim(std::string s, const char* removeChars = 0);
        /// @brief  Stores data from a character-delimited line into a vector.
        static void splitLineByCharacter(std::vector<std::string>& v,
                                                const std::string& s,
                                                       const char& c);
        /// @brief  Verifies a file entry as a floating-point number and converts it to a double.
        static double convertToDouble(const std::string& str,
                                      const std::string& subsystem = TS_HS_UNKNOWN,
                                      const std::string& locationInfo = "");

        /// @brief  Checks accessibility of a requested file and opens it for parsing.
        static void validateFile(const char* fileName);

        /// @brief  Parses a file, saving each non-commented out line into the vLines member.
        static void parseLines(std::vector<std::string>& linesVector,
                                             const char* fileName,
                                             const int   maxLineWidth = 50000,
                                             const char* removeChars  = 0,
                                             const char* commentChars = 0
                                              );
    protected:
        /// @brief  Checks accessibility of a requested file and opens it for parsing.
        static void openFile(std::ifstream& theIfstream, const char* fileName);

        /// @brief  Checks accessibility of a requested file and opens it for parsing.
        static void closeFile(std::ifstream& theIfstream);

    private:
        /// @details  Default constructor unavailable since declared private and not implemented.
        ParseTool();
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ParseTool(const ParseTool&);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        const ParseTool& operator =(const ParseTool&);
};

/// @}

#endif
