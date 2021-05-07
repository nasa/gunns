#ifndef UtGunnsElectBattery_EXISTS
#define UtGunnsElectBattery_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_ELECTRICAL_BATTERY_LINK    GUNNS Electrical Battery Unit Test
/// @ingroup  UT_GUNNS_ELECTRICAL_BATTERY
///
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Electrical Battery model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/electrical/Batt/GunnsElectBattery.hh"
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectBattery and befriend UtGunnsElectBattery.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectBattery : public GunnsElectBattery
{
    public:
        FriendlyGunnsElectBattery();
        virtual ~FriendlyGunnsElectBattery();
        friend class UtGunnsElectBattery;
};

inline FriendlyGunnsElectBattery::FriendlyGunnsElectBattery() : GunnsElectBattery() {};
inline FriendlyGunnsElectBattery::~FriendlyGunnsElectBattery() {}

// Forward class declarations.
class TsLinearInterpolator;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Battery model unit tests.
////
/// @details  This class provides the unit tests for the model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectBattery: public CppUnit::TestFixture
{
    public:
        UtGunnsElectBattery();
        virtual ~UtGunnsElectBattery();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRestart();
        void testUpdateStateParallel();
        void testUpdateStateSeries();
        void testUpdateFlux();
        void testThermalRunaway();
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsElectBattery);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testUpdateStateParallel);
        CPPUNIT_TEST(testUpdateStateSeries);
        CPPUNIT_TEST(testUpdateFlux);
        CPPUNIT_TEST(testThermalRunaway);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        enum {N_NODES = 2};
        GunnsElectBatteryConfigData* tConfigData;                 /**< (--)     Pointer to nominal config data. */
        GunnsElectBatteryInputData*  tInputData;                  /**< (--)     Pointer to nominal input data. */
        FriendlyGunnsElectBattery*   tArticle;                    /**< (--)     Pointer to test article. */
        std::vector<GunnsBasicLink*> tLinks;                      /**< (--)     Links vector. */
        std::string                  tName;                       /**< (--)     Nominal name. */
        GunnsBasicNode               tNodes[N_NODES];             /**< (--)     Nominal connected nodes. */
        GunnsNodeList                tNodeList;                   /**< (--)     Network node structure. */
        int                          tPort0;                      /**< (--)     Nominal inlet port index. */
        int                          tPort1;                      /**< (--)     Nominal outlet port index. */
        int                          tNumCells;                   /**< (--)     Nominal config data. */
        bool                         tCellsInParallel;            /**< (--)     Nominal config data. */
        double                       tCellResistance;             /**< (ohm)    Nominal config data. */
        double                       tInterconnectResistance;     /**< (ohm)    Nominal config data. */
        double                       tMaxCapacity;                /**< (amp*hr) Nominal config data. */
        TsLinearInterpolator*        tSocVocTable;                /**< (--)     Nominal config data. */
        bool                         tMalfBlockageFlag;           /**< (--)     Nominal input data. */
        double                       tMalfBlockageValue;          /**< (--)     Nominal input data. */
        bool                         tMalfThermalRunawayFlag;     /**< (--)     Nominal input data. */
        double                       tMalfThermalRunawayDuration; /**< (s)      Nominal input data. */
        double                       tMalfThermalRunawayInterval; /**< (s)      Nominal input data. */
        double                       tSoc;                        /**< (--)     Nominal input data. */
        static int                   TEST_ID;                     /**< (--)     Test identification number. */
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsElectBattery(const UtGunnsElectBattery& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsElectBattery& operator =(const UtGunnsElectBattery& that);
};

///@}

#endif
