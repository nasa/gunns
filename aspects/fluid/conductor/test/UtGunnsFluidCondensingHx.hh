#ifndef UtGunnsFluidCondensingHx_EXISTS
#define UtGunnsFluidCondensingHx_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_CONDENSING_HX  Condensing HX Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Condensing HX link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidCondensingHx.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidCondensingHx and befriend UtGunnsFluidCondensingHx.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidCondensingHx : public GunnsFluidCondensingHx
{
    public:
        FriendlyGunnsFluidCondensingHx();
        virtual ~FriendlyGunnsFluidCondensingHx();
        friend class UtGunnsFluidCondensingHx;
};
inline FriendlyGunnsFluidCondensingHx::FriendlyGunnsFluidCondensingHx()
    : GunnsFluidCondensingHx() {};
inline FriendlyGunnsFluidCondensingHx::~FriendlyGunnsFluidCondensingHx() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Condensing HX unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Condensing HX link
///           model within the CPPUnit framework.
///
/// @note     Because GunnsFluidCondensingHx was a later refactoring of
///           GunnsFluidCondensingHxSeparator, a lot of functionality of GunnsFluidCondensingHx is
///           tested in UtGunnsFluidCondensingHxSeparator.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidCondensingHx: public CppUnit::TestFixture {
    public:
        /// @brief    Default constructs this Condensing HX Flow Path unit test.
        UtGunnsFluidCondensingHx();
        /// @brief    Default destructs this Condensing HX Flow Path unit test.
        virtual ~UtGunnsFluidCondensingHx();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config data.
        void testConfig();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testInitialization();
        /// @brief    Tests computeHeatTransferCoefficient method.
        void testHtc();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidCondensingHx);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testHtc);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 3};
        FluidProperties::FluidType        tTypes[N_FLUIDS];     /**< (--)       Constituent fluid types array */
        double                            tFractions[N_FLUIDS]; /**< (--)       Constituent fluid mass fractions array */
        DefinedFluidProperties*           tFluidProperties;     /**< (--)       Predefined fluid properties */
        PolyFluidConfigData*              tFluidConfig;         /**< (--)       Fluid config data */
        PolyFluidInputData*               tFluidInput;          /**< (--)       Fluid input data */
        std::vector<GunnsBasicLink*>      tLinks;               /**< (--)       Link vector */
        std::string                       tName;                /**< (--)       Test name */
        GunnsFluidNode                    tNodes[N_NODES];      /**< (--)       Test connected nodes */
        GunnsNodeList                     tNodeList;            /**< (--)       Test node structure */
        int                               tPort0;               /**< (--)       Test inlet port index */
        int                               tPort1;               /**< (--)       Test outlet port index */
        double                            tMaxConductivity;     /**< (m2)       Nominal config data */
        int                               tHxNumSegments;       /**< (--)       Nominal config data */
        double                            tHxDryHtcCoeff0;      /**< (W/K)      Nominal config data */
        double                            tHxDryHtcCoeff1;      /**< (W*s/K/kg) Nominal config data */
        double                            tHxDryHtcExponent;    /**< (--)       Nominal config data */
        double                            tHxDryHtcLimit;       /**< (W/K)      Nominal config data */
        GunnsFluidCondensingHxConfigData* tConfigData;          /**< (--)       Nominal config data */
        bool                              tMalfBlockageFlag;    /**< (--)       Nominal input data */
        double                            tMalfBlockageValue;   /**< (--)       Nominal input data */
        double                            tWallTemperature;     /**< (K)        Nominal input data */
        GunnsFluidCondensingHxInputData*  tInputData;           /**< (--)       nominal input data */
        FriendlyGunnsFluidCondensingHx*   tArticle;             /**< (--)       Test article */
        static int                        TEST_ID;              /**< (--)       Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidCondensingHx(const UtGunnsFluidCondensingHx&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidCondensingHx& operator =(const UtGunnsFluidCondensingHx&);
};

///@}

#endif
