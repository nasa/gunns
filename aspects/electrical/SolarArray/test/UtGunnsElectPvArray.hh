#ifndef UtGunnsElectPvArray_EXISTS
#define UtGunnsElectPvArray_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_PHOTOVOLTAIC_ARRAY    Photovoltaic Array Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Photovoltaic Array Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/SolarArray/GunnsElectPvArray.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectPvArray and befriend UtGunnsElectPvArray.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectPvArray : public GunnsElectPvArray
{
    public:
        FriendlyGunnsElectPvArray() {;}
        virtual ~FriendlyGunnsElectPvArray() {;}
        friend class UtGunnsElectPvArray;
        friend class UtGunnsElectPvRegShunt;
        friend class UtGunnsElectPvRegConv;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Photovoltaic Array Link  unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectPvArray within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectPvArray: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectPvArray();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectPvArray();
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
        /// @brief  Tests the initialization method with nominal values and custom number of strings list.
        void testCustomStringsInitialization();
        /// @brief  Tests the initialization method with errors.
        void testInitializationErrors();
        /// @brief  Tests the restart method.
        void testRestart();
        /// @brief  Tests the step and updateState methods.
        void testStep();
        /// @brief  Tests the getter and setter methods.
        void testAccessors();
        /// @brief  Tests the confirmSolutionAcceptable method.
        void testConfirmSolutionAcceptable();
        /// @brief  Tests the computeFlows method.
        void testComputeFlows();
        /// @brief  Tests the predictLoadAtVoltage method.
        void testLoadAtVoltage();
        /// @brief  Tests the loadAtPower method.
        void testLoadAtPower();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectPvArrayric unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectPvArray);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testCustomStringsInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testConfirmSolutionAcceptable);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testLoadAtVoltage);
        CPPUNIT_TEST(testLoadAtPower);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        enum {N_NODES = 2};
        std::vector<GunnsBasicLink*> tLinks;                       /**< (--)   Network links vector. */
        GunnsBasicNode               tNodes[N_NODES];              /**< (--)   Network nodes. */
        GunnsNodeList                tNodeList;                    /**< (--)   Network nodes list. */
        int                          tPort0;                       /**< (--)   Port 0 node mapping. */
        std::string                  tName;                        /**< (--)   Test article name. */
        GunnsElectPvArrayConfigData* tConfigData;                  /**< (--)   Pointer to config data. */
        GunnsElectPvArrayInputData*  tInputData;                   /**< (--)   Pointer to input data. */
        FriendlyGunnsElectPvArray*   tArticle;                     /**< (--)   Pointer to article under test. */
        unsigned int                 tNumSections;                 /**< (--)   Nominal config data. */
        unsigned int                 tNumStrings;                  /**< (--)   Nominal config data. */
        double                       tSourceAngleExponent;         /**< (--)   Nominal config data. */
        double                       tBacksideReduction;           /**< (--)   Nominal config data. */
        bool                         tSourceAngleEdgeOn;           /**< (--)   Nominal config data. */
        double                       tRefSourceFluxMagnitude;      /**< (W/m2) Nominal config data. */
        double                       tBlockingDiodeVoltageDrop;    /**< (v)    Nominal config data. */
        double                       tBypassDiodeVoltageDrop;      /**< (v)    Nominal config data. */
        unsigned int                 tBypassDiodeInterval;         /**< (--)   Nominal config data. */
        unsigned int                 tNumCells;                    /**< (--)   Nominal config data. */
        double                       tCellSurfaceArea;             /**< (m2)   Nominal config data. */
        double                       tCellEfficiency;              /**< (--)   Nominal config data. */
        double                       tCellSeriesResistance;        /**< (ohm)  Nominal config data. */
        double                       tCellShuntResistance;         /**< (ohm)  Nominal config data. */
        double                       tCellOpenCircuitVoltage;      /**< (v)    Nominal config data. */
        double                       tCellRefTemperature;          /**< (K)    Nominal config data. */
        double                       tCellTemperatureVoltageCoeff; /**< (1/K)  Nominal config data. */
        double                       tCellTemperatureCurrentCoeff; /**< (1/K)  Nominal config data. */
        double                       tSourceFluxMagnitude;         /**< (W/m2) Nominal input data. */
        double                       tSourceAngle;                 /**< (r)    Nominal input data. */
        double                       tSourceExposedFraction;       /**< (--)   Nominal input data. */
        double                       tTemperature;                 /**< (K)    Nominal input data. */
        static int                   TEST_ID;                      /**< (--)   Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvArray(const UtGunnsElectPvArray& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectPvArray& operator =(const UtGunnsElectPvArray& that);
};

///@}

#endif
