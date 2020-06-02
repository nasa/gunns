#ifndef UtGunnsFluidReactor_EXISTS
#define UtGunnsFluidReactor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_REACTOR    Chemical Reactor Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Chemical Reactor link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidReactor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from UtGunnsFluidReactor and befriend UtGunnsFluidReactor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidReactor : public GunnsFluidReactor {
    public:
        FriendlyGunnsFluidReactor();
        virtual ~FriendlyGunnsFluidReactor();
        friend class UtGunnsFluidReactor;
};
inline FriendlyGunnsFluidReactor::FriendlyGunnsFluidReactor() : GunnsFluidReactor() {}
inline FriendlyGunnsFluidReactor::~FriendlyGunnsFluidReactor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Chemical Reactor unit tests.
////
/// @details  This class provides the unit tests for the Chemical Reactor within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidReactor: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Chemical Reactor unit test.
        UtGunnsFluidReactor();
        /// @brief    Default destructs this Chemical Reactor unit test.
        virtual ~UtGunnsFluidReactor();
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
        /// @brief    Tests multiple initialization.
        void testMultipleInitialization();
        /// @brief    Tests accessor methods.
        void testAccess();
        /// @brief    Tests modifier methods.
        void testModify();
        /// @brief    Tests update fluid method.
        void testUpdateFluid();
        /// @brief    Tests compute flows method.
        void testComputeFlows();
        /// @brief    Tests compute flows method for the H2_REMOVAL reaction.
        void testComputeFlowsH2Removal();
        /// @brief    Tests heats of reaction.
        void testHeatOfReaction();
        /// @brief    Tests initialize method exceptions.
        void testInitializationException();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidReactor);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testMultipleInitialization);
        CPPUNIT_TEST(testAccess);
        CPPUNIT_TEST(testModify);
        CPPUNIT_TEST(testUpdateFluid);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testComputeFlowsH2Removal);
        CPPUNIT_TEST(testHeatOfReaction);
        CPPUNIT_TEST(testInitializationException);
        CPPUNIT_TEST_SUITE_END();
        enum {N_NODES = 3, N_REACTIONS = 4, N_COMPOUNDS = 9, N_CONSTITUENTS = 8 };
        DefinedFluidProperties*       mFluidProperties;                                /**< (--)    Defined fluid properties. */
        FluidProperties::FluidType    mTypes[UtGunnsFluidReactor::N_CONSTITUENTS];     /**< (--)    Array of Fluid Types. */
        double                        mFractions[UtGunnsFluidReactor::N_CONSTITUENTS]; /**< (--)    Array of fluid mass fractions. */
        PolyFluidConfigData*          mFluidConfig;                                    /**< (--)    Fluid config data. */
        PolyFluidInputData*           mFluidInput;                                     /**< (--)    Fluid input data. */
        std::vector<GunnsBasicLink*>  mLinks;                                          /**< (--)    Link vector. */
        std::string                   mName;                                           /**< (--)    Nominal name. */
        GunnsFluidNode                mNodes[N_NODES];                                 /**< (--)    Fluid nodes. */
        GunnsNodeList                 mNodeList;                                       /**< (--)    Node List. */
        int                           mPort0;                                          /**< (--)    Nominal inlet port index. */
        int                           mPort1;                                          /**< (--)    Nominal outlet port index. */
        double                        mMaxConductivity;                                /**< (m2)    Nominal maximum conductivity. */
        double                        mExpansionScaleFactor;                           /**< (--)    Nominal scale factor for isentropic gas cooling. */
        PolyFluid*                    mInput;                                          /**< (--)    Nominal input atmosphere. */
        PolyFluid*                    mOutput;                                         /**< (--)    Nominal output atmosphere. */
        DefinedChemicalReactions      mReactions;                                      /**< (--)    Defined chemical reactions. */
        ChemicalReaction::Type        mReactionTypes[N_REACTIONS];                     /**< (--)    Nominal array of reaction types. */
        DefinedChemicalCompounds      mCompounds;                                      /**< (--)    Defined chemical compounds. */
        ChemicalCompound::Type        mCompoundTypes[N_COMPOUNDS];                     /**< (--)    Nominal array of compound types. */
        double                        mThermalLength;                                  /**< (m)     Tube length for thermal convection. */
        double                        mThermalDiameter;                                /**< (m)     Tube inner diameter for thermal convection. */
        double                        mSurfaceRoughness;                               /**< (m)     Tube wall surface roughness for thermal convection. */
        double                        mThermalSurfaceArea;                             /**< (m2)    Tube inner surface area for thermal convection. */
        double                        mThermalROverD;                                  /**< (--)    Tube surface roughness over diameter for thermal convection. */
        double*                       mReactantEfficiency;                             /**< (--)    Nominal reactant efficiencies. */
        double*                       mReagentEfficiency;                              /**< (--)    Nominal reagent efficiencies. */
        GunnsFluidReactorConfigData*  mConfigData;                                     /**< (--)    Nominal Chemical Reactor configuration data. */
        bool                          mMalfBlockageFlag;                               /**< (--)    Blockage malfunction flag. */
        double                        mMalfBlockageValue;                              /**< (--)    Blockage malfunction value. */
        double                        mTotalMasses[N_COMPOUNDS];                       /**< (kg)    Nominal array of compound total masses. */
        double                        mWallTemperature;                                /**< (K)     Tube wall temperature for thermal convection  (simbus input from thermal aspect). */
        GunnsFluidReactorInputData*   mInputData;                                      /**< (--)    Nominal Chemical Reactor input data. */
        double                        mFluidTemperature;                               /**< (K)     Temperature of the fluid in the reactor. */
        double                        mWallHeatFlux;                                   /**< (W)     Convection heat flow from the fluid to the tube wall (simbus output to thermal aspect). */
        FriendlyGunnsFluidReactor*    mArticle;                                        /**< (--)    Nominal Chemical Reactor. */
        double                        mFlowRate;                                       /**< (kg/s)  Nominal mass flow rate. */
        double                        mTimeStep;                                       /**< (s)     Nominal time step. */
        double                        mTolerance;                                      /**< (--)    Nominal tolerance for comparison of expected and returned values. */
        static int                    TEST_ID;                                         /**< (--)    Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidReactor(const UtGunnsFluidReactor&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidReactor& operator =(const UtGunnsFluidReactor&);
};

///@}

#endif
