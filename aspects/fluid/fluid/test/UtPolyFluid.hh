#ifndef UtPolyFluid_EXISTS
#define UtPolyFluid_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_FLUID_POLY_FLUID     PolyFluid Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the PolyFluid class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/fluid/PolyFluid.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from PolyFluid and befriend UtPolyFluid.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyPolyFluid : public PolyFluid {
    public:
        FriendlyPolyFluid();
        FriendlyPolyFluid(const PolyFluidConfigData& configData,
                          const PolyFluidInputData&  inputData);
        FriendlyPolyFluid(const FriendlyPolyFluid& that, const std::string& name,
                          const bool copyTc = true);
        virtual ~FriendlyPolyFluid();
        friend class UtPolyFluid;
    private:
        FriendlyPolyFluid& operator =(const FriendlyPolyFluid&);
        FriendlyPolyFluid(const FriendlyPolyFluid&);
};
inline FriendlyPolyFluid::FriendlyPolyFluid() : PolyFluid() {};
inline FriendlyPolyFluid::FriendlyPolyFluid(const PolyFluidConfigData& configData,
                                            const PolyFluidInputData&  inputData)
    : PolyFluid(configData, inputData) {}
inline FriendlyPolyFluid::FriendlyPolyFluid(const FriendlyPolyFluid& that, const std::string& name,
                                            const bool copyTc)
    : PolyFluid(that, name, copyTc) {}
inline FriendlyPolyFluid::~FriendlyPolyFluid() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PolyFluid unit tests.
////
/// @details  This class provides the unit tests for the PolyFluid within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtPolyFluid : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this MonoFluid unit test.
        UtPolyFluid();
        /// @brief    Default destructs this MonoFluid unit test.
        virtual ~UtPolyFluid();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        void testConfigAndInput();
        void testNominalConstruction();
        void testDefaultConstruction();
        void testCopyConstruction();
        void testNominalInitialization();
        void testCompositeStateAccessors();
        void testCompositePropertyAccessors();
        void testSetFlowrate();
        void testSetPressure();
        void testSetMass();
        void testSetMole();
        void testSetMassAndMassFractions();
        void testSetMoleAndMoleFractions();
        void testSetTemperature();
        void testSetConstituentMass();
        void testUpdateMass();
        void testStateUpdaters();
        void testStateUpdatersOppositeFlows();
        void testComputes();
        void testSetSequence();
        void testEdits();
        void testComputeTemperature();
        void testLiquidPressureDensity();
        void testMultipleInitialization();
        void testFractionNormalization();
        void testConfigAndInputConstructionExceptions();
        void testConstructionExceptions();
        void testInitializationExceptions();
        void testStateAccessorExceptions();
        void testStateModifierExceptions();
        void testStateUpdaterExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtPolyFluid);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testNominalConstruction);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testCopyConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testCompositeStateAccessors);
        CPPUNIT_TEST(testCompositePropertyAccessors);
        CPPUNIT_TEST(testSetFlowrate);
        CPPUNIT_TEST(testSetPressure);
        CPPUNIT_TEST(testSetMass);
        CPPUNIT_TEST(testSetMole);
        CPPUNIT_TEST(testSetMassAndMassFractions);
        CPPUNIT_TEST(testSetMoleAndMoleFractions);
        CPPUNIT_TEST(testSetTemperature);
        CPPUNIT_TEST(testSetConstituentMass);
        CPPUNIT_TEST(testUpdateMass);
        CPPUNIT_TEST(testStateUpdaters);
        CPPUNIT_TEST(testStateUpdatersOppositeFlows);
        CPPUNIT_TEST(testComputes);
        CPPUNIT_TEST(testSetSequence);
        CPPUNIT_TEST(testEdits);
        CPPUNIT_TEST(testComputeTemperature);
        CPPUNIT_TEST(testLiquidPressureDensity);
        CPPUNIT_TEST(testMultipleInitialization);
        CPPUNIT_TEST(testFractionNormalization);
        CPPUNIT_TEST(testConfigAndInputConstructionExceptions);
        CPPUNIT_TEST(testConstructionExceptions);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testStateAccessorExceptions);
        CPPUNIT_TEST(testStateModifierExceptions);
        CPPUNIT_TEST(testStateUpdaterExceptions);
        CPPUNIT_TEST_SUITE_END();
        /// Typedef for number of constituent fluids in composite fluid
        enum {NSINGLE = 1, NDUAL = 2, NMULTI = 7};
        /// (K)    Temperature of the fluid
        double                     mTemperature;
        /// (Pa)   Pressure of the fluid
        double                     mPressure;
        /// (kg/s) Mass flow rate of the fluid
        double                     mFlowRate;
        /// (kg)   Mass of the fluid
        double                     mMass;
        /// (kg)   Moles of the multi constituent fluid
        double                     mMole1;
        /// (kg)   Moles of the single liquid fluid
        double                     mMole2;
        /// (kg)   Moles of the single non-zero fraction fluid
        double                     mMole3;
        /// --     Array of constituent fluid types for multi constituent test article
        FluidProperties::FluidType mType1[UtPolyFluid::NMULTI];
        /// --     Array of constituent fluid types for single liquid test article
        FluidProperties::FluidType mType2[UtPolyFluid::NMULTI];
        /// --     Array of constituent fluid types for single non-zero fraction test article
        FluidProperties::FluidType mType3[UtPolyFluid::NMULTI];
        /// --     Array of constituent mass fractions
        double                     mMassFraction1[UtPolyFluid::NMULTI];
        /// --     Array of constituent mass fractions
        double                     mMassFraction2[UtPolyFluid::NSINGLE];
        /// --     Array of constituent mass fractions
        double                     mMassFraction3[UtPolyFluid::NDUAL];
        /// --     Array of constituent mole fractions
        double                     mMoleFraction1[UtPolyFluid::NMULTI];
        /// --     Array of constituent mole fractions
        double                     mMoleFraction2[UtPolyFluid::NSINGLE];
        /// --     Array of constituent mole fractions
        double                     mMoleFraction3[UtPolyFluid::NDUAL];
        /// --     Pointer to the defined fluid properties
        DefinedFluidProperties*    mProperties;
        /// --     Pointer to the configuration data for multi constituent test article
        PolyFluidConfigData*       mConfigData1;
        /// --     Pointer to the configuration data for single liquid test article
        PolyFluidConfigData*       mConfigData2;
        /// --     Pointer to the configuration data for single non-zero fraction test article
        PolyFluidConfigData*       mConfigData3;
        /// --     Pointer to the initialization data for multi constituent test article
        PolyFluidInputData*        mInputData1;
        /// --     Pointer to the initialization data for single liquid test article
        PolyFluidInputData*        mInputData2;
        /// --     Pointer to the initialization data for single non-zero fraction test article
        PolyFluidInputData*        mInputData3;
        /// --     Pointer to the PolyFluid multi constituent test article
        FriendlyPolyFluid*         mArticle1;
        /// --     Pointer to the PolyFluid single liquid test article
        FriendlyPolyFluid*         mArticle2;
        /// --     Pointer to the PolyFluid single non-zero fraction test article
        FriendlyPolyFluid*         mArticle3;
        /// (s)    Nominal time step
        double                     mTimeStep;
        /// --     Nominal tolerance for comparison of expected and returned values
        double                     mTolerance;
        static int                 TEST_ID;  /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtPolyFluid(const UtPolyFluid&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtPolyFluid& operator =(const UtPolyFluid&);
};

/// @}

#endif
