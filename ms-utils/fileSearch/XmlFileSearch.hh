#ifndef XmlFileSearch_EXISTS
#define XmlFileSearch_EXISTS
/**
@defgroup TSM_UTILITIES_FILE_SEARCH_XmlFileSearch Tool used to serach for xml files in a directory.
@ingroup  TSM_UTILITIES_FILE_SEARCH

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (This class provides the ability to search for regular xml files in the specified directory
   passed in the initialize method.
   Also, one is able to specify the maximum number of files to look for. All of the files will be
   stored as a string in a local vector. Client could call a getter to get the vector of the list
   of files.

   Tool is intended to be used in init else be aware of all the exceptions it throws.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   (The tool will ignore all hidden files and subdirectories.)

LIBRARY DEPENDENCY:
   (fileSearch/XmlFileSearch.o)

PROGRAMMERS:
   ((Jose A. Perez) (L3 Comm) (October 16, 2012) (TS21) (Initial)
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <vector>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Provides the Xml File Search functions. Please read PURPOSE: and ASSUMPTIONS: above for
/// more details.
///////////////////////////////////////////////////////////////////////////////////////////////////
class XmlFileSearch
{
    TS_MAKE_SIM_COMPATIBLE(XmlFileSearch);

    public:
        static const std::string NO_FILE_MSG;    /**< (--) No file message, non integral. */

        /// @brief    Default constructs this xml file search object.
        XmlFileSearch();

        /// @brief    Default destructs for this xml file search object.
        virtual ~XmlFileSearch();

        /// @brief    Call this function to get the vector of all found files.
        std::vector<std::string> getFileList() const;

        /// @brief    Call this function to search directory for regular files.
        void searchDirectory(
            const char* fileDirectory    = "",
            const int   maxNumberOfFiles = 100);

    protected:
        std::vector<std::string> fileList; /**< (--) Vector containing the found files.*/

        /// @brief    Extracts extension from file name.
        std::string obtainExt(
            const std::string pathName);

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        XmlFileSearch(const XmlFileSearch&);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        const XmlFileSearch& operator =(const XmlFileSearch&);
};

/// @}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @return std::vector<std::string>  (--) Vector of the file list.
/// @details  Getter for the vector containing all the file list.
///////////////////////////////////////////////////////////////////////////////////////////////////
inline std::vector<std::string> XmlFileSearch::getFileList() const
{
    return fileList;
}
#endif
