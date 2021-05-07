#ifndef UtGunnsElectBatteryCell_EXISTS
#define UtGunnsElectBatteryCell_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_ELECTRICAL_BATTERY_CELL    GUNNS Electrical Battery Cell Unit Test
/// @ingroup  UT_GUNNS_ELECTRICAL_BATTERY
///
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Electrical Battery Cell model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/electrical/Batt/GunnsElectBatteryCell.hh"
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectBatteryCell and befriend UtGunnsElectBatteryCell.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectBatteryCell : public GunnsElectBatteryCell
{
    public:
        FriendlyGunnsElectBatteryCell();
        virtual ~FriendlyGunnsElectBatteryCell();
        friend class UtGunnsElectBatteryCell;
};

inline FriendlyGunnsElectBatteryCell::FriendlyGunnsElectBatteryCell() : GunnsElectBatteryCell() {};
inline FriendlyGunnsElectBatteryCell::~FriendlyGunnsElectBatteryCell() {}

// Forward class declarations.
class TsLinearInterpolator;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Battery cell model unit tests.
////
/// @details  This class provides the unit tests for the model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectBatteryCell: public CppUnit::TestFixture
{
    public:
        UtGunnsElectBatteryCell();
        virtual ~UtGunnsElectBatteryCell();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testUpdate();
        void testUpdateMalfOpen();
        void testUpdateMalfShort();
        void testUpdateMalfBoth();
        void testUpdateMalfCapacity();
        void testUpdateMalfRunaway();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsElectBatteryCell);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testUpdateMalfOpen);
        CPPUNIT_TEST(testUpdateMalfShort);
        CPPUNIT_TEST(testUpdateMalfBoth);
        CPPUNIT_TEST(testUpdateMalfCapacity);
        CPPUNIT_TEST(testUpdateMalfRunaway);
        CPPUNIT_TEST_SUITE_END();
        GunnsElectBatteryCellConfigData* tConfigData;                 /**< (--)     Pointer to nominal config data. */
        GunnsElectBatteryCellInputData*  tInputData;                  /**< (--)     Pointer to nominal input data. */
        FriendlyGunnsElectBatteryCell*   tArticle;                    /**< (--)     Pointer to test article. */
        double                           tResistance;                 /**< (ohm)    Nominal config data. */
        double                           tMaxCapacity;                /**< (amp*hr) Nominal config data. */
        bool                             tMalfOpenCircuit;            /**< (--)     Nominal input data. */
        bool                             tMalfShortCircuit;           /**< (--)     Nominal input data. */
        bool                             tMalfCapacityFlag;           /**< (--)     Nominal input data. */
        double                           tMalfCapacityValue;          /**< (amp*hr) Nominal input data. */
        bool                             tMalfThermalRunawayFlag;     /**< (--)     Nominal input data. */
        double                           tMalfThermalRunawayDuration; /**< (s)      Nominal input data. */
        double                           tSoc;                        /**< (--)     Nominal input data. */
        std::string                      tName;                       /**< (--)     Nominal initialization data. */
        TsLinearInterpolator*            tSocVocTable;                /**< (--)     Nominal run-time data. */
        static int                       TEST_ID;                     /**< (--)     Test identification number. */
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsElectBatteryCell(const UtGunnsElectBatteryCell& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsElectBatteryCell& operator =(const UtGunnsElectBatteryCell& that);
};

///@}

#endif
