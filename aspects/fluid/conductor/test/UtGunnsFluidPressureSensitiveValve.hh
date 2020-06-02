#ifndef UtGunnsFluidPressureSensitiveValve_EXISTS
#define UtGunnsFluidPressureSensitiveValve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_PRESSURE_SENSITIVE_VALVE   Pressure Sensitive Valve Unit Test
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Pressure Sensitive Valve link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/conductor/GunnsFluidPressureSensitiveValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidPressureSensitiveValve and befriend UtGunnsFluidPressureSensitiveValve.
///
/// @reqt testUpdateStateNominal     R.TS222-0063 TS21 ECLSS models shall perform pressure regulator control.
/// @reqt testUpdateStateMalfunction R.TS222-0068 TS21 ECLSS models shall provide malfunctions to freeze valves at non-target positions.
/// @reqt testUpdateStateNominal     R.TS222-0082 TS21 ECLSS models shall simulate the functionality of valves.
/// @reqt testUpdateStateMalfunction R.TS222-0083 TS21 ECLSS models shall malfunction valves.
/// @reqt testUpdateStateNominal     R.TS228-0001 TS21 thermal models shall simulate the functionality of valves.
/// @reqt testUpdateStateMalfunction R.TS228-0029 TS21 thermal models shall provide valve malfunctions.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidPressureSensitiveValve : public GunnsFluidPressureSensitiveValve
{
    public:
        FriendlyGunnsFluidPressureSensitiveValve();
        virtual ~FriendlyGunnsFluidPressureSensitiveValve();
        friend class UtGunnsFluidPressureSensitiveValve;
};
inline FriendlyGunnsFluidPressureSensitiveValve::FriendlyGunnsFluidPressureSensitiveValve() : GunnsFluidPressureSensitiveValve() {};
inline FriendlyGunnsFluidPressureSensitiveValve::~FriendlyGunnsFluidPressureSensitiveValve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Sensitive Valve unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Pressure Sensitive Valve link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidPressureSensitiveValve: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Pressure Sensitive Valve unit test.
        UtGunnsFluidPressureSensitiveValve();
        /// @brief    Default destructs this Pressure Sensitive Valve unit test.
        virtual ~UtGunnsFluidPressureSensitiveValve();
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
        /// @brief    Tests step method.
        void testStep();
        /// @brief    Tests compute flows method.
        void testComputeFlows();
        /// @brief    Tests compute flows method with internal fluid.
        void testComputeFlowsWithInternalFluid();
        /// @brief    Tests tuning method.
        void testTuning();
        /// @brief    Tests update state method (nominal).
        void testUpdateStateNominal();
        /// @brief    Tests update state method (malfunction).
        void testUpdateStateMalfunction();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidPressureSensitiveValve);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testComputeFlowsWithInternalFluid);
        CPPUNIT_TEST(testTuning);
        CPPUNIT_TEST(testUpdateStateNominal);
        CPPUNIT_TEST(testUpdateStateMalfunction);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 4, N_FLUIDS = 2};
        FluidProperties::FluidType                    mTypes[N_FLUIDS];        /**< (--)            Constituent fluid types array. */
        double                                        mFractions[N_FLUIDS];    /**< (--)            Constituent fluid mass fractions array. */
        DefinedFluidProperties*                       mFluidProperties;        /**< (--)            Predefined fluid properties. */
        PolyFluidConfigData*                          mFluidConfig;            /**< (--)            Fluid config data. */
        PolyFluidInputData*                           mFluidInput0;            /**< (--)            Fluid input data for node 0. */
        PolyFluidInputData*                           mFluidInput1;            /**< (--)            Fluid input data for node 1. */
        PolyFluidInputData*                           mFluidInput2;            /**< (--)            Fluid input data for node 2. */
        PolyFluidInputData*                           mFluidInput3;            /**< (--)            Fluid input data for node 3. */
        std::vector<GunnsBasicLink*>                  mLinks;                  /**< (--)            Link vector. */
        std::string                                   mName;                   /**< (--)            Nominal name. */
        GunnsFluidNode                                mNodes[N_NODES];         /**< (--)            Nominal connected nodes. */
        GunnsNodeList                                 mNodeList;               /**< (--)            Network node structure. */
        int                                           mPort0;                  /**< (--)            Nominal inlet port node index. */
        int                                           mPort1;                  /**< (--)            Nominal outlet port node index. */
        int                                           mPort2;                  /**< (--)            Nominal inlet pressure port node index. */
        int                                           mPort3;                  /**< (--)            Nominal outlet pressure port node index. */
        double                                        mMaxConductivity;        /**< (m2)            Nominal maximum conductivity. */
        double                                        mExpansionScaleFactor;   /**< (--)            Nominal scale factor for isentropic gas cooling. */
        double                                        mRateLimit;              /**< (one/s)         Nominal fractional position rate limit. */
        double                                        mThermalLength;          /**< (m)             Tube length for thermal convection. */
        double                                        mThermalDiameter;        /**< (m)             Tube inner diameter for thermal convection. */
        double                                        mSurfaceRoughness;       /**< (m)             Tube wall surface roughness for thermal convection. */
        double                                        mThermalSurfaceArea;     /**< (m2)            Tube inner surface area for thermal convection       */
        double                                        mThermalROverD;          /**< (--)            Tube surface roughness over diameter for convection  */
        GunnsFluidPressureSensitiveValveConfigData*   mConfigData;             /**< (--)            Pointer to the nominal configuration data. */
        bool                                          mMalfBlockageFlag;       /**< (--)            Blockage malfunction flag. */
        double                                        mMalfBlockageValue;      /**< (--)            Blockage malfunction value. */
        double                                        mPosition;               /**< (--)            Fractional position of this valve. */
        bool                                          mMalfLeakThruFlag;       /**< (--)            Leak through rate malfunction flag. */
        double                                        mMalfLeakThruValue;      /**< (kg/s)          Leak through rate malfunction value. */
        bool                                          mMalfPressureBiasFlag;   /**< (--)            Control pressure bias malfunction flag. */
        double                                        mMalfPressureBiasValue;  /**< (kPa)           Control pressure bias malfunction value. */
        double                                        mSetPointPressureBias;   /**< (kPa)           Set point pressure bias value. */
        double                                        mWallTemperature;        /**<   (K)           Tube wall temperature for thermal convection           */
        bool                                          mMalfStuckFlag;          /**< (--)            Stuck at current position malfunction flag. */
        bool                                          mMalfFailToFlag;         /**< (--)            Fail to position malfunction flag. */
        double                                        mMalfFailToValue;        /**< (--)            Fail to position malfunction value. */
        GunnsFluidPressureSensitiveValveInputData*    mInputData;              /**< (--)            Pointer to the nominal input data. */
        FriendlyGunnsFluidPressureSensitiveValve*     mArticle;                /**< (--)            Pointer to the friendly Pressure Sensitive Valve under test. */
        double                                        mPreviousLeakRate;       /**< (kg/s)          Previous leak thru rate value. */
        double                                        mLeakConductivity;       /**< (m2)            Conductivity equivalent to the leak. */
        GunnsFluidUtils::TuningMode                   mTuneMode;               /**< (--)            Auto-tunes the link to desired flow type. */
        double                                        mTuneMassFlow;           /**< (kg/s)          The desired mass flow for link tuning. */
        double                                        mTuneVolFlow;            /**< (m3/s)          The desired volumetric flow for link tuning. */
        double                                        mTuneDeltaT;             /**< (K)             The desired delta-temperature for link tuning. */
        double                                        mEffectiveConductivity;  /**< (m2)            Effective conductivity of the link. */
        double                                        mSystemConductance;      /**< (kg*mol/kPa/s)  Limited molar conductance. */
        double                                        mLastSystemConductance;  /**< (kg*mol/kPa/s)  Last-pass value of mSystemConductance. */
        double                                        mControlPressure;        /**< (kPa)           Valve control pressure. */
        double                                        mWallHeatFlux;           /**< (W)             Convection heat flux from the fluid to the tube wall */
        double                                        mTimeStep;               /**< (s)             Nominal time step. */
        double                                        mTolerance;              /**< (--)            Nominal tolerance for comparison of expected and returned values. */
        static int                                    TEST_ID;                 /**< (--)            Test identification number. */
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidPressureSensitiveValve(const UtGunnsFluidPressureSensitiveValve& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidPressureSensitiveValve& operator =(const UtGunnsFluidPressureSensitiveValve& that);
};

///@}

#endif
