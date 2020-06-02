#ifndef UtGunnPumpCavitation_EXISTS
#define UtGunnPumpCavitation_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_POTENTIAL_PUMP_CAVITATION Pump Cavitation Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_POTENTIAL
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Pump Cavitation utility class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/potential/GunnsPumpCavitation.hh"
#include "core/GunnsFluidNode.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsPumpCavitation and befriend UtGunnPumpCavitation.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsPumpCavitation : public GunnsPumpCavitation
{
    public:
        FriendlyGunnsPumpCavitation();
        virtual ~FriendlyGunnsPumpCavitation();
        friend class UtGunnsPumpCavitation;
    private:
        FriendlyGunnsPumpCavitation(const FriendlyGunnsPumpCavitation&);
        FriendlyGunnsPumpCavitation& operator =(const FriendlyGunnsPumpCavitation&);
};
inline FriendlyGunnsPumpCavitation::FriendlyGunnsPumpCavitation() : GunnsPumpCavitation() {}
inline FriendlyGunnsPumpCavitation::~FriendlyGunnsPumpCavitation() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Pump Cavitation unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Pump Cavitation utility class
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsPumpCavitation : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Pump Cavitation unit test.
        UtGunnsPumpCavitation();
        /// @brief    Default destructs this Pump Cavitation unit test.
        virtual ~UtGunnsPumpCavitation();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests the initialize method.
        void testInitialize();
        /// @brief    Tests the computeVaporPressure method.
        void testVaporPressure();
        /// @brief    Tests the update method.
        void testUpdate();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsPumpCavitation);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialize);
        CPPUNIT_TEST(testVaporPressure);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST_SUITE_END();

        ///  @brief   Enumeration for the number of fluid constituents.
        enum {N_FLUIDS = 2};
        FluidProperties::FluidType   tTypes[N_FLUIDS];     /**< (--) Constituent fluid types array */
        double                       tFractions[N_FLUIDS]; /**< (--) Constituent fluid mass fractions array */
        DefinedFluidProperties*      tFluidProperties;     /**< (--) Predefined fluid properties */
        PolyFluidConfigData*         tFluidConfig;         /**< (--) Fluid config data */
        PolyFluidInputData*          tFluidInput;          /**< (--) Fluid input data for the node */
        GunnsFluidNode               tNode;                /**< (--) Test node */
        FriendlyGunnsPumpCavitation* tArticle;             /**< (--) Article under test */
        static int                   TEST_ID;              /**< (--) Test identification number. */

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsPumpCavitation(const UtGunnsPumpCavitation&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsPumpCavitation& operator =(const UtGunnsPumpCavitation&);
};

///@}

#endif
