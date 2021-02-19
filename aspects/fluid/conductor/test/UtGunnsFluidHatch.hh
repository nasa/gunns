#ifndef UtGunnsFluidHatch_EXISTS
#define UtGunnsFluidHatch_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_HATCH    Hatch Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Hatch link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidHatch.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidHatch and befriend UtGunnsFluidHatch.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidHatch : public GunnsFluidHatch
{
    public:
        FriendlyGunnsFluidHatch();
        virtual ~FriendlyGunnsFluidHatch();
        friend class UtGunnsFluidHatch;
};
inline FriendlyGunnsFluidHatch::FriendlyGunnsFluidHatch() : GunnsFluidHatch() {};
inline FriendlyGunnsFluidHatch::~FriendlyGunnsFluidHatch() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidNode and befriend UtGunnsFluidHatch.
///
/// @details  An extension of the fluid node class used by the unit under test that befriends the
///           unit test case driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidNode : public GunnsFluidNode
{
    public:
        FriendlyGunnsFluidNode();
        virtual ~FriendlyGunnsFluidNode();
        friend class UtGunnsFluidHatch;
};
inline FriendlyGunnsFluidNode::FriendlyGunnsFluidNode() : GunnsFluidNode() {};
inline FriendlyGunnsFluidNode::~FriendlyGunnsFluidNode() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hatch unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Hatch link model within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidHatch: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Hatch unit test.
        UtGunnsFluidHatch();
        /// @brief    Default constructs this Check Hatch unit test.
        virtual ~UtGunnsFluidHatch();
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
        /// @brief    Tests update state method (nominal).
        void testUpdateStateNominal();
        /// @brief    Tests update state method (malfunction).
        void testUpdateStateMalfunction();
        /// @brief    Tests update fluid method (neither diffusion nor conduction).
        void testUpdateFluidNeither();
        /// @brief    Tests update fluid method (diffusion).
        void testUpdateFluidDiffuse();
        /// @brief    Tests update fluid method (conduction).
        void testUpdateFluidConduct();
        /// @brief    Tests update fluid method (diffusion and conduction).
        void testUpdateFluidBoth();
        /// @brief    Tests transport fluid method.
        void testTransportFluid();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests restart method.
        void testRestart();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidHatch);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateStateNominal);
        CPPUNIT_TEST(testUpdateStateMalfunction);
        CPPUNIT_TEST(testUpdateFluidNeither);
        CPPUNIT_TEST(testUpdateFluidDiffuse);
        CPPUNIT_TEST(testUpdateFluidConduct);
        CPPUNIT_TEST(testUpdateFluidBoth);
        CPPUNIT_TEST(testTransportFluid);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 3, N_FLUIDS = 2};
        ///  @brief   --     Constituent fluid types array.
        FluidProperties::FluidType    mTypes[N_FLUIDS];
        /// @brief    --     Constituent fluid mass fractions array.
        double                        mFractions[N_FLUIDS];
        ///  @brief   --     Predefined fluid properties.
        DefinedFluidProperties*       mFluidProperties;
        ///  @brief   --     Fluid config data.
        PolyFluidConfigData*          mFluidConfig;
        /// @brief    --     Fluid input data for node 0.
        PolyFluidInputData*           mFluidInput0;
        /// @brief    --     Fluid input data for node 1.
        PolyFluidInputData*           mFluidInput1;
        ///  @brief   --     Link vector.
        std::vector<GunnsBasicLink*>  mLinks;
        ///  @brief   --     Nominal name.
        std::string                   mName;
        ///  @brief   --     Nominal connected nodes.
        GunnsFluidNode                mNodes[N_NODES];
        ///  @brief   --     Network node structure.
        GunnsNodeList                 mNodeList;
        ///  @brief   --     Nominal inlet port index.
        int                           mPort0;
        ///  @brief   --     Nominal outlet port index.
        int                           mPort1;
        ///  @brief   (m2)   Nominal maximum conductivity.
        double                        mMaxConductivity;
        ///  @brief   --     Nominal scale factor for isentropic gas cooling.
        double                        mExpansionScaleFactor;
        /// @brief    (m)    Distance from hatch to center of port 0 node volume.
        double                        mLength0;
        /// @brief    (m)    Distance from hatch to center of port 1 node volume.
        double                        mLength1;
        ///   @brief   --    Pointer to nominal configuration data.
        GunnsFluidHatchConfigData*    mConfigData;
        /// @brief    --     Blockage malfunction flag.
        bool                          mMalfBlockageFlag;
        /// @brief    --     Blockage malfunction value.
        double                        mMalfBlockageValue;
        ///  @brief    --    Fractional position of this Hatch.
        double                        mPosition;
        /// @brief    --     Initial leak rate malfunction flag for this Leak.
        bool                          mMalfLeakThruFlag;
        /// @brief    (kg/s) Initial leak rate malfunction value for this Leak.
        double                        mMalfLeakThruValue;
        ///   @brief   --    Pointer to nominal input data.
        GunnsFluidHatchInputData*     mInputData;
        ///  @brief   --     Pointer to the friendly Hatch under test.
        FriendlyGunnsFluidHatch*      mArticle;
        /// @brief    (kg/s) Previous leak rate value for this Leak.
        double                        mPreviousLeakRate;
        /// @brief    (m2)   Conductivity equivalent to the leak rate for this Leak.
        double                        mLeakConductivity;
        ///  @brief   (s)    Nominal flow rate.
        double                        mFlowRate;
        ///  @brief   (s)    Nominal time step.
        double                        mTimeStep;
        ///  @brief   --     Nominal tolerance for comparison of expected and returned values.
        double                        mTolerance;
        double                        mThermalLength;           /**<   (m)      Tube length for thermal convection                     */
        double                        mThermalDiameter;         /**<   (m)      Tube inner diameter for thermal convection             */
        double                        mSurfaceRoughness;        /**<   (m)      Tube wall surface roughness for thermal convection     */
        double                        mWallTemperature;         /**<   (K)      Tube wall temperature for thermal convection           */
        static const double           PI;                       /**<   (--)     Units conversion constant                              */
        static int                    TEST_ID;                  /**<   (--)     Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidHatch(const UtGunnsFluidHatch&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidHatch& operator =(const UtGunnsFluidHatch&);
};

///@}

#endif
