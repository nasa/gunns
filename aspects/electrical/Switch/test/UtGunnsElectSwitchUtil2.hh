#ifndef UtGunnsElectSwitchUtil2_EXISTS_
#define UtGunnsElectSwitchUtil2_EXISTS_

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_SWITCH_UTIL_2 Electrical Switch Utility class Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit tests for GunnsElectSwitchUtil2 in the CPPUNIT test framework.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "aspects/electrical/Switch/GunnsElectSwitchUtil2.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectSwitchUtil2 and befriend UtGunnsElectSwitchUtil2.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectSwitchUtil2: public GunnsElectSwitchUtil2 {
public:
    FriendlyGunnsElectSwitchUtil2();
    virtual ~FriendlyGunnsElectSwitchUtil2();
    friend class UtGunnsElectSwitchUtil2;
};

inline FriendlyGunnsElectSwitchUtil2::FriendlyGunnsElectSwitchUtil2() : GunnsElectSwitchUtil2() {}
inline FriendlyGunnsElectSwitchUtil2::~FriendlyGunnsElectSwitchUtil2() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Switch Utility model variant 2 unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Electrical Switch Utility Variant 2
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectSwitchUtil2 : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Electrical Switch Utility Variant 2 unit test.
        UtGunnsElectSwitchUtil2();
        /// @brief    Default destructs this Electrical Switch Utility Variant 2 unit test.
        virtual ~UtGunnsElectSwitchUtil2();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config data.
        void testConfigData();
        /// @brief    Tests input data.
        void testInputData();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests nominal initialization.
        void testInitialization();
        /// @brief    Tests initialization errors.
        void testInitializationErrors();
        /// @brief    Tests runtime input value constraints.
        void testInputConstraints();
        /// @brief    Tests the updateState method.
        void testUpdateState();
        /// @brief    Tests the updateTrips method.
        void testUpdateTrips();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsElectSwitchUtil2);
        CPPUNIT_TEST(testConfigData);
        CPPUNIT_TEST(testInputData);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testInputConstraints);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testUpdateTrips);
        CPPUNIT_TEST_SUITE_END();
        std::string                      tName;                       /**< (1)   Nominal initialization data. */
        float                            tResistance;                 /**< (ohm) Nominal config data. */
        unsigned int                     tTripPriority;               /**< (1)   Nominal config data. */
        bool                             tPosition;                   /**< (1)   Nominal input data. */
        bool                             tPositionCommand;            /**< (1)   Nominal input data. */
        bool                             tResetTripsCommand;          /**< (1)   Nominal input data. */
        float                            tInputUnderVoltageTripLimit; /**< (V)   Nominal input data. */
        float                            tInputUnderVoltageTripReset; /**< (V)   Nominal input data. */
        float                            tInputOverVoltageTripLimit;  /**< (V)   Nominal input data. */
        float                            tInputOverVoltageTripReset;  /**< (V)   Nominal input data. */
        float                            tPosOverCurrentTripLimit;    /**< (amp) Nominal input data. */
        float                            tNegOverCurrentTripLimit;    /**< (amp) Nominal input data. */
        GunnsElectSwitchUtil2ConfigData* tConfigData;                 /**< (1)   Pointer to test config data. */
        GunnsElectSwitchUtil2InputData*  tInputData;                  /**< (1)   Pointer to test input data. */
        FriendlyGunnsElectSwitchUtil2*   tArticle;                    /**< (1)   Pointer to the article under test. */
        static int                       TEST_ID;                     /**< (1)   Test identification number. */

        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtGunnsElectSwitchUtil2(const UtGunnsElectSwitchUtil2&);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        UtGunnsElectSwitchUtil2 &operator = (const UtGunnsElectSwitchUtil2&);

};

/// @}

#endif
