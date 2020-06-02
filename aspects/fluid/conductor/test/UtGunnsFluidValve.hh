#ifndef UtGunnsFluidValve_EXISTS
#define UtGunnsFluidValve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_VALVE    Valve Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Valve link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidValve and befriend UtGunnsFluidValve.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidValve : public GunnsFluidValve
{
    public:
        FriendlyGunnsFluidValve();
        virtual ~FriendlyGunnsFluidValve();
        friend class UtGunnsFluidValve;
};
inline FriendlyGunnsFluidValve::FriendlyGunnsFluidValve() : GunnsFluidValve() {};
inline FriendlyGunnsFluidValve::~FriendlyGunnsFluidValve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Valve link model within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidValve: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Valve unit test.
        UtGunnsFluidValve();
        /// @brief    Default constructs this Check Valve unit test.
        virtual ~UtGunnsFluidValve();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config data.
        void testConfig();
        /// @brief    Tests input data.
        void testInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests initialize method configured for no thermal convection.
        void testNoConvectionInitialization();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update state method (nominal).
        void testUpdateStateNominal();
        /// @brief    Tests update state method (malfunction).
        void testUpdateStateMalfunction();
        /// @brief    Tests update fluid method
        void testUpdateFluid();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidValve);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testNoConvectionInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateStateNominal);
        CPPUNIT_TEST(testUpdateStateMalfunction);
        CPPUNIT_TEST(testUpdateFluid);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 2};
        FluidProperties::FluidType    mTypes[N_FLUIDS];         /**<   (--)     Constituent fluid types array.                       */
        double                        mFractions[N_FLUIDS];     /**<   (--)     Constituent fluid mass fractions array.              */
        DefinedFluidProperties*       mFluidProperties;         /**<   (--)     Predefined fluid properties.                         */
        PolyFluidConfigData*          mFluidConfig;             /**<   (--)     Fluid config data.                                   */
        PolyFluidInputData*           mFluidInput0;             /**<   (--)     Fluid input data for node 0.                         */
        PolyFluidInputData*           mFluidInput1;             /**<   (--)     Fluid input data for node 1.                         */
        std::vector<GunnsBasicLink*>  mLinks;                   /**<   (--)     Link vector.                                         */
        std::string                   mName;                    /**<   (--)     Nominal name.                                        */
        GunnsFluidNode                mNodes[N_NODES];          /**<   (--)     Nominal connected nodes.                             */
        GunnsNodeList                 mNodeList;                /**<   (--)     Network node structure.                              */
        int                           mPort0;                   /**<   (--)     Nominal inlet port index.                            */
        int                           mPort1;                   /**<   (--)     Nominal outlet port index.                           */
        double                        mMaxConductivity;         /**<   (m2)     Nominal maximum conductivity.                        */
        double                        mExpansionScaleFactor;    /**<   (--)     Nominal scale factor for isentropic gas cooling.     */
        GunnsFluidValveConfigData*    mConfigData;              /**<   (--)     Pointer to nominal configuration data.               */
        bool                          mMalfBlockageFlag;        /**<   (--)     Blockage malfunction flag.                           */
        double                        mMalfBlockageValue;       /**<   (--)     Blockage malfunction value.                          */
        double                        mPosition;                /**<   (--)     Fractional position of this Valve.                   */
        bool                          mMalfLeakThruFlag;        /**<   (--)     Initial leak rate malfunction flag for this Leak.    */
        double                        mMalfLeakThruValue;       /**<   (kg/s)   Initial leak rate malfunction value for this Leak.   */
        GunnsFluidValveInputData*     mInputData;               /**<   (--)     Pointer to nominal input data.                       */
        FriendlyGunnsFluidValve*      mArticle;                 /**<   (--)     Pointer to the friendly Valve under test.            */
        double                        mPreviousLeakRate;        /**<   (kg/s)   Previous leak rate value for this Leak.                */
        double                        mLeakConductivity;        /**<   (m2)     Conductivity equivalent to the leak rate for this Leak.*/
        double                        mFlowRate;                /**<   (s)      Nominal flow rate.                                     */
        double                        mTimeStep;                /**<   (s)      Nominal time step.                                     */
        double                        mTolerance;               /**<   (--)     Nominal tolerance for comparison of expected and returned values.*/
        double                        mThermalLength;           /**<   (m)      Tube length for thermal convection                     */
        double                        mThermalDiameter;         /**<   (m)      Tube inner diameter for thermal convection             */
        double                        mSurfaceRoughness;        /**<   (m)      Tube wall surface roughness for thermal convection     */
        double                        mWallTemperature;         /**<   (K)      Tube wall temperature for thermal convection           */
        static const double           PI;                       /**<   (--)     Units conversion constant                              */
        static int                    TEST_ID;                  /**<   (--)     Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidValve(const UtGunnsFluidValve&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidValve& operator =(const UtGunnsFluidValve&);
};

///@}

#endif
