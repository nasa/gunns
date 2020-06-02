#ifndef UtGunnsFluidAdsorber_EXISTS
#define UtGunnsFluidAdsorber_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_ADSORBER   Adsorber Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Adsorber link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidAdsorber.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidAdsorber and befriend UtGunnsFluidAdsorber.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidAdsorber : public GunnsFluidAdsorber
{
    public:
        FriendlyGunnsFluidAdsorber();
        virtual ~FriendlyGunnsFluidAdsorber();
        friend class UtGunnsFluidAdsorber;
};
inline FriendlyGunnsFluidAdsorber::FriendlyGunnsFluidAdsorber() : GunnsFluidAdsorber() {};
inline FriendlyGunnsFluidAdsorber::~FriendlyGunnsFluidAdsorber() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Adsorber unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Adsorber link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidAdsorber: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Adsorber unit test.
        UtGunnsFluidAdsorber();
        /// @brief    Default constructs this Adsorber unit test.
        virtual ~UtGunnsFluidAdsorber();
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
        /// @brief    Tests initialize method with no trace compounds.
        void testNominalInitializationNoTc();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update fluid method (nominal).
        void testUpdateFluidNominal();
        /// @brief    Tests update fluid method (no/reverse flow).
        void testUpdateFluidOffNominal();
        /// @brief    Tests update fluid method (desorbtionCycle).
        void testUpdateFluidDesorbtion();
        /// @brief    Tests specific port mapping rules.
        void testPortMapping();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests restart method.
        void testRestart();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidAdsorber);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testNominalInitializationNoTc);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateFluidNominal);
        CPPUNIT_TEST(testUpdateFluidOffNominal);
        CPPUNIT_TEST(testUpdateFluidDesorbtion);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();
        /// @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 2, N_TC = 3};
        DefinedFluidProperties*        mFluidProperties;       /**< (--)    Defined fluid properties. */
        FluidProperties::FluidType     mTypes[N_FLUIDS];       /**< (--)    Array of Fluid Types. */
        double                         mFractions[N_FLUIDS];   /**< (--)    Array of fluid mass fractions. */
        DefinedChemicalCompounds*      mCompoundProperties;    /**< (--)    Defined chemical compounds. */
        ChemicalCompound::Type         mTcTypes[N_TC];         /**< (--)    Array of trace compound types. */
        GunnsFluidTraceCompoundsConfigData* mFluidTcConfig;    /**< (--)    Fluid trace compounds config. */
        double                         mTcFractions[N_TC];     /**< (--)    Trace compounds mole fractions for nodes. */
        GunnsFluidTraceCompoundsInputData*  mFluidTcInput;     /**< (--)    Trace compounds input data for nodes. */
        PolyFluidConfigData*           mFluidConfig;           /**< (--)    Fluid config data. */
        PolyFluidInputData*            mFluidInput0;           /**< (--)    Fluid 0 input data. */
        PolyFluidInputData*            mFluidInput1;           /**< (--)    Fluid 1 input data. */
        std::vector<GunnsBasicLink*>   mLinks;                 /**< (--)    Link vector. */
        std::string                    mName;                  /**< (--)    Nominal name. */
        GunnsFluidNode                 mNodes[N_NODES];        /**< (--)    Fluid nodes. */
        GunnsNodeList                  mNodeList;              /**< (--)    Node List. */
        int                            mPort0;                 /**< (--)    Nominal inlet port index. */
        int                            mPort1;                 /**< (--)    Nominal outlet port index. */
        double                         mMaxConductivity;       /**< (m2)    Nominal maximum conductivity. */
        double                         mExpansionScaleFactor;  /**< (--)    Nominal scale factor for isentropic gas cooling. */
        FluidProperties::FluidType     mGasType;               /**< (--)    Fluid type of constituent gas being sorbed. */
        int                            mGasIndex;              /**< (--)    Index of constituent gas being sorbed. */
        double                         mEfficiency;            /**< (--)    Efficiency of gas adsorbtion from atmosphere. */
        double                         mMaxAdsorbtionRate;     /**< (kg/s)  Maximum mass rate at which gas can be adsorbed from atmosphere. */
        double                         mMaxAdsorbedMass;       /**< (kg)    Maximum amount of adsorbed mass. */
        double                         mDesorbtionRate;        /**< (kg/s)  Mass rate at which adsorbed gas is desorbed. */
        double                         mThermalLength;         /**< (m)     Tube length for thermal convection. */
        double                         mThermalDiameter;       /**< (m)     Tube inner diameter for thermal convection. */
        double                         mSurfaceRoughness;      /**< (m)     Tube wall surface roughness for thermal convection. */
        double                         mThermalSurfaceArea;    /**< (m2)    Tube inner surface area for thermal convection. */
        double                         mThermalROverD;         /**< (--)    Tube surface roughness over diameter for thermal convection. */
        GunnsFluidAdsorberConfigData*  mConfigData;            /**< (-)     Pointer to nominal configuration data. */
        bool                           mMalfBlockageFlag;      /**< (--)    Blockage malfunction flag. */
        double                         mMalfBlockageValue;     /**< (--)    Blockage malfunction value. */
        bool                           mMalfEfficiencyFlag;    /**< (--)    Efficiency malfunction flag. */
        bool                           mMalfEfficiencyValue;   /**< (--)    Efficiency malfunction value. */
        bool                           mDesorbtionCycle;       /**< (--)    Desorbtion cycle flag (simbus input from signal aspect). */
        double                         mAdsorbedMass;          /**< (kg)    Adsorbed mass in the adsorber. */
        double                         mWallTemperature;       /**< (K)     Tube wall temperature for thermal convection. */
        double                         mTcEfficiencyData[N_TC];      /**< (--)  Trace compounds absorption efficiencies. */
        double                         mTcMaxAdsorbedMassData[N_TC]; /**< (kg)  Trace compounds max absorbable masses. */
        double                         mTcAdsorbedMassData[N_TC];    /**< (kg)  Trace compounds absorbed masses. */
        GunnsFluidTraceCompoundsInputData mTcEfficiency;       /**< (--)    Trace compounds absorption efficiencies. */
        GunnsFluidTraceCompoundsInputData mTcMaxAdsorbedMass;  /**< (kg)    Trace compounds max absorbable masses. */
        GunnsFluidTraceCompoundsInputData mTcAdsorbedMass;     /**< (kg)    Trace compounds absorbed masses. */
        GunnsFluidAdsorberInputData*   mInputData;             /**< (--)    Pointer to nominal input data. */
        double                         mFluidTemperature;      /**< (K)     Temperature of the fluid in the reactor. */
        double                         mWallHeatFlux;          /**< (W)     Convection heat flow from the fluid to the tube wall (simbus output to thermal aspect). */
        double                         mMass;                  /**< (kg)    Adsorbed mass this cycle. */
        double                         mSorbtionFlowRate;      /**< (kg/s)  Sorbtion mass flow rate. */
        FriendlyGunnsFluidAdsorber*    mArticle;               /**< (--)    Pointer to the friendly adsorber under test. */
        double                         mFlowRate;              /**< (kg/s)  Nominal mass flow rate. */
        double                         mTimeStep;              /**< (s)     Nominal time step. */
        double                         mTolerance;             /**< (--)    Nominal tolerance for comparison of expected and returned values. */
        static int                    TEST_ID;                 /**< (--)    Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidAdsorber(const UtGunnsFluidAdsorber&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidAdsorber& operator =(const UtGunnsFluidAdsorber&);
};

///@}

#endif
