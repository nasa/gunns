#ifndef UtGunnsElectPvString2_EXISTS
#define UtGunnsElectPvString2_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_PHOTOVOLTAIC_STRING_V2    Photovoltaic String Version 2 Model Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2024 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Photovoltaic String Version 2 Model
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/SolarArray/GunnsElectPvString2.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectPvString2 and befriend UtGunnsElectPvString2.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectPvString2 : public GunnsElectPvString2
{
    public:
        FriendlyGunnsElectPvString2()
            : GunnsElectPvString2() {;}
        FriendlyGunnsElectPvString2(const GunnsElectPvStringConfigData* configData,
                                    const GunnsElectPvStringInputData*  inputData)
            : GunnsElectPvString2(configData, inputData) {;}
        virtual ~FriendlyGunnsElectPvString2() {;}
        friend class UtGunnsElectPvString2;
        friend class UtGunnsElectPvSection;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Photovoltaic String Model unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectPvString2 within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectPvString2: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectPvString2();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectPvString2();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests cell config data.
        void testCellConfig();
        /// @brief  Tests cell equivalent circuit model construction.
        void testCellEquivProps();
        /// @brief  Tests cell equivalent circuit model initialization.
        void testCellEquivInit();
        /// @brief  Tests cell equivalent circuit model initialization method with errors.
        void testCellEquivInitErrors();
        /// @brief  Tests cell equivalent circuit model update function.
        void testCellEquivUpdate();
        /// @brief  Tests cell equivalent circuit model compute current function.
        void testCellEquivCurrent();
        /// @brief  Tests cell equivalent circuit model compute voltage function.
        void testCellEquivVoltage();
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
        /// @brief  Tests the loadAtPower method.
        void testLoadAtPower();
        /// @brief  Tests the loadAtVoltage method.
        void testLoadAtVoltage();
        /// @brief  Tests the loadAtConductance method.
        void testLoadAtConductance();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectPvString2ric unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectPvString2);
        CPPUNIT_TEST(testCellConfig);
        CPPUNIT_TEST(testCellEquivProps);
        CPPUNIT_TEST(testCellEquivInit);
        CPPUNIT_TEST(testCellEquivInitErrors);
        CPPUNIT_TEST(testCellEquivUpdate);
        CPPUNIT_TEST(testCellEquivCurrent);
        CPPUNIT_TEST(testCellEquivVoltage);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testActiveCells);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testLoadAtPower);
        CPPUNIT_TEST(testLoadAtVoltage);
        CPPUNIT_TEST(testLoadAtConductance);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        std::string                   tName;                        /**< (--)   Test article name. */
        GunnsElectPvStringConfigData* tConfigData;                  /**< (--)   Pointer to config data. */
        GunnsElectPvStringInputData*  tInputData;                   /**< (--)   Pointer to input data. */
        FriendlyGunnsElectPvString2*  tArticle;                     /**< (--)   Pointer to article under test. */
        double                        tCellSurfaceArea;             /**< (m2)   Nominal config data. */
        double                        tCellEfficiency;              /**< (1)    Nominal config data. */
        double                        tCellSeriesResistance;        /**< (ohm)  Nominal config data. */
        double                        tCellShuntResistance;         /**< (ohm)  Nominal config data. */
        double                        tCellOpenCircuitVoltage;      /**< (V)    Nominal config data. */
        double                        tCellRefTemperature;          /**< (K)    Nominal config data. */
        double                        tCellTemperatureVoltageCoeff; /**< (1/K)  Nominal config data. */
        double                        tCellTemperatureCurrentCoeff; /**< (1/K)  Nominal config data. */
        double                        tCellShortCircuitCurrent;     /**< (amp)  Nominal config data. */
        double                        tCellMppVoltage;              /**< (V)    Nominal config data. */
        double                        tCellMppCurrent;              /**< (amp)  Nominal config data. */
        double                        tCellPhotoFlux;               /**< (W/m2) Nominal config data. */
        double                        tCellIdeality;                /**< (1)    Nominal config data. */
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
        UtGunnsElectPvString2(const UtGunnsElectPvString2& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvString2& operator =(const UtGunnsElectPvString2& that);
};

///@}

#endif
