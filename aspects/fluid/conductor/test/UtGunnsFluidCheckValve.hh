#ifndef UtGunnsFluidCheckValve_EXISTS
#define UtGunnsFluidCheckValve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_CHECK_VALVE    Check Valve Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Check Valve link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidCheckValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidCheckValve and befriend UtGunnsFluidCheckValve.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidCheckValve : public GunnsFluidCheckValve
{
    public:
         FriendlyGunnsFluidCheckValve();
        virtual ~FriendlyGunnsFluidCheckValve();
        friend class UtGunnsFluidCheckValve;
};
inline FriendlyGunnsFluidCheckValve::FriendlyGunnsFluidCheckValve() : GunnsFluidCheckValve() {};
inline FriendlyGunnsFluidCheckValve::~FriendlyGunnsFluidCheckValve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Check Valve unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Check Valve link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidCheckValve: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Valve unit test.
        UtGunnsFluidCheckValve();
        /// @brief    Default destructs this Check Valve unit test.
        virtual ~UtGunnsFluidCheckValve();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config and input data.
        void testConfigAndInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update state method.
        void testUpdateStateNominal();
        /// @brief    Tests update state method (rate limiting).
        void testUpdateStateRateLimited();
        /// @brief    Tests update state method (no range).
        void testUpdateStateNoRange();
        /// @brief    Tests update state method (malfunction).
        void testUpdateStateMalfunction();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidCheckValve);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateStateNominal);
        CPPUNIT_TEST(testUpdateStateRateLimited);
        CPPUNIT_TEST(testUpdateStateNoRange);
        CPPUNIT_TEST(testUpdateStateMalfunction);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 4, N_FLUIDS = 2};
        FluidProperties::FluidType        mTypes[N_FLUIDS];        /**< (--)            Constituent fluid types array. */
        double                            mFractions[N_FLUIDS];    /**< (--)            Constituent fluid mass fractions array. */
        DefinedFluidProperties*           mFluidProperties;        /**< (--)            Predefined fluid properties. */
        PolyFluidConfigData*              mFluidConfig;            /**< (--)            Fluid config data. */
        PolyFluidInputData*               mFluidInput0;            /**< (--)            Fluid input data for node 0. */
        PolyFluidInputData*               mFluidInput1;            /**< (--)            Fluid input data for node 1. */
        std::vector<GunnsBasicLink*>      mLinks;                  /**< (--)            Link vector. */
        std::string                       mName;                   /**< (--)            Nominal name. */
        GunnsFluidNode                    mNodes[N_NODES];         /**< (--)            Nominal connected nodes. */
        GunnsNodeList                     mNodeList;               /**< (--)            Network node structure. */
        int                               mPort0;                  /**< (--)            Nominal inlet port node index. */
        int                               mPort1;                  /**< (--)            Nominal outlet port node index. */
        double                            mMaxConductivity;        /**< (m2)            Nominal maximum conductivity. */
        double                            mExpansionScaleFactor;   /**< (--)            Nominal scale factor for isentropic gas cooling. */
        double                            mRateLimit;              /**< (one/s)         Nominal fractional position rate limit. */
        double                            mThermalLength;          /**< (m)             Tube length for thermal convection. */
        double                            mThermalDiameter;        /**< (m)             Tube inner diameter for thermal convection. */
        double                            mSurfaceRoughness;       /**< (m)             Tube wall surface roughness for thermal convection. */
        double                            mThermalSurfaceArea;     /**< (m2)            Tube inner surface area for thermal convection       */
        double                            mThermalROverD;          /**< (--)            Tube surface roughness over diameter for convection  */
        double                            mClosePressure;          /**< (kPa)           Pressure at which valve is closed.  */
        double                            mOpenPressure;           /**< (kPa)           Pressure at which valve is open.  */
        GunnsFluidCheckValveConfigData*   mConfigData;             /**< (--)            Pointer to the nominal configuration data. */
        bool                              mMalfBlockageFlag;       /**< (--)            Blockage malfunction flag. */
        double                            mMalfBlockageValue;      /**< (--)            Blockage malfunction value. */
        double                            mPosition;               /**< (--)            Fractional position of this valve. */
        bool                              mMalfLeakThruFlag;       /**< (--)            Leak through rate malfunction flag. */
        double                            mMalfLeakThruValue;      /**< (kg/s)          Leak through rate malfunction value. */
        double                            mWallTemperature;        /**< (K)             Tube wall temperature for thermal convection           */
        bool                              mMalfStuckFlag;          /**< (--)            Stuck at current position malfunction flag. */
        bool                              mMalfFailToFlag;         /**< (--)            Fail to position malfunction flag. */
        double                            mMalfFailToValue;        /**< (--)            Fail to position malfunction value. */
        GunnsFluidCheckValveInputData*    mInputData;              /**< (--)            Pointer to the nominal input data. */
        FriendlyGunnsFluidCheckValve*     mArticle;                /**< (--)            Pointer to the friendly Check Valve under test. */
        double                            mPreviousLeakRate;       /**< (kg/s)          Previous leak thru rate value. */
        double                            mLeakConductivity;       /**< (m2)            Conductivity equivalent to the leak. */
        double                            mWallHeatFlux;           /**< (W)             Convection heat flux from the fluid to the tube wall */
        double                            mFlowRate;               /**< (kg/s)          Nominal mass flow rate. */
        double                            mTimeStep;               /**< (s)             Nominal time step. */
        double                            mTolerance;              /**< (--)            Nominal tolerance for comparison of expected and returned values. */
        static int                        TEST_ID;                 /**< (--)            Test identification number. */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidCheckValve(const UtGunnsFluidCheckValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidCheckValve& operator =(const UtGunnsFluidCheckValve&);
};

///@}

#endif
