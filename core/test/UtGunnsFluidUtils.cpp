/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "UtGunnsFluidUtils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Utilities model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidUtils::UtGunnsFluidUtils()
    :
    mFluidProperties(),
    mTcProperties(),
    mFluidTcConfig1(),
    mFluidConfig1(),
    mFluidConfig2(),
    fractions(),
    mFluidInput1(),
    mFluidInput2(),
    mFluidInput3(),
    tFluid1(),
    tFluid2(),
    tFluid3()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Utilities model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidUtils::~UtGunnsFluidUtils()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::tearDown()
{
    delete tFluid3;
    delete mFluidInput3;
    delete tFluid2;
    delete tFluid1;
    delete mFluidInput2;
    delete mFluidInput1;
    delete[] fractions;
    delete mFluidConfig2;
    delete mFluidConfig1;
    delete mFluidProperties;
    delete mFluidTcConfig1;
    delete mTcProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::setUp()
{
    mTcProperties = new DefinedChemicalCompounds();
    mTcTypes[0] = ChemicalCompound::CH2O;
    mTcTypes[1] = ChemicalCompound::C2H6O;
    mTcTypes[2] = ChemicalCompound::C4H4O;
    mTcTypes[3] = ChemicalCompound::C8H10;
    mFluidTcConfig1 = new GunnsFluidTraceCompoundsConfigData(mTcTypes, 4, "mFluidTcConfig1");

    mFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[3];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    types[2]    = FluidProperties::GUNNS_WATER;
    mFluidConfig1 = new PolyFluidConfigData(mFluidProperties, types, 3, mFluidTcConfig1);
    types[0]    = FluidProperties::GUNNS_O2;
    types[1]    = FluidProperties::GUNNS_N2;
    types[2]    = FluidProperties::GUNNS_CO2;
    mFluidConfig2 = new PolyFluidConfigData(mFluidProperties, types, 3);

    fractions = new double[3];
    fractions[0] = 0.5;
    fractions[1] = 0.5;
    fractions[2] = 0.0;
    mFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    mFluidInput2 = new PolyFluidInputData(283.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    // initialize the gases
    tFluid1 = new PolyFluid(*mFluidConfig1, *mFluidInput1);
    tFluid2 = new PolyFluid(*mFluidConfig1, *mFluidInput2);

    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 1.0;
    mFluidInput3 = new PolyFluidInputData(283.15,                   //temperature
                                          689.475,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions
    // initialize the liquids
    tFluid3 = new PolyFluid(*mFluidConfig1, *mFluidInput3);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests calculation of fluid link admittance.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testAdmittance()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidUtils ...... 01: testAdmittance ........................";

    /// - Set up fluid pressures and densities to test values.
    double pressure1 = 210.0;
    double pressure2 = 200.0;
    tFluid1->setPressure(pressure1);
    tFluid2->setPressure(pressure2);
    double density1  = tFluid1->getDensity();
    double density2  = tFluid2->getDensity();
    double molWeight = tFluid1->getMWeight();

    /// - Calculate expected link admittance, with default pressure exponent (0.5).
    double effectiveConductivity = 1.0;
    double expectedAdmittance = effectiveConductivity * sqrt( 1000.0 ) *
            sqrt( 0.5 * (density1 + density2) / (pressure1 - pressure2)) / molWeight;

    /// - Get link admittance and verify.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance,
                                 GunnsFluidUtils::computeAdmittance(effectiveConductivity,
                                                                    1.0,
                                                                    tFluid1,
                                                                    tFluid2),
                                 FLT_EPSILON);

    /// - Set up fluids with differing non-zero molecular weights and verify the admittance is
    ///   computed from their average.  Test pressure exponent limited to 0.5.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.8, 0.0};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                 200.0,                  // pressure
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction
    PolyFluid fluid3(*mFluidConfig1, fluidInit);
    fluid3.setPressure(pressure2);
    density2  = fluid3.getDensity();
    molWeight = 0.5 * (tFluid1->getMWeight() + fluid3.getMWeight());
    expectedAdmittance = effectiveConductivity * sqrt( 1000.0 ) *
            sqrt( 0.5 * (density1 + density2) / (pressure1 - pressure2)) / molWeight;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance,
                                 GunnsFluidUtils::computeAdmittance(effectiveConductivity,
                                                                    1.0,
                                                                    tFluid1,
                                                                    &fluid3,
                                                                    0.25),
                                 FLT_EPSILON);

    /// - Test pressure exponent limited to 1.
    expectedAdmittance = effectiveConductivity;
    expectedAdmittance *= 0.5 * (density1 + density2) * 1000.0 * (pressure1 - pressure2);
    expectedAdmittance = expectedAdmittance / (pressure1 - pressure2) / molWeight;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance,
                                 GunnsFluidUtils::computeAdmittance(effectiveConductivity,
                                                                    1.0,
                                                                    tFluid1,
                                                                    &fluid3,
                                                                    1.5),
                                 FLT_EPSILON);

    /// - Test for pressure exponent configured to 0.75.
    expectedAdmittance = effectiveConductivity;
    expectedAdmittance *= powf( 0.5 * (density1 + density2) * 1000.0 * (pressure1 - pressure2), 0.75);
    expectedAdmittance = expectedAdmittance / (pressure1 - pressure2) / molWeight;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance,
                                 GunnsFluidUtils::computeAdmittance(effectiveConductivity,
                                                                    1.0,
                                                                    tFluid1,
                                                                    &fluid3,
                                                                    0.75),
                                 FLT_EPSILON);

    /// - Set the port 1 fluid to have zero molecular weight and verify the admittance is computed
    ///   from the port 0 fluid's value.
    fluid3.resetState();
    density2  = fluid3.getDensity();
    molWeight = tFluid1->getMWeight();
    expectedAdmittance = effectiveConductivity * sqrt( 1000.0 ) *
            sqrt( 0.5 * (density1 + density2) / pressure1) / molWeight;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, fluid3.getDensity(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, fluid3.getMWeight(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance,
                                 GunnsFluidUtils::computeAdmittance(effectiveConductivity,
                                                                    1.0,
                                                                    tFluid1,
                                                                    &fluid3),
                                 FLT_EPSILON);

    /// - Verify the admittance is calculated from the port 1 fluid's molecular weight when the port
    ///   0 fluid's value is zero.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance,
                                 GunnsFluidUtils::computeAdmittance(effectiveConductivity,
                                                                    1.0,
                                                                    &fluid3,
                                                                    tFluid1),
                                 FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests calculation of fluid link admittance when the link delta-pressure is below the
///           minimum for linearization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testLowDpAdmittance()
{
    std::cout << "\n UtGunnsFluidUtils ...... 02: testLowDpAdmittance ...................";

    /// - Set up link port pressures and densities to test values.
    double pressure1 = 200.0;
    double pressure2 = pressure1;
    tFluid1->setPressure(pressure1);
    tFluid2->setPressure(pressure2);
    double density1  = tFluid1->getDensity();
    double density2  = tFluid2->getDensity();
    double molWeight = tFluid1->getMWeight();

    /// - Calculate expected link admittance.
    double effectiveConductivity = 1.0;
    double expectedAdmittance = effectiveConductivity * sqrt( 1000.0 ) *
            sqrt( 0.5 * (density1 + density2) / (1.0)) / molWeight;

    /// - Get link admittance and verify.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance,
                                 GunnsFluidUtils::computeAdmittance(effectiveConductivity,
                                                                    1.0,
                                                                    tFluid1,
                                                                    tFluid2),
                                 FLT_EPSILON);

    /// - Verify admittance is zero if both fluids have zero molecular weight.
    tFluid1->resetState();
    tFluid2->resetState();
    expectedAdmittance = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAdmittance,
                                 GunnsFluidUtils::computeAdmittance(effectiveConductivity,
                                                                    1.0,
                                                                    tFluid1,
                                                                    tFluid2),
                                 FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the computeCapacitance method of the GunnsFluidUtils class.  We also
///           use this as a redundant check on some of the PolyFluid fluid property calculations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testComputeCapacitance()
{
    std::cout << "\n UtGunnsFluidUtils ...... 03: testComputeCapacitance ................";

    /// - Set up our test fluid properties.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                 100.0,                  // pressure
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction

    /// - Create the test fluid
    PolyFluid tFluid(*mFluidConfig2, fluidInit);
    double volume = 1.0;

    /// - Verify fluid properties relevant to capacitance - this is a redundant check on PolyFluid.
    double molWeight = tFluid.getMWeight();    // Should be 2.8836520501689176e+01

    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.8836520501689176e+01, molWeight,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0,                  tFluid.getPressure(), DBL_EPSILON);

    /// - Call PolyFluid directly for the same density perturbation that computeCapacitance does,
    ///   and verify it matches this hardcoded result.  This is a redundant check on PolyFluid.
    double rho1 = tFluid.computeDensity(300.0,  99.9); // Should be 1.1549213620615355e+00
    double rho2 = tFluid.computeDensity(300.0, 100.1); // Should be 1.1572335169405372e+00

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1549213620615355e+00, rho1, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1572335169405372e+00, rho2, DBL_EPSILON);

    /// - Duplicate the capacitance equation here.  The 0.2 is the delta-pressure used in our
    ///   density perturbation.
    double capacitance = (rho2 - rho1) * volume / (molWeight * 0.2);

    /// - Call computeCapacitance and verify correct mCapacitance and return value result.
    double tResult = GunnsFluidUtils::computeCapacitance(&tFluid, volume);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(capacitance, tResult,            DBL_EPSILON);

    /// - Mess up the fluid's molecular weight to result in a negative capacitance, and verify that
    ///   computeCapacitance constrains it to be positive.
    tFluid.setMass(0,  2.1);
    tFluid.setMass(1, -2.0);
    tFluid.setMass(2,  0.0);
    tFluid.updateMass();
    CPPUNIT_ASSERT(0 > tFluid.getMWeight());
    tResult = GunnsFluidUtils::computeCapacitance(&tFluid, volume);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tResult,            DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method makes sure the capacitance returned from a fluid with minimum pressure is
