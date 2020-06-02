#ifndef UtGunnsElectPvRegConv_EXISTS
#define UtGunnsElectPvRegConv_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_PHOTOVOLTAIC_CONVERTER_REGULATOR    Photovoltaic Array Converter Regulator Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Photovoltaic Array Converter Regulator Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/SolarArray/GunnsElectPvRegConv.hh"
#include "core/GunnsSensorAnalogWrapper.hh"
#include "UtGunnsElectPvArray.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectPvRegConv and befriend UtGunnsElectPvRegConv.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectPvRegConv : public GunnsElectPvRegConv
{
    public:
        FriendlyGunnsElectPvRegConv() {;}
        virtual ~FriendlyGunnsElectPvRegConv() {;}
        friend class UtGunnsElectPvRegConv;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Photovoltaic Array Converter Regulator Link unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectPvRegConv within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectPvRegConv: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectPvRegConv();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectPvRegConv();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests config data.
        void testConfig();
        /// @brief  Tests input data.
        void testInput();
        /// @brief  Tests construction methods.
        void testConstruction();
        /// @brief  Tests the initialization method with nominal values.
        void testNominalInitialization();
        /// @brief  Tests the initialization method with errors.
        void testInitializationErrors();
        /// @brief  Tests the restart method.
        void testRestart();
        /// @brief  Tests the step method.
        void testStep();
        /// @brief  Tests the minorStep method.
        void testMinorStep();
        /// @brief  Tests the getter and setter methods.
        void testAccessors();
        /// @brief  Tests the confirmSolutionAcceptable method.
        void testConfirmSolutionAcceptable();
        /// @brief  Tests the computeFlows method.
        void testComputeFlows();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectPvRegConv unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectPvRegConv);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testMinorStep);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testConfirmSolutionAcceptable);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        enum {N_NODES = 2};
        std::vector<GunnsBasicLink*>   tLinks;                 /**< (--)  Network links vector. */
        GunnsBasicNode                 tNodes[N_NODES];        /**< (--)  Network nodes. */
        GunnsNodeList                  tNodeList;              /**< (--)  Network nodes list. */
        int                            tPort0;                 /**< (--)  Port 0 node mapping. */
        std::string                    tName;                  /**< (--)  Test article name. */
        GunnsElectPvRegConvConfigData* tConfigData;            /**< (--)  Pointer to config data. */
        GunnsElectPvRegConvInputData*  tInputData;             /**< (--)  Pointer to input data. */
        FriendlyGunnsElectPvRegConv*   tArticle;               /**< (--)  Pointer to article under test. */
        double                         tVoltageConvLimit;      /**< (v)   Nominal config data. */
        double                         tVoltageConvEfficiency; /**< (--)  Nominal config data. */
        GunnsSensorAnalogWrapper       tSensorIin;             /**< (--)  Sensor object. */
        GunnsSensorAnalogWrapper       tSensorVin;             /**< (--)  Sensor object. */
        GunnsSensorAnalogWrapper       tSensorIout;            /**< (--)  Sensor object. */
        GunnsSensorAnalogWrapper       tSensorVout;            /**< (--)  Sensor object. */
        double                         tInOverCurrentTrip;     /**< (amp) Nominal config data. */
        double                         tInOverVoltageTrip;     /**< (v)   Nominal config data. */
        double                         tOutOverCurrentTrip;    /**< (amp) Nominal config data. */
        double                         tOutOverVoltageTrip;    /**< (v)   Nominal config data. */
        double                         tOutUnderVoltageTrip;   /**< (v)   Nominal config data. */
        unsigned int                   tTripPriority;          /**< (v)   Nominal config data. */
        FriendlyGunnsElectPvArray*     tArray;                 /**< (--)  Test PV array. */
        GunnsElectPvArrayConfigData*   tArrayConfig;           /**< (--)  Test PV array config data. */
        GunnsElectPvArrayInputData*    tArrayInput;            /**< (--)  Test PV array input data. */
        double                         tVoltageSetpoint;       /**< (v)   Nominal input data. */
        double                         tVoltageSetpointDelta;  /**< (v)   Nominal input data. */
        bool                           tPowered;               /**< (--)  Nominal input data. */
        bool                           tEnabled;               /**< (--)  Nominal input data. */
        static int                     TEST_ID;                /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvRegConv(const UtGunnsElectPvRegConv& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvRegConv& operator =(const UtGunnsElectPvRegConv& that);
};

///@}

#endif
