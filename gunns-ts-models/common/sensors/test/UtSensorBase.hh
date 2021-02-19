#ifndef UtSensorBase_EXISTS
#define UtSensorBase_EXISTS

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

#include "common/sensors/SensorBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from SensorBase and befriend UtSensorBase.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlySensorBase : public SensorBase
{
    public:
        FriendlySensorBase();
        virtual ~FriendlySensorBase();
        friend class UtSensorBase;
        virtual void update(const double);
    protected:
        virtual void processInput();
};
inline FriendlySensorBase::FriendlySensorBase() : SensorBase() {}
inline FriendlySensorBase::~FriendlySensorBase() {}
inline void FriendlySensorBase::update(const double) {}
inline void FriendlySensorBase::processInput() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Unit tests for SensorBase
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtSensorBase : public CppUnit::TestFixture {

public:
    UtSensorBase(void);
    virtual ~UtSensorBase( void );
    void setUp();
    void tearDown();
    void testConfigData();
    void testInputData();
    void testConstruction();
    void testInitialize();
    void testInitializeExceptions();
    void testAccessors();
    void testDeterminePower();
    void testDegradation();

private:
    CPPUNIT_TEST_SUITE(UtSensorBase);
    CPPUNIT_TEST(testConfigData);
    CPPUNIT_TEST(testInputData);
    CPPUNIT_TEST(testConstruction);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testInitializeExceptions);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testDeterminePower);
    CPPUNIT_TEST(testDegradation);
    CPPUNIT_TEST_SUITE_END();

    FriendlySensorBase         tArticle;                   /**< (--) Test article */
    std::string                tName;                      /**< (--) Nominal article name */

    /// @brief Copy constructor is not implemented.
    UtSensorBase(const UtSensorBase &rhs);

    /// @brief Assignment operator is not implemented.
    UtSensorBase& operator= (const UtSensorBase &rhs);
};

/// @}

#endif //UtSensorBase_EXISTS
