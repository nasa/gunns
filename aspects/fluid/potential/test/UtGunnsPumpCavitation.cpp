/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/potential/GunnsPumpCavitation.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "strings/UtResult.hh"

#include "UtGunnsPumpCavitation.hh"

/// @details  Test identification number.
int UtGunnsPumpCavitation::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Pump Cavitation unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsPumpCavitation::UtGunnsPumpCavitation()
    :
    CppUnit::TestFixture(),
    tTypes(),
    tFractions(),
    tFluidProperties(),
    tFluidConfig(),
    tFluidInput(),
    tNode(),
    tArticle()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Pump Cavitation unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsPumpCavitation::~UtGunnsPumpCavitation()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsPumpCavitation::setUp()
{
    /// - Define the node fluid.
    tFluidProperties      = new DefinedFluidProperties();
    tTypes[0]             = FluidProperties::GUNNS_WATER;
    tTypes[1]             = FluidProperties::GUNNS_N2;
    tFractions[0]         = 1.0;
    tFractions[1]         = 0.0;
    tFluidConfig          = new PolyFluidConfigData(tFluidProperties, tTypes, N_FLUIDS);
    tFluidInput           = new PolyFluidInputData(283.0,
                                                   109.0,
                                                     0.0,
                                                     0.0,
                                                     tFractions);

    /// - Initialize the nodes.
    tNode.initialize("UtNode1", tFluidConfig);
    tNode.getContent()->initialize(*tFluidConfig, *tFluidInput);

    tArticle = new FriendlyGunnsPumpCavitation();

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsPumpCavitation::tearDown()
{
    delete tArticle;
    delete tFluidInput;
    delete tFluidConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the default construction of the GUNNS Pump Cavitation class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsPumpCavitation::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    CPPUNIT_ASSERT(0.0   == tArticle->mAutoCavitationDuration);
    CPPUNIT_ASSERT(0.0   == tArticle->mAutoCavitationAmplitude);
    CPPUNIT_ASSERT(false == tArticle->mEnableAutoCavitation);
    CPPUNIT_ASSERT(false == tArticle->mMalfFullCavitationFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfFullCavitationDuration);
    CPPUNIT_ASSERT(false == tArticle->mMalfPartialCavitationFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfPartialCavitationDuration);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfPartialCavitationAmplitude);
    CPPUNIT_ASSERT(0.0   == tArticle->mCavitationElapsedTime);
    CPPUNIT_ASSERT(0.0   == tArticle->mInletVaporPressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mDuration);
    CPPUNIT_ASSERT(0.0   == tArticle->mCavitationFraction);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the initialize method of the GUNNS Pump Cavitation class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsPumpCavitation::testInitialize()
{
    UT_RESULT;

    tArticle->initialize(1.0, 2.0, true);
    CPPUNIT_ASSERT(1.0   == tArticle->mAutoCavitationDuration);
    CPPUNIT_ASSERT(2.0   == tArticle->mAutoCavitationAmplitude);
    CPPUNIT_ASSERT(true  == tArticle->mEnableAutoCavitation);
    CPPUNIT_ASSERT(false == tArticle->mMalfFullCavitationFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfFullCavitationDuration);
    CPPUNIT_ASSERT(false == tArticle->mMalfPartialCavitationFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfPartialCavitationDuration);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfPartialCavitationAmplitude);
    CPPUNIT_ASSERT(0.0   == tArticle->mCavitationElapsedTime);
    CPPUNIT_ASSERT(0.0   == tArticle->mInletVaporPressure);
    CPPUNIT_ASSERT(0.0   == tArticle->mDuration);
    CPPUNIT_ASSERT(0.0   == tArticle->mCavitationFraction);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeVaporPressure method of the GUNNS Pump Cavitation class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsPumpCavitation::testVaporPressure()
{
    UT_RESULT;

    {
        /// - Test vapor pressure for ammonia.
        const FluidProperties::FluidType type = FluidProperties::GUNNS_AMMONIA;
        tArticle->computeVaporPressure(type, &tNode);
        const double expectedVp = tFluidProperties->getProperties(type)->getSaturationPressure(tNode.getContent()->getTemperature());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVp, tArticle->mInletVaporPressure, DBL_EPSILON);
    } {
        /// - Test vapor pressure for propylene glycol 50%.
        const FluidProperties::FluidType type = FluidProperties::GUNNS_PG50;
        tArticle->computeVaporPressure(type, &tNode);
        const double expectedVp = tFluidProperties->getProperties(type)->getSaturationPressure(tNode.getContent()->getTemperature());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVp, tArticle->mInletVaporPressure, DBL_EPSILON);
    } {
        /// - Test vapor pressure for propylene glycol 30%.
        const FluidProperties::FluidType type = FluidProperties::GUNNS_PG30;
        tArticle->computeVaporPressure(type, &tNode);
        const double expectedVp = tFluidProperties->getProperties(type)->getSaturationPressure(tNode.getContent()->getTemperature());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVp, tArticle->mInletVaporPressure, DBL_EPSILON);
    } {
        /// - Test vapor pressure for HFE-7000.
        const FluidProperties::FluidType type = FluidProperties::GUNNS_HFE7000;
        tArticle->computeVaporPressure(type, &tNode);
        const double expectedVp = tFluidProperties->getProperties(type)->getSaturationPressure(tNode.getContent()->getTemperature());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVp, tArticle->mInletVaporPressure, DBL_EPSILON);
    } {
        /// - Test vapor pressure for HFE-7100.
        const FluidProperties::FluidType type = FluidProperties::GUNNS_HFE7100;
        tArticle->computeVaporPressure(type, &tNode);
        const double expectedVp = tFluidProperties->getProperties(type)->getSaturationPressure(tNode.getContent()->getTemperature());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVp, tArticle->mInletVaporPressure, DBL_EPSILON);
    } {
        /// - Test vapor pressure for water.
        const FluidProperties::FluidType type = FluidProperties::GUNNS_WATER;
        tArticle->computeVaporPressure(type, &tNode);
        const double expectedVp = tFluidProperties->getProperties(type)->getSaturationPressure(tNode.getContent()->getTemperature());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVp, tArticle->mInletVaporPressure, DBL_EPSILON);
    } {
        /// - Test vapor pressure for liquid oxygen.
        const FluidProperties::FluidType type = FluidProperties::GUNNS_OXYGEN;
        tArticle->computeVaporPressure(type, &tNode);
        const double expectedVp = tFluidProperties->getProperties(type)->getSaturationPressure(tNode.getContent()->getTemperature());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVp, tArticle->mInletVaporPressure, DBL_EPSILON);
    } {
        /// - Test vapor pressure for liquid methane.
        const FluidProperties::FluidType type = FluidProperties::GUNNS_METHANE;
        tArticle->computeVaporPressure(type, &tNode);
        const double expectedVp = tFluidProperties->getProperties(type)->getSaturationPressure(tNode.getContent()->getTemperature());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVp, tArticle->mInletVaporPressure, DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the update method of the GUNNS Pump Cavitation class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsPumpCavitation::testUpdate()
{
    UT_RESULT;

    /// - Test partial cavitation malf.
    tArticle->setMalfPartialCavitation(true, 1.0, 10.0);
    CPPUNIT_ASSERT(true == tArticle->mMalfPartialCavitationFlag);
    CPPUNIT_ASSERT(1.0  == tArticle->mMalfPartialCavitationDuration);
    CPPUNIT_ASSERT(10.0 == tArticle->mMalfPartialCavitationAmplitude);
    double pumpSource                         = 100.0;
    tArticle->update(pumpSource, 0.1, &tNode, FluidProperties::GUNNS_WATER);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.1, tArticle->mCavitationElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, tArticle->mDuration,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, tArticle->mCavitationFraction,    DBL_EPSILON);
    CPPUNIT_ASSERT(100.0 != pumpSource);
    CPPUNIT_ASSERT(109.0 >= pumpSource);
    CPPUNIT_ASSERT( 91.0 <= pumpSource);

    /// - Test de-activation of partial cavitation malf.
    tArticle->setMalfPartialCavitation();
    CPPUNIT_ASSERT(false == tArticle->mMalfPartialCavitationFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfPartialCavitationDuration);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfPartialCavitationAmplitude);
    pumpSource                                = 100.0;
    tArticle->update(pumpSource, 0.1, &tNode, FluidProperties::GUNNS_WATER);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, tArticle->mCavitationElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, tArticle->mDuration,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, tArticle->mCavitationFraction,    DBL_EPSILON);
    CPPUNIT_ASSERT(100.0 == pumpSource);

    /// - Test full cavitation malf without partial cavitation.
    tArticle->setMalfFullCavitation(true, 1.0);
    CPPUNIT_ASSERT(true == tArticle->mMalfFullCavitationFlag);
    CPPUNIT_ASSERT(1.0  == tArticle->mMalfFullCavitationDuration);
    tArticle->mCavitationElapsedTime          = 0.0;
    double expectedFraction = 2.0 * sqrt(0.01) * 0.1;
    double expectedSource   = (1.0 - expectedFraction) * pumpSource;
    tArticle->update(pumpSource, 0.1, &tNode, FluidProperties::GUNNS_WATER);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.1,              tArticle->mCavitationElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,              tArticle->mDuration,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFraction, tArticle->mCavitationFraction,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedSource,   pumpSource,                       DBL_EPSILON);

    /// - Test full cavitation malf with partial cavitation included.
    tArticle->mMalfPartialCavitationAmplitude = 10.0;
    pumpSource                                = 100.0;
    expectedFraction += 2.0 * sqrt(expectedFraction) * 0.1;
    expectedSource    = (1.0 - expectedFraction) * pumpSource;
    tArticle->update(pumpSource, 0.1, &tNode, FluidProperties::GUNNS_WATER);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.2,              tArticle->mCavitationElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,              tArticle->mDuration,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFraction, tArticle->mCavitationFraction,    DBL_EPSILON);
    CPPUNIT_ASSERT(expectedSource       != pumpSource);
    CPPUNIT_ASSERT(expectedSource + 8.0 >= pumpSource);
    CPPUNIT_ASSERT(expectedSource - 8.0 <= pumpSource);

    /// - Test cavitation washout doesn't include partial cavitation.
    tArticle->setMalfFullCavitation();
    CPPUNIT_ASSERT(false == tArticle->mMalfFullCavitationFlag);
    CPPUNIT_ASSERT(0.0   == tArticle->mMalfFullCavitationDuration);
    tArticle->mCavitationFraction             = 0.5;
    pumpSource                                = 100.0;
    expectedFraction = 0.5 - 2.0 * sqrt(1.0 - 0.5) * 0.1;
    expectedSource   = (1.0 - expectedFraction) * pumpSource;
    tArticle->update(pumpSource, 0.1, &tNode, FluidProperties::GUNNS_WATER);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,              tArticle->mCavitationElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,              tArticle->mDuration,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFraction, tArticle->mCavitationFraction,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedSource,   pumpSource,                       DBL_EPSILON);

    /// - Test auto-cavitation enabled.
    tArticle->mEnableAutoCavitation           = true;
    tArticle->mAutoCavitationAmplitude        = 10.0;
    tArticle->mAutoCavitationDuration         = 1.0;
    tArticle->mCavitationFraction             = 0.0;
    pumpSource                                = 100.0;
    tNode.setPotential(0.0);
    expectedFraction = 2.0 * sqrt(0.01) * 0.1;
    expectedSource   = (1.0 - expectedFraction) * pumpSource;
    tArticle->update(pumpSource, 0.1, &tNode, FluidProperties::GUNNS_WATER);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.1,              tArticle->mCavitationElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,              tArticle->mDuration,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFraction, tArticle->mCavitationFraction,    DBL_EPSILON);
    CPPUNIT_ASSERT(expectedSource       != pumpSource);
    CPPUNIT_ASSERT(expectedSource + 9.0 >= pumpSource);
    CPPUNIT_ASSERT(expectedSource - 9.0 <= pumpSource);

    /// - Test auto-cavitation disabled.
    tArticle->mEnableAutoCavitation           = false;
    pumpSource                                = 100.0;
    expectedFraction                          = 0.0;
    expectedSource                            = pumpSource;
    tArticle->update(pumpSource, 0.1, &tNode, FluidProperties::GUNNS_WATER);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,              tArticle->mCavitationElapsedTime, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,              tArticle->mDuration,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedFraction, tArticle->mCavitationFraction,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedSource,   pumpSource,                       DBL_EPSILON);

    UT_PASS_LAST;
}
