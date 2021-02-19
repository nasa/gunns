#ifndef UtSensorBooleanBi_EXISTS
#define UtSensorBooleanBi_EXISTS

/**
@defgroup  <classname>  some text goes here
@ingroup   <ingroup>

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
 - ()

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ()

 PROGRAMMERS:
 - ((Jason Harvey) (L-3 Communications) (2012-04) (TS21) (Initial))
@{
*/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "common/sensors/SensorBooleanBi.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from SensorBooleanBi and befriend UtSensorBooleanBi.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlySensorBooleanBi : public SensorBooleanBi
{
    public:
        FriendlySensorBooleanBi();
        virtual ~FriendlySensorBooleanBi();
        friend class UtSensorBooleanBi;
};
inline FriendlySensorBooleanBi::FriendlySensorBooleanBi() : SensorBooleanBi() {}
inline FriendlySensorBooleanBi::~FriendlySensorBooleanBi() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Unit tests for TsSensorBase
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtSensorBooleanBi : public CppUnit::TestFixture {

public:
    UtSensorBooleanBi(void);
    virtual ~UtSensorBooleanBi(void);
    void setUp();
    void tearDown();
    void testConfigData();
    void testInputData();
    void testConstruction();
    void testInitialize();
    void testInitializeExceptions();
    void testAccessors();
    void testUpdateNominal();
    void testUpdateNoPower();
    void testMalfFailTo();
    void testMalfStuck();
    void testMalfPerfect();
    void testMalfInteractions();
    void testSense();

private:
    CPPUNIT_TEST_SUITE(UtSensorBooleanBi);
    CPPUNIT_TEST(testConfigData);
    CPPUNIT_TEST(testInputData);
    CPPUNIT_TEST(testConstruction);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testInitializeExceptions);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testUpdateNominal);
    CPPUNIT_TEST(testUpdateNoPower);
    CPPUNIT_TEST(testMalfFailTo);
    CPPUNIT_TEST(testMalfStuck);
    CPPUNIT_TEST(testMalfPerfect);
    CPPUNIT_TEST(testMalfInteractions);
    CPPUNIT_TEST(testSense);
    CPPUNIT_TEST_SUITE_END();

    FriendlySensorBooleanBi    tArticle;                   /**< (--) Test article */
    std::string                tName;                      /**< (--) Nominal article name */

    /// @brief Copy constructor is not implemented.
    UtSensorBooleanBi(const UtSensorBooleanBi &rhs);

    /// @brief Assignment operator is not implemented.
    UtSensorBooleanBi& operator= (const UtSensorBooleanBi &rhs);
};

/// @}

#endif //UtSensorBooleanBi_EXISTS
