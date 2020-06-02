#ifndef UtGunnsFluidPotential_EXISTS
#define UtGunnsFluidPotential_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_POTENTIAL    Gunns Fluid Potential Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Potential
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidPotential.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidPotential and befriend UtGunnsFluidPotential.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidPotential : public GunnsFluidPotential
{
    public:
        FriendlyGunnsFluidPotential();
        virtual ~FriendlyGunnsFluidPotential();
        friend class UtGunnsFluidPotential;
};
inline FriendlyGunnsFluidPotential::FriendlyGunnsFluidPotential() : GunnsFluidPotential() {};
inline FriendlyGunnsFluidPotential::~FriendlyGunnsFluidPotential() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Potential  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidPotential: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidPotential(const UtGunnsFluidPotential& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidPotential& operator =(const UtGunnsFluidPotential& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidPotential);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testAccessorsAndSetters);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testComputeFlowsWithInternalFluid);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsFluidPotentialConfigData* mConfigData;

        /// --     Pointer to nominal input data
        GunnsFluidPotentialInputData*  mInputData;

        /// --     Test Article
        FriendlyGunnsFluidPotential*   mArticle;

        /// --     Potential Name
        std::string                    mLinkName;

        /// --     Link Max Conductivity
        double                         mMaxConductivity;

        /// --     Link Expansion Scale Factor
        double                         mExpansionScaleFactor;

        /// --     Link Source Pressure
        double                         mSourcePressure;

        /// --     Network Nodes
        GunnsFluidNode                 mNodes[2];

        /// --     Node List
        GunnsNodeList                  mNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*>   mLinks;

        /// --     Nominal inlet port index
        int                            mPort0;

        /// --     Nominal outlet port index
        int                            mPort1;

        /// (s)    Nominal time step
        double                         mTimeStep;

        /// --     Nominal tolerance for comparison of expected and returned values
        double                         mTolerance;

        /// --     Predefined fluid properties
        DefinedFluidProperties*        mFluidProperties;
        /// --     Fluid 1 config data
        PolyFluidConfigData*           mFluidConfig;
        /// --     Fluid 1 input data
        PolyFluidInputData*            mFluidInput1;
        /// --     Fluid 2input data
        PolyFluidInputData*            mFluidInput2;
        /// --     Fluid Fractions
        double*                        mFractions;


    public:
        UtGunnsFluidPotential();
        virtual ~UtGunnsFluidPotential();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testAccessorsAndSetters();
        void testStep();
        void testComputeFlows();
        void testComputeFlowsWithInternalFluid();
};

///@}

#endif
