#ifndef UTEXCEPTIONS_H_
#define UTEXCEPTIONS_H_

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_UTILITIES_SOFTWARE_EXCEPTIONS_EXCEPTIONs Exceptions Unit Tests
/// @ingroup   UT_UTILITIES_SOFTWARE_EXCEPTIONS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit test class for exceptions.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "TsBaseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Exceptions fit unit tests.
////
/// @details  This class provides the unit tests for the Exceptions classes within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////

class UtExceptions: public CppUnit::TestFixture {

    private:

    CPPUNIT_TEST_SUITE(UtExceptions);
            CPPUNIT_TEST(testOutOfBoundsException);
            CPPUNIT_TEST(testNumericalException);
            CPPUNIT_TEST(testUnknownException);
            CPPUNIT_TEST(testConfigurationException);
            CPPUNIT_TEST(testInitializationException);
            CPPUNIT_TEST(testParseException);
            CPPUNIT_TEST(testInitializationOperatorEqual);
            CPPUNIT_TEST(testConfigurationOperatorEqual);
            CPPUNIT_TEST(testUnknownOperatorEqual);
            CPPUNIT_TEST(testParseOperatorEqual);
            CPPUNIT_TEST(testNumericalOperatorEqual);
            CPPUNIT_TEST(testOutOfBoundsOperatorEqual);
            CPPUNIT_TEST(testInvalidStateOperatorEqual);
            CPPUNIT_TEST(testHsAndException);
            CPPUNIT_TEST(testTsHsMsgClassAndException);
            CPPUNIT_TEST(testTsInitializationExceptionWhatMethod);
            CPPUNIT_TEST(testTsNumericalExceptionWhatMethod);

        CPPUNIT_TEST_SUITE_END();

        void getException(int flag) throw (TsBaseException);
        void validateNumericalException(const TsBaseException* be);
        void validateUnknownException(const TsBaseException* be);
        void validateOutOfBoundsException(const TsBaseException* be);
        void validateConfigurationException(const TsBaseException* be);
        void validateInitializationException(const TsBaseException* be);
        void validateParseException(const TsBaseException* be);


    public:

        void setUp();
        void tearDown();

        void testOutOfBoundsException();
        void testUnknownException();
        void testNumericalException();
        void testConfigurationException();
        void testInitializationException();
        void testParseException();
        void testInitializationOperatorEqual();
        void testConfigurationOperatorEqual();
        void testUnknownOperatorEqual();
        void testParseOperatorEqual();
        void testOutOfBoundsOperatorEqual();
        void testNumericalOperatorEqual();
        void testInvalidStateOperatorEqual();

        void testHsAndException();
        void testTsHsMsgClassAndException();
        void testTsInitializationExceptionWhatMethod();
        void testTsNumericalExceptionWhatMethod();

};

/// @}

#endif
