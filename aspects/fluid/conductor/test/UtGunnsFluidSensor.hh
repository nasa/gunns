#ifndef UtGunnsFluidSensor_EXISTS
#define UtGunnsFluidSensor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_SENSOR    Sensor Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Sensor link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidSensor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSensor and befriend UtGunnsFluidSensor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSensor : public GunnsFluidSensor
{
    public:
        FriendlyGunnsFluidSensor();
        virtual ~FriendlyGunnsFluidSensor();
        friend class UtGunnsFluidSensor;
};
inline FriendlyGunnsFluidSensor::FriendlyGunnsFluidSensor() : GunnsFluidSensor() {};
inline FriendlyGunnsFluidSensor::~FriendlyGunnsFluidSensor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sensor unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Sensor link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSensor: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Sensor unit test.
        UtGunnsFluidSensor();
        /// @brief    Default destructs this Sensor unit test.
        virtual ~UtGunnsFluidSensor();
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
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests computeFlows method.
        void testComputeFlows();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidSensor);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        enum {N_NODES = 2, N_FLUIDS = 2};                   /**< (--) Enumeration for the number of nodes and fluid constituents.*/
        FluidProperties::FluidType   mTypes[N_FLUIDS];      /**< (--) Constituent fluid types array. */
        double                       mFractions[N_FLUIDS];  /**< (--) Constituent fluid mass fractions array. */
        DefinedFluidProperties*      mFluidProperties;      /**< (--) Predefined fluid properties. */
        PolyFluidConfigData*         mFluidConfig;          /**< (--) Fluid config data. */
        PolyFluidInputData*          mFluidInput0;          /**< (--) Fluid input data for node 0. */
        PolyFluidInputData*          mFluidInput1;          /**< (--) Fluid input data for node 1. */
        std::vector<GunnsBasicLink*> mLinks;                /**< (--) Link vector. */
        std::string                  mName;                 /**< (--) Nominal name. */
        GunnsFluidNode               mNodes[N_NODES];       /**< (--) Nominal connected nodes. */
        GunnsNodeList                mNodeList;             /**< (--) Network node structure. */
        int                          mPort0;                /**< (--) Nominal inlet port index. */
        int                          mPort1;                /**< (--) Nominal outlet port index. */
        double                       mMaxConductivity;      /**< (m2) Nominal maximum conductivity */
        double                       mExpansionScaleFactor; /**< (--) Nominal scale factor for isentropic gas cooling. */
        GunnsFluidSensorConfigData*  mConfigData;           /**< (--) Pointer to nominal configuration data. */
        bool                         mMalfBlockageFlag;     /**< (--) Blockage malfunction flag. */
        double                       mMalfBlockageValue;    /**< (--) Blockage malfunction value. */
        GunnsFluidSensorInputData*   mInputData;            /**< (--) Pointer to nominal input data. */
        FriendlyGunnsFluidSensor*    mArticle;              /**< (--) Pointer to the friendly Sensor under test. */
        double                       mTimeStep;             /**< (s)  Nominal time step. */
        double                       mTolerance;            /**< (--) Nominal tolerance for comparison of expected and returned values. */
        static int                   TEST_ID;               /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSensor(const UtGunnsFluidSensor&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSensor& operator =(const UtGunnsFluidSensor&);
};

///@}

#endif
