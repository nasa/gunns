#ifndef UtSensorBooleanAi_EXISTS
#define UtSensorBooleanAi_EXISTS

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

#include "common/sensors/SensorBooleanAi.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from SensorBooleanAi and befriend UtSensorBooleanAi.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlySensorBooleanAi : public SensorBooleanAi
{
    public:
        FriendlySensorBooleanAi();
        virtual ~FriendlySensorBooleanAi();
        friend class UtSensorBooleanAi;
};
inline FriendlySensorBooleanAi::FriendlySensorBooleanAi() : SensorBooleanAi() {}
inline FriendlySensorBooleanAi::~FriendlySensorBooleanAi() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Unit tests for TsSensorBase
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtSensorBooleanAi : public CppUnit::TestFixture {

public:
    UtSensorBooleanAi(void);
    virtual ~UtSensorBooleanAi(void);
    void setUp();
    void tearDown();
    void testConfigData();
    void testInputData();
    void testConstruction();
    void testInitialize();
    void testInitializeExceptions();
    void testAccessors();
    void testUpdateNominal();
    void testSense();

private:
    CPPUNIT_TEST_SUITE(UtSensorBooleanAi);
    CPPUNIT_TEST(testConfigData);
    CPPUNIT_TEST(testInputData);
    CPPUNIT_TEST(testConstruction);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testInitializeExceptions);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testUpdateNominal);
    CPPUNIT_TEST(testSense);
    CPPUNIT_TEST_SUITE_END();

    FriendlySensorBooleanAi    tArticle;                   /**< (--) Test article */
    std::string                tName;                      /**< (--) Nominal article name */

    /// @brief Copy constructor is not implemented.
    UtSensorBooleanAi(const UtSensorBooleanAi &rhs);

    /// @brief Assignment operator is not implemented.
    UtSensorBooleanAi& operator= (const UtSensorBooleanAi &rhs);
};

/// @}

#endif //UtSensorBooleanAi_EXISTS
