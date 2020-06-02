#ifndef UtSimCompatibility_EXISTS
#define UtSimCompatibility_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_UTILITIES_SOFTWARE_SIM_COMPATIBILITY   Sim Compatibility Unit Tests
/// @ingroup   UT_UTILITIES_SOFTWARE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the sim compatibility macros.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Base test class for sim compatibility macros.
////////////////////////////////////////////////////////////////////////////////////////////////////
class A
{
    public:
        A(const double x = 1.0) : mX(x) {};
        A(const A& that) : mX(that.mX) {};
        virtual ~A() {};
        A& operator =(const A& that) { mX = that.mX; return *this; };
        virtual double getX() const { return mX; };
        virtual void setX(const double x) { mX = x; };
        double mX; // a double
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Derived test class for sim compatibility macros.
////////////////////////////////////////////////////////////////////////////////////////////////////
class B : public A
{
    public:
        B(const double x = 1.0) : A(x) {};
        B(const B& that) : A(that) {};
        virtual ~B() {};
        B& operator =(const B& that) { A::operator =(that); return *this; };
        virtual double getX() const { return 2.0 * mX; };
        virtual void setX(const double x) { mX = 2.0 * x; };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sim Compatibility macro unit tests.
////
/// @details  This class provides the unit tests for the Sim Compatibility macros within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtSimCompatibility : public CppUnit::TestFixture
{
    private:
        CPPUNIT_TEST_SUITE(UtSimCompatibility);
        CPPUNIT_TEST(testNewPrimObject);
        CPPUNIT_TEST(testNewPrimObjectNamed);
        CPPUNIT_TEST(testNewPrimArray);
        CPPUNIT_TEST(testNewPrimArrayNamed);
        CPPUNIT_TEST(testNewPrimPointerArray);
        CPPUNIT_TEST(testNewPrimPointerArrayNamed);
        CPPUNIT_TEST(testNewPrimMultiPointerArray);
        CPPUNIT_TEST(testNewPrimMultiPointerArrayNamed);
        CPPUNIT_TEST(testStrdup);
        CPPUNIT_TEST(testNewStlObject);
        CPPUNIT_TEST(testNewStlObjectNamed);
        CPPUNIT_TEST(testNewClassObject);
        CPPUNIT_TEST(testNewClassObjectNamed);
        CPPUNIT_TEST(testNewClassArray);
        CPPUNIT_TEST(testNewClassArrayNamed);
        CPPUNIT_TEST(testNewClassArrayArg);
        CPPUNIT_TEST(testNewClassArrayArgNamed);
        CPPUNIT_TEST(testNewClassPointerArray);
        CPPUNIT_TEST(testNewClassPointerArrayNamed);
        CPPUNIT_TEST(testNewClassMultiPointerArray);
        CPPUNIT_TEST(testNewClassMultiPointerArrayNamed);
        CPPUNIT_TEST(testIsAllocated);
        CPPUNIT_TEST(testDeleteObject);
        CPPUNIT_TEST(testDeleteArray);
        CPPUNIT_TEST_SUITE_END();
    public:
        void tearDown();
        void setUp();
        void testNewPrimObject();
        void testNewPrimObjectNamed();
        void testNewPrimArray();
        void testNewPrimArrayNamed();
        void testNewPrimPointerArray();
        void testNewPrimPointerArrayNamed();
        void testNewPrimMultiPointerArray();
        void testNewPrimMultiPointerArrayNamed();
        void testStrdup();
        void testNewStlObject();
        void testNewStlObjectNamed();
        void testNewClassObject();
        void testNewClassObjectNamed();
        void testNewClassArray();
        void testNewClassArrayNamed();
        void testNewClassArrayArg();
        void testNewClassArrayArgNamed();
        void testNewClassPointerArray();
        void testNewClassPointerArrayNamed();
        void testNewClassMultiPointerArray();
        void testNewClassMultiPointerArrayNamed();
        void testIsAllocated();
        void testDeleteObject();
        void testDeleteArray();
};

/// @}

#endif
