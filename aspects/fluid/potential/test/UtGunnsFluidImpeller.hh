#ifndef UtGunnsFluidImpeller_EXISTS
#define UtGunnsFluidImpeller_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_POTENTIAL_IMPELLER     Impeller Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_POTENTIAL
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @todo - complete unit tests
///
/// @details  Unit Tests for the GUNNS Fluid Impeller link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/potential/GunnsFluidImpeller.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidImpeller and befriend UtGunnsFluidImpeller.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidImpeller : public GunnsFluidImpeller
{
    public:
        FriendlyGunnsFluidImpeller();
        virtual ~FriendlyGunnsFluidImpeller();
        friend class UtGunnsFluidImpeller;
};
inline FriendlyGunnsFluidImpeller::FriendlyGunnsFluidImpeller() : GunnsFluidImpeller() {};
inline FriendlyGunnsFluidImpeller::~FriendlyGunnsFluidImpeller() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Impeller unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Impeller link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidImpeller: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Impeller unit test.
        UtGunnsFluidImpeller();
        /// @brief    Default destructs this Impeller unit test.
        virtual ~UtGunnsFluidImpeller();
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
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidImpeller);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testUpdateFluid);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 2};
        FluidProperties::FluidType     mTypes[N_FLUIDS];        /**< (--)                   Constituent fluid types array.  */
        double                         mFractions[N_FLUIDS];    /**< (--)                   Constituent fluid mass fractions array. */
        DefinedFluidProperties*        mFluidProperties;        /**< (--)                   Predefined fluid properties. */
        PolyFluidConfigData*           mFluidConfig;            /**< (--)                   Fluid config data. */
        PolyFluidInputData*            mFluidInput0;            /**< (--)                   Fluid input data for node 0. */
        PolyFluidInputData*            mFluidInput1;            /**< (--)                   Fluid input data for node 1. */
        std::vector<GunnsBasicLink*>   mLinks;                  /**< (--)                   Link vector. */
        std::string                    mName;                   /**< (--)                   Nominal name. */
        GunnsFluidNode                 mNodes[N_NODES];         /**< (--)                   Nominal connected nodes. */
        GunnsNodeList                  mNodeList;               /**< (--)                   Network node structure. */
        int                            mPort0;                  /**< (--)                   Nominal inlet port index. */
        int                            mPort1;                  /**< (--)                   Nominal outlet port index. */
        double                         mMaxConductivity;        /**< (m2)                   Nominal maximum conductivity. */
        double                         mExpansionScaleFactor;   /**< (--)                   Nominal scale factor for isentropic gas cooling. */
        double                         mReferenceDensity;       /**< (kg/m3)                Reference fluid density for this Impeller power curve. */
        double                         mReferenceSpeed;         /**< (rev/min)              Reference speed for this Impeller power curve. */
        double                         mThermalLength;          /**< (m)                    Tube length for thermal convection. */
        double                         mThermalDiameter;        /**< (m)                    Tube inner diameter for thermal convection. */
        double                         mSurfaceRoughness;       /**< (m)                    Tube wall surface roughness for thermal convection. */
        double                         mPowerCurveCoefficient;  /**< (kPa*min2/rev2*m3/kg)  Impeller power curve coefficient. */
        double                         mThermalSurfaceArea;     /**< (m2)                   Impeller surface area for thermal convection. */
        double                         mThermalROverD;          /**< (--)                   Impeller surface roughness over diameter for convection. */
        GunnsFluidImpellerConfigData*  mConfigData;             /**< (--)                   Pointer to the nominal configuration data. */
        bool                           mMalfBlockageFlag;       /**< (--)                   Blockage malfunction flag. */
        double                         mMalfBlockageValue;      /**< (--)                   Blockage malfunction value. */
        double                         mSourcePressure;         /**< (kPa)                  Source (reference) fluid delta pressure. */
        double                         mMotorSpeed;             /**< (rev/min)              Initial speed of the motor        */
        double                         mWallTemperature;        /**< (K)                    Initial impeller wall temperature */
        GunnsFluidImpellerInputData*   mInputData;              /**< (--)                   Pointer to the nominal input data. */
        FriendlyGunnsFluidImpeller*    mArticle;                /**< (--)                   Pointer to the friendly impeller under test. */
        double                         mWallHeatFlux;           /**< (W)                    Heat flux from fluid to wall output to simbus. */
        double                         mFlowRate;               /**< (kg/s)                 Nominal mass flow rate. */
        double                         mTimeStep;               /**< (s)                    Nominal time step. */
        double                         mTolerance;              /**< (--)                   Nominal tolerance for comparison of expected and returned values. */
        static int                    TEST_ID;                  /**< (--)                   Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidImpeller(const UtGunnsFluidImpeller&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidImpeller& operator =(const UtGunnsFluidImpeller&);
};

///@}

#endif
