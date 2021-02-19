#ifndef UtGunnsFluidHeatExchanger_EXISTS
#define UtGunnsFluidHeatExchanger_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_HEAT_EXHANGER    Heat Exchanger Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Heat Exchanger link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidHeatExchanger.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidHeatExchanger and befriend UtGunnsFluidHeatExchanger.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidHeatExchanger : public GunnsFluidHeatExchanger
{
    public:
        FriendlyGunnsFluidHeatExchanger();
        virtual ~FriendlyGunnsFluidHeatExchanger();
        friend class UtGunnsFluidHeatExchanger;
        PolyFluid* getFluid();
        double getEnergyGain(int segment);
        void setSegmentTemp(int i, double temp);
        double getSegmentTemp(int i);
};
inline FriendlyGunnsFluidHeatExchanger::FriendlyGunnsFluidHeatExchanger()
    : GunnsFluidHeatExchanger() {};
inline FriendlyGunnsFluidHeatExchanger::~FriendlyGunnsFluidHeatExchanger() {}
inline PolyFluid* FriendlyGunnsFluidHeatExchanger::getFluid() { return mInternalFluid; }
/// @brief compute and return segment energy gain
inline double FriendlyGunnsFluidHeatExchanger::getEnergyGain(int segment) {
    if(0 == mSegEnergyGain || segment < 0 || segment >= mNumSegs) {
        return 0.0;
    }
    return mSegEnergyGain[segment];
}
/// @brief segment temperature modifier
inline void FriendlyGunnsFluidHeatExchanger::setSegmentTemp(int i, double temp)
{mSegTemperature[i] = temp;}
/// @brief segment temperature accessor
inline double FriendlyGunnsFluidHeatExchanger::getSegmentTemp(int i)
{return mSegTemperature[i];}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Check Valve unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Check Valve link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidHeatExchanger: public CppUnit::TestFixture {
    public:
        /// @brief    Default constructs this Heat Exchanger Flow Path unit test.
        UtGunnsFluidHeatExchanger();
        /// @brief    Default destructs this Heat Exchanger Flow Path unit test.
        virtual ~UtGunnsFluidHeatExchanger();
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
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update fluid (no flow).
        void testNoFlow ();
        /// @brief    Tests update fluid (same temperatures).
        void testTemperatures ();
        /// @brief    Tests update fluid (nominal and reverse flow).
        void testFlowDirections ();
        /// @brief    Tests computeHeatTransferCoefficient method.
        void testHtc();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests restart method.
        void testRestart();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidHeatExchanger);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testNoFlow);
        CPPUNIT_TEST(testTemperatures);
        CPPUNIT_TEST(testFlowDirections);
        CPPUNIT_TEST(testHtc);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 1};
        FluidProperties::FluidType         mTypes[N_FLUIDS];           /**< (--)   Constituent fluid types array. */
        double                             mFractions[N_FLUIDS];       /**< (--)   Constituent fluid mass fractions array. */
        DefinedFluidProperties*            mFluidProperties;           /**< (--)   Predefined fluid properties. */
        PolyFluidConfigData*               mFluidConfig;               /**< (--)   Fluid config data. */
        PolyFluidInputData*                mFluidInput0;               /**< (--)   Fluid input data for node 0. */
        PolyFluidInputData*                mFluidInput1;               /**< (--)   Fluid input data for node 1. */
        std::vector<GunnsBasicLink*>       mLinks;                     /**< (--)   Link vector. */
        std::string                        mName;                      /**< (--)   Nominal name. */
        GunnsFluidNode                     mNodes[N_NODES];            /**< (--)   Nominal connected nodes. */
        GunnsNodeList                      mNodeList;                  /**< (--)   Network node structure. */
        int                                mPort0;                     /**< (--)   Nominal inlet port index. */
        int                                mPort1;                     /**< (--)   Nominal outlet port index. */
        double                             mMaxConductivity;           /**< (m2)   Nominal maximum conductivity. */
        double                             mExpansionScaleFactor;      /**< (--)   Nominal scale factor for isentropic gas cooling. */
        int                                mNumSegs;                   /**< (--)   Number of segments for this Heat Exchanger. */
        GunnsFluidHeatExchangerConfigData* mConfigData;                /**< (--)   Pointer to nominal configuration data. */
        bool                               mMalfBlockageFlag;          /**< (--)   Blockage malfunction flag. */
        double                             mMalfBlockageValue;         /**< (--)   Blockage malfunction value. */
        double                             mHeatTransferCoefficient;   /**< (W/K)  Default heat transfer coefficient (W/K). */
        double                             mInitialSegmentTemperature; /**< (K)    Initial value for segment temperatures. */
        GunnsFluidHeatExchangerInputData*  mInputData;                 /**< (--)   Pointer to nominal input data. */
        FriendlyGunnsFluidHeatExchanger*   mArticle;                   /**< (--)   Pointer to the friendly Leak under test. */
        double                             mSegEnergyGain[10];         /**< (W)    Array of segment energy gains. */
        double                             mTotalEnergyGain;           /**< (W)    Total energy gain. */
        double                             mFlowRate;                  /**< (kg/s) Nominal mass flow rate. */
        double                             mTimeStep;                  /**< (s)    Nominal time step. */
        double                             mTolerance;                 /**< (--)   Nominal tolerance for comparison of expected and returned values. */
        static int                         TEST_ID;                    /**< (--)   Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidHeatExchanger(const UtGunnsFluidHeatExchanger&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidHeatExchanger& operator =(const UtGunnsFluidHeatExchanger&);
};

///@}

#endif
