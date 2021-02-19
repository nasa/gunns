/************************************** TRICK HEADER **********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

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
    ((simulation/hs/TsHsMsg.o)
     (software/exceptions/TsParseException.o))

PROGRAMMERS:
   ((Joe Valerioti) L3) (Initial) (Apr 2012))
***************************************************************************************************/
#include "ParseTool.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsParseException.hh"
#include "math/MsMath.hh" //needed for MsMath::isInRange() in convertToDouble()
#include <cerrno>    //needed for error reporting in openFile()
#include <unistd.h>  //needed for access() function
#include <libgen.h>  //needed for dirname() function
#include <algorithm> //needed for find_if
#include <cstring>   //needed for strerror
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor required by Trick10 although it should never be used.
////////////////////////////////////////////////////////////////////////////////////////////////////
ParseTool::~ParseTool()
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      fileName     (--)  name of text file to parse
///
/// @throw          TsParseException if unable to open file becuase it does not exist, we don't have
///                 read access, or some other unknown reason.
///
/// @details        Checks accessibility of the requested text file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ParseTool::validateFile(const char* fileName)
{
    /// - Create temporary ifstream object for checking.
    std::ifstream anIfstream;

    /// - Check for null pointer.
    if (fileName == 0 )
    {
        /// - Throw exception if file name pointer is null.
        TS_HS_EXCEPTION(TS_HS_ERROR, "Parse", "passed a null fileName char*", TsParseException,
                "file open error", "ParseTool");
    }

    /// - Make sure the ifstream is not already open.
    if ( true == anIfstream.is_open())
    {
        /// - Throw exception if file name pointer is null.
        TS_HS_EXCEPTION(TS_HS_ERROR, "Parse", "file is already open", TsParseException,
                "file open error", fileName);
    }

    /// - Create temporary char* for string duplication. The dirname() method below cannot take an
    ///   argument that is const.
    char* tempName = new char [strlen(fileName) + 1];
    strcpy(tempName, fileName);

    /// - Determine if the file exists and we have the permissions to access both it and the
    ///   directory. To do this we used the access() function, which returns zero if successful.
    ///   F_OK is a constant that indicates we are interested only in finding the file, and R_OK
    ///   indicates a check for read access.
    bool doesFileExist         = !access(tempName,F_OK);
    bool isFileAccessible      = !access(tempName,R_OK);
    bool isDirectoryAccessible = !access(dirname(tempName),R_OK);

    /// - Delete the temporary name pointer.
    delete [] tempName;

    /// - Check that the file exists.
    if (not doesFileExist)
    {
        /// - Throw exception on file not found.
        TS_HS_EXCEPTION(TS_HS_ERROR, "Parse", "file does not exist", TsParseException,
                "file open error", fileName);

    /// - Check that the file can be accessed.
    } else if (not isFileAccessible)
    {
        /// - Throw exception on file not accessible.
        TS_HS_EXCEPTION(TS_HS_ERROR, "Parse", "file not accessible", TsParseException,
                "file open error", fileName);

    /// - Check that the directory is accessible.
    } else if (not isDirectoryAccessible)
    {
        /// - Throw exception on failed access to directory.
        TS_HS_EXCEPTION(TS_HS_ERROR, "Parse", "directory not accessible", TsParseException,
                "file open error", fileName);
    }

    /// - Open the file for parsing if no errors were encountered above.
    anIfstream.open(fileName);

    /// - If open() failed for some unknown reason, throw exception.
    if (anIfstream.fail())
    {
        /// - Throw exception on failure to open file. The strerror() function returns a string
        ///   with information as to why the open failed.
        TS_HS_EXCEPTION(TS_HS_ERROR, "Parse", strerror(errno), TsParseException,
                "file open error", fileName);
    }

    /// - Close the ifstream.
    closeFile(anIfstream);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  str          (--)  string entry in a column of a config-file
/// @param[in]  subsystem    (--)  subsystem string to be used in error reporting
/// @param[in]  locationInfo (--)  optional, string to report error location
///
/// @return     newDouble  (--)  double created from value in "str"
///
/// @details  Converts a string to a double, with checks and error reporting.
///
/// @note     A string appended to a valid floating-point number, like "4.15string", will be
///           be successfully converted to 4.15. However, the warning will be issued, alerting
///           the user that a string was detected but will still assume a value of 4.15.
////////////////////////////////////////////////////////////////////////////////////////////////////
double ParseTool::convertToDouble(
        const std::string& str,
        const std::string& subsystem,
        const std::string& locationInfo)
{
    /// - Create a char* indicator, used to determine if string-to-double conversion was successful.
    char* shouldBeEmpty = 0;

    /// - Call the strtod() function. This library function converts a string to a double. If the
    ///   string cannot be converted, it returns a "zero" and populates the char** argument
    ///   (shouldBeEmpty) with the failed string.
    double newDouble = strtod(str.c_str(), &shouldBeEmpty);

    /// - Here, strcmp() returns a 1 if the char* does NOT point to an empty string.
    if (strcmp("", shouldBeEmpty))
    {
        /// - Issue warnings.
        TsHsMsg msg(TS_HS_WARNING, subsystem);
        msg << locationInfo << " A string read where a float expected: " << shouldBeEmpty;
        hsSendMsg(msg);

        msg.clear();
        msg << "Assuming value of " << newDouble << ".";
        hsSendMsg(msg);

    /// - Make sure the value returned is in range.
    } else if (!MsMath::isInRange(-DBL_MAX, newDouble, DBL_MAX))
    {
        /// - Issue first warning.
        TsHsMsg msg(TS_HS_WARNING, subsystem);
        msg << locationInfo << " Double outside of range [-DBL_MAX,DBL_MAX]";
        hsSendMsg(msg);

        /// - Limit the range.
        newDouble = MsMath::limitRange(-DBL_MAX, newDouble, DBL_MAX);

        /// - Issue second warning.
        msg.clear();
        msg << "Assuming value of " << newDouble << ".";
        hsSendMsg(msg);
    }
    /// - Return the value of the double.
    return newDouble;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]  s             (--)  string of any size
/// @param[in]      removeChars   (--)  optional; characters to remove from the string
///
/// @return         (--) copy string trimmed of its preceding/trailing whitespace and 'c' characters
///
/// @details  Trims quotes (both single and double) and preceding/trailing whitespace from a string.
///
/// @warning  A left/right quotes character (“ / ”, different than ") cannot be trimmed. This
///           character is created when you explicitly enter a " into an OpenOffice document,
///           as opposed to those that are automatically generated as text delimiters.
///
/// @note     To prevent OpenOffice from changing ["] into [”], go to: Tools > AutoCorrect Options.
///           In the Custom Quotes tab, uncheck "Replace" for double quotes.
////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string ParseTool::trim(std::string s, const char* removeChars)
{
    /// - The the char c will default to zero if no trim characters provided.
    if (removeChars != 0)
    {
        /// - Declare i to be the index of the first occurrence of the 'removeChars' char array.
        std::string::size_type i = s.find_first_of(removeChars);

        /// - When i does not equal npos, a 'removeChar' character is found within the string.
        while (i != std::string::npos)
        {
            /// - Erase the one character at index i.
            s.erase(i,1);

            /// - Find the next occurrence of a removeChar
            i = s.find_first_of(removeChars);
        }
    }

    /// - Trim whitespace from start.
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));

    /// - Trim whitespace from end.
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());

    /// - Return new and improved string.
    return s;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]  v   reference to vector that will contain the separated sub-strings
