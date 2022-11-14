/*
@file   UtSorbantProperties.cpp
@brief  Unit tests for SorbantProperties

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include <iostream>
#include "properties/SorbantProperties.hh"
#include "UtSorbantProperties.hh"
#include "strings/UtResult.hh"
#include "software/exceptions/TsInitializationException.hh"

int UtSorbantProperties::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Sorbant Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSorbantProperties::UtSorbantProperties()
    :
    CppUnit::TestFixture()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Sorbant Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSorbantProperties::~UtSorbantProperties()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSorbantProperties::setUp()
{
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSorbantProperties::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSorbantProperties::testDefinedSorbantsConstruction()
{
    UT_RESULT_FIRST;

    {
        /// @test object for creation of properties for each compound type.
        DefinedSorbantProperties sorbants;
        for (int i = 0; i < SorbantProperties::CUSTOM; i++) {
            const SorbantProperties::Type expected = static_cast<SorbantProperties::Type>(i);
            const SorbantProperties::Type returned = sorbants.getSorbant(expected)->getType();
            CPPUNIT_ASSERT(expected == returned);
        }
    } {
        /// @test pointer for creation of properties for each compound type.
        DefinedSorbantProperties* sorbants = new DefinedSorbantProperties;
        for (int i = 0; i < SorbantProperties::CUSTOM; i++) {
            const SorbantProperties::Type expected = static_cast<SorbantProperties::Type>(i);
            const SorbantProperties::Type returned = sorbants->getSorbant(expected)->getType();
            CPPUNIT_ASSERT(expected == returned);
        }
        delete sorbants;
    }

    /// @test sorbants have their sorbates and interactions.
    DefinedSorbantProperties sorbants;
    {
        const SorbantProperties* sorbant = sorbants.getSorbant(SorbantProperties::SILICA_GEL_B125);
        const std::vector<SorbateProperties>* sorbates = sorbant->getSorbates();
        CPPUNIT_ASSERT(2                     == sorbates->size());
        CPPUNIT_ASSERT(ChemicalCompound::H2O == sorbates->at(0).getCompound()->mType);
        CPPUNIT_ASSERT(ChemicalCompound::CO2 == sorbates->at(1).getCompound()->mType);
        const std::vector<SorbateInteractingCompounds>* blocking = sorbates->at(1).getBlockingCompounds();
        CPPUNIT_ASSERT(1                     == blocking->size());
        CPPUNIT_ASSERT(ChemicalCompound::H2O == blocking->at(0).mCompound);
        const std::vector<SorbateInteractingCompounds>* offgas = sorbates->at(1).getOffgasCompounds();
        CPPUNIT_ASSERT(0                     == offgas->size());
    } {
        const SorbantProperties* sorbant = sorbants.getSorbant(SorbantProperties::SILICA_GEL_40);
        const std::vector<SorbateProperties>* sorbates = sorbant->getSorbates();
        CPPUNIT_ASSERT(2                     == sorbates->size());
        CPPUNIT_ASSERT(ChemicalCompound::H2O == sorbates->at(0).getCompound()->mType);
        CPPUNIT_ASSERT(ChemicalCompound::CO2 == sorbates->at(1).getCompound()->mType);
        const std::vector<SorbateInteractingCompounds>* blocking = sorbates->at(1).getBlockingCompounds();
        CPPUNIT_ASSERT(1                     == blocking->size());
        CPPUNIT_ASSERT(ChemicalCompound::H2O == blocking->at(0).mCompound);
        const std::vector<SorbateInteractingCompounds>* offgas = sorbates->at(1).getOffgasCompounds();
        CPPUNIT_ASSERT(0                     == offgas->size());
    } {
        const SorbantProperties* sorbant = sorbants.getSorbant(SorbantProperties::ZEO_5A_RK38);
        const std::vector<SorbateProperties>* sorbates = sorbant->getSorbates();
        CPPUNIT_ASSERT(2                     == sorbates->size());
        CPPUNIT_ASSERT(ChemicalCompound::H2O == sorbates->at(0).getCompound()->mType);
        CPPUNIT_ASSERT(ChemicalCompound::CO2 == sorbates->at(1).getCompound()->mType);
        const std::vector<SorbateInteractingCompounds>* blocking = sorbates->at(1).getBlockingCompounds();
        CPPUNIT_ASSERT(1                     == blocking->size());
        CPPUNIT_ASSERT(ChemicalCompound::H2O == blocking->at(0).mCompound);
        const std::vector<SorbateInteractingCompounds>* offgas = sorbates->at(1).getOffgasCompounds();
        CPPUNIT_ASSERT(0                     == offgas->size());
    } {
        const SorbantProperties* sorbant = sorbants.getSorbant(SorbantProperties::ZEO_5A_522);
        const std::vector<SorbateProperties>* sorbates = sorbant->getSorbates();
        CPPUNIT_ASSERT(2                     == sorbates->size());
        CPPUNIT_ASSERT(ChemicalCompound::H2O == sorbates->at(0).getCompound()->mType);
        CPPUNIT_ASSERT(ChemicalCompound::CO2 == sorbates->at(1).getCompound()->mType);
        const std::vector<SorbateInteractingCompounds>* blocking = sorbates->at(1).getBlockingCompounds();
        CPPUNIT_ASSERT(1                     == blocking->size());
        CPPUNIT_ASSERT(ChemicalCompound::H2O == blocking->at(0).mCompound);
        const std::vector<SorbateInteractingCompounds>* offgas = sorbates->at(1).getOffgasCompounds();
        CPPUNIT_ASSERT(0                     == offgas->size());
    } {
        const SorbantProperties* sorbant = sorbants.getSorbant(SorbantProperties::ZEO_13X_544);
        const std::vector<SorbateProperties>* sorbates = sorbant->getSorbates();
        CPPUNIT_ASSERT(2                     == sorbates->size());
        CPPUNIT_ASSERT(ChemicalCompound::H2O == sorbates->at(0).getCompound()->mType);
        CPPUNIT_ASSERT(ChemicalCompound::CO2 == sorbates->at(1).getCompound()->mType);
        const std::vector<SorbateInteractingCompounds>* blocking = sorbates->at(1).getBlockingCompounds();
        CPPUNIT_ASSERT(1                     == blocking->size());
        CPPUNIT_ASSERT(ChemicalCompound::H2O == blocking->at(0).mCompound);
        const std::vector<SorbateInteractingCompounds>* offgas = sorbates->at(1).getOffgasCompounds();
        CPPUNIT_ASSERT(0                     == offgas->size());
    } {
        //TODO test SA9T when it's implemented
    } {
        const SorbantProperties* sorbant = sorbants.getSorbant(SorbantProperties::GLASS_BEADS_LATTICE);
        const std::vector<SorbateProperties>* sorbates = sorbant->getSorbates();
        CPPUNIT_ASSERT(0                     == sorbates->size());
    } {
        const SorbantProperties* sorbant = sorbants.getSorbant(SorbantProperties::GLASS_BEADS_RANDOM);
        const std::vector<SorbateProperties>* sorbates = sorbant->getSorbates();
        CPPUNIT_ASSERT(0                     == sorbates->size());
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for properties of the defined sorbants.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSorbantProperties::testDefinedSorbantsProperties()
{
    UT_RESULT;

    /// - Set up sorbant properties constants.  These should match the values from
    ///   DefinedSorbantProperites.
    const double densities[SorbantProperties::CUSTOM] = {
            1240.0, 1240.0, 1370.0, 1190.0, 1260.0, 1.0, 2500.0, 2500.0
    };
    const double porosities[SorbantProperties::CUSTOM] = {
            0.348, 0.415, 0.445, 0.331, 0.457, 0.0, 0.26, 0.365
    };
    const double specificHeats[SorbantProperties::CUSTOM] = {
            870.0, 870.0, 650.0, 750.0, 800.0, 1.0, 840.0, 840.0
    };
    const double enclosureVolume = 1.0;

    /// @test sorbant computeVolume and computeThermalCapacity.
    DefinedSorbantProperties sorbants;
    for (unsigned int i=0; i<SorbantProperties::CUSTOM; ++i) {
        const SorbantProperties* sorbant = sorbants.getSorbant(static_cast<SorbantProperties::Type>(i));
        const double expectedV  = enclosureVolume * (1.0 - porosities[i]);
        const double expectedTc = specificHeats[i] * densities[i] * expectedV;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV,  sorbant->computeVolume(enclosureVolume),    DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTc, sorbant->computeThermalCapacity(expectedV), DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for properties of the defined sorbates.  We don't do an exhaustive test of exact
///           values for all sorbates in all sorbants here.  Rather, we do an easy > 0 check, and
///           we'll test for exact values for specific sorbates in further tests below.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSorbantProperties::testDefinedSorbatesProperties()
{
    UT_RESULT;

    const double partialPressure = 2.0;
    const double temperature     = 294.261;

    /// @test sorbate computeLoadingEquil, computeLoadingRate, computeHeatFlux.
    DefinedSorbantProperties sorbants;
    for (unsigned int i=0; i<SorbantProperties::CUSTOM; ++i) {
        const SorbantProperties* sorbant = sorbants.getSorbant(static_cast<SorbantProperties::Type>(i));
        const std::vector<SorbateProperties>* sorbates = sorbant->getSorbates();
        for (unsigned int j=0; j<sorbates->size(); ++j) {
            const double loadingEquil = sorbates->at(j).computeLoadingEquil(partialPressure, temperature);
            const double loadingRate  = sorbates->at(j).computeLoadingRate(loadingEquil, 0.0);
            const double heat         = sorbates->at(j).computeHeatFlux(loadingRate);
            CPPUNIT_ASSERT(loadingEquil > 0.0);
            CPPUNIT_ASSERT(loadingRate  > 0.0);
            /// - This assumes absorption is always exothermic.
            CPPUNIT_ASSERT(heat         > 0.0);
        }
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for custom sorbants and sorbates.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSorbantProperties::testCustomSorbant()
{
    UT_RESULT;

    /// - Define sorbant & sorbate properties values.
    const double density      = 1000.0;
    const double porosity     = 0.5;
    const double specificHeat = 500.0;
    const double h2oTothA     = 180.0;
    const double h2oTothB     = 3.0e-5;
    const double h2oTothE     = 1000.0;
    const double h2oTothT0    = -0.001;
    const double h2oTothC0    = 22.0;
    const double h2oDh        = -50.0;
    const double h2oKm        = 0.001;
    const double co2TothA     = 180.0;
    const double co2TothB     = 3.0e-5;
    const double co2TothE     = 1000.0;
    const double co2TothT0    = -0.001;
    const double co2TothC0    = 22.0;
    const double co2Dh        = -50.0;
    const double co2Km        = 0.001;
    const double enclosureVol = 1.0;

    /// - Set up blocking and offgasing interaction compounds.
    SorbateInteractingCompounds h2oBlockingCo2;
    h2oBlockingCo2.mCompound    = ChemicalCompound::H2O;
    h2oBlockingCo2.mInteraction = 1.0;
    std::vector<SorbateInteractingCompounds> blockingCompounds;
    blockingCompounds.push_back(h2oBlockingCo2);

    SorbateInteractingCompounds co2OffgasNh3;
    co2OffgasNh3.mCompound    = ChemicalCompound::NH3;
    co2OffgasNh3.mInteraction = 1.0e-6;
    std::vector<SorbateInteractingCompounds> offgasCompounds;
    offgasCompounds.push_back(co2OffgasNh3);

    /// - Create the custom sorbant and add custom sorbates.
    SorbantProperties sorbant(SorbantProperties::CUSTOM, density, porosity, specificHeat);
    sorbant.addSorbate(ChemicalCompound::H2O, 0, 0,
            h2oTothA, h2oTothB, h2oTothE, h2oTothT0, h2oTothC0, h2oDh, h2oKm);
    sorbant.addSorbate(ChemicalCompound::CO2, &blockingCompounds, &offgasCompounds,
            h2oTothA, h2oTothB, h2oTothE, h2oTothT0, h2oTothC0, h2oDh, h2oKm);

    /// @test sorbant computeVolume and computeThermalCapacity.
    const double expectedV  = enclosureVol * (1.0 - porosity);
    const double expectedTc = specificHeat * density * expectedV;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV,  sorbant.computeVolume(enclosureVol),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTc, sorbant.computeThermalCapacity(expectedV), DBL_EPSILON);

    const double partialPressure = 2.0;
    const double temperature     = 294.261;
    const double expectedH2oLoadingEquil = computeExpectedLoadingEquil(h2oTothA, h2oTothB,
            h2oTothE, h2oTothT0, h2oTothC0, partialPressure,temperature);
    const double expectedH2oLoadingRate = h2oKm * (expectedH2oLoadingEquil - 0.0);
    const double expectedH2oHeat = -expectedH2oLoadingRate * h2oDh * 1.0e6;

    const double expectedCo2LoadingEquil = computeExpectedLoadingEquil(co2TothA, co2TothB,
            co2TothE, co2TothT0, co2TothC0, partialPressure,temperature);
    const double expectedCo2LoadingRate = co2Km * (expectedCo2LoadingEquil - 0.0);
    const double expectedCo2Heat = -expectedCo2LoadingRate * co2Dh * 1.0e6;

    /// @test sorbate computeLoadingEquil, computeLoadingRate, computeHeatFlux.
    const std::vector<SorbateProperties>* sorbates = sorbant.getSorbates();
    const double h2oLoadingEquil = sorbates->at(0).computeLoadingEquil(partialPressure, temperature);
    const double h2oLoadingRate  = sorbates->at(0).computeLoadingRate(h2oLoadingEquil, 0.0);
    const double h2oHeat         = sorbates->at(0).computeHeatFlux(h2oLoadingRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oLoadingEquil, h2oLoadingEquil, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oLoadingRate,  h2oLoadingRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedH2oHeat,         h2oHeat,         DBL_EPSILON);

    const double co2LoadingEquil = sorbates->at(1).computeLoadingEquil(partialPressure, temperature);
    const double co2LoadingRate  = sorbates->at(1).computeLoadingRate(co2LoadingEquil, 0.0);
    const double co2Heat         = sorbates->at(1).computeHeatFlux(co2LoadingRate);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2LoadingEquil, co2LoadingEquil, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2LoadingRate,  co2LoadingRate,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCo2Heat,         co2Heat,         DBL_EPSILON);

    /// @test sorbate blocking and offgas compounds.
    CPPUNIT_ASSERT(0                     == sorbates->at(0).getBlockingCompounds()->size());
    CPPUNIT_ASSERT(0                     == sorbates->at(0).getOffgasCompounds()->size());
    CPPUNIT_ASSERT(1                     == sorbates->at(1).getBlockingCompounds()->size());
    CPPUNIT_ASSERT(1                     == sorbates->at(1).getOffgasCompounds()->size());
    CPPUNIT_ASSERT(ChemicalCompound::H2O == sorbates->at(1).getBlockingCompounds()->at(0).mCompound);
    CPPUNIT_ASSERT(1.0                   == sorbates->at(1).getBlockingCompounds()->at(0).mInteraction);
    CPPUNIT_ASSERT(ChemicalCompound::NH3 == sorbates->at(1).getOffgasCompounds()->at(0).mCompound);
    CPPUNIT_ASSERT(1.0e-6                == sorbates->at(1).getOffgasCompounds()->at(0).mInteraction);

    /// @test sorbant copy constructor.
    SorbantProperties* sorbant2 = new SorbantProperties(sorbant);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV,  sorbant2->computeVolume(enclosureVol),       DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedTc, sorbant2->computeThermalCapacity(expectedV), DBL_EPSILON);

    /// @test sorbant assignment operator for code coverage.
    SorbantProperties sorbant3(SorbantProperties::CUSTOM, 1.0, 1.0, 1.0);
    sorbant3 = *sorbant2;
    delete sorbant2;

    /// @test sorbate assignment operator for code coverage.
    DefinedChemicalCompounds definedChemicalCompounds;
    const ChemicalCompound* compound = definedChemicalCompounds.getCompound(ChemicalCompound::H2O);
    SorbateProperties* sorbate = new SorbateProperties(compound, 0, 0, 1.0, 1.0, 1.0, 1.0, 1.0, -50.0, 1.0);
    *sorbate = sorbates->at(0);
    delete sorbate;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for exceptions thrown during sorbant and sorbate construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSorbantProperties::testConstructionExceptions()
{
    UT_RESULT;

    /// @test exception from sorbant density = 0.
    CPPUNIT_ASSERT_THROW(SorbantProperties(SorbantProperties::CUSTOM, 0.0, 1.0, 1.0), TsInitializationException);

    /// @test exception from sorbant porosity < 0 and > 1.
    CPPUNIT_ASSERT_THROW(SorbantProperties(SorbantProperties::CUSTOM, 1.0, -0.01, 1.0), TsInitializationException);
    CPPUNIT_ASSERT_THROW(SorbantProperties(SorbantProperties::CUSTOM, 1.0, 1.001, 1.0), TsInitializationException);

    /// @test exception from sorbant specific heat = 0.
    CPPUNIT_ASSERT_THROW(SorbantProperties(SorbantProperties::CUSTOM, 1.0, 1.0, 0.0), TsInitializationException);

    /// - Create a good sorbant.
    SorbantProperties sorbant(SorbantProperties::CUSTOM, 1.0, 1.0, 1.0);

    /// @test exception from sorbate Km = 0.
    CPPUNIT_ASSERT_THROW(sorbant.addSorbate(ChemicalCompound::H2O, 0, 0, 1.767e+2, 2.787e-5, 1.093e+3, -1.190e-3, 2.213e+1, -50.2, 0.0), TsInitializationException);

    /// @test exception from blocking compound = sorbate compound.
    SorbateInteractingCompounds blocking;
    blocking.mCompound = ChemicalCompound::H2O;
    blocking.mInteraction = 1.0;
    std::vector<SorbateInteractingCompounds> blockingCompounds;
    blockingCompounds.push_back(blocking);
    CPPUNIT_ASSERT_THROW(sorbant.addSorbate(ChemicalCompound::H2O, &blockingCompounds, 0, 1.767e+2, 2.787e-5, 1.093e+3, -1.190e-3, 2.213e+1, -50.2, 1.0), TsInitializationException);

    /// @test exception from blocking interaction < 0 and > 1.
    blockingCompounds.at(0).mCompound    = ChemicalCompound::CO2;
    blockingCompounds.at(0).mInteraction = -0.001;
    CPPUNIT_ASSERT_THROW(sorbant.addSorbate(ChemicalCompound::H2O, &blockingCompounds, 0, 1.767e+2, 2.787e-5, 1.093e+3, -1.190e-3, 2.213e+1, -50.2, 1.0), TsInitializationException);
    blockingCompounds.at(0).mInteraction = 1.001;
    CPPUNIT_ASSERT_THROW(sorbant.addSorbate(ChemicalCompound::H2O, &blockingCompounds, 0, 1.767e+2, 2.787e-5, 1.093e+3, -1.190e-3, 2.213e+1, -50.2, 1.0), TsInitializationException);

    /// @test exception from offgas compound = sorbate compound.
    SorbateInteractingCompounds offgas;
    offgas.mCompound = ChemicalCompound::H2O;
    offgas.mInteraction = 1.0;
    std::vector<SorbateInteractingCompounds> offgasCompounds;
    offgasCompounds.push_back(offgas);
    CPPUNIT_ASSERT_THROW(sorbant.addSorbate(ChemicalCompound::H2O, 0, &offgasCompounds, 1.767e+2, 2.787e-5, 1.093e+3, -1.190e-3, 2.213e+1, -50.2, 1.0), TsInitializationException);

    /// @test exception from blocking interaction < 0.
    offgasCompounds.at(0).mCompound    = ChemicalCompound::CO2;
    offgasCompounds.at(0).mInteraction = -0.001;
    CPPUNIT_ASSERT_THROW(sorbant.addSorbate(ChemicalCompound::H2O, 0, &offgasCompounds, 1.767e+2, 2.787e-5, 1.093e+3, -1.190e-3, 2.213e+1, -50.2, 1.0), TsInitializationException);

    UT_PASS_LAST;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] tothA (kg*mol/kg/kPa) Sorbate Toth isotherm parameter a0 in this sorbant.
/// @param[in] tothB (1/kPa)         Sorbate Toth isotherm parameter b0 in this sorbant.
/// @param[in] tothE (K)             Sorbate Toth isotherm parameter E in this sorbant.
/// @param[in] tothT (--)            Sorbate Toth isotherm parameter t0 in this sorbant.
/// @param[in] tothC (K)             Sorbate Toth isotherm parameter c0 in this sorbant.
/// @param[in] pp    (kPa)           Partial pressure of the sorbate in the freestream.
/// @param[in] T     (K)             Temperature of the freestream.
///
/// @returns  double (kg*mol/m3) Toth equilibrium loading.
///
/// @details  Compute the Toth equilibrium loading for test.  Assumes all arguments are in valid ranges.
////////////////////////////////////////////////////////////////////////////////////////////////////
double UtSorbantProperties::computeExpectedLoadingEquil(const double tothA, const double tothB,
        const double tothE, const double tothT, const double tothC, const double pp, const double T) const
{
    const double EoverT = MsMath::limitRange(0.0, tothE / T, 100.0);
    const double expT   = exp(EoverT);
    const double a      = tothA * expT;
    const double b      = tothB * expT;
          double tT     = MsMath::limitRange(-100.0, tothT + tothC / T, 100.0);
                 tT     = MsMath::innerLimit(-0.1, tT, 0.1);
    const double denom  = powf(1.0 + powf(b * pp, tT), 1.0 / tT);
    return(a * pp / std::max(denom, DBL_EPSILON));
}
