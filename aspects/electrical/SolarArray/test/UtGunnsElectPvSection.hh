#ifndef UtGunnsElectPvSection_EXISTS
#define UtGunnsElectPvSection_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_PHOTOVOLTAIC_SECTIONS    Photovoltaic Section Model Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2024 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Photovoltaic Section Model
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/SolarArray/GunnsElectPvSection.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectPvSection and befriend UtGunnsElectPvSection.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectPvSection : public GunnsElectPvSection
{
    public:
        FriendlyGunnsElectPvSection() {;}
        FriendlyGunnsElectPvSection(const GunnsElectPvSectionConfigData* configData)
            : GunnsElectPvSection(configData) {;}
        virtual ~FriendlyGunnsElectPvSection() {;}
        friend class UtGunnsElectPvSection;
        friend class UtGunnsElectPvArray;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Photovoltaic Section Model  unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectPvSection within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectPvSection: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectPvSection();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectPvSection();
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
        /// @brief  Tests the update method.
        void testUpdate();
        /// @brief  Tests the getter and setter methods.
        void testAccessors();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectPvSectionric unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectPvSection);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        static const unsigned int      tNumStrings = 3;              /**< (--)   Number of test article strings. */
        std::string                    tName;                        /**< (--)   Test article name. */
        GunnsElectPvSectionConfigData* tConfigData;                  /**< (--)   Pointer to config data. */
        GunnsElectPvSectionInputData*  tInputData;                   /**< (--)   Pointer to input data. */
        FriendlyGunnsElectPvSection*   tArticle;                     /**< (--)   Pointer to article under test. */
        double                         tSourceAngleExponent;         /**< (--)   Nominal config data. */
        double                         tBacksideReduction;           /**< (--)   Nominal config data. */
        bool                           tSourceAngleEdgeOn;           /**< (--)   Nominal config data. */
        double                         tRefSourceFluxMagnitude;      /**< (W/m2) Nominal config data. */
        double                         tBlockingDiodeVoltageDrop;    /**< (v)    Nominal config data. */
        double                         tBypassDiodeVoltageDrop;      /**< (v)    Nominal config data. */
        int                            tBypassDiodeInterval;         /**< (--)   Nominal config data. */
        int                            tNumCells;                    /**< (--)   Nominal config data. */
        double                         tCellSurfaceArea;             /**< (m2)   Nominal config data. */
        double                         tCellEfficiency;              /**< (--)   Nominal config data. */
        double                         tCellSeriesResistance;        /**< (ohm)  Nominal config data. */
        double                         tCellShuntResistance;         /**< (ohm)  Nominal config data. */
        double                         tCellOpenCircuitVoltage;      /**< (v)    Nominal config data. */
        double                         tCellRefTemperature;          /**< (K)    Nominal config data. */
        double                         tCellTemperatureVoltageCoeff; /**< (1/K)  Nominal config data. */
        double                         tCellTemperatureCurrentCoeff; /**< (1/K)  Nominal config data. */
        double                         tSourceFluxMagnitude;         /**< (W/m2) Nominal input data. */
        double                         tSourceAngle;                 /**< (r)    Nominal input data. */
        double                         tSourceExposedFraction;       /**< (--)   Nominal input data. */
        double                         tTemperature;                 /**< (K)    Nominal input data. */
        static int                     TEST_ID;                      /**< (--)   Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvSection(const UtGunnsElectPvSection& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvSection& operator =(const UtGunnsElectPvSection& that);
};

///@}

#endif
