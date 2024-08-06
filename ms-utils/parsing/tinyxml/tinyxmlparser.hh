/*
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
 (This file was created by the NASA GUNNS team just for the ms-utils port of tinyxml, in order to
  provide a .hh file to go along with tinyxmlparser.cpp.  This makes the GUNNS lib makefiles happy.)

LIBRARY DEPENDENCY:
 ((tinyxmlparser.o))
 */

#ifndef TIXML_PARSER_INCLUDED
#define TIXML_PARSER_INCLUDED

#include "tinyxml.hh"

class TiXmlParsingData
{
    friend class TiXmlDocument;
  public:
    void Stamp( const char* now, TiXmlEncoding encoding );

    const TiXmlCursor& Cursor() { return cursor; }

  private:
    // Only used by the document!
    TiXmlParsingData( const char* start, int _tabsize, int row, int col );

    TiXmlCursor     cursor;
    const char*     stamp;
    int             tabsize;
};

#endif // TIXML_PARSER_INCLUDED
