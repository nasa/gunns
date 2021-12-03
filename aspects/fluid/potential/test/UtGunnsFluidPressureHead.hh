#ifndef UtGunnsFluidPressureHead_EXISTS
#define UtGunnsFluidPressureHead_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_POTENTIAL_PRESSURE_HEAD_SPOTTER GUNNS Fluid Pressure Head Spotter Unit Test
/// @ingroup  UT_GUNNS_FLUID_POTENTIAL
///
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Pressure Head Spotter class
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/potential/GunnsFluidPressureHead.hh"
#include "core/GunnsFluidPotential.hh"
#include "aspects/fluid/capacitor/GunnsFluidAccum.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidPressureHead and befriend UtGunnsFluidPressureHead.
///
/// @details  Class derived from the unit under test.  It has a constructor with the same arguments
///           as the parent and a default destructor, but it befriends the unit test case driver
///           class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidPressureHead : public GunnsFluidPressureHead
{
    public:
        FriendlyGunnsFluidPressureHead(GunnsFluidLink& link)
            : GunnsFluidPressureHead(link) {;}
        virtual ~FriendlyGunnsFluidPressureHead() {;}
        friend class UtGunnsFluidPressureHead;
};

class FriendlyGunnsFluidPressureHeadAccum : public GunnsFluidAccum
{
    public:
        FriendlyGunnsFluidPressureHeadAccum() {;}
        virtual ~FriendlyGunnsFluidPressureHeadAccum() {;}
        friend class UtGunnsFluidPressureHead;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterConfigData.
///
/// @details  Derives from GunnsNetworkSpotterConfigData and is used to test that a dynamic_cast of
///           this type to the GunnsFluidPressureHeadConfigData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsFluidPressureHeadConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        BadGunnsFluidPressureHeadConfigData(const std::string& name) : GunnsNetworkSpotterConfigData(name) {}
        virtual ~BadGunnsFluidPressureHeadConfigData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test implementation of GunnsNetworkSpotterInputData.
///
/// @details  Derives from GunnsNetworkSpotterInputData and is used to test that a dynamic_cast of
///           this type to the GunnsFluidPressureHeadInputData test article type can fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
class BadGunnsFluidPressureHeadInputData : public GunnsNetworkSpotterInputData
{
    public:
        BadGunnsFluidPressureHeadInputData() {}
        virtual ~BadGunnsFluidPressureHeadInputData() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Pressure Head Spotter Unit Tests.
///
/// @details  This class provides the unit tests for the GunnsFluidPressureHead class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidPressureHead : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this GunnsFluidPressureHead unit test.
        UtGunnsFluidPressureHead();
        /// @brief    Default destructs this GunnsFluidPressureHead unit test.
        virtual ~UtGunnsFluidPressureHead();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests the config data class.
        void testConfig();
        /// @brief    Tests the input data class.
        void testInput();
        /// @brief    Tests default constructors.
        void testDefaultConstruction();
        /// @brief    Tests initialization.
        void testInitialize();
        /// @brief    Tests initialization exceptions.
        void testInitializeExceptions();
        /// @brief    Tests the stepPreSolver method with a potential link.
        void testPreSolverPotential();
        /// @brief    Tests the stepPreSolver method with an accumulator link.
        void testPreSolverAccum();
        /// @brief    Tests the stepPostSolver method.
        void testPostSolver();
        /// @brief    Tests the setter and getter methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidPressureHead);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testInitializeExceptions);
        CPPUNIT_TEST(testPreSolverPotential);
        CPPUNIT_TEST(testPreSolverAccum);
        CPPUNIT_TEST(testPostSolver);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 3, N_FLUIDS = 1};
        FluidProperties::FluidType        tTypes[N_FLUIDS];     /**< (--)   Constituent fluid types array */
        double                            tFractions[N_FLUIDS]; /**< (--)   Constituent fluid mass fractions array */
        DefinedFluidProperties*           tFluidProperties;     /**< (--)   Predefined fluid properties */
        PolyFluidConfigData*              tFluidConfig;         /**< (--)   Fluid config data */
        PolyFluidInputData*               tFluidInput0;         /**< (--)   Fluid input data for node 0 */
        PolyFluidInputData*               tFluidInput1;         /**< (--)   Fluid input data for node 1 */
        std::vector<GunnsBasicLink*>      tLinks;               /**< (--)   Links vector */
        GunnsFluidNode                    tNodes[N_NODES];      /**< (--)   Network nodes */
        GunnsNodeList                     tNodeList;            /**< (--)   Network nodes list */
        FriendlyGunnsFluidPressureHead    tArticle;             /**< (--)   Test article */
        std::string                       tName;                /**< (--)   Instance name */
        GunnsFluidPressureHeadConfigData* tConfig;              /**< (--)   Nominal config data */
        GunnsFluidPressureHeadInputData*  tInput;               /**< (--)   Nominal input data */
        double                            tFluidColumn[3];      /**< (m)    Nominal input data */
        double                            tAcceleration[3];     /**< (m/s2) Nominal input data */
        bool                              tReverseAcceleration; /**< (--)   Nominal input data */
        double                            tRotationDcm[9];      /**< (--)   Nominal input data */
        bool                              tTransposeRotation;   /**< (--)   Nominal input data */
        GunnsFluidConductor               tConductorLink;       /**< (--)   Test conductor link */
        GunnsFluidPotential               tPotentialLink;       /**< (--)   Test potential link */
        GunnsFluidAccum                   tAccumLink;           /**< (--)   Test accumulator link */
        double                            tTimeStep;            /**< (--)   Time step size for this test */
        static int                        TEST_ID;              /**< (--)   Test identification number. */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidPressureHead(const UtGunnsFluidPressureHead& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidPressureHead& operator =(const UtGunnsFluidPressureHead& that);
};

///@}

#endif
