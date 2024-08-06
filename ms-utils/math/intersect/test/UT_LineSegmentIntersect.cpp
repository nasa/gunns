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

#include "UT_LineSegmentIntersect.hh"

// truth data created from Blender 3D
// and can be viewed in the following files:
//  - data/triangle-test.blend
//  - data/cylinder-test.blend
//  - data/sphere-test.blend
//  - data/spheroid-test.blend

// for each shape, we test three cases:
//      1) intersect ray but not segment
//      2) intersect ray and segment
//      3) no intersection

// starting point for all line segments used in testing (end points are defined in each test function)
static const double startPoint[3] = { 3.73, 3.94, 4.13};

// triangle definition
static const double triangleA[3] = { 0.15, 1.33, -1.18};
static const double triangleB[3] = { 2.01, 0.11, 2.12};
static const double triangleC[3] = { -1.96, -1.12, -0.80};

// cylinder definition
static const double cylinderA[3] = { 0.297, 1.558, 2.235};
static const double cylinderB[3] = { -0.133, 0.187, 0.844};
static const double cylinderR = 0.72;

// sphere definition
static const double sphereC[3] = { -0.25, -0.19, 0.55};
static const double sphereR = 1.0;

// spheroid definition
static const double spheroidC[3] = { 0.84, -0.45, 1.10};
static const double spheroidR[3] = { 0.50, 0.75, 0.60};

// only accurate to the hundredths place
// because truth data for the intersection points were found manually in Blender
static const double epsilon = 0.005;

void UT_LineSegmentIntersect::
        triangleIntersectRayAndSegment()
{
    double endPoint[3] = { -0.81, -0.77, -1.16};
    double x[3];

    CPPUNIT_ASSERT(
            LineSegmentIntersect::triangle(
                startPoint,
                endPoint,
                triangleA,
                triangleB,
                triangleC,
                x));

    double intersect[3] = { 0.190, 0.268, 0.005};
    for (int i=0; i<3; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(intersect[i], x[i], epsilon);
    }
}

void UT_LineSegmentIntersect::
        triangleIntersectRayOnly()
{
    double endPoint[3] = { 0.45, 0.80, 0.33};
    CPPUNIT_ASSERT(
            !LineSegmentIntersect::triangle(
                startPoint,
                endPoint,
                triangleA,
                triangleB,
                triangleC));
}

void UT_LineSegmentIntersect::
        triangleNoIntersect()
{
    double endPoint[3] = { 0.74, 0.64, -1.16};
    CPPUNIT_ASSERT(
            !LineSegmentIntersect::triangle(
                startPoint,
                endPoint,
                triangleA,
                triangleB,
                triangleC));
}

void UT_LineSegmentIntersect::
        cylinderIntersectRayAndSegment()
{
    double endPoint[3] = { -1.14, -1.09, -0.40};
    double x[3];

    CPPUNIT_ASSERT(
            LineSegmentIntersect::cylinder(
                startPoint,
                endPoint,
                cylinderA,
                cylinderB,
                cylinderR,
                x));

    double intersect[3] = { 0.790, 0.905, 1.396};
    for (int i=0; i<3; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(intersect[i], x[i], epsilon);
    }
}

void UT_LineSegmentIntersect::
        cylinderIntersectRayOnly()
{
    double endPoint[3] = { 0.45, 0.80, 0.33};
    CPPUNIT_ASSERT(
            !LineSegmentIntersect::cylinder(
                startPoint,
                endPoint,
                cylinderA,
                cylinderB,
                cylinderR));
}

void UT_LineSegmentIntersect::
        cylinderNoIntersect()
{
    double endPoint[3] = { -0.14, 1.28, 0.25};
    CPPUNIT_ASSERT(
            !LineSegmentIntersect::cylinder(
                startPoint,
                endPoint,
                cylinderA,
                cylinderB,
                cylinderR));
}

