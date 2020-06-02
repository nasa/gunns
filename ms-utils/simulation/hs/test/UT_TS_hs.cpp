/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
(
    (TsHsStringMngr.o)
    (TS_hs_msg.o)
)
*******************************************************************************/

#include "TsHsStringMngr.hh"
#include "TS_hs_msg.h"

#include "UT_TS_hs.hh"

void UT_TS_hs::setUp() {
    mngr = new TsHsStringMngr();
    mngr->registerSelf();
}

void UT_TS_hs::tearDown() {
    delete mngr;
}

void UT_TS_hs::send() {
    double x = 3.14;
    int n = 1024;
    const char * s = "fhqwhgads";
    TS_hs_fileline_msg(__FILE__, 123, TS_HS_INFO, "TEST", "%d%s%.2f", n, s, x);
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123||1|TEST|00000|0|0|1024fhqwhgads3.14"), mngr->str());
}

// This test has been placed at the bottom to avoid the #line directives
// affecting anything else, since cppunit uses __LINE__ to report lines numbers
// for test failures.
void UT_TS_hs::sendMacro() {
#line 123
    TS_hs_msg(TS_HS_INFO, "TEST", "");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123||1|TEST|00000|0|0|"), mngr->str());
}
