#ifndef UtGunnsFluidSimpleQd_EXISTS
#define UtGunnsFluidSimpleQd_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_SIMPLE_QD   Simple Quick-Disconnect Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Simple Quick-Disconnect link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidSimpleQd.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSimpleQd and befriend UtGunnsFluidSimpleQd.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSimpleQd : public GunnsFluidSimpleQd
{
    public:
        FriendlyGunnsFluidSimpleQd();
        virtual ~FriendlyGunnsFluidSimpleQd();
        friend class UtGunnsFluidSimpleQd;
};
inline FriendlyGunnsFluidSimpleQd::FriendlyGunnsFluidSimpleQd() : GunnsFluidSimpleQd() {};
inline FriendlyGunnsFluidSimpleQd::~FriendlyGunnsFluidSimpleQd() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Simple Quick-Disconnect unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Simple Quick-Disconnect model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSimpleQd: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Simple Quick-Disconnect unit test.
        UtGunnsFluidSimpleQd();
        /// @brief    Default destructs this Simple Quick-Disconnect unit test.
        virtual ~UtGunnsFluidSimpleQd();
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
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update state method.
        void testUpdateState();
        /// @brief    Tests update fluid method.
        void testUpdateFluid();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidSimpleQd);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testNoConvectionInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testUpdateFluid);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 2};
        FluidProperties::FluidType   tTypes[N_FLUIDS];      /**< (--) Constituent fluid types array */
        double                       tFractions[N_FLUIDS];  /**< (--) Constituent fluid mass fractions array */
        DefinedFluidProperties*      tFluidProperties;      /**< (--) Predefined fluid properties */
        PolyFluidConfigData*         tFluidConfig;          /**< (--) Fluid config data */
        PolyFluidInputData*          tFluidInput0;          /**< (--) Fluid input data for node 0 */
        PolyFluidInputData*          tFluidInput1;          /**< (--) Fluid input data for node 1 */
        std::vector<GunnsBasicLink*> tLinks;                /**< (--) Link vector */
        std::string                  tName;                 /**< (--) Nominal name */
        GunnsFluidNode               tNodes[N_NODES];       /**< (--) Nominal connected nodes */
        GunnsNodeList                tNodeList;             /**< (--) Network node structure */
        int                          tPort0;                /**< (--) Nominal inlet port index */
        int                          tPort1;                /**< (--) Nominal outlet port index */
        double                       tMaxConductivity;      /**< (m2) Nominal maximum conductivity */
        double                       tExpansionScaleFactor; /**< (--) Nominal scale factor for isentropic gas cooling */
        double                       tThermalLength;        /**< (m)  Tube length for thermal convection */
        double                       tThermalDiameter;      /**< (m)  Tube inner diameter for thermal convection */
        double                       tSurfaceRoughness;     /**< (m)  Tube wall surface roughness for thermal convection */
        GunnsFluidSimpleQdConfigData* tConfigData;          /**< (--) Pointer to nominal config data */
        bool                         tBlockageFlag;         /**< (--) Blockage malf flag */
        double                       tBlockage;             /**< (--) Blockage malf value */
        GunnsFluidSimpleQd::State    tState;                /**< (--) Initial connection state of the QD */
        double                       tWallTemperature;      /**< (K)  Initial wall temperature */
        GunnsFluidSimpleQdInputData* tInputData;            /**< (--) Pointer to nominal input data */
        FriendlyGunnsFluidSimpleQd*  tArticle;              /**< (--) Pointer to test article */
        static const double          PI;                    /**< (--) Units conversion constant */
        static int                   TEST_ID;               /**< (--) Test identification number. */

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSimpleQd(const UtGunnsFluidSimpleQd&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSimpleQd& operator =(const UtGunnsFluidSimpleQd&);
};

///@}

#endif
