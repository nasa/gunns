/**
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include <iostream>
#include <cmath>
#include <cfloat>

#include "UtGunnsFluidNode.hh"
#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidNode::UtGunnsFluidNode()
    :
    tFluidConfig(),
    tFluid2Config(),
    tFluid3Config(),
    tTcConfig(),
    tFractions(),
    tFluidInput(),
    mFluidProperties(),
    tNode(),
    tNode2(),
    tNode3()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidNode::~UtGunnsFluidNode()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::tearDown()
{
    delete tFluidInput;
    delete tFluid3Config;
    delete tFluid2Config;
    delete tFluidConfig;
    delete tTcConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::setUp()
{
    const int numTcTypes = 2;
    ChemicalCompound::Type tcTypesList[numTcTypes] = {
            ChemicalCompound::NH3,
            ChemicalCompound::CH4
    };
    tTcConfig = new GunnsFluidTraceCompoundsConfigData(tcTypesList, numTcTypes, "tTcConfig");

    static const int NumFluidTypes = 3;
    static FluidProperties::FluidType FluidTypesList[NumFluidTypes] = {
            FluidProperties::GUNNS_O2,
            FluidProperties::GUNNS_N2,
            FluidProperties::GUNNS_CO2
    };

    tFluidConfig = new PolyFluidConfigData(&mFluidProperties, FluidTypesList, NumFluidTypes);

    static const int NumLiquidTypes = 1;
    static FluidProperties::FluidType LiquidTypesList[NumLiquidTypes] = {
            FluidProperties::GUNNS_WATER
    };

    tFluid2Config = new PolyFluidConfigData(&mFluidProperties, LiquidTypesList, NumLiquidTypes);

    tFluid3Config = new PolyFluidConfigData(&mFluidProperties, FluidTypesList, NumFluidTypes, tTcConfig);

    tFractions[0] = 1.0;
    tFractions[1] = 0.0;
    tFractions[2] = 0.0;

    tFluidInput = new PolyFluidInputData(283.15,                 //temperature
                                         689.475728,             //pressure
                                         0.0,                    //flowRate
                                         0.0,                    //mass
                                         tFractions);            //massFraction
    /// - Initial node setup.
    tNode .initialize("UtTestNode1", tFluidConfig);
    tNode2.initialize("UtTestNode2", tFluid2Config);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testDefaultConstruction()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidNode ....... 01: testDefaultConstruction ...............";

    CPPUNIT_ASSERT(0 == tNode3.mFluidConfig);
    CPPUNIT_ASSERT(0 == tNode3.mTcInflow.mState);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mVolume,               DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mPreviousVolume,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mThermalCapacitance,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mCompression,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mInflowHeatFlux,       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mNetHeatFlux,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mThermalDampingMass,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mExpansionDeltaT,      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mExpansionScaleFactor, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mPreviousPressure,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mPreviousTemperature,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mMassError,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,     tNode3.mPressureCorrection,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,     tNode3.mCorrectGain,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0E-11, tNode3.mErrorThreshold,       DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidNode ....... 02: testNominalInitialization .............";

    /// - Check the node's fluid config data
    CPPUNIT_ASSERT_EQUAL(tFluidConfig->mNTypes, tNode.mFluidConfig->mNTypes);
    CPPUNIT_ASSERT_EQUAL(tFluidConfig->mTypes,  tNode.mFluidConfig->mTypes);

    /// - Check the initial fluid properties inside the node
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.32501, tNode.mContent.getPressure(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(270.0,     tNode.mContent.getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.32501,
            tNode.mContent.getPartialPressure(FluidProperties::GUNNS_O2),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0,
            tNode.mContent.getPartialPressure(FluidProperties::GUNNS_N2),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0,
            tNode.mContent.getPartialPressure(FluidProperties::GUNNS_CO2),   DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.32501, tNode.mInflow.getPressure(),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(270.0,     tNode.mInflow.getTemperature(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.32501,
            tNode.mInflow.getPartialPressure(FluidProperties::GUNNS_O2),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0,
            tNode.mInflow.getPartialPressure(FluidProperties::GUNNS_N2),     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0,
            tNode.mInflow.getPartialPressure(FluidProperties::GUNNS_CO2),    DBL_EPSILON);

    /// - Check other init terms
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.32501, tNode.mPreviousPressure,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(270.0,     tNode.mPreviousTemperature,      DBL_EPSILON);

    // - Initialize node with trace compounds and a given polyfluid
    tNode3.initialize("UtTestNode3", tFluid3Config, tFluidInput);

    // - Check the node's fluid properties
    CPPUNIT_ASSERT_EQUAL(tFluidInput->mTemperature, tNode3.mContent.getTemperature());
    CPPUNIT_ASSERT_EQUAL(tFluidInput->mPressure, tNode3.mContent.getPressure());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(689.475728, tNode3.mPreviousPressure,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(283.15,     tNode3.mPreviousTemperature,      DBL_EPSILON);
    CPPUNIT_ASSERT(tNode3.mTcInflow.mState);
    CPPUNIT_ASSERT(0.0 == tNode3.mTcInflow.mState[0]);
    CPPUNIT_ASSERT(0.0 == tNode3.mTcInflow.mState[1]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the prepareForStart method of the GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testPrepareForStart()
{
    std::cout << "\n UtGunnsFluidNode ....... 03: testPrepareForStart ...................";

    tNode.mContent.setPressure   (200.0);
    tNode.mContent.setTemperature(350.0);
    tNode.initVolume(42.0);

    tNode.prepareForStart();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, tNode.mPotential,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(200.0, tNode.mPreviousPressure,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(350.0, tNode.mPreviousTemperature, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 42.0, tNode.mPreviousVolume,      DBL_EPSILON);

    /// - Test a case where a non-capacitive node was given an initial fluid containin a mass, and
    ///   verify the mass is reset to zero.
    PolyFluidInputData fluidInit(283.0,             //temperature
                                 689.0,             //pressure
                                   1.0,             //flowRate
                                   1.0,             //mass
                                 tFractions);       //massFraction

    /// - Initial node setup.
    tNode3.initialize("UtTestNode3", tFluidConfig, &fluidInit);
    tNode3.prepareForStart();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(689.0, tNode3.mPotential,           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(689.0, tNode3.mPreviousPressure,    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(283.0, tNode3.mPreviousTemperature, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, tNode3.mPreviousVolume,      DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 == tNode3.getContent()->getMass());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the computeThermalCapacitance method of the GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testComputeThermalCapacitance()
{
    std::cout << "\n UtGunnsFluidNode ....... 04: testComputeThermalCapacitance .........";

    /// - Set up our test fluid properties.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                 100.0,                  // pressure **switched back to Pa
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction

    /// - Load the node with the test fluid and set the node volume.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(1.0);

    /// - Set the last-pass of temperature for the delta-temperature term.
    tNode.mPreviousTemperature = 299.999;

    double molWeight = tNode.getContent()->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, tNode.getContent()->getTemperature(), DBL_EPSILON);

    /// - Call PolyFluid directly for the same density perturbation that computeCapacitance does,
    ///   and verify it matches this hardcoded result.  This is a redundant check on PolyFluid.
    double rho1 = tFluid.computeDensity(300.0 * 0.999, 100.0); // Should be 1.1572346741752115
    double rho2 = tFluid.computeDensity(300.0 * 1.001, 100.0); // Should be 1.1549225169840522

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1572346741752115, rho1, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1549225169840522, rho2, DBL_EPSILON);

    /// - Duplicate the capacitance equation here.  We use unit volume.  The 300.0 * 0.002 is the
    ///   delta-temperature used in our density perturbation.
    double capacitance = (rho1 - rho2) / (molWeight * 300.0 * 0.002);
    double thermalSource = capacitance *
            (tNode.getContent()->getTemperature() - tNode.mPreviousTemperature);

    /// - Call computeThermalCapacitance and verify correct mCapacitance and return value result.
    double tResult = tNode.computeThermalCapacitance();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(thermalSource, tResult,                   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(capacitance,   tNode.mThermalCapacitance, DBL_EPSILON);

    /// - Verify mPreviousTemperature is set for next pass.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNode.getContent()->getTemperature(),
            tNode.mPreviousTemperature, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method makes sure the thermal capacitance returned from a node with minimum
///           temperature is correct.  This is identical to the testComputeThermalCapacitance above
///           except for temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testLowTempThermalCapacitance()
{
    std::cout << "\n UtGunnsFluidNode ....... 05: testLowTempThermalCapacitance .........";

    /// - Set up our test fluid properties.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(FLT_EPSILON,            // temperature
                                 100.0,                  // pressure **switched back to Pa
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction

    /// - Load the node with the test fluid and set the node volume.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(1.0);

    /// - Set the last-pass of temperature for the delta-temperature term.
    tNode.mPreviousTemperature = 0.0;

    double molWeight = tNode.getContent()->getMWeight();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(FLT_EPSILON, tNode.getContent()->getTemperature(), DBL_EPSILON);

    /// - Call PolyFluid directly for the same density perturbation that computeCapacitance does,
    ///   and verify it matches this hardcoded result.  This is a redundant check on PolyFluid.
    double T1 = 0.999 * 0.5 * FLT_EPSILON;
    double T2 = 1.001 * 0.5 * FLT_EPSILON;
    double rho1 = tFluid.computeDensity(T1, 100.0); // Should be 5.5885273962925748
    double rho2 = tFluid.computeDensity(T2, 100.0); // Should be 5.5885273962925748

    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.5885273962925748, rho1, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.5885273962925748, rho2, DBL_EPSILON);

    /// - Duplicate the capacitance equation here.  We use unit volume.
    double capacitance = (rho1 - rho2) / (molWeight * (T2 - T1));
    double thermalSource = capacitance *
            (tNode.getContent()->getTemperature() - tNode.mPreviousTemperature);

    /// - Call computeThermalCapacitance and verify correct mCapacitance and return value result.
    tNode.getContent()->setTemperature(0.5 * FLT_EPSILON);
    double tResult = tNode.computeThermalCapacitance();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(thermalSource, tResult,                   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(capacitance,   tNode.mThermalCapacitance, DBL_EPSILON);

    /// - Verify mPreviousTemperature is set for next pass.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNode.getContent()->getTemperature(),
            tNode.mPreviousTemperature, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the thermal capacitance of a node with zero volume.  This is
///           identical to testComputeThermalCapacitance except we let volume remain zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testZeroVolumeThermalCapacitance()
{
    std::cout << "\n UtGunnsFluidNode ....... 06: testZeroVolumeThermalCapacitance ......";

    /// - Set up our test fluid properties.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                 100.0,                  // pressure **switched back to Pa
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction

    /// - Load the node with the test fluid.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);

    /// - Set the last-pass of temperature for the delta-temperature term.
    tNode.mPreviousTemperature = 299.999;

    double molWeight = tNode.getContent()->getMWeight();

    double rho1 = tFluid.computeDensity(300.0 * 0.999, 100.0);
    double rho2 = tFluid.computeDensity(300.0 * 1.001, 100.0);

    /// - Thermal capacitance of a non-capacitive node should be zero.
    double capacitance   = 0.0;
    double thermalSource = 0.0;

    /// - Call computeCapacitance and verify correct mCapacitance and return value result.
    double tResult = tNode.computeThermalCapacitance();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(thermalSource, tResult,                   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(capacitance,   tNode.mThermalCapacitance, DBL_EPSILON);

    /// - Verify mPreviousTemperature is set for next pass.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNode.getContent()->getTemperature(),
            tNode.mPreviousTemperature, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the computeCompression method of GunnsFluidNode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testComputeCompression()
{
    std::cout << "\n UtGunnsFluidNode ....... 07: testComputeCompression ................";

    /// - Set up our test fluid properties.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                 100.0,                  // pressure
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction

    /// - Load the node with the test fluid and set the node volume.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(1.0);

    /// - Set the last-pass of volume for the delta-volume affect.
    tNode.mPreviousVolume = 0.999;

    double compression = tNode.getContent()->getDensity() * (tNode.mPreviousVolume - tNode.mVolume)
                       / tNode.getContent()->getMWeight();
    double tResult = tNode.computeCompression();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(compression, tResult, DBL_EPSILON);

    /// - Verify mPreviousVolume is set for next pass.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNode.mVolume, tNode.mPreviousVolume, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initVolume method of GunnsFluidNode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testInitVolume()
{
    std::cout << "\n UtGunnsFluidNode ....... 08: testInitVolume ........................";

    /// - Set up our test fluid properties.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                 100.0,                  // pressure
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction

    /// - Load the node with the test fluid and set the node volume.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(1.0);

    /// - Verify new volume, last volume, and new mass.  The mass check is a redundant test of the
    ///   fluid classes.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tNode.mVolume,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tNode.mPreviousVolume, 0.0);

    double tResult = tNode.getMass();                    // Should be 1.1560774395010363
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1560774395010363, tResult, DBL_EPSILON);

    /// - Verify an exception is thrown if a negative volume is attempted.
    CPPUNIT_ASSERT_THROW(tNode.initVolume(-DBL_EPSILON), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the setVolume method of GunnsFluidNode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testSetVolume()
{
    std::cout << "\n UtGunnsFluidNode ....... 09: testSetVolume .........................";

    /// - Define the expected density of the default fluid (100% O2 at STP).
    double expectedDensity = 1.4442816983082385;

    /// - Set the volume to non-zero when it was previously zero, and verify the last pass of volume
    ///   is set and mass is initialized as well.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.mVolume,                     0.0);
    tNode.setVolume(1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tNode.mVolume,                     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tNode.mPreviousVolume,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0 * expectedDensity, tNode.getMass(), 0.0);

    /// - Now set the volume to another non-zero value, and check the last pass volume and mass are
    ///   unchanged.
    tNode.setVolume(2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, tNode.mVolume,                     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tNode.mPreviousVolume,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0 * expectedDensity, tNode.getMass(), 0.0);

    /// - Now set the volume to zero, and verify the last pass volume is also set to zero.  Also
    ///   check that the mass is zero.
    tNode.setVolume(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.mVolume,                     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.mPreviousVolume,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.getMass(),                   0.0);

    /// - Verify an exception is thrown if a negative volume is attempted.
    CPPUNIT_ASSERT_THROW(tNode.setVolume(-DBL_EPSILON), TsOutOfBoundsException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the updateMass method of GunnsFluidNode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testUpdateMass()
{
    std::cout << "\n UtGunnsFluidNode ....... 10: testUpdateMass ........................";

    /// - Set up our test fluid properties.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                 100.0,                  // pressure
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction

    /// - Load the node with the test fluid and set the node volume.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(2.0);

    /// - Now change the volume leaving all other properties unchanged, then explicitly call
    ///   updateMass to reset the mass for the new volume and verify.
    tNode.setVolume(1.0);
    tNode.updateMass();

    /// - This expected mass is the same value as from testInitVolume above, because the fluid
    ///   conditions are the same.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1560774395010363, tNode.getMass(), DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the collectInflux method of GunnsFluidNode.  Use the method to add
///           several incoming fluid to the node's mInflow collection fluid.  The incoming fluids
///           have different mixture, temperature, as well as a mix of negative & positive total
///           flow rates and component mass fractions.  This is a redundant check of the
///           PolyFluid::addState method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testCollectInflux()
{
    std::cout << "\n UtGunnsFluidNode ....... 11: testCollectInflux .....................";

    /// - Set up the initial incoming fluid.
    double fractions1[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit1(300.0,                  // temperature
                                    1.0,                  // pressure
                                    0.0,                  // flowrate
                                    0.0,                  // mass
                                  fractions1);            // massFraction
    PolyFluid tFluid1(*tFluidConfig, fluidInit1);
    double h1 = tFluid1.getSpecificEnthalpy();

    /// - Add the initial fluid to the mInflow fluid at a rate of 1.0 kg/s and verify.
    tNode.collectInflux(1.0, &tFluid1);

    /// - TODO temperature fails DBL_EPSILON with 300.000000000001
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, tNode.mInflow.getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, tNode.mInflow.getFlowRate(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.2,
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_O2),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.79,
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_N2),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.01,
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_CO2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0 * h1, tNode.mInflowHeatFlux,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0,      tNode.mInfluxRate,         DBL_EPSILON);

    /// - Now set up the second incoming fluid with components going in opposite directions.
    double fractions2[FluidProperties::NO_FLUID] = {0.7, -0.2, 0.5};
    PolyFluidInputData fluidInit2(250.0,                  // temperature
                                    1.0,                  // pressure
                                    0.0,                  // flowrate
                                    0.0,                  // mass
                                  fractions2);            // massFraction
    PolyFluid tFluid2(*tFluidConfig, fluidInit2);
    double h2 = tFluid2.getSpecificEnthalpy();

    /// - Remove the second fluid from the initial fluid in mInflow at a rate of 0.5 kg/s and
    ///   verify.
    tNode.collectInflux(-0.5, &tFluid2);

    //TODO actually do the math for temperature
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.5, tNode.mInflow.getFlowRate(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (0.2  * 1.0 - 0.7 * 0.5) / (1.0 - 0.5),
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_O2),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (0.79 * 1.0 + 0.2 * 0.5) / (1.0 - 0.5),
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_N2),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (0.01 * 1.0 - 0.5 * 0.5) / (1.0 - 0.5),
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_CO2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0 * h1 - 0.5 * h2,
            tNode.mInflowHeatFlux, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.5, tNode.mInfluxRate, DBL_EPSILON);

    /// - Mixed enthalpy is the average of enthalpies weighted by flow rates.
    double hMix = (h1 * 1.0 - h2 * 0.5) / (1.0 - 0.5);
    /// - Mixed temperature is mixed enthalpy / mixed specific heat.
    double expectedT = hMix / tNode.mInflow.getSpecificHeat();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(hMix, tNode.mInflow.getSpecificEnthalpy(), DBL_EPSILON);
    /// - TODO Again, round-off error in temperature fails DBL_EPSILON.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedT, tNode.mInflow.getTemperature(), FLT_EPSILON);

    /// - Set up a third incoming fluid.
    double fractions3[FluidProperties::NO_FLUID] = {0.3, 0.8, -0.1};
    PolyFluidInputData fluidInit3(325.0,                  // temperature
                                    1.0,                  // pressure
                                    0.0,                  // flowrate
                                    0.0,                  // mass
                                  fractions3);            // massFraction
    PolyFluid tFluid3(*tFluidConfig, fluidInit3);
    double h3 = tFluid3.getSpecificEnthalpy();

    /// - Remove the third fluid from the mInflow mixture at a rate of 1.0 kg/s and verify.  This
    ///   should result in a net outflow from the node.
    tNode.collectInflux(-1.0, &tFluid3);

    CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, tNode.mInflow.getFlowRate(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (0.2  * 1.0 - 0.7 * 0.5 - 0.3 * 1.0) / (1.0 - 0.5 - 1.0),
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_O2),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (0.79 * 1.0 + 0.2 * 0.5 - 0.8 * 1.0) / (1.0 - 0.5 - 1.0),
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_N2),   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (0.01 * 1.0 - 0.5 * 0.5 + 0.1 * 1.0) / (1.0 - 0.5 - 1.0),
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_CO2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0 * h1 - 0.5 * h2 - 1.0 * h3,
            tNode.mInflowHeatFlux, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, tNode.mInfluxRate, DBL_EPSILON);

    /// - Mixed enthalpy is the average of enthalpies weighted by flow rates.
    hMix = (h1 * 1.0 - h2 * 0.5 - h3 * 1.0) / (1.0 - 0.5 - 1.0);
    /// - Mixed temperature is mixed enthalpy / mixed specific heat.
    expectedT = hMix / tNode.mInflow.getSpecificHeat();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(hMix, tNode.mInflow.getSpecificEnthalpy(), DBL_EPSILON);
    /// - TODO Again, round-off error in temperature fails DBL_EPSILON.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedT, tNode.mInflow.getTemperature(), FLT_EPSILON);

    /// - Test a case which forces the PolyFluid::addState method to throw an exception, and verify
    ///   it is caught by the node.  Set the added fluid's mFlowRate to zero, and pass zero as the
    ///   fluxRate argument to collectInflux.
    tFluid3.setFlowRate(0.0);
    tNode.mInflow.setFlowRate(0.0);
    CPPUNIT_ASSERT_NO_THROW(tNode.collectInflux(0.0, &tFluid3));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the collectHeatFlux method of GunnsFluidNode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testCollectHeatFlux()
{
    std::cout << "\n UtGunnsFluidNode ....... 12: testCollectHeatFlux ...................";

    tNode.collectHeatFlux(1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tNode.mInflowHeatFlux, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the collectTc method of GunnsFluidNode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testCollectTc()
{
    std::cout << "\n UtGunnsFluidNode ....... 13: testCollectTc .........................";

    /// - Initialize node with trace compounds and a given polyfluid.
    tNode3.initialize("UtTestNode3", tFluid3Config, tFluidInput);

    /// - Test exception from collectTc with index argument out of bounds.
    CPPUNIT_ASSERT_THROW(tNode3.collectTc(-1, 1.0), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(tNode3.collectTc(2,  1.0), TsOutOfBoundsException);

    /// - Test exception from collectTc with no trace compounds in the node config.
    CPPUNIT_ASSERT_THROW(tNode.collectTc(0, 1.0), TsOutOfBoundsException);

    /// - Test nominal call to collectTc.
    CPPUNIT_ASSERT_NO_THROW(tNode3.collectTc(0, 1.0));
    CPPUNIT_ASSERT(1.0 == tNode3.mTcInflow.mState[0]);
    CPPUNIT_ASSERT(0.0 == tNode3.mTcInflow.mState[1]);
    CPPUNIT_ASSERT_NO_THROW(tNode3.collectTc(1, -1.0));
    CPPUNIT_ASSERT( 1.0 == tNode3.mTcInflow.mState[0]);
    CPPUNIT_ASSERT(-1.0 == tNode3.mTcInflow.mState[1]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the resetFlows method of GunnsFluidNode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testResetFlows()
{
    std::cout << "\n UtGunnsFluidNode ....... 14: testResetFlows ........................";

    /// - Set up an incoming fluid.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                   1.0,                  // pressure
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction
    PolyFluid tFluid(*tFluidConfig, fluidInit);

    /// - Add inflows and outflows to the node and change the fluid pressure.
    tNode.collectInflux(2.0, &tFluid);
    tNode.collectOutflux(1.0);
    tNode.setPotential(150.0);

    /// - Reset the node flows and verify.
    tNode.resetFlows();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   tNode.mInflowHeatFlux,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(150.0, tNode.mPreviousPressure, 0.0);

    /// - Verify the mInflow fluid properties have been cleared out for next pass.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.mInflow.getTemperature(),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.mInflow.getPressure(),     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.mInflow.getFlowRate(),     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_O2),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_N2),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            tNode.mInflow.getMassFraction(FluidProperties::GUNNS_CO2), 0.0);

    /// - Verify the mOutflow properties have been set equal to the mContent.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNode.mContent.getTemperature(),
            tNode.getOutflow()->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNode.mContent.getPressure(),
            tNode.getOutflow()->getPressure(),    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNode.mContent.getFlowRate(),
            tNode.getOutflow()->getFlowRate(),    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNode.mContent.getMassFraction(FluidProperties::GUNNS_O2),
            tNode.getOutflow()->getMassFraction(FluidProperties::GUNNS_O2),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNode.mContent.getMassFraction(FluidProperties::GUNNS_N2),
            tNode.getOutflow()->getMassFraction(FluidProperties::GUNNS_N2),  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNode.mContent.getMassFraction(FluidProperties::GUNNS_CO2),
            tNode.getOutflow()->getMassFraction(FluidProperties::GUNNS_CO2), 0.0);

    /// - Set up a node with trace compounds and verify the TC inflow state is reset.
    tNode3.initialize("UtTestNode3", tFluid3Config, tFluidInput);
    tNode3.collectTc(0,  1.0);
    tNode3.collectTc(1, -1.0);
    tNode3.resetFlows();
    CPPUNIT_ASSERT(0.0 == tNode3.mTcInflow.mState[0]);
    CPPUNIT_ASSERT(0.0 == tNode3.mTcInflow.mState[1]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the integrateFlows method of GunnsFluidNode for nominal flows in &
///           out of a capacitive node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testFlowsToCapacitiveNode()
{
    std::cout << "\n UtGunnsFluidNode ....... 15: testFlowsToCapacitiveNode .............";

    double initTemperature = 300.0;
    double initPressure    = 100.0;

    /// - Set up our test fluid properties.
    double initFractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(initTemperature,           // temperature
                                 initPressure,              // pressure
                                 0.0,                       // flowrate
                                 0.0,                       // mass
                                 initFractions);            // massFraction

    /// - Load the node with the initial test fluid and set the node physical properties.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(1000.0);
    tNode.setThermalDampingMass(10000.0);
    tNode.setUndampedHeatFlux(10.0);
    tNode.setExpansionScaleFactor(1.0);

    double initMass = tNode.getMass();

    /// - Set up an incoming fluid.
    double fractionsIn[FluidProperties::NO_FLUID] = {0.99, 0.0, 0.01};
    PolyFluidInputData fluidInitIn(290.0,                  // temperature
                                     1.0,                  // pressure
                                     0.0,                  // flowrate
                                     0.0,                  // mass
                                   fractionsIn);           // massFraction
    PolyFluid tFluidIn(*tFluidConfig, fluidInitIn);

    /// - Add inflows, outflows and extra heat to the node.
    double dt           = 0.1;
    double inFlowRate   = 0.0006;
    double outFlowRate  = 0.0003;
    double heatFlux     = 100.0;
    tNode.collectInflux(inFlowRate, &tFluidIn);
    tNode.collectOutflux(outFlowRate);
    tNode.collectHeatFlux(heatFlux);

    /// - Test scheduled outflux and is overflowing indication.
    tNode.scheduleOutflux(outFlowRate / tFluid.getMWeight());
    CPPUNIT_ASSERT(not tNode.isOverflowing(dt));

    /// - Set up last pass temperature.
    tNode.mPreviousTemperature = 299.9;

    /// - Get node capacitance and thermal source vector term.
    double capacitance   = GunnsFluidUtils::computeCapacitance(&tFluid, 1000.0);
    double thermalSource = tNode.computeThermalCapacitance();

    /// - Compute a new pressure as if we had a Gunns solver, using a dT of 0.1 sec:
    ///
    ///                      C/dT * Pnew = C/dt * Pold + Ct
    ///
    double netMolarRate = inFlowRate / tFluidIn.getMWeight() - outFlowRate / tFluid.getMWeight();
    double Pnew = ((capacitance * initPressure / dt) + thermalSource + netMolarRate)
            / (capacitance / dt);
    tNode.setPotential(Pnew);

    /// - Call integrateFlows with dT = 0.1 and verify outputs.
    tNode.integrateFlows(dt);

    double expectedMass     = initMass + (inFlowRate - outFlowRate) * dt;
    double expectedFractO2  = ( initFractions[0] * (initMass - outFlowRate * dt)
                              + fractionsIn[0] * outFlowRate * dt ) / expectedMass;
    double expectedFractN2  = ( initFractions[1] * (initMass - outFlowRate * dt)
                              + fractionsIn[1] * outFlowRate * dt ) / expectedMass;
    double expectedFractCO2 = ( initFractions[2] * (initMass - outFlowRate * dt)
                              + fractionsIn[2] * outFlowRate * dt ) / expectedMass;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,
            tNode.getMass(),                                     DBL_EPSILON * initMass);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inFlowRate - outFlowRate,
            tNode.mNetFlux,                                      DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inFlowRate - outFlowRate,
            tNode.mFluxThrough,                                  DBL_EPSILON);

    //TODO these fail DBL_EPSILON perhaps due to round-off?
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractO2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_O2),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractN2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_N2),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractCO2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_CO2), FLT_EPSILON);

    /// - These next expected values were copied from the model when it was in a known working
    ///   state, as reproducing all the calculations here is too lengthy.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.1265567926902804e+00,
            tNode.mExpansionDeltaT,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.6970693521675838e+02,
            tNode.mNetHeatFlux,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.9892042687444513e+02,
            tNode.mContent.getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.2139340101989546e+00,
            tNode.mMassError,                DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.5330664536240491e+01,
            tNode.mThermalDampingHeat,       DBL_EPSILON);

    /// - Test a case where the inflows are such that it would cause a negative constituent mass.
    tNode.resetFlows();

    fractionsIn[0] = 0.0;
    fractionsIn[1] = 0.0;
    fractionsIn[2] = 1.0;
    inFlowRate     =-1000.0;
    PolyFluid tFluidIn2(*tFluidConfig, fluidInitIn);
    tNode.collectInflux(inFlowRate, &tFluidIn2);
    initMass = tNode.mContent.getMass();

    tNode.integrateFlows(dt);

    expectedMass = initMass + inFlowRate * dt;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,
            tNode.mContent.getMass(),                            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_O2) +
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_N2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_CO2), DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the integrateFlows method of GunnsFluidNode for the special case
///           where there is only outflow and no inflow.  This is an identical test setup as
///           testFlowsToCapacitiveNode except we leave out the incoming flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testIntegrateFlowsNoInflow()
{
    std::cout << "\n UtGunnsFluidNode ....... 16: testIntegrateFlowsNoInflow ............";

    double initTemperature = 300.0;
    double initPressure    = 100.0;

    /// - Set up our test fluid properties.
    double initFractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(initTemperature,           // temperature
                                 initPressure,              // pressure
                                 0.0,                       // flowrate
                                 0.0,                       // mass
                                 initFractions);            // massFraction

    /// - Load the node with the initial test fluid and set the node physical properties.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(1000.0);
    tNode.setThermalDampingMass(10000.0);
    tNode.setExpansionScaleFactor(1.0);

    double initMass = tNode.getMass();

    /// - Add inflows, outflows and extra heat to the node.
    double dt          = 0.1;
    double outFlowRate = 0.0003;
    double heatFlux    = 100.0;
    tNode.collectOutflux(outFlowRate);
    tNode.collectHeatFlux(heatFlux);

    /// - Test scheduled outflux and is overflowing indication.
    tNode.scheduleOutflux(outFlowRate / tFluid.getMWeight());
    CPPUNIT_ASSERT(not tNode.isOverflowing(dt));

    /// - Set up last pass temperature.
    tNode.mPreviousTemperature = 299.9;

    /// - Get node capacitance and thermal source vector term.
    double capacitance   = GunnsFluidUtils::computeCapacitance(&tFluid, 1000.0);
    double thermalSource = tNode.computeThermalCapacitance();

    /// - Compute a new pressure as if we had a Gunns solver, using a dT of 0.1 sec:
    ///
    ///                      C/dT * Pnew = C/dt * Pold + Ct
    ///
    double netMolarRate = -outFlowRate / tFluid.getMWeight();
    double Pnew = ((capacitance * initPressure / dt) + thermalSource + netMolarRate)
            / (capacitance / dt);
    tNode.setPotential(Pnew);

    /// - Call integrateFlows with dT = 0.1 and verify outputs.
    tNode.integrateFlows(dt);

    double expectedMass     = initMass - outFlowRate * dt;
    double expectedFractO2  = initFractions[0];
    double expectedFractN2  = initFractions[1];
    double expectedFractCO2 = initFractions[2];

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,
            tNode.getMass(),                 initMass * DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-outFlowRate,
            tNode.mNetFlux,                  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            tNode.mFluxThrough,              0.0);

    //TODO these fail DBL_EPSILON perhaps due to round-off?
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractO2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_O2),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractN2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_N2),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractCO2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_CO2), FLT_EPSILON);

    /// - These next expected values were copied from the model when it was in a known working
    ///   state, as reproducing all the calculations here is too lengthy.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.1265607870959684e+00,
            tNode.mExpansionDeltaT,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.5317826724299834e+00,
            tNode.mNetHeatFlux,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.9892042232057139e+02,
            tNode.mContent.getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.2139514696457354e+00,
            tNode.mMassError,                DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 8.9637240894744565e+01,
            tNode.mThermalDampingHeat,       DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the integrateFlows method of GunnsFluidNode for the special case
///           where there is only inflow and no outflow.  This is an identical test setup as
///           testFlowsToCapacitiveNode except we leave out the outgoing flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testIntegrateFlowsNoOutflow()
{
    std::cout << "\n UtGunnsFluidNode ....... 17: testIntegrateFlowsNoOutflow ...........";

    double initTemperature = 300.0;
    double initPressure    = 100.0;

    /// - Set up our test fluid properties.
    double initFractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(initTemperature,           // temperature
                                 initPressure,              // pressure
                                 0.0,                       // flowrate
                                 0.0,                       // mass
                                 initFractions);            // massFraction

    /// - Load the node with the initial test fluid and set the node physical properties.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(1000.0);
    tNode.setThermalDampingMass(10000.0);
    tNode.setUndampedHeatFlux(10.0);
    tNode.setExpansionScaleFactor(1.0);

    double initMass = tNode.getMass();

    /// - Set up an incoming fluid.
    double fractionsIn[FluidProperties::NO_FLUID] = {0.99, 0.0, 0.01};
    PolyFluidInputData fluidInitIn(290.0,                  // temperature
                                     1.0,                  // pressure
                                     0.0,                  // flowrate
                                     0.0,                  // mass
                                   fractionsIn);           // massFraction
    PolyFluid tFluidIn(*tFluidConfig, fluidInitIn);

    /// - Add inflows, outflows and extra heat to the node.
    double dt           = 0.1;
    double inFlowRate   = 0.0006;
    double heatFlux     = 100.0;
    tNode.collectInflux(inFlowRate, &tFluidIn);

    /// - Set up last pass temperature.
    tNode.mPreviousTemperature = 299.9;

    /// - Get node capacitance and thermal source vector term.
    double capacitance   = GunnsFluidUtils::computeCapacitance(&tFluid, 1000.0);
    double thermalSource = tNode.computeThermalCapacitance();

    /// - Compute a new pressure as if we had a Gunns solver, using a dT of 0.1 sec:
    ///
    ///                      C/dT * Pnew = C/dt * Pold + Ct
    ///
    double netMolarRate = inFlowRate / tFluidIn.getMWeight();
    double Pnew = ((capacitance * initPressure / dt) + thermalSource + netMolarRate)
            / (capacitance / dt);
    tNode.setPotential(Pnew);

    /// - Call integrateFlows with dT = 0.1 and verify outputs.
    tNode.integrateFlows(dt);

    double expectedMass     = initMass + inFlowRate * dt;
    double expectedFractO2  = initFractions[0] * initMass / expectedMass;
    double expectedFractN2  = initFractions[1] * initMass / expectedMass;
    double expectedFractCO2 = initFractions[2] * initMass / expectedMass;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, tNode.getMass(),    DBL_EPSILON * initMass);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inFlowRate,   tNode.mNetFlux,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,          tNode.mFluxThrough, DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractO2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_O2),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractN2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_N2),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractCO2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_CO2), FLT_EPSILON);

    /// - These next expected values were copied from the model when it was in a known working
    ///   state, as reproducing all the calculations here is too lengthy.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.1265545704047781e+00,
            tNode.mExpansionDeltaT,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.6217515254432837e+02,
            tNode.mNetHeatFlux,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.9892042816818559e+02,
            tNode.mContent.getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.2139290967547822e+00,
            tNode.mMassError,                DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.4306575432607476e+01,
            tNode.mThermalDampingHeat,       DBL_EPSILON);

    /// - Check the outflow fluid is the same as the contents.
    CPPUNIT_ASSERT(tNode.mOutflow.getMWeight()     == tNode.mContent.getMWeight());
    CPPUNIT_ASSERT(tNode.mOutflow.getDensity()     == tNode.mContent.getDensity());
    CPPUNIT_ASSERT(tNode.mOutflow.getPressure()    == tNode.mContent.getPressure());
    CPPUNIT_ASSERT(tNode.mOutflow.getTemperature() == tNode.mContent.getTemperature());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the integrateFlows method of GunnsFluidNode for the special case
///           where there is overflow caused by the outflow being too great.  We make the flows very
///           large compared to the node volume.  We assume steady flow (no pressure change).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testOutflowOverflow()
{
    std::cout << "\n UtGunnsFluidNode ....... 18: testOutflowOverflow ...................";

    const double initTemperature = 300.0;
    const double initPressure    = 100.0;
    const double volume          = 0.01;
    const double dt              = 0.1;

    /// - Set up our test fluid properties.
    double initFractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(initTemperature,           // temperature
                                 initPressure,              // pressure
                                 0.0,                       // flowrate
                                 0.0,                       // mass
                                 initFractions);            // massFraction

    /// - Load the node with the initial test fluid and set the node physical properties.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(volume);
    const double initMass = tNode.getMass();

    /// - Set up an incoming fluid.
    double fractionsIn[FluidProperties::NO_FLUID] = {0.99, 0.0, 0.01};
    PolyFluidInputData fluidInitIn(290.0,                  // temperature
                                     1.0,                  // pressure
                                     0.0,                  // flowrate
                                     0.0,                  // mass
                                   fractionsIn);           // massFraction
    PolyFluid tFluidIn(*tFluidConfig, fluidInitIn);

    /// - Add inflows and outflows to the node, test is overflowing indication.
    const double thruFlux = 0.033; // molar rate (kg*mol/s)
    tNode.scheduleOutflux(thruFlux);
    CPPUNIT_ASSERT(tNode.isOverflowing(dt));
    const double inFlowRate  = thruFlux * tFluidIn.getMWeight();
    tNode.collectInflux(inFlowRate, &tFluidIn);

    /// - Call integrateFlows with dt = 0.1 and verify outputs.
    const double initMoles        = tNode.getContent()->getMole();
    const double thruMoles        = thruFlux * dt - initMoles;
    const double thruMass         = thruMoles * tFluidIn.getMWeight();
    const double outMass          = initMass + thruMass;
    const double outFlow          = outMass / dt;
    const double solutionMass     = tFluidIn.computeDensity(290.0, initPressure) * volume;
    const double expectedMass     = initMass + inFlowRate * dt - outMass;
    const double expectedNetFlux  = inFlowRate - outFlow;
    const double expectedFluxThru = std::min(inFlowRate, outFlow);
    const double expectedFractO2  = fractionsIn[0];
    const double expectedFractN2  = fractionsIn[1];
    const double expectedFractCO2 = fractionsIn[2];
    const double expectedMassErr  = expectedMass - solutionMass;

    tNode.integrateFlows(dt);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,     tNode.getMass(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNetFlux,  tNode.mNetFlux,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFluxThru, tNode.mFluxThrough, DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractO2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_O2),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractN2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_N2),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractCO2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_CO2), FLT_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tNode.mExpansionDeltaT,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0,           tNode.mContent.getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassErr, tNode.mMassError,                DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(outFlow,         tNode.getOutflow()->getFlowRate(), DBL_EPSILON);

    /// - Now do a test with the same outflux, but the inflow is so small that it causes the node
    ///   to limit resulting mass from going negative.  The resulting mixture should equal the
    ///   inflow.
    {
        tNode.resetFlows();
        tNode.scheduleOutflux(thruFlux);
        CPPUNIT_ASSERT(tNode.isOverflowing(dt));
        const double inFlowRate = thruFlux * tFluidIn.getMWeight() * 1.0e-6;

        /// - Set up a different incoming fluid than previous step.
        double fractionsIn[FluidProperties::NO_FLUID] = {0.01, 0.49, 0.5};
        PolyFluidInputData fluidInitIn(320.0,                  // temperature
                                         1.0,                  // pressure
                                         0.0,                  // flowrate
                                         0.0,                  // mass
                                       fractionsIn);           // massFraction
        PolyFluid tFluidIn(*tFluidConfig, fluidInitIn);

        tNode.collectInflux(inFlowRate, &tFluidIn);

        const double expectedMass     = DBL_EPSILON;
        const double expectedFractO2  = fractionsIn[0];
        const double expectedFractN2  = fractionsIn[1];
        const double expectedFractCO2 = fractionsIn[2];

        CPPUNIT_ASSERT_NO_THROW(tNode.integrateFlows(dt));

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass, tNode.getMass(), DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractO2,
                tNode.mContent.getMassFraction(FluidProperties::GUNNS_O2),  FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractN2,
                tNode.mContent.getMassFraction(FluidProperties::GUNNS_N2),  FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractCO2,
                tNode.mContent.getMassFraction(FluidProperties::GUNNS_CO2), FLT_EPSILON);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the special case where the combination of inflows and outflows would
///           result in the node mass going negative.  We take the previous test case and provide no
///           inflow, and the outflow is enough to drain more mass than the node has.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testInflowOverflow()
{
    std::cout << "\n UtGunnsFluidNode ....... 19: testInflowOverflow ....................";

    const double initTemperature = 300.0;
    const double initPressure    = 100.0;
    const double volume          = 0.01;
    const double dt              = 0.1;

    /// - Set up our test fluid properties.
    double initFractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(initTemperature,           // temperature
                                 initPressure,              // pressure
                                 0.0,                       // flowrate
                                 0.0,                       // mass
                                 initFractions);            // massFraction

    /// - Load the node with the initial test fluid and set the node physical properties.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(volume);
    const double initMass = tNode.getMass();

    /// - Add outflows to the node, test is overflowing indication.
    const double outFlowRate = 1.0;
    const double outFlux     = outFlowRate / tFluid.getMWeight();
    tNode.scheduleOutflux(outFlux);
    CPPUNIT_ASSERT(tNode.isOverflowing(dt));

    /// - Call integrateFlows with dt = 0.1 and verify outputs.
    tNode.integrateFlows(dt);

    const double expectedMass     = DBL_EPSILON;
    const double expectedNetFlux  = -initMass / dt;
    const double expectedFractO2  = initFractions[0];
    const double expectedFractN2  = initFractions[1];
    const double expectedFractCO2 = initFractions[2];
    const double solutionMass     = tFluid.computeDensity(initTemperature, initPressure) * volume;
    const double expectedMassErr  = expectedMass - solutionMass;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMass,    tNode.getMass(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNetFlux, tNode.mNetFlux,     DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tNode.mFluxThrough, DBL_EPSILON);

    //TODO these fail DBL_EPSILON perhaps due to round-off?
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractO2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_O2),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractN2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_N2),  FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFractCO2,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_CO2), FLT_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,             tNode.mExpansionDeltaT,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(initTemperature, tNode.mContent.getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassErr, tNode.mMassError,                DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the integrateFlows method of GunnsFluidNode for nominal flows into
///           a non-capacitive node.  In this case the node properties should be completely replaced
///           by the properties of the inflow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testFlowsToNonCapacitiveNode()
{
    std::cout << "\n UtGunnsFluidNode ....... 20: testFlowsToNonCapacitiveNode ..........";

    double initTemperature = 300.0;
    double initPressure    = 100.0;

    /// - Set up our test fluid properties.
    double initFractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(initTemperature,           // temperature
                                 initPressure,              // pressure
                                 0.0,                       // flowrate
                                 0.0,                       // mass
                                 initFractions);            // massFraction

    /// - Load the node with the initial test fluid and set the node physical properties.  Leave the
    ///   node volume at zero.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);

    /// - Set up an incoming fluid.
    double fractionsIn[FluidProperties::NO_FLUID] = {0.99, 0.0, 0.01};
    PolyFluidInputData fluidInitIn(290.0,                  // temperature
                                     1.0,                  // pressure
                                     0.0,                  // flowrate
                                     0.0,                  // mass
                                   fractionsIn);           // massFraction
    PolyFluid tFluidIn(*tFluidConfig, fluidInitIn);

    /// - Add inflows to the node.
    double dt           = 0.1;
    double inFlowRate   = 0.0006;
    tNode.collectInflux(inFlowRate, &tFluidIn);

    /// - Add outflows to the node, test is overflowing indication.
    double molarFlux    = inFlowRate * tFluidIn.getMWeight();
    double outFlowRate  = molarFlux * tFluid.getMWeight();
    tNode.scheduleOutflux(molarFlux);
    CPPUNIT_ASSERT(tNode.isOverflowing(dt));
    tNode.collectOutflux(outFlowRate);

    /// - Inject extra heat flux not associated with a mass flow.  The net heat flux thru the node
    ///   is expected to be this value, since the heat advection in the in/out flow cancels out
    ///   since equal mass of inflow leaves via the outflow.
    double expectedNetHeatFlux = 10.0;
    tNode.collectHeatFlux(expectedNetHeatFlux);

    /// - Call integrateFlows and verify outputs.
    tNode.integrateFlows(dt);
    const double expectedFluxThru = std::min(inFlowRate, outFlowRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNode.getMass(),    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,              tNode.mNetFlux,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFluxThru, tNode.mFluxThrough, 0.0);

    //TODO these fail DBL_EPSILON perhaps due to round-off?
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidIn.getMassFraction(FluidProperties::GUNNS_O2),
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_O2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidIn.getMassFraction(FluidProperties::GUNNS_N2),
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_N2),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluidIn.getMassFraction(FluidProperties::GUNNS_CO2),
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_CO2), DBL_EPSILON);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            tNode.mExpansionDeltaT,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNetHeatFlux,
            tNode.mNetHeatFlux,              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNode.getInflow()->getTemperature(),
            tNode.mContent.getTemperature(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( tNode.getInflow()->getDensity(),
            tNode.mContent.getDensity(),     FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( initPressure,
            tNode.mContent.getPressure(),    DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,
            tNode.mMassError,                0.0);

    /// - Test a case where the inflows are such that it would cause a negative constituent mass
    ///   fraction.
    tNode.resetFlows();

    fractionsIn[0] = 0.0;
    fractionsIn[1] = 0.0;
    fractionsIn[2] = 1.0;
    PolyFluid tFluidIn2(*tFluidConfig, fluidInitIn);

    tNode.collectInflux( 0.0009, &tFluidIn);
    tNode.collectInflux(-0.0003, &tFluidIn2);
    tNode.collectOutflux(0.0006);

    tNode.integrateFlows(dt);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_O2) +
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_N2), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            tNode.mContent.getMassFraction(FluidProperties::GUNNS_CO2), DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the getter & setter methods that haven't already been covered.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testAccessMethods()
{
    std::cout << "\n UtGunnsFluidNode ....... 21: testAccessMethods .....................";

    tNode.setVolume(42.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 42.0, tNode.getVolume(), 0.0);

    tNode.setExpansionScaleFactor(0.42);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.42, tNode.mExpansionScaleFactor, 0.0);
    CPPUNIT_ASSERT_THROW(tNode.setExpansionScaleFactor(-1.0), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(tNode.setExpansionScaleFactor( 2.0), TsOutOfBoundsException);

    CPPUNIT_ASSERT_THROW(tNode.setThermalDampingMass(-1.0), TsOutOfBoundsException);

    CPPUNIT_ASSERT(tNode.getFluidConfig() == tFluidConfig);

    tNode.mNetHeatFlux =  5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.0, tNode.getNetHeatFlux(), 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the computePressureCorrection method of the GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testPressureCorrection()
{
    std::cout << "\n UtGunnsFluidNode ....... 22: testPressureCorrection ................";

    double initTemperature = 300.0;
    double initPressure    = 100.0;

    /// - Set up our test fluid properties.
    double initFractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(initTemperature,           // temperature
                                 initPressure,              // pressure
                                 0.0,                       // flowrate
                                 0.0,                       // mass
                                 initFractions);            // massFraction

    /// - Load the node with the initial test fluid and set the node physical properties.
    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(0.01);

    /// - Now change the node pressure slightly to set up an error between actual and ideal.  This
    ///   will give -1% error.
    tNode.setPotential(initPressure * 1.01);

    /// - Call computePressureError and verify the initial correction.
    double expectedGain       = 1.0;
    double expectedCorrection = -0.01 * expectedGain * initPressure;
    double returnedCorrection = tNode.computePressureCorrection();

    double tolerance = initPressure * DBL_EPSILON;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedGain,       tNode.mCorrectGain,        tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedCorrection, tNode.mPressureCorrection, tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedCorrection, returnedCorrection,        tolerance);

    /// - Now set the node pressure in the opposite direction to test the oscillation-damping logic.
    ///   This will give +0.5% error.
    tNode.setPotential(initPressure * 0.995);
    expectedGain       = 0.5;
    expectedCorrection = 0.005 * expectedGain * initPressure;
    returnedCorrection = tNode.computePressureCorrection();

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedGain,       tNode.mCorrectGain,        tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedCorrection, tNode.mPressureCorrection, tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedCorrection, returnedCorrection,        tolerance);

    /// - Now set the node pressure in the same direction and verify the gain is increasing, but
    ///   small enough that the actual correction is switched off.
    tNode.setPotential(initPressure * 0.99999999999999);
    expectedGain       = 0.505;
    expectedCorrection = 0.0;
    returnedCorrection = tNode.computePressureCorrection();

    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedGain,       tNode.mCorrectGain,        tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedCorrection, tNode.mPressureCorrection, tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedCorrection, returnedCorrection,        tolerance);

    /// - Now test that no correction is used for a non-capacitive node.
    tNode.initVolume(0.0);
    tNode.setPotential(initPressure * 1.01);
    expectedCorrection = 0.0;
    returnedCorrection = tNode.computePressureCorrection();
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedCorrection, tNode.mPressureCorrection, tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( expectedCorrection, returnedCorrection,        tolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests exceptions thrown during the initialize method of the GunnsFluidNode
///           class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidNode ....... 23: testInitializationExceptions ..........";

    /// - Attempt to initialize the Fluid without specifying a fluid config
    CPPUNIT_ASSERT_THROW(tNode3.initialize("Node3"), TsInitializationException);

    /// - Attempt to initialize the node with no initial fluid state and with a bad fluid config.
    tFluidConfig->mNTypes = 0;
    CPPUNIT_ASSERT_THROW(tNode3.initialize("Node3", tFluidConfig), TsInitializationException);

    /// - Attempt to initialize the node with a bad initial fluid state.
    tFluidConfig->mNTypes = 3;
    tFluidInput->mMassFraction = 0;
    CPPUNIT_ASSERT_THROW(tNode3.initialize("Node3", tFluidConfig, tFluidInput), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the validate method of the GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testValidate()
{
    std::cout << "\n UtGunnsFluidNode ....... 24: testValidate ..........................";
    // Initialize Name of Fluid
    tNode3.initialize("UtTestNode3",tFluidConfig);
    /// - First try to validate a node with null fluid config pointer and verify results.
    tNode3.mFluidConfig = 0;
    CPPUNIT_ASSERT_THROW(tNode3.validate(), TsInitializationException);

    /// - Set up a node with nominal properties and check good validation.
    double initFractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                 100.0,                  // pressure
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 initFractions);         // massFraction

    PolyFluid tFluid(*tFluidConfig, fluidInit);
    tNode.getContent()->setState(&tFluid);
    tNode.initVolume(0.01);

    tNode.validate();
    CPPUNIT_ASSERT(tNode.mInitFlag       == true);
    CPPUNIT_ASSERT(tNode.isInitialized() == true);

    /// - Try to validate a node with zero pressure and verify results.
    tNode.setPotential(0.0);
    CPPUNIT_ASSERT_THROW(tNode.validate(), TsInitializationException);
    CPPUNIT_ASSERT(tNode.mInitFlag       == false);

    /// - Try to validate a node with zero temperature and verify results.
    tNode.setPotential(100.0);
    tNode.getContent()->setTemperature(0.0);
    CPPUNIT_ASSERT_THROW(tNode.validate(), TsInitializationException);
    CPPUNIT_ASSERT(tNode.mInitFlag       == false);

    /// - Try to validate a node with bad mass error and verify results.
    tNode.getContent()->setTemperature(300.0);
    double initMass = tNode.getMass();
    tNode.getContent()->setMass(initMass * 1.1);
    CPPUNIT_ASSERT_THROW(tNode.validate(), TsInitializationException);
    CPPUNIT_ASSERT(tNode.mInitFlag       == false);

    /// - If PolyFluid is working correctly, then it is impossible to get fluid pressure, density
    ///   & temperature to disagree (in other words, violate the state equation).  So we can't
    ///   exercise a check in the actual GunnsFluidNode class.  Instead, we do a check here in the
    ///   unit test as a redundant sanity check on the PolyFluid's state equation accuracy.
    tNode.getContent()->setMass(initMass);
    double idealDensity = tNode.getContent()->computeDensity(tNode.getContent()->getTemperature(),
                                                             tNode.getContent()->getPressure());
    double idealPressure = tNode.getContent()->computePressure(tNode.getContent()->getTemperature(),
                                                               idealDensity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(idealDensity,  tNode.getContent()->getDensity(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(idealPressure, tNode.getContent()->getPressure(), FLT_EPSILON);

    double idealTemperature =
            tNode.getContent()->computeTemperature(tNode.getContent()->getSpecificEnthalpy());
    double idealSpecificEnthalpy = tNode.getContent()->computeSpecificEnthalpy(idealTemperature);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(idealTemperature,
            tNode.getContent()->getTemperature(), FLT_EPSILON); // These fail DBL_ due to roundoff
    CPPUNIT_ASSERT_DOUBLES_EQUAL(idealSpecificEnthalpy,
            tNode.getContent()->getSpecificEnthalpy(), FLT_EPSILON);

    /// - Set up a liquid mixture fluid and perform the same checks on it.
    double initLiquidFractions[FluidProperties::NO_FLUID] = {1.0};
    PolyFluidInputData liquidInit(300.0,                     // temperature
                                  100.0,                     // pressure
                                    0.0,                     // flowrate
                                    0.0,                     // mass
                                  initLiquidFractions);      // massFraction

    PolyFluid tLiquid(*tFluid2Config, liquidInit);
    tNode2.getContent()->setState(&tLiquid);
    tNode2.initVolume(1.0);

    idealDensity = tNode2.getContent()->computeDensity(tNode2.getContent()->getTemperature(),
                                                       tNode2.getContent()->getPressure());
    idealPressure = tNode2.getContent()->computePressure(tNode2.getContent()->getTemperature(),
                                                         idealDensity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(idealDensity,  tNode2.getContent()->getDensity(),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(idealPressure, tNode2.getContent()->getPressure(),
            100.0 * FLT_EPSILON);

    idealTemperature =
            tNode2.getContent()->computeTemperature(tNode2.getContent()->getSpecificEnthalpy());
    idealSpecificEnthalpy = tNode2.getContent()->computeSpecificEnthalpy(idealTemperature);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(idealTemperature,
            tNode2.getContent()->getTemperature(), FLT_EPSILON); // These fail DBL_ due to roundoff
    CPPUNIT_ASSERT_DOUBLES_EQUAL(idealSpecificEnthalpy,
            tNode2.getContent()->getSpecificEnthalpy(), FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the resetContentState method of the GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testResetContent()
{
    std::cout << "\n UtGunnsFluidNode ....... 25: testResetContent ......................";

    tNode.resetContentState();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.getContent()->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.getContent()->getPressure(),    0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the restart method of the GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testRestart()
{
    std::cout << "\n UtGunnsFluidNode ....... 26: testRestart ...........................";

    /// - Load up the node with some non-zero values, then restart it and verify stuff got reset.
    tNode.initVolume(0.5);

    tNode.mPreviousVolume                      = 1.0;
    tNode.mThermalCapacitance                  = 2.0;
    tNode.mCompression                         = 3.0;
    tNode.mInflowHeatFlux                      = 4.0;
    tNode.mNetHeatFlux                         = 5.0;
    tNode.mUndampedHeatFlux                    = 6.0;
    tNode.mThermalDampingHeat                  = 7.0;
    tNode.mExpansionDeltaT                     = 8.0;

    /// - Load mInfluxRate to verify that the base class is restarted too.
    tNode.mInfluxRate                          = 11.0;

    tNode.restart();

    CPPUNIT_ASSERT(0.5 == tNode.mPreviousVolume);
    CPPUNIT_ASSERT(0.0 == tNode.mThermalCapacitance);
    CPPUNIT_ASSERT(0.0 == tNode.mCompression);
    CPPUNIT_ASSERT(0.0 == tNode.mInflowHeatFlux);
    CPPUNIT_ASSERT(0.0 == tNode.mNetHeatFlux);
    CPPUNIT_ASSERT(0.0 == tNode.mUndampedHeatFlux);
    CPPUNIT_ASSERT(0.0 == tNode.mThermalDampingHeat);
    CPPUNIT_ASSERT(0.0 == tNode.mExpansionDeltaT);
    CPPUNIT_ASSERT(0.0 == tNode.mInfluxRate);
    CPPUNIT_ASSERT(0.0 == tNode.mMassError);

    /// - Change the node's mContent to give it some mass error, and verify this causes the mass
    ///   to be reset.
    const double tMass = tNode.getContent()->getMass();
    tNode.getContent()->setMass(tMass + 1.01 * FLT_EPSILON);

    tNode.restart();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMass, tNode.getContent()->getMass(), DBL_EPSILON);
    CPPUNIT_ASSERT(0.0 == tNode.mMassError);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the inflow & outflow of trace compounds in the GunnsFluidNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidNode::testTraceCompounds()
{
    std::cout << "\n UtGunnsFluidNode ....... 27: testTraceCompounds ....................";

    /// - Define nominal fluid config data with trace compounds.
    DefinedChemicalCompounds definedCompounds;
    ChemicalCompound::Type types[2] = {ChemicalCompound::CO, ChemicalCompound::H2O};
    GunnsFluidTraceCompoundsConfigData tcConfig(types, 2, "tcConfig");

    static const int NumFluidTypes = 3;
    static FluidProperties::FluidType FluidTypesList[NumFluidTypes] = {
            FluidProperties::GUNNS_O2,
            FluidProperties::GUNNS_N2,
            FluidProperties::GUNNS_CO2
    };
    delete tFluidConfig;
    tFluidConfig = new PolyFluidConfigData(&mFluidProperties, FluidTypesList, NumFluidTypes, &tcConfig);

    /// - Define nominal input data with trace compounds.
    double initTemperature                          = 300.0;
    double initPressure                             = 100.0;
    double initFractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    double initMoleFractions[2]                     = {5.0e-5, 6.0e-6};
    GunnsFluidTraceCompoundsInputData tcInput(initMoleFractions);

    PolyFluidInputData fluidInit(initTemperature,           // temperature
                                 initPressure,              // pressure
                                 0.0,                       // flowrate
                                 0.0,                       // mass
                                 initFractions,             // massFraction
                                 &tcInput);                 // trace compounds

    /// - Initial node setup.
    GunnsFluidNodeUnitTest article;
    article.initialize("article", tFluidConfig, &fluidInit);
    article.prepareForStart();
    article.initVolume(1000.0);

    const double initMassCO  = article.getContent()->getTraceCompounds()->getMass(ChemicalCompound::CO);
    const double initMassH2O = article.getContent()->getTraceCompounds()->getMass(ChemicalCompound::H2O);

    /// - Set up an incoming fluid.
    double fractionsIn[FluidProperties::NO_FLUID] = {0.99, 0.0, 0.01};
    double moleFractionsIn[2]                    = {6.0e-6, 7.0e-7};
    GunnsFluidTraceCompoundsInputData tcIn(moleFractionsIn);
    PolyFluidInputData fluidInitIn(290.0,                  // temperature
                                     1.0,                  // pressure
                                     0.0,                  // flowrate
                                     0.0,                  // mass
                                   fractionsIn,            // massFraction
                                   &tcIn);                 // trace compounds
    PolyFluid tFluidIn(*tFluidConfig, fluidInitIn);

    /// - Test in & out flows of trace compounds to a capacitive node.
    const double extraTcInflowCO  =  1.0e-8;
    const double extraTcInflowH2O = -1.0e-8;
    double dt         = 0.1;
    double inFlowRate = 0.0006;
    article.collectTc(0, extraTcInflowCO);
    article.collectInflux(inFlowRate, &tFluidIn);
    article.collectTc(1, extraTcInflowH2O);

    /// - Add outflows to the node, test is overflowing indication.
    double outFlowRate = 0.0003;
    double outFlux     = outFlowRate / article.getContent()->getMWeight();
    article.scheduleOutflux(outFlux);
    CPPUNIT_ASSERT(not tNode.isOverflowing(dt));
    article.collectOutflux(outFlowRate);

    double expectedMassCO  = initMassCO + dt * definedCompounds.getCompound(ChemicalCompound::CO)->mMWeight
                           * (article.getInflow()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO)
                            * inFlowRate / article.getInflow()->getMWeight()
                            - article.getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO)
                            * outFlowRate / article.getContent()->getMWeight())
                           + extraTcInflowCO * dt;
    double expectedMassH2O = initMassH2O + dt * definedCompounds.getCompound(ChemicalCompound::H2O)->mMWeight
                           * (article.getInflow()->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O)
                            * inFlowRate / article.getInflow()->getMWeight()
                            - article.getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O)
                            * outFlowRate / article.getContent()->getMWeight())
                           + extraTcInflowH2O * dt;

    article.integrateFlows(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassCO,  article.getContent()->getTraceCompounds()->getMass(ChemicalCompound::CO),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassH2O, article.getContent()->getTraceCompounds()->getMass(ChemicalCompound::H2O), DBL_EPSILON);

    /// - Test trace compounds persistence when no flows.
    article.resetFlows();
    article.integrateFlows(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassCO,  article.getContent()->getTraceCompounds()->getMass(ChemicalCompound::CO),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedMassH2O, article.getContent()->getTraceCompounds()->getMass(ChemicalCompound::H2O), DBL_EPSILON);

    /// - Test in & out flows of trace compounds to a non-capacitive node.
    article.initVolume(0.0);
    article.resetFlows();
    article.collectInflux(inFlowRate, &tFluidIn);
    article.collectOutflux(outFlowRate);
    article.integrateFlows(dt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.getContent()->getTraceCompounds()->getMass(ChemicalCompound::CO),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.getContent()->getTraceCompounds()->getMass(ChemicalCompound::H2O), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article.getInflow()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO),
                                 article.getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::CO),
                                 DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(article.getInflow()->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O),
                                 article.getContent()->getTraceCompounds()->getMoleFraction(ChemicalCompound::H2O),
                                 DBL_EPSILON);

    std::cout << "... Pass";
}