/// @param[in]      s   string to split
/// @param[in]      c   character that delimits string
///
/// @details        This function splits up a string according to the delimiter character passed in
///                 (the char c argument). It stores each piece of the input string into the vector
///                 that was passed in by reference. This can be then accessed in order to pull
///                 out the desired data.
///
/// @warning        Any previous data stored in the input string vector v will be cleared.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ParseTool::splitLineByCharacter(std::vector<std::string>& v,
                                            const std::string& s,
                                                   const char& c)
{
    /// - Clear the storage vector.
    v.clear();

    /// - Declare a temporary string for substring storage.
    std::string sub = "";

    /// - Declare i to be the start index of the string.
    std::string::size_type i = 0;

    /// - Declare j to be the index of the first occurrence of the 'c' character.
    std::string::size_type j = s.find(c);

    /// - When j does not equal std::string::npos, the 'c' character is found within the string.
    while (j != std::string::npos)
    {
        /// - Push back the substring up to the next 'c' character.
        sub = s.substr(i, j-i);
        v.push_back( trim(sub) );

        /// - Set i to be the next character past j.
        i = j+1;

        /// - Set j to the index of the next occurrence of 'c', starting from index i.
        j = s.find(c, i);
    }

    /// - Push back the substring between 'c' and the end. If the while loop above was never
    ///   entered, then this will be the entire (trimmed) string.
    sub = s.substr(i, s.length() );
    v.push_back( trim(sub) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]  theIfstream  (--)  std::ifstream object to associate with a given text file
/// @param[in]      fileName     (--)  name of text file to parse
///
/// @throw          TsParseException if unable to open file becuase it does not exist, we don't have
///                 read access, or some other unknown reason.
///
/// @details        Checks accessibility of the requested text file and opens it for parsing.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ParseTool::openFile(std::ifstream& theIfstream, const char* fileName)
{
    /// - Validate that the file can be successfully opened
    validateFile(fileName);

    /// - Open the file for parsing if no errors were encountered above.
    theIfstream.open(fileName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]  theIfstream  (--)  std::ifstream object to associate with a given text file
///
/// @details        Closes an ifstream if opened.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ParseTool::closeFile(std::ifstream& theIfstream)
{
    /// - Close it if opened.
    if ( true == theIfstream.is_open())
    {
        /// - Close the opened ifstream.
        theIfstream.close();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]  linesVector  (--) reference to vector that will contain each line of parsed file
/// @param[in]      fileName     (--) name of text file to parse
/// @param[in]      maxLineWidth (--) optional; the max amount of character per line (default 50000)
/// @param[in]      removeChars  (--) optional; tells parser to ignore ANY of these characters if
///                                   if found in the file
/// @param[in]      commentChars (--) optional; tells parser to ignore text that comes after this
///                                   EXACT string (up until the start of the next line)
///
/// @throw          TsParseException if error found during file parsing.
///
/// @details        Parses a file, saving each relevant line (not blank or commented out) into
///                 the reference string vector.
///
/// @warning        Any previous data stored in the input string vector will be cleared.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ParseTool::parseLines(std::vector<std::string>& linesVector,
                                         const char* fileName,
                                         const int   maxLineWidth,
                                         const char* removeChars,
                                         const char* commentChars)
{
    /// - Clear the lines vector.
    linesVector.clear();

    /// - Declare an ifstream object to be associated with the opened file.
    std::ifstream theIfstream;

    /// - Open the file for parsing.
    openFile(theIfstream, fileName);

    /// - Declare a temporary char array to hold the contents of a line.
    char charLine[maxLineWidth];

    /// - Declare a line counter to report an error at a specific line.
    int lineCounter = 0;

    /// - This loop examines each line of the ifstream until end-of-file (eof) is reached.
    while ( not theIfstream.eof() )
    {
        /// - Increment line counter.
        lineCounter++;

        /// - Extract a line from the ifstream into the charLine array.
        theIfstream.getline(charLine, maxLineWidth);

        /// - Make sure no errors were encountered from the getline() call.
        if (theIfstream.bad())
        {
            /// - Build error report.
            std::ostringstream msg;
            msg << "line " << lineCounter;

            /// - Throw exception.
            TS_HS_EXCEPTION(TS_HS_ERROR, "Parse", msg.str(), TsParseException,
                    "unknown parsing error", fileName);

        /// - A fail() error occurs when the file has more characters in a line than was given
        ///   by the maxLineWidth argument.
        } else if (!theIfstream.eof() && theIfstream.fail())
        {
            /// - Build error report.
            std::ostringstream msg;
            msg << "line " << lineCounter;

            /// - Throw exception.
            TS_HS_EXCEPTION(TS_HS_ERROR, "Parse", msg.str(), TsParseException,
                    "too many characters in a line", fileName);
        }

        /// - Store the char array into a string for better functionality.
        std::string line = charLine;

        /// - Trim the whitespace and remove the 'removeChar' from the line.
        line = trim(line, removeChars);

        /// - Create line indices.
        std::string::size_type i = 0;
        std::string::size_type j = line.size();

        /// - Logic to handle comment character specification.
        if (commentChars != 0)
        {
            /// - Look for any occurrences of commentChars, a character string that
            ///   designates everything after it as a comment.
            j = line.find(commentChars);

            /// - Limit the size of j.
            if (j > line.size())
            {
                j = line.size();
            }
        }

        /// - Cut the line to only be between i and j. This effectively stores all the data
        ///   between the opening whitespace and the first comment designator (or end of line).
        line = line.substr(i,j);

        /// - If the line is not empty, push it onto the linesVector, which will hold all of
        ///   the relevant lines.
        if ( not line.empty() )
        {
            linesVector.push_back( trim(line) );
        }
    }

    /// - Close the ifstream.
    closeFile(theIfstream);
}
