/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
     (TsHsStringMngr.o)
     (TsHsMsgWrapper.o)
 )
 *******************************************************************************/
#include <iostream>
#include <iomanip>  // For stream insertion operator testing

#include "TsHsStringMngr.hh"
#include "TsHsMsgWrapper.hh"
#include "UtTsHsMsgWrapper.hh"

void UtTsHsMsgWrapper::setUp()
{
    mStringMngr = new TsHsStringMngr();
    mStringMngr->registerSelf();
}

void UtTsHsMsgWrapper::tearDown()
{
    delete mStringMngr;
}

void UtTsHsMsgWrapper::testMacroFunctions()
{
    // Test tsHsSendMsgFileLine(...string...)
    tsHsSendMsgFileLineC(__FILE__, 123, "function", TS_HS_WARNING, "TEST", "message_field");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|function|2|TEST|00000|0|0|message_field"), mStringMngr->str());

    // Test the hsSendMsg(...) macro.
    // This test has been placed at the bottom to avoid the #line directives
    // affecting anything else, since cppunit uses __LINE__ to report lines numbers
    // for test failures.

    // Test hsSendMsg(severity,subsys,mtext) form of macro call
#line 123
    hsSendMsg(TS_HS_DEBUG, "TEST", "new_message_field");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|testMacroFunctions|0|TEST|00000|0|0|new_message_field"), mStringMngr->str());
}

