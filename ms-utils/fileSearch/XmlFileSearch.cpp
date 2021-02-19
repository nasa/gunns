/************************************** TRICK HEADER **********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (software/exceptions/TsInitializationException.o)
    (simulation/hs/TsHsMsg.o)
 )

 PROGRAMMERS:
 (
  ((Jose A. Perez) (L3 Comm) (October 16, 2012) (TS21) (Initial))
 )
***************************************************************************************************/

#include <algorithm>
#include "XmlFileSearch.hh"
#include "software/exceptions/TsHsException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <dirent.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the File Search object.
///////////////////////////////////////////////////////////////////////////////////////////////////
XmlFileSearch::XmlFileSearch()
    :
    fileList (0)
{
    //Nothing to do here.
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the File Search object.
///////////////////////////////////////////////////////////////////////////////////////////////////
XmlFileSearch::~XmlFileSearch()
{
    //Nothing to do here.
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Declaration of static const no file msg. Here since it is non integral.
///////////////////////////////////////////////////////////////////////////////////////////////////
const std::string XmlFileSearch::NO_FILE_MSG = "Xml File Search: There were no files found.";

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fileDirectory    (--) File directory to search for regular xml files.
/// @param[in]  maxNumberOfFiles (--) Max number of files to search for.
/// @details  Method to search directory. Stores found xml files in vector, ignores hidden files and
///           subdirectories.
/// @throws TsInitializationException If input data is set incorrectly.
///////////////////////////////////////////////////////////////////////////////////////////////////
void XmlFileSearch::searchDirectory(
    const char* fileDirectory,
    const int   maxNumberOfFiles)
{
    /// - Throw an exception if max number of files is zero.
    if(0 >= maxNumberOfFiles)
    {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GENERIC, "Invalid Initialization Data", TsInitializationException,
             "Xml File Search: Max number of files must be greater than zero.", fileDirectory);
    }

    DIR *dir = opendir(fileDirectory);
    /// - Throw an exception if directory is null.
    if(0 == dir)
    {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GENERIC, "Invalid Initialization Data", TsInitializationException,
            "Xml File Search: Unable to load file, check file, file existence, ect.", fileDirectory);
    }

    dirent *dirEntry = readdir(dir);
    while(dirEntry != 0)
    {
        if(DT_REG == dirEntry->d_type)
        {
            if (dirEntry->d_name[0] != '.')
            {
                std::string extension = obtainExt(std::string(dirEntry->d_name));
                if(extension == "xml")
                {
                    fileList.push_back(std::string(dirEntry->d_name));
                }
            }
        }
        /// - Throw an exception if number of files is greater than 100.
        if(maxNumberOfFiles < static_cast<const int>(fileList.size()))
        {
            closedir(dir);
            TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GENERIC, "Invalid Initialization Data", TsInitializationException,
               "Notice: Have found more than the max number of files limit.", fileDirectory);
        }

        dirEntry = readdir(dir);
    }

    closedir(dir);

    /// - Throw an exception if no files were found.
    if(fileList.empty())
    {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GENERIC, "Invalid Initialization Data", TsInitializationException,
            NO_FILE_MSG, fileDirectory);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] pathName    (--) File name.
/// @returns   std::string (--) Returns the file extension.
/// @details   Private method used to extract out the file extension.
///////////////////////////////////////////////////////////////////////////////////////////////////
std::string XmlFileSearch::obtainExt(
    const std::string pathName)
{
    std::string ext = "";

    //Finds the last period character of the file name
    if((pathName.compare(".")) != static_cast<const int>(std::string::npos))
    {
        int period = pathName.find_last_of(".");

        //Use  + 1 so will not include the period.
        ext = pathName.substr(period + 1);
    }

    return ext;
}
