#ifndef UtGunnsElectPvString_EXISTS
#define UtGunnsElectPvString_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_PHOTOVOLTAIC_STRING    Photovoltaic String Model Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2024 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Photovoltaic String Model
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/SolarArray/GunnsElectPvString.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectPvStringInputData and befriend UtGunnsElectPvString.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectPvStringInputData : public GunnsElectPvStringInputData
{
    public:
        FriendlyGunnsElectPvStringInputData() {;}
        virtual ~FriendlyGunnsElectPvStringInputData() {;}
        friend class UtGunnsElectPvString;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectPvString and befriend UtGunnsElectPvString.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectPvString : public GunnsElectPvString
{
    public:
        FriendlyGunnsElectPvString() {;}
        FriendlyGunnsElectPvString(const GunnsElectPvStringConfigData* configData,
                                   const GunnsElectPvStringInputData*  inputData)
            : GunnsElectPvString(configData, inputData) {;}
        virtual ~FriendlyGunnsElectPvString() {;}
        friend class UtGunnsElectPvString;
        friend class UtGunnsElectPvSection;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectPvCellEquivCircuit and befriend UtGunnsElectPvString.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectPvCellEquivCircuit : public GunnsElectPvCellEquivCircuit
{
    public:
        FriendlyGunnsElectPvCellEquivCircuit() {;}
        virtual ~FriendlyGunnsElectPvCellEquivCircuit() {;}
        friend class UtGunnsElectPvString;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Photovoltaic String Model unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectPvString within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectPvString: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectPvString();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectPvString();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests cell config data.
        void testCellConfig();
        /// @brief  Tests cell equivalent properties data.
        void testCellEquivProps();
        /// @brief  Tests load state data.
        void testLoadState();
        /// @brief  Tests config data.
        void testConfig();
        /// @brief  Tests input data.
        void testInput();
        /// @brief  Tests input data overrides.
        void testInputOverrides();
        /// @brief  Tests construction methods.
        void testConstruction();
        /// @brief  Tests the initialization method with nominal values.
        void testNominalInitialization();
        /// @brief  Tests the initialization method with errors.
        void testInitializationErrors();
        /// @brief  Tests number of active cells & bypassed groups.
        void testActiveCells();
        /// @brief  Tests the step method.
        void testStep();
        /// @brief  Tests the getter and setter methods.
        void testAccessors();
        /// @brief  Tests the loadAtMpp method.
        void testLoadAtMpp();
        /// @brief  Tests the loadAtPower method.
        void testLoadAtPower();
        /// @brief  Tests the loadAtVoltage method.
        void testLoadAtVoltage();
        /// @brief  Tests the loadAtConductance method.
        void testLoadAtConductance();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectPvStringric unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectPvString);
        CPPUNIT_TEST(testCellConfig);
        CPPUNIT_TEST(testCellEquivProps);
        CPPUNIT_TEST(testLoadState);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testInputOverrides);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testActiveCells);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testLoadAtMpp);
        CPPUNIT_TEST(testLoadAtPower);
        CPPUNIT_TEST(testLoadAtVoltage);
        CPPUNIT_TEST(testLoadAtConductance);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        std::string                   tName;                        /**< (--)   Test article name. */
        GunnsElectPvStringConfigData* tConfigData;                  /**< (--)   Pointer to config data. */
        GunnsElectPvStringInputData*  tInputData;                   /**< (--)   Pointer to input data. */
        FriendlyGunnsElectPvString*   tArticle;                     /**< (--)   Pointer to article under test. */
        double                        tCellSurfaceArea;             /**< (m2)   Nominal config data. */
        double                        tCellEfficiency;              /**< (--)   Nominal config data. */
        double                        tCellSeriesResistance;        /**< (ohm)  Nominal config data. */
        double                        tCellShuntResistance;         /**< (ohm)  Nominal config data. */
        double                        tCellOpenCircuitVoltage;      /**< (v)    Nominal config data. */
        double                        tCellRefTemperature;          /**< (K)    Nominal config data. */
        double                        tCellTemperatureVoltageCoeff; /**< (1/K)  Nominal config data. */
        double                        tCellTemperatureCurrentCoeff; /**< (1/K)  Nominal config data. */
        double                        tBlockingDiodeVoltageDrop;    /**< (v)    Nominal config data. */
        double                        tBypassDiodeVoltageDrop;      /**< (v)    Nominal config data. */
        int                           tBypassDiodeInterval;         /**< (--)   Nominal config data. */
        int                           tNumCells;                    /**< (--)   Nominal config data. */
        double                        tPhotoFlux;                   /**< (W/m2) Nominal input data. */
        double                        tSourceExposedFraction;       /**< (--)   Nominal input data. */
        double                        tTemperature;                 /**< (K)    Nominal input data. */
        static int                    TEST_ID;                      /**< (--)   Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvString(const UtGunnsElectPvString& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvString& operator =(const UtGunnsElectPvString& that);
};

///@}

#endif
