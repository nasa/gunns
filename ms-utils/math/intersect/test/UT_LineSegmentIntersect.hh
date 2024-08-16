/********************************* TRICK HEADER *******************************
PURPOSE:
    (Unit Test Line Segment Intersect)

ASSUMPTIONS AND LIMITATIONS:
    ((TBD))

LIBRARY DEPENDENCY:
    (
       (math/intersect/LineSegmentIntersect.o)
    )

PROGRAMMERS:
    ( ((Shaun Williams) (L-3) (April 2011) (TS21)) )
*******************************************************************************/

#ifndef UT_LineSegmentIntersect_EXISTS
#define UT_LineSegmentIntersect_EXISTS

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "math/intersect/LineSegmentIntersect.hh"

#include "software/unit_test/TsAssert.hh"

class UT_LineSegmentIntersect: public CppUnit::TestFixture
{

    private:

        CPPUNIT_TEST_SUITE(UT_LineSegmentIntersect);

        CPPUNIT_TEST(triangleIntersectRayAndSegment);
        CPPUNIT_TEST(triangleIntersectRayOnly);
        CPPUNIT_TEST(triangleNoIntersect);

        CPPUNIT_TEST(cylinderIntersectRayAndSegment);
        CPPUNIT_TEST(cylinderIntersectRayOnly);
        CPPUNIT_TEST(cylinderNoIntersect);

        CPPUNIT_TEST(sphereIntersectRayAndSegment);
        CPPUNIT_TEST(sphereIntersectRayOnly);
        CPPUNIT_TEST(sphereNoIntersect);

        CPPUNIT_TEST(spheroidIntersectRayAndSegment);
        CPPUNIT_TEST(spheroidIntersectRayOnly);
        CPPUNIT_TEST(spheroidNoIntersect);

        CPPUNIT_TEST_SUITE_END();

    public:

        void triangleIntersectRayAndSegment();
        void triangleIntersectRayOnly();
        void triangleNoIntersect();

        void cylinderIntersectRayAndSegment();
        void cylinderIntersectRayOnly();
        void cylinderNoIntersect();

        void sphereIntersectRayAndSegment();
        void sphereIntersectRayOnly();
        void sphereNoIntersect();

        void spheroidIntersectRayAndSegment();
        void spheroidIntersectRayOnly();
        void spheroidNoIntersect();
};

#endif
