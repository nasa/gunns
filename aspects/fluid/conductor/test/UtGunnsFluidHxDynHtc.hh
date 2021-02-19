#ifndef UtGunnsFluidHxDynHtc_EXISTS
#define UtGunnsFluidHxDynHtc_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_HEAT_EXHANGER_DYN_HTC Heat Exchanger With Dynamic HTC Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Heat Exchanger With Dynamic HTC link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidHxDynHtc.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidHxDynHtc and befriend UtGunnsFluidHxDynHtc.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidHxDynHtc : public GunnsFluidHxDynHtc
{
    public:
        FriendlyGunnsFluidHxDynHtc() {;}
        virtual ~FriendlyGunnsFluidHxDynHtc() {;}
        friend class UtGunnsFluidHxDynHtc;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Check Valve unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Check Valve link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidHxDynHtc: public CppUnit::TestFixture {
    public:
        /// @brief    Default constructs this Heat Exchanger Flow Path unit test.
        UtGunnsFluidHxDynHtc();
        /// @brief    Default destructs this Heat Exchanger Flow Path unit test.
        virtual ~UtGunnsFluidHxDynHtc();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config and input data.
        void testConfigAndInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testInitialization ();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests computeHeatTransferCoefficient method.
        void testHtc();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidHxDynHtc);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testHtc);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        /// @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 1};
        FluidProperties::FluidType    tTypes[N_FLUIDS];           /**< (--)       Constituent fluid types array. */
        double                        tFractions[N_FLUIDS];       /**< (--)       Constituent fluid mass fractions array. */
        DefinedFluidProperties*       tFluidProperties;           /**< (--)       Predefined fluid properties. */
        PolyFluidConfigData*          tFluidConfig;               /**< (--)       Fluid config data. */
        PolyFluidInputData*           tFluidInput;                /**< (--)       Fluid input data for nodes. */
        std::vector<GunnsBasicLink*>  tLinks;                     /**< (--)       Link vector. */
        std::string                   tName;                      /**< (--)       Nominal name. */
        GunnsFluidNode                tNodes[N_NODES];            /**< (--)       Nominal connected nodes. */
        GunnsNodeList                 tNodeList;                  /**< (--)       Network node structure. */
        int                           tPort0;                     /**< (--)       Nominal inlet port index. */
        int                           tPort1;                     /**< (--)       Nominal outlet port index. */
        double                        tMaxConductivity;           /**< (m2)       Nominal maximum conductivity. */
        double                        tExpansionScaleFactor;      /**< (--)       Nominal scale factor for isentropic gas cooling. */
        int                           tNumSegs;                   /**< (--)       Number of segments for this Heat Exchanger. */
        double                        tHtcCoeff0;                 /**< (W/K)      Nominal config data. */
        double                        tHtcCoeff1;                 /**< (W*s/K/kg) Nominal config data. */
        double                        tHtcExponent;               /**< (--)       Nominal config data. */
        double                        tHtcLimit;                  /**< (W/K)      Nominal config data. */
        GunnsFluidHxDynHtcConfigData* tConfigData;                /**< (--)       Pointer to nominal configuration data. */
        bool                          tMalfBlockageFlag;          /**< (--)       Blockage malfunction flag. */
        double                        tMalfBlockageValue;         /**< (--)       Blockage malfunction value. */
        double                        tInitialSegmentTemperature; /**< (K)        Initial value for segment temperatures. */
        GunnsFluidHxDynHtcInputData*  tInputData;                 /**< (--)       Pointer to nominal input data. */
        FriendlyGunnsFluidHxDynHtc*   tArticle;                   /**< (--)       Pointer to the friendly Leak under test. */
        static int                    TEST_ID;                    /**< (--)       Test identification number. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidHxDynHtc(const UtGunnsFluidHxDynHtc&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidHxDynHtc& operator =(const UtGunnsFluidHxDynHtc&);
};

///@}

#endif