void UT_LineSegmentIntersect::
        sphereIntersectRayAndSegment()
{
    double endPoint[3] = { -1.14, -1.09, -0.40};
    double x[3];
    CPPUNIT_ASSERT(
            LineSegmentIntersect::sphere(
                startPoint,
                endPoint,
                sphereC,
                sphereR,
                x));

    double intersect[3] = { 0.361, 0.461, 0.997};
    for (int i=0; i<3; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(intersect[i], x[i], epsilon);
    }
}

void UT_LineSegmentIntersect::
        sphereIntersectRayOnly()
{
    double endPoint[3] = { 0.51, 0.59, 1.14};
    CPPUNIT_ASSERT(
            !LineSegmentIntersect::sphere(
                startPoint,
                endPoint,
                sphereC,
                sphereR));
}

void UT_LineSegmentIntersect::
        sphereNoIntersect()
{
    double endPoint[3] = { 0.61, 1.13, 0.18};
    CPPUNIT_ASSERT(
            !LineSegmentIntersect::sphere(
                startPoint,
                endPoint,
                sphereC,
                sphereR));
}

void UT_LineSegmentIntersect::
        spheroidIntersectRayAndSegment()
{
    double endPoints[][3] = {
        { 0.43, -0.81, 1.16},
        { -0.528, -1.364, 0.312 },
        { -0.043, -1.031, 0.158 },
        { -0.240, -0.899, 0.093 },
        { 0.406, -0.808, 0.052 },
        { 0.100, -0.741, -0.017 },
        { 0.474, -0.989, -0.092 },
        { 0.738, -1.325, 0.096 },
        { 0.848, -1.558, 0.395 },
        { 0.557, -1.632, 0.732 },
        { 0.697, -1.717, 0.963 },
        { 0.532, -1.697, 1.098 },
        { -1.650, -3.656, -0.684 },
    };

    for (int i=0; i<sizeof(endPoints) / sizeof(double[3]); ++i) {
        double x[3];
        CPPUNIT_ASSERT(
                LineSegmentIntersect::spheroid(
                    startPoint,
                    endPoints[i],
                    spheroidC,
                    spheroidR[0],
                    spheroidR[1],
                    spheroidR[2],
                    x));

        if (i==0) {
            // currently only checking intersection point of the first case
            double intersect[3] = { 0.933, -0.087, 1.612};
            for (int j=0; j<3; ++j) {
                CPPUNIT_ASSERT_DOUBLES_EQUAL(intersect[i], x[i], epsilon);
            }
        }
    }
}

void UT_LineSegmentIntersect::
        spheroidIntersectRayOnly()
{
    double endPoints[][3] = {
        { 1.28, 0.26, 1.71},
        { 1.134, 0.288, 1.829 },
        { 1.062, 0.461, 1.634 },
        { 1.181, 0.694, 1.257 },
        { 1.549, 0.382, 1.459 },
        { 1.311, 0.828, 1.159 },
        { 1.286, 0.648, 1.417 },
        { 1.059, 0.360, 1.400 },
    };

    for (int i=0; i<sizeof(endPoints) / sizeof(double[3]); ++i) {
        CPPUNIT_ASSERT(
                !LineSegmentIntersect::spheroid(
                    startPoint,
                    endPoints[i],
                    spheroidC,
                    spheroidR[0],
                    spheroidR[1],
                    spheroidR[2]));
    }

}

void UT_LineSegmentIntersect::
        spheroidNoIntersect()
{
    double endPoints[][3] = {
        { 1.85, 0.20, 1.39 },
        { -0.265, -0.774, 0.414 },
        { 0.126, -0.732, -0.091 },
        { 0.499, -0.961, -0.136 },
        { 0.546, -1.707, 1.126 },
        { 5.187, 6.573, 5.702 },
        { -3.698, 6.283, 2.271 },
    };

    for (int i=0; i<sizeof(endPoints) / sizeof(double[3]); ++i) {
        CPPUNIT_ASSERT(
                !LineSegmentIntersect::spheroid(
                    startPoint,
                    endPoints[i],
                    spheroidC,
                    spheroidR[0],
                    spheroidR[1],
                    spheroidR[2]));
    }
}