///           correct.  This is identical to the testComputeCapacitance above except for pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testLowPressureCapacitance()
{
    std::cout << "\n UtGunnsFluidUtils ...... 04: testLowPressureCapacitance ............";

    /// - Set up our test fluid properties.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                 FLT_EPSILON,            // pressure
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction

    /// - Create the test fluid.
    PolyFluid tFluid(*mFluidConfig2, fluidInit);
    double volume = 1.0;

    /// - Verify fluid properties relevant to capacitance - this is a redundant check on PolyFluid.
    double molWeight = tFluid.getMWeight();    // Should be 2.8836520501689176e+01

    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.8836520501689176e+01, molWeight,            DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(FLT_EPSILON,            tFluid.getPressure(), DBL_EPSILON);

    /// - Call PolyFluid directly for the same density perturbation that computeCapacitance does,
    ///   and verify it matches this hardcoded result.  This is a redundant check on PolyFluid.
    double P1 = 0.999 * 0.5 * FLT_EPSILON;
    double P2 = 1.001 * 0.5 * FLT_EPSILON;
    double rho1 = tFluid.computeDensity(300.0, P1);         // Should be 6.8838677529188106e-10
    double rho2 = tFluid.computeDensity(300.0, P2);         // Should be 6.8976492699416704e-10

    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.8838677529188106e-10, rho1, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.8976492699416704e-10, rho2, DBL_EPSILON);

    /// - Duplicate the capacitance equation here.
    double capacitance = (rho2 - rho1) * volume / (molWeight * (P2 - P1));

    /// - Call computeCapacitance and verify correct mCapacitance and return value result.
    tFluid.setPressure(0.5 * FLT_EPSILON);
    double tResult = GunnsFluidUtils::computeCapacitance(&tFluid, volume);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(capacitance, tResult,            DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method makes sure the capacitance returned for zero volume is zero.  Test is
///           identical to testComputeCapacitance except that volume is zero and we leave off the
///           extra PolyFluid tests.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testZeroVolumeCapacitance()
{
    std::cout << "\n UtGunnsFluidUtils ...... 05: testZeroVolumeCapacitance .............";

    /// - Set up our test fluid properties.
    double fractions[FluidProperties::NO_FLUID] = {0.2, 0.79, 0.01};
    PolyFluidInputData fluidInit(300.0,                  // temperature
                                 100.0,                  // pressure
                                   0.0,                  // flowrate
                                   0.0,                  // mass
                                 fractions);             // massFraction

    /// - Load the node with the test fluid and leave the volume alone (zero).
    PolyFluid tFluid(*mFluidConfig2, fluidInit);
    double volume = 0.0;

    /// - Zero volume should result in zero capacitance.
    double capacitance = 0.0;

    /// - Call computeCapacitance and verify correct mCapacitance and return value result.
    double tResult = GunnsFluidUtils::computeCapacitance(&tFluid, volume);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(capacitance, tResult,            DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeIsentropicTemperature method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testComputeIsentropicTemperature()
{
    std::cout << "\n UtGunnsFluidUtils ...... 06: testComputeIsentropicTemperature ......";

    /// - Test a gas compression case
    double expansionScaleFactor = 0.5;
    double initialP             = 300.0;
    double finalP               = 310.0;
    double Pratio               = finalP / initialP;
    double gamma                = tFluid1->getAdiabaticIndex();
    double initialT             = tFluid1->getTemperature();

    double finalT = initialT * pow(Pratio, ((gamma - 1.0) / gamma));
    finalT = initialT + expansionScaleFactor * (finalT - initialT);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(finalT,
                                 GunnsFluidUtils::computeIsentropicTemperature(expansionScaleFactor,
                                                                               initialP,
                                                                               finalP,
                                                                               tFluid1),
                                 FLT_EPSILON);

    /// - Test a gas expansion case
    expansionScaleFactor = 1.0;
    initialP             = 300.0;
    finalP               = 290.0;
    Pratio               = finalP / initialP;
    gamma                = tFluid1->getAdiabaticIndex();
    initialT             = tFluid1->getTemperature();

    finalT = initialT * pow(Pratio, ((gamma - 1.0) / gamma));
    finalT = initialT + expansionScaleFactor * (finalT - initialT);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(finalT,
                                 GunnsFluidUtils::computeIsentropicTemperature(expansionScaleFactor,
                                                                               initialP,
                                                                               finalP,
                                                                               tFluid1),
                                 FLT_EPSILON);

    /// - Test a case when initial pressure is zero (protect divide by zero)
    initialP = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(initialT,
                                 GunnsFluidUtils::computeIsentropicTemperature(expansionScaleFactor,
                                                                               initialP,
                                                                               finalP,
                                                                               tFluid1),
                                 FLT_EPSILON);

    /// - Test a liquid case
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tFluid3->getTemperature(),
                                 GunnsFluidUtils::computeIsentropicTemperature(expansionScaleFactor,
                                                                               initialP,
                                                                               finalP,
                                                                               tFluid3),
                                 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the predictConductivity method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testPredictConductivity()
{
    std::cout << "\n UtGunnsFluidUtils ...... 07: testPredictConductivity ...............";

    /// - Test a normal case with default pressure exponent (0.5)
    double desiredMdot = 1.0;
    double dP = tFluid1->getPressure() - tFluid2->getPressure();
    double avgDensity = 0.5 * (tFluid1->getDensity() + tFluid2->getDensity());
    double expectedConductivity = desiredMdot / sqrt(dP * 1000.0 * avgDensity);
    double minLinearizationP = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedConductivity,
                                 GunnsFluidUtils::predictConductivity(desiredMdot,
                                                                      minLinearizationP,
                                                                      tFluid1,
                                                                      tFluid2),
                                 DBL_EPSILON);

    /// - Test with pressure exponent limited 0.5
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedConductivity,
                                 GunnsFluidUtils::predictConductivity(desiredMdot,
                                                                      minLinearizationP,
                                                                      tFluid1,
                                                                      tFluid2,
                                                                      0.25),
                                 FLT_EPSILON);

    /// - Test with pressure exponent limited to 1.
    expectedConductivity = desiredMdot / (dP * 1000.0 * avgDensity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedConductivity,
                                 GunnsFluidUtils::predictConductivity(desiredMdot,
                                                                      minLinearizationP,
                                                                      tFluid1,
                                                                      tFluid2,
                                                                      1.25),
                                 FLT_EPSILON);

    /// - Test with pressure exponent 0.75.
    expectedConductivity = desiredMdot / powf(dP * 1000.0 * avgDensity, 0.75);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedConductivity,
                                 GunnsFluidUtils::predictConductivity(desiredMdot,
                                                                      minLinearizationP,
                                                                      tFluid1,
                                                                      tFluid2,
                                                                      0.75),
                                 FLT_EPSILON);

    /// - Test using a negative desired flow rate
    desiredMdot = -1.0;
    expectedConductivity = fabs(desiredMdot) / sqrt(dP * 1000.0 * avgDensity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedConductivity,
                                 GunnsFluidUtils::predictConductivity(desiredMdot,
                                                                      minLinearizationP,
                                                                      tFluid1,
                                                                      tFluid2),
                                 FLT_EPSILON);

    /// - Test using a reverse pressure gradient
    desiredMdot = 1.0;
    dP = tFluid1->getPressure() - tFluid2->getPressure();
    expectedConductivity = fabs(desiredMdot) / sqrt(fabs(dP) * 1000.0 * avgDensity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedConductivity,
                                 GunnsFluidUtils::predictConductivity(desiredMdot,
                                                                      minLinearizationP,
                                                                      tFluid2,
                                                                      tFluid1),
                                 FLT_EPSILON);

    /// - Test a zero dP case
    tFluid2->setPressure(tFluid1->getPressure());
    expectedConductivity = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedConductivity,
                                 GunnsFluidUtils::predictConductivity(desiredMdot,
                                                                      minLinearizationP,
                                                                      tFluid1,
                                                                      tFluid2),
                                 0.0);

    /// - Test a dP < minimum linearization potential case
    tFluid2->setPressure(tFluid1->getPressure() - 0.5);
    dP = tFluid1->getPressure() - tFluid2->getPressure();
    avgDensity = 0.5 * (tFluid1->getDensity() + tFluid2->getDensity());
    expectedConductivity = fabs(desiredMdot) / sqrt(minLinearizationP * 1000.0 * avgDensity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedConductivity,
                                 GunnsFluidUtils::predictConductivity(desiredMdot,
                                                                      minLinearizationP,
                                                                      tFluid1,
                                                                      tFluid2),
                                 FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the predictExpansionScaleFactor method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testPredictExpansionScaleFactor()
{
    std::cout << "\n UtGunnsFluidUtils ...... 08: testPredictExpansionScaleFactor .......";

    /// - Test a normal case - this results in a scale factor between 0 and 1.
    double desiredDT = 1.0;
    double pressureRatio = tFluid2->getPressure() / tFluid1->getPressure();
    double supplyT = tFluid1->getTemperature();
    double gamma   = tFluid1->getAdiabaticIndex();
    double expectedFactor = -desiredDT / (supplyT * (pow(pressureRatio, (gamma-1.0)/gamma) - 1.0));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFactor,
                                 GunnsFluidUtils::predictExpansionScaleFactor(desiredDT,
                                                                              tFluid1,
                                                                              tFluid2),
                                 FLT_EPSILON);

    /// - Test using a reverse pressure gradient
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFactor,
                                 GunnsFluidUtils::predictExpansionScaleFactor(desiredDT,
                                                                              tFluid2,
                                                                              tFluid1),
                                 FLT_EPSILON);

    /// - Test upper limit
    desiredDT = 100.0;
    expectedFactor = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFactor,
                                 GunnsFluidUtils::predictExpansionScaleFactor(desiredDT,
                                                                              tFluid1,
                                                                              tFluid2),
                                 0.0);

    /// - Test using a liquid
    tFluid3->setPressure(800.0);
    expectedFactor = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFactor,
                                 GunnsFluidUtils::predictExpansionScaleFactor(desiredDT,
                                                                              tFluid3,
                                                                              tFluid1),
                                 0.0);

    /// - Test equal pressures
    tFluid2->setPressure(tFluid1->getPressure());
    expectedFactor = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFactor,
                                 GunnsFluidUtils::predictExpansionScaleFactor(desiredDT,
                                                                              tFluid1,
                                                                              tFluid2),
                                 0.0);

    /// - Test zero fluid pressures
    tFluid1->setPressure(0.0);
    tFluid2->setPressure(0.0);
    expectedFactor = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFactor,
                                 GunnsFluidUtils::predictExpansionScaleFactor(desiredDT,
                                                                              tFluid1,
                                                                              tFluid2),
                                 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the buildTransformMap method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testBuildTransformMap()
{
    std::cout << "\n UtGunnsFluidUtils ...... 09: testBuildTransformMap .................";

    int transformMap[4];

    /// - Check the transform map when convertToType is present in the out fluid.
    GunnsFluidUtils::buildTransformMap(transformMap, mFluidConfig1, mFluidConfig2,
                                       FluidProperties::GUNNS_N2);

    CPPUNIT_ASSERT_EQUAL(3, transformMap[0]);      // size of the input fluid
    CPPUNIT_ASSERT_EQUAL(1, transformMap[1]);      // in N2 -> out N2
    CPPUNIT_ASSERT_EQUAL(0, transformMap[2]);      // in O2 -> out O2
    CPPUNIT_ASSERT_EQUAL(1, transformMap[3]);      // in WATER -> out N2

    /// - Check for when convertToType = NO_FLUID.
    GunnsFluidUtils::buildTransformMap(transformMap, mFluidConfig1, mFluidConfig2,
                                       FluidProperties::NO_FLUID);

    CPPUNIT_ASSERT_EQUAL( 3, transformMap[0]);     // size of the input fluid
    CPPUNIT_ASSERT_EQUAL( 1, transformMap[1]);     // in N2 -> out N2
    CPPUNIT_ASSERT_EQUAL( 0, transformMap[2]);     // in O2 -> out O2
    CPPUNIT_ASSERT_EQUAL(-1, transformMap[3]);     // in WATER -> out NO_FLUID

    /// - When convertToType is invalid, should get an exception.
    try {
        GunnsFluidUtils::buildTransformMap(transformMap, mFluidConfig1, mFluidConfig2,
                                           FluidProperties::GUNNS_AMMONIA);
    } catch (TsInitializationException& e) {
        CPPUNIT_ASSERT( true );
    } catch ( ... ) {
        CPPUNIT_ASSERT( false );
    }

    /// - Check for an exception for a resulting empty map.
    FluidProperties::FluidType type[1] = {FluidProperties::GUNNS_CH4};

    PolyFluidConfigData* fluidConfig;
    fluidConfig = new PolyFluidConfigData(mFluidProperties, type, 1);
    try {
        GunnsFluidUtils::buildTransformMap(transformMap, fluidConfig, mFluidConfig2,
                                           FluidProperties::NO_FLUID);
    } catch (TsInitializationException& e) {
        CPPUNIT_ASSERT( true );
    } catch ( ... ) {
        CPPUNIT_ASSERT( false );
    }
    delete fluidConfig;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the transformState method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testTransformState()
{
    std::cout << "\n UtGunnsFluidUtils ...... 10: testTransformState ....................";

    /// - Build a transform map from mFluidConfig2 to mFluidConfig1.
    int transformMap[4];
    GunnsFluidUtils::buildTransformMap(transformMap, mFluidConfig2, mFluidConfig1,
                                       FluidProperties::GUNNS_N2);

    /// - Set up a mass fractions array for the incoming fluid.
    double massFractions[3] = {0.2, 0.79, 0.01};

    /// - Call transformState method with incoming properties, map, & returned fluid
    GunnsFluidUtils::transformState(tFluid1, 100.0, 300.0, massFractions, transformMap);

    /// - Verify properties of returned fluid
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, tFluid1->getPressure(),    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, tFluid1->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2,   tFluid1->getMassFraction(FluidProperties::GUNNS_O2), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8,   tFluid1->getMassFraction(FluidProperties::GUNNS_N2), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   tFluid1->getTraceCompounds()->getMoleFractions()[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   tFluid1->getTraceCompounds()->getMoleFractions()[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   tFluid1->getTraceCompounds()->getMoleFractions()[2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   tFluid1->getTraceCompounds()->getMoleFractions()[3], 0.0);

    /// - Verify trace compounds are added to the returned fluid.
    double tcMoleFractions[4] = {1.0e-5, 2.0e-6, 3.0e-7, 4.0e-8};

    /// - Call transformState method with incoming properties, map, & returned fluid
    GunnsFluidUtils::transformState(tFluid1, 100.0, 300.0, massFractions, transformMap, tcMoleFractions);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(100.0, tFluid1->getPressure(),    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, tFluid1->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2,   tFluid1->getMassFraction(FluidProperties::GUNNS_O2), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8,   tFluid1->getMassFraction(FluidProperties::GUNNS_N2), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tcMoleFractions[0], tFluid1->getTraceCompounds()->getMoleFractions()[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tcMoleFractions[1], tFluid1->getTraceCompounds()->getMoleFractions()[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tcMoleFractions[2], tFluid1->getTraceCompounds()->getMoleFractions()[2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tcMoleFractions[3], tFluid1->getTraceCompounds()->getMoleFractions()[3], 0.0);

    /// - Build a new map that converts the CO2 to NO_FLUID.
    GunnsFluidUtils::buildTransformMap(transformMap, mFluidConfig2, mFluidConfig1,
                                       FluidProperties::NO_FLUID);

    /// - Verify exception thrown if all of the incoming is converted to NO_FLUID.  The PolyFluid
    ///   class should throw the exception because the mass fractions don't sum to 1.
    massFractions[0] = 0.0;
    massFractions[1] = 0.0;
    massFractions[2] = 1.0;

    try {
        GunnsFluidUtils::transformState(tFluid1, 100.0, 300.0, massFractions, transformMap);
    } catch (TsOutOfBoundsException& e) {
        CPPUNIT_ASSERT( true );
    } catch ( ... ) {
        CPPUNIT_ASSERT( false );
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeConvectiveHeatTransferCoefficient, computeFlowRegimeFactor,
///           computeReynoldsNumber, computeNusseltNumber & computeDarcyFrictionFactor methods in
///           GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testConvectiveCoefficient()
{
    std::cout << "\n UtGunnsFluidUtils ...... 11: testConvectiveCoefficient .............";

    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    PolyFluidConfigData config(mFluidProperties, types, 2);

    double fractions[2] = {1.0, 0.0};
    PolyFluidInputData input(260.0,                   //temperature
                             200.0,                   //pressure
                               0.0,                   //flowRate
                               0.0,                   //mass
                               fractions);            //massFractions
    // initialize the gases
    PolyFluid fluid(config, input);
    const double flowrate[8] = {2.0E-3,   5.0E-2,  -1.0E-1,   0.0,      1.0e+0,   2.1E10,   1.0E-2, 8.0E-3};
    const double rOverD[8]   = {1.905E-6, 7.620E-6, 3.810E-6, 2.540E-6, 1.905E-6, 9.525E-7, 5.0E-3, 1.905E-6};
    const double length[8]   = {0.2,      0.05,     0.1,      0.15,     0.0,      0.4,      0.2,    0.2};
    const double expected[8] = {4.1259710879E-001,  //laminar
                                6.1896956606E+001,  //turbulent
                                3.1150838573E+001,  //turbulent, negative flow direction
                                0.0,                //zero flow
                                0.0,                //zero diameter
                                3.6521894736E+003,  //max Re limit
                                1.6344938409E+000,  //hi R over D
                                8.9605005660E-001}; //transition

    for (int i = 0; i < 8; ++i) {
        fluid.setPressure(200.0 + i * 50.0);
        fluid.setTemperature(260.0 + i * 10.0);

        const double returned = GunnsFluidUtils::computeConvectiveHeatTransferCoefficient(flowrate[i],
                                                                                          &fluid,
                                                                                          rOverD[i],
                                                                                          length[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected[i], returned, 1.0e-06);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeConvectiveHeatFlux methods in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testConvectiveHeatFlux()
{
    std::cout << "\n UtGunnsFluidUtils ...... 12: testConvectiveHeatFlux ................";

    // initialize the test fluid
    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    PolyFluidConfigData config(mFluidProperties, types, 2);

    double fractions[2] = {0.4, 0.6};
    PolyFluidInputData input(280.0,                  //temperature
                             110.0,                  //pressure
                              0.0,                   //flowRate
                              0.0,                   //mass
                             fractions);             //massFractions
    PolyFluid fluid(config, input);

    /// @test     Convective heat transfer with positive flow rate.
    double flowRate        = 0.1;
    double rOverD          = 0.1;
    double diameter        = 0.1;
    double surfaceArea     = 1.0;
    double wallTemperature = 300.0;
    double hIn             = fluid.getSpecificEnthalpy();

    double heatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(&fluid,
                                                                 flowRate,
                                                                 rOverD,
                                                                 diameter,
                                                                 surfaceArea,
                                                                 wallTemperature);
    double hOut            = fluid.getSpecificEnthalpy();
    double expectedHeat    = flowRate * (hIn - hOut);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, heatFlux, FLT_EPSILON);
    CPPUNIT_ASSERT(280.0           < fluid.getTemperature());
    CPPUNIT_ASSERT(wallTemperature > fluid.getTemperature());
    fluid.setTemperature(280.0);

    /// @test     Convective heat transfer with negative flow rate.
    flowRate               = -0.1;
    hIn                    = fluid.getSpecificEnthalpy();
    heatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(&fluid,
                                                          flowRate,
                                                          rOverD,
                                                          diameter,
                                                          surfaceArea,
                                                          wallTemperature);
    hOut                   = fluid.getSpecificEnthalpy();
    expectedHeat           = -flowRate * (hIn - hOut);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, heatFlux, FLT_EPSILON);
    CPPUNIT_ASSERT(280.0           < fluid.getTemperature());
    CPPUNIT_ASSERT(wallTemperature > fluid.getTemperature());
    fluid.setTemperature(280.0);

    /// @test     Convective heat transfer to cooler wall than fluid.
    wallTemperature        = 260.0;
    hIn                    = fluid.getSpecificEnthalpy();
    heatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(&fluid,
                                                          flowRate,
                                                          rOverD,
                                                          diameter,
                                                          surfaceArea,
                                                          wallTemperature);
    hOut                   = fluid.getSpecificEnthalpy();
    expectedHeat           = -flowRate * (hIn - hOut);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat, heatFlux, FLT_EPSILON);
    CPPUNIT_ASSERT(280.0           > fluid.getTemperature());
    CPPUNIT_ASSERT(wallTemperature < fluid.getTemperature());
    fluid.setTemperature(280.0);

    /// @test     No convective heat transfer when flow rate is zero.
    flowRate               = 0.0;
    heatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(&fluid,
                                                          flowRate,
                                                          rOverD,
                                                          diameter,
                                                          surfaceArea,
                                                          wallTemperature);
    CPPUNIT_ASSERT(0.0 == heatFlux);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(280.0, fluid.getTemperature(), DBL_EPSILON);

    /// @test     Limiting excessively high UA.  Verify that the exit temperature and returned heat
    ///           flux agree.
    flowRate        = 0.1;
    double UA       = 1.0E15;
    wallTemperature = 300.0;
    hIn             = fluid.getSpecificEnthalpy();
    hOut            = fluid.computeSpecificEnthalpy(wallTemperature);
    expectedHeat    = flowRate * (hIn - hOut);
    heatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(&fluid,
                                                          flowRate,
                                                          UA,
                                                          wallTemperature);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(wallTemperature, fluid.getTemperature(), 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHeat,    heatFlux,               FLT_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeConvectiveHeatTransferCoefficient method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testConductiveHeatFlux()
{
    std::cout << "\n UtGunnsFluidUtils ...... 13: testConductiveHeatFlux ................";

    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    PolyFluidConfigData config(mFluidProperties, types, 2);

    double fractions0[2] = {0.4, 0.6};
    PolyFluidInputData input0(280.0,                  //temperature
                              110.0,                  //pressure
                               0.0,                   //flowRate
                               0.0,                   //mass
                              fractions0);            //massFractions
    double fractions1[2] = {0.6, 0.4};
    PolyFluidInputData input1(280.0,                  //temperature
                              110.0,                  //pressure
                               0.0,                   //flowRate
                               0.0,                   //mass
                              fractions1);            //massFractions
    // initialize the gases
    PolyFluid fluid0(config, input0);
    PolyFluid fluid1(config, input1);
    const double area[5]     = {10.0, 1.0, 10.0, 1.0, 100.0};
    const double length0[5]  = {10.0, 1.0,  0.1, 1.0,  10.0};
    const double length1[5]  = { 0.1, 1.0, 10.0, 0.1,  10.0};
    const double expected[5] = {-0.90730937,
                                -0.24093612,
                                 0.00000000,
                                 0.45018317,
                                 4.83399658};

    for (int i = 0; i < 5; ++i) {
        fluid0.setTemperature(260.0 + i * 10.0);
        fluid1.setTemperature(300.0 - i * 10.0);
        const double returned = GunnsFluidUtils::computeConductiveHeatFlux(area[i],
                                                                           &fluid0,
                                                                           &fluid1,
                                                                           length0[i],
                                                                           length1[i]);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected[i], returned, 1.0e-08);
    }

    /// @test    Out of bounds exception on total length of zero.
    CPPUNIT_ASSERT_THROW(GunnsFluidUtils::computeConductiveHeatFlux(1.0,
                                                                    &fluid0,
                                                                    &fluid1,
                                                                    0.0,
                                                                    0.0),
                         TsOutOfBoundsException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the computeConvectiveHeatTransferCoefficient method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testGasDiffusion()
{
    std::cout << "\n UtGunnsFluidUtils ...... 14: testGasDiffusion ......................";

    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_O2;
    PolyFluidConfigData config(mFluidProperties, types, 2);

    double fractions[2] = {0.5, 0.5};
    PolyFluidInputData input(280.0,                   //temperature
                             110.0,                   //pressure
                               0.0,                   //flowRate
                               0.0,                   //mass
                               fractions);            //massFractions
    // initialize the gases
    PolyFluid fluid0(config, input);
    PolyFluid fluid1(config, input);
    PolyFluid fluid2(config, input);
    const double area[5]     = {1.0,  1.0,  10.0,         1.0, 100.0};
    const double fraction[5] = {0.40, 0.45,  0.500000001, 0.55, 0.60};
    const double length0[5]  = {0.5,  1.0,   0.1,         1.0,  10.0};
    const double length1[5]  = {0.5,  1.0,  10.0,         0.1,  10.0};
    // Expected values of net mass flux for sigma = 3.0E-19.  These are calculated in the
    // GunnsFluidUtilsExpectedValues.ods spreadsheet.
    double expected[5] = {6.9620668E-07,
                          1.73861040E-07,
                          -6.88306822E-15,
                          -3.19292266E-07,
                          -3.48103340E-06};

    for (int i = 0; i < 5; ++i) {
        fluid0.setPressure(100.0);
        fluid0.setMass(0, fraction[i]);
        fluid0.setMass(1, 1.0 - fraction[i]);
        fluid0.updateMass();
        fluid0.setTemperature(260.0 + i * 5.0);
        fluid1.setPressure(100.0);
        fluid1.setMass(1, fraction[i]);
        fluid1.setMass(0, 1.0 - fraction[i]);
        fluid1.updateMass();
        fluid1.setTemperature(280.0 - i * 5.0);
        fluid2.resetState();
        const double returned = GunnsFluidUtils::computeGasDiffusion(&fluid2,
                                                                     area[i],
                                                                     &fluid0,
                                                                     &fluid1,
                                                                     0.0,
                                                                     length0[i],
                                                                     length1[i]);

        /// @test    net mass flux
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected[i], returned, 1.0e-08);
        /// @test    temperature
        if (i > 1) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL(fluid0.getTemperature(), fluid2.getTemperature(), 1.0e-08);
        } else {
            CPPUNIT_ASSERT_DOUBLES_EQUAL(fluid1.getTemperature(), fluid2.getTemperature(), 1.0e-08);
        }
    }

    /// @test    Out of bounds exception on total length of zero.
    CPPUNIT_ASSERT_THROW(GunnsFluidUtils::computeGasDiffusion(&fluid2,
                                                              1.0,
                                                              &fluid0,
                                                              &fluid1,
                                                              0.0,
                                                              0.0,
                                                              0.0),
                        TsOutOfBoundsException);

    /// @test   Interference of bulk flow on diffusion - positive bulk flow
    // Refer to GunnsFluidUtilsExpectedValues.ods spreadsheet for expected result calculations.
    expected[0] = 6.82255e-07;
    expected[1] = 4.84801e-07;
    expected[2] = 2.87347e-07;
    expected[3] = 8.98927e-08;
    expected[4] = 0.0;
    double bulkFlowRate = 0.0;
    for (int i = 0; i < 5; ++i) {
        bulkFlowRate = i * 1.5E-06;
        fluid0.setPressure(100.0);
        fluid0.setMass(0, fraction[0]);
        fluid0.setMass(1, 1.0 - fraction[0]);
        fluid0.updateMass();
        fluid0.setTemperature(260.0);
        fluid1.setPressure(100.0);
        fluid1.setMass(1, fraction[0]);
        fluid1.setMass(0, 1.0 - fraction[0]);
        fluid1.updateMass();
        fluid1.setTemperature(260.0);
        fluid2.resetState();

        const double returned = GunnsFluidUtils::computeGasDiffusion(&fluid2,
                                                                     area[0],
                                                                     &fluid0,
                                                                     &fluid1,
                                                                     bulkFlowRate,
                                                                     length0[0],
                                                                     length1[0]);

        /// @test    net mass flux
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected[i], returned, 1.0e-08);
    }

    /// @test   Interference of bulk flow on diffusion - negative bulk flow
    // Refer to GunnsFluidUtilsExpectedValues.ods spreadsheet for expected result calculations.
    expected[0] = 6.82255e-07;
    expected[1] = 4.79485e-07;
    expected[2] = 2.76715e-07;
    expected[3] = 7.39454e-08;
    expected[4] = 0.0;
    bulkFlowRate = 0.0;
    for (int i = 0; i < 5; ++i) {
        bulkFlowRate = i * -1.5E-06;
        fluid0.setPressure(100.0);
        fluid0.setMass(0, fraction[0]);
        fluid0.setMass(1, 1.0 - fraction[0]);
        fluid0.updateMass();
        fluid0.setTemperature(260.0);
        fluid1.setPressure(100.0);
        fluid1.setMass(1, fraction[0]);
        fluid1.setMass(0, 1.0 - fraction[0]);
        fluid1.updateMass();
        fluid1.setTemperature(260.0);
        fluid2.resetState();

        const double returned = GunnsFluidUtils::computeGasDiffusion(&fluid2,
                                                                     area[0],
                                                                     &fluid0,
                                                                     &fluid1,
                                                                     bulkFlowRate,
                                                                     length0[0],
                                                                     length1[0]);

        /// @test    net mass flux
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected[i], returned, 1.0e-08);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the testRelativeHumidityH2O method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testRelativeHumidityH2O()
{
    std::cout << "\n UtGunnsFluidUtils ...... 15: testRelativeHumidityH2O ...............";

    /// - Set up a test fluid with H2O in it.
    FluidProperties::FluidType types[2];
    types[0]    = FluidProperties::GUNNS_N2;
    types[1]    = FluidProperties::GUNNS_H2O;
    PolyFluidConfigData* fluidConfig = new PolyFluidConfigData(mFluidProperties, types, 2);

    double fractions[2] = {0.9, 0.1};
    PolyFluidInputData*  fluidInput  = new PolyFluidInputData(283.15,                //temperature
                                                              101.325,               //pressure
                                                                0.0,                 //flowRate
                                                                0.0,                 //mass
                                                              fractions);            //massFractions

    PolyFluid* fluid = new PolyFluid(*fluidConfig, *fluidInput);
    const FluidProperties* propertiesH2O = fluid->getProperties(FluidProperties::GUNNS_H2O);

    /// @test The relative humidity of the test fluid.
    const double expectedHumidity = fluid->getPartialPressure(FluidProperties::GUNNS_H2O) /
            propertiesH2O->getSaturationPressure(fluid->getTemperature());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedHumidity,
            GunnsFluidUtils::computeRelativeHumidityH2O(fluid), DBL_EPSILON);

    delete fluid;
    delete fluidInput;
    delete fluidConfig;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the convertMoleFractionToMassFraction method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testMoleToMassFraction()
{
    std::cout << "\n UtGunnsFluidUtils ...... 16: testMoleToMassFraction ................";

    /// - Test nominal mixture case.
    fractions[0] = 0.5;
    fractions[1] = 0.5;
    fractions[2] = 0.0;

    double outFractions[3] = {0.0, 0.0, 0.0};

    GunnsFluidUtils::convertMoleFractionToMassFraction(outFractions, fractions, mFluidConfig1);

    double temp[3];
    temp[0] = fractions[0] * 28.0134;         // molecular weight of N2
    temp[1] = fractions[1] * 31.9988;         // molecular weight of O2
    temp[2] = fractions[2] * 18.0153;         // molecular weight of H2O
    const double sum = temp[0] + temp[1] + temp[2];

    const double expectedN2  = temp[0] / sum;
    const double expectedO2  = temp[1] / sum;
    const double expectedH2O = temp[2] / sum;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedN2,  outFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2,  outFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2O, outFractions[2], DBL_EPSILON);

    /// - Test a case where the input array is zero, verify the output array is unchanged.
    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 0.0;

    GunnsFluidUtils::convertMoleFractionToMassFraction(outFractions, fractions, mFluidConfig1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedN2,  outFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2,  outFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2O, outFractions[2], DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the convertMassFractionToMoleFraction method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testMassToMoleFraction()
{
    std::cout << "\n UtGunnsFluidUtils ...... 17: testMassToMoleFraction ................";

    /// - Test nominal mixture case.
    fractions[0] = 0.3;
    fractions[1] = 0.5;
    fractions[2] = 0.2;

    double outFractions[3] = {0.0, 0.0, 0.0};

    GunnsFluidUtils::convertMassFractionToMoleFraction(outFractions, fractions, mFluidConfig1);

    double temp[3];
    temp[0] = fractions[0] / 28.0134;         // molecular weight of N2
    temp[1] = fractions[1] / 31.9988;         // molecular weight of O2
    temp[2] = fractions[2] / 18.0153;         // molecular weight of H2O
    const double sum = temp[0] + temp[1] + temp[2];

    const double expectedN2  = temp[0] / sum;
    const double expectedO2  = temp[1] / sum;
    const double expectedH2O = temp[2] / sum;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedN2,  outFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2,  outFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2O, outFractions[2], DBL_EPSILON);

    /// - Test a case where the input array is zero, verify the output array is unchanged.
    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 0.0;

    GunnsFluidUtils::convertMoleFractionToMassFraction(outFractions, fractions, mFluidConfig1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedN2,  outFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2,  outFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2O, outFractions[2], DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the convertPartialPressureToMassFraction method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testPpToMoleFraction()
{
    std::cout << "\n UtGunnsFluidUtils ...... 18: testPpToMoleFraction ..................";

    /// - Test nominal mixture case.
    fractions[0] = 78.0;
    fractions[1] = 20.0;
    fractions[2] =  2.0;
    const double sum = fractions[0] + fractions[1] + fractions[2];

    double outFractions[3] = {0.0, 0.0, 0.0};

    GunnsFluidUtils::convertPartialPressureToMoleFraction(outFractions, fractions, mFluidConfig1);

    const double expectedN2  = fractions[0] / sum;
    const double expectedO2  = fractions[1] / sum;
    const double expectedH2O = fractions[2] / sum;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedN2,  outFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2,  outFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2O, outFractions[2], DBL_EPSILON);

    /// - Test a case where the input array is zero, verify the output array is unchanged.
    fractions[0] = 0.0;
    fractions[1] = 0.0;
    fractions[2] = 0.0;

    GunnsFluidUtils::convertPartialPressureToMoleFraction(outFractions, fractions, mFluidConfig1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedN2,  outFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2,  outFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2O, outFractions[2], DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the convertMoleFractionToPartialPressure method in GunnsFluidUtils.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidUtils::testMoleToPpFraction()
{
    std::cout << "\n UtGunnsFluidUtils ...... 19: testMoleToPpFraction ..................";

    /// - Test nominal mixture case.
    fractions[0] = 0.78;
    fractions[1] = 0.20;
    fractions[2] = 0.02;

    double outFractions[3] = {0.0, 0.0, 0.0};

    GunnsFluidUtils::convertMoleFractionToPartialPressure(outFractions, fractions, mFluidConfig1,
                                                          tFluid1->getPressure());

    const double expectedN2  = fractions[0] * tFluid1->getPressure();
    const double expectedO2  = fractions[1] * tFluid1->getPressure();
    const double expectedH2O = fractions[2] * tFluid1->getPressure();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedN2,  outFractions[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedO2,  outFractions[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2O, outFractions[2], DBL_EPSILON);

    std::cout << "... Pass";
}
