#ifndef UtGunnsFluidPipe_EXISTS
#define UtGunnsFluidPipe_EXISTS

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_PIPE     Pipe Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Pipe link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidPipe.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidPipe and befriend UtGunnsFluidPipe.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidPipe : public GunnsFluidPipe
{
    public:
        FriendlyGunnsFluidPipe();
        virtual ~FriendlyGunnsFluidPipe();
        friend class UtGunnsFluidPipe;
};
inline FriendlyGunnsFluidPipe::FriendlyGunnsFluidPipe() : GunnsFluidPipe() {};
inline FriendlyGunnsFluidPipe::~FriendlyGunnsFluidPipe() {}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Pipe unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Pipe link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidPipe: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Leak unit test.
        UtGunnsFluidPipe();
        /// @brief    Default destructs this Leak unit test.
        virtual ~UtGunnsFluidPipe();
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
        void testUpdateState();
        /// @brief    Tests update fluid method.
        void testUpdateFluid();
        /// @brief    Tests update fluid method (no flow).
        void testUpdateFluidNoFLow();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidPipe);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testUpdateFluid);
        CPPUNIT_TEST(testUpdateFluidNoFLow);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 2};
        FluidProperties::FluidType    mTypes[N_FLUIDS];         /**< (--)     Constituent fluid types array.  */
        double                        mFractions[N_FLUIDS];     /**< (--)     Constituent fluid mass fractions array. */
        DefinedFluidProperties*       mFluidProperties;         /**< (--)     Predefined fluid properties. */
        PolyFluidConfigData*          mFluidConfig;             /**< (--)     Fluid config data. */
        PolyFluidInputData*           mFluidInput0;             /**< (--)     Fluid input data for node 0. */
        PolyFluidInputData*           mFluidInput1;             /**< (--)     Fluid input data for node 1. */
        std::vector<GunnsBasicLink*>  mLinks;                   /**< (--)     Link vector. */
        std::string                   mName;                    /**< (--)     Nominal name. */
        GunnsFluidNode                mNodes[N_NODES];          /**< (--)     Nominal connected nodes. */
        GunnsNodeList                 mNodeList;                /**< (--)     Network node structure. */
        int                           mPort0;                   /**< (--)     Nominal inlet port index. */
        int                           mPort1;                   /**< (--)     Nominal outlet port index. */
        double                        mMaxConductivity;         /**< (m2)     Nominal maximum conductivity. */
        double                        mExpansionScaleFactor;    /**< (--)     Nominal scale factor for isentropic gas cooling. */
        double                        mThermalLength;           /**< (m)      Tube length for thermal convection. */
        double                        mThermalDiameter;         /**< (m)      Tube inner diameter for thermal convection. */
        double                        mSurfaceRoughness;        /**< (m)      Tube wall surface roughness for thermal convection. */
        double                        mThermalSurfaceArea;      /**< (m2)     Tube inner surface area for thermal convection       */
        double                        mThermalROverD;           /**< (--)     Tube surface roughness over diameter for convection  */
        GunnsFluidPipeConfigData*     mConfigData;              /**< (--)     Pointer to nominal configuration data.               */
        bool                          mMalfBlockageFlag;        /**< (--)     Blockage malfunction flag. */
        double                        mMalfBlockageValue;       /**< (--)     Blockage malfunction value. */
        double                        mWallTemperature;         /**< (K)      Tube wall temperature for thermal convection. */
        GunnsFluidPipeInputData*      mInputData;               /**< (--)     Pointer to nominal input data. */
        FriendlyGunnsFluidPipe*       mArticle;                 /**< (--)     Pointer to the friendly Pipe under test. */
        double                        mWallHeatFlux;            /**< (W)      Convection heat flux from the fluid to the tube wall */
        double                        mFlowRate;                /**< (kg/s)   Nominal mass flow rate. */
        double                        mTimeStep;                /**< (s)      Nominal time step. */
        double                        mTolerance;               /**< (--)     Nominal tolerance for comparison of expected and returned values. */
        static int                    TEST_ID;                  /**< (--)     Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidPipe(const UtGunnsFluidPipe&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidPipe& operator =(const UtGunnsFluidPipe&);
};

///@}

#endif
