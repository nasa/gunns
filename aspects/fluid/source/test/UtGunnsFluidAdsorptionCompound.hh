#ifndef UtGunnsFluidAdsorptionCompound_EXISTS
#define UtGunnsFluidAdsorptionCompound_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_ADSORBED_COMPOUND   Adsorbed Compound Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Adsorbed Compound model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidAdsorptionCompound.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidAdsorptionCompound and befriend UtGunnsFluidAdsorptionCompound.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidAdsorptionCompound : public GunnsFluidAdsorptionCompound
{
    public:
        FriendlyGunnsFluidAdsorptionCompound();
        virtual ~FriendlyGunnsFluidAdsorptionCompound();
        friend class UtGunnsFluidAdsorptionCompound;
};
inline FriendlyGunnsFluidAdsorptionCompound::FriendlyGunnsFluidAdsorptionCompound() : GunnsFluidAdsorptionCompound() {};
inline FriendlyGunnsFluidAdsorptionCompound::~FriendlyGunnsFluidAdsorptionCompound() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Adsorber unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Absorbed Compound model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidAdsorptionCompound: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Adsorber unit test.
        UtGunnsFluidAdsorptionCompound();
        /// @brief    Default constructs this Adsorber unit test.
        virtual ~UtGunnsFluidAdsorptionCompound();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests initialize method with a trace compound type.
        void testNominalInitializationTc();
        /// @brief    Tests copy construction.
        void testCopyConstruction();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests sorb method for a fluid type compound.
        void testSorb();
        /// @brief    Tests sorb method for a trace compound.
        void testSorbTc();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidAdsorptionCompound);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testNominalInitializationTc);
        CPPUNIT_TEST(testCopyConstruction);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testSorb);
        CPPUNIT_TEST(testSorbTc);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        /// @brief   Enumeration for the number of fluid constituents.
        enum {N_FLUIDS = 3, N_TC = 3};
        DefinedFluidProperties*               tFluidProperties;       /**< (--)    Defined fluid properties. */
        FluidProperties::FluidType            tTypes[N_FLUIDS];       /**< (--)    Array of Fluid Types. */
        double                                tFractions[N_FLUIDS];   /**< (--)    Array of fluid mass fractions. */
        DefinedChemicalCompounds*             tCompoundProperties;    /**< (--)    Defined chemical compounds. */
        ChemicalCompound::Type                tTcTypes[N_TC];         /**< (--)    Array of trace compound types. */
        GunnsFluidTraceCompoundsConfigData*   tFluidTcConfig;    /**< (--)    Fluid trace compounds config. */
        double                                tTcFractions[N_TC];     /**< (--)    Trace compounds mole fractions for nodes. */
        GunnsFluidTraceCompoundsInputData*    tFluidTcInput;     /**< (--)    Trace compounds input data for nodes. */
        PolyFluidConfigData*                  tFluidConfig;           /**< (--)    Fluid config data. */
        PolyFluidInputData*                   tFluidInput;            /**< (--)    Fluid input data. */
        PolyFluid*                            tFluid;                 /**< (--)    Fluid object. */
        std::string                           tName;                  /**< (--)    Nominal name. */
        GunnsFluidAdsorptionCompound          tConfig;                /**< (--)    Nominal config data object. */
        FriendlyGunnsFluidAdsorptionCompound* tArticle;               /**< (--)    Pointer to the friendly adsorber under test. */
        static int                            TEST_ID;                 /**< (--)    Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidAdsorptionCompound(const UtGunnsFluidAdsorptionCompound&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidAdsorptionCompound& operator =(const UtGunnsFluidAdsorptionCompound&);
};

///@}

#endif
