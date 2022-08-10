/*
@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((properties/ChemicalCompound.o)
     (aspects/fluid/fluid/GunnsFluidTraceCompounds.o))
*/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"
#include "UtGunnsFluidTraceCompounds.hh"

/// @details  Test identification number.
int UtGunnsFluidTraceCompounds::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid GunnsFluidTraceCompounds model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidTraceCompounds::UtGunnsFluidTraceCompounds()
    :
    CppUnit::TestFixture(),
    tName(""),
    tMole(0.0),
    tType(),
    tMoleFraction(),
    tProperties(0),
    tConfigData(0),
    tInputData(0),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid GunnsFluidTraceCompounds model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidTraceCompounds::~UtGunnsFluidTraceCompounds()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::setUp()
{
    /// - Define common initialization data
    tName       = "tArticle";
    tMole       = 42.0;

    /// - Define chemical compounds properties
    tProperties = new DefinedChemicalCompounds();

    /// - Define initialization data for multi-compound test article
    tType[0]    = ChemicalCompound::CH2O;  tMoleFraction[0] = 1.0e-1;
    tType[1]    = ChemicalCompound::C2H6O; tMoleFraction[1] = 2.0e-2;
    tType[2]    = ChemicalCompound::C4H4O; tMoleFraction[2] = 3.0e-3;
    tType[3]    = ChemicalCompound::C8H10; tMoleFraction[3] = 4.0e-4;
    tType[4]    = ChemicalCompound::H3PO4; tMoleFraction[4] = 5.0e-5;
    tType[5]    = ChemicalCompound::O2;    tMoleFraction[5] = 6.0e-6;
                                           tMoleFraction[6] = 7.0e-7;
    tConfigData = new GunnsFluidTraceCompoundsConfigData(tType,
                                                         UtGunnsFluidTraceCompounds::NMULTI - 1,
                                                         "tConfigData");
    tConfigData->addCompound(28.0101, "CO", FluidProperties::GUNNS_CO);
    tInputData  = new GunnsFluidTraceCompoundsInputData(tMoleFraction);

    /// - Define the test article
    tArticle    = new FriendlyGunnsFluidTraceCompounds(tMole);

    /// - Increment the test identification number.
    ++TEST_ID;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::tearDown()
{
    /// - Delete the objects created in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tProperties;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config & input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testConfigAndInput()
{
    UT_RESULT_FIRST;

    {
        /// @test exception for default (unnamed) config construction.
        CPPUNIT_ASSERT_THROW(GunnsFluidTraceCompoundsConfigData defaultConfig,
                             TsInitializationException);
    } {
        /// @test exception for providing a types array but nTypes < 1.
        CPPUNIT_ASSERT_THROW(GunnsFluidTraceCompoundsConfigData defaultConfig(tType, 0, "defaultConfig"),
                             TsInitializationException);
    } {
        /// @test exception for specifying non-zero # of types but not providing a types array.
        CPPUNIT_ASSERT_THROW(GunnsFluidTraceCompoundsConfigData defaultConfig(0, 1, "defaultConfig"),
                             TsInitializationException);
    } {
        /// @test exception for giving a ChemicalCompound::Type < 1.
        ChemicalCompound::Type types[2];
        types[0] = static_cast<ChemicalCompound::Type>(-1);
        CPPUNIT_ASSERT_THROW(GunnsFluidTraceCompoundsConfigData defaultConfig(types, 2, "defaultConfig"),
                             TsInitializationException);
    } {
        /// @test exception for giving a ChemicalCompound::Type >= NO_COMPOUND.
        ChemicalCompound::Type types[2];
        types[0] = ChemicalCompound::O2;
        types[1] = static_cast<ChemicalCompound::Type>(-1);
        CPPUNIT_ASSERT_THROW(GunnsFluidTraceCompoundsConfigData defaultConfig(types, 2, "defaultConfig"),
                             TsInitializationException);
    }

    /// @test for multi-compound input construction
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        CPPUNIT_ASSERT(tMoleFraction[i] == tInputData->mState[i]);
    }

    /// @test for default input construction
    GunnsFluidTraceCompoundsInputData defaultInput;
    CPPUNIT_ASSERT(0 == defaultInput.mState);

    /// @test for adding extra compounds
    GunnsFluidTraceCompoundsConfigData testConfig(tType, UtGunnsFluidTraceCompounds::NMULTI - 1, "testConfig");

    /// @test exception thrown for zero Molecular Weight
    CPPUNIT_ASSERT_THROW(testConfig.addCompound(0.0, "test"), TsInitializationException);

    /// @test exception thrown for blank name.
    CPPUNIT_ASSERT_THROW(testConfig.addCompound(2.0, ""), TsInitializationException);

    /// @test exception thrown for NO_COMPOUND.
    CPPUNIT_ASSERT_THROW(testConfig.addCompound(ChemicalCompound::NO_COMPOUND), TsInitializationException);

    testConfig.addCompound(1.0, "compound1");
    testConfig.addCompound(2.0, "compound2", FluidProperties::GUNNS_O2);
    testConfig.addCompound(ChemicalCompound::H2);
    testConfig.addCompound(3.0, "compound3", FluidProperties::NO_FLUID, ChemicalCompound::LIOH);
    CPPUNIT_ASSERT(1.0                           == testConfig.mCompounds[0 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mMWeight);
    CPPUNIT_ASSERT("compound1"                   == testConfig.mCompounds[0 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mName);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID     == testConfig.mCompounds[0 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mFluidType);
    CPPUNIT_ASSERT(ChemicalCompound::NO_COMPOUND == testConfig.mCompounds[0 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mType);

    CPPUNIT_ASSERT(2.0                           == testConfig.mCompounds[1 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mMWeight);
    CPPUNIT_ASSERT("compound2"                   == testConfig.mCompounds[1 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mName);
    CPPUNIT_ASSERT(FluidProperties::GUNNS_O2     == testConfig.mCompounds[1 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mFluidType);
    CPPUNIT_ASSERT(ChemicalCompound::NO_COMPOUND == testConfig.mCompounds[1 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mType);

    CPPUNIT_ASSERT(2.01588                       == testConfig.mCompounds[2 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mMWeight);
    CPPUNIT_ASSERT("H2"                          == testConfig.mCompounds[2 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mName);
    CPPUNIT_ASSERT(FluidProperties::GUNNS_H2     == testConfig.mCompounds[2 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mFluidType);
    CPPUNIT_ASSERT(ChemicalCompound::H2          == testConfig.mCompounds[2 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mType);

    CPPUNIT_ASSERT(3.0                           == testConfig.mCompounds[3 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mMWeight);
    CPPUNIT_ASSERT("compound3"                   == testConfig.mCompounds[3 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mName);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID     == testConfig.mCompounds[3 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mFluidType);
    CPPUNIT_ASSERT(ChemicalCompound::LIOH        == testConfig.mCompounds[3 + UtGunnsFluidTraceCompounds::NMULTI - 1]->mType);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testDefaultConstruction()
{
    UT_RESULT;

    double tMole = 42.0;
    FriendlyGunnsFluidTraceCompounds defaultArticle(tMole);
    CPPUNIT_ASSERT(""    == defaultArticle.mName);
    CPPUNIT_ASSERT(0     == defaultArticle.mConfig);
    CPPUNIT_ASSERT(0     == defaultArticle.mMass);
    CPPUNIT_ASSERT(0     == defaultArticle.mMoleFraction);
    CPPUNIT_ASSERT(tMole == defaultArticle.mFluidMoles);
    CPPUNIT_ASSERT(false == defaultArticle.mInitFlag);

    /// @test the no-argument constructor
    FriendlyGunnsFluidTraceCompounds noArgArticle;
    CPPUNIT_ASSERT(""    == noArgArticle.mName);
    CPPUNIT_ASSERT(0     == noArgArticle.mConfig);
    CPPUNIT_ASSERT(0     == noArgArticle.mMass);
    CPPUNIT_ASSERT(0     == noArgArticle.mMoleFraction);
    CPPUNIT_ASSERT(0.0   == noArgArticle.mFluidMoles);
    CPPUNIT_ASSERT(false == noArgArticle.mInitFlag);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testNominalInitialization()
{
    UT_RESULT;

    /// @test for nominal initialization data
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));
    CPPUNIT_ASSERT(tName       == tArticle->mName);
    CPPUNIT_ASSERT(tConfigData == tArticle->mConfig);
    CPPUNIT_ASSERT(0           != tArticle->mMass);
    CPPUNIT_ASSERT(0           != tArticle->mMoleFraction);
    CPPUNIT_ASSERT(tMole       == tArticle->mFluidMoles);
    CPPUNIT_ASSERT(true        == tArticle->mInitFlag);

    /// @test initial masses and mole fractions
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI-1; i++) {
        const double moles = tMole * tMoleFraction[i];
        const double MW    = tProperties->getCompound(tType[i])->mMWeight;
        const double mass  = moles * MW;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mass,             tArticle->mMass[i],         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[i], tArticle->mMoleFraction[i], DBL_EPSILON);
    }
    const double moles = tMole * tMoleFraction[6];
    const double MW    = tProperties->getCompound(ChemicalCompound::CO)->mMWeight;
    const double mass  = moles * MW;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mass,             tArticle->mMass[6],         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[6], tArticle->mMoleFraction[6], DBL_EPSILON);

    /// @test for nominal initialization data with no input supplied
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, 0, tName));
    CPPUNIT_ASSERT(tName       == tArticle->mName);
    CPPUNIT_ASSERT(tConfigData == tArticle->mConfig);
    CPPUNIT_ASSERT(0           != tArticle->mMass);
    CPPUNIT_ASSERT(0           != tArticle->mMoleFraction);
    CPPUNIT_ASSERT(tMole       == tArticle->mFluidMoles);
    CPPUNIT_ASSERT(true        == tArticle->mInitFlag);

    /// @test initial masses and mole fractions
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMass[i],         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMoleFraction[i], DBL_EPSILON);
    }

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for copy construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testCopyConstruction()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    /// @test for nominal copy construction
    double      copyMole = 1.23;
    std::string copyName = "copyName";
    FriendlyGunnsFluidTraceCompounds* copyArticle = 0;
    CPPUNIT_ASSERT_NO_THROW(copyArticle = new FriendlyGunnsFluidTraceCompounds(*tArticle,
                                                                                copyMole,
                                                                                copyName));
    CPPUNIT_ASSERT(copyName        == copyArticle->mName);
    CPPUNIT_ASSERT(tConfigData     == copyArticle->mConfig);
    CPPUNIT_ASSERT(0               != copyArticle->mMass);
    CPPUNIT_ASSERT(tArticle->mMass != copyArticle->mMass);
    CPPUNIT_ASSERT(tArticle->mMass != copyArticle->mMoleFraction);
    CPPUNIT_ASSERT(copyMole        == copyArticle->mFluidMoles);
    CPPUNIT_ASSERT(true            == copyArticle->mInitFlag);

    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI - 1; i++) {
        const double moles = copyMole * tMoleFraction[i];
        const double MW    = tProperties->getCompound(tType[i])->mMWeight;
        const double mass  = moles * MW;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mass,             copyArticle->mMass[i],         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[i], copyArticle->mMoleFraction[i], DBL_EPSILON);
    }
    const double moles = copyMole * tMoleFraction[6];
    const double MW    = tProperties->getCompound(ChemicalCompound::CO)->mMWeight;
    const double mass  = moles * MW;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mass,             copyArticle->mMass[6],         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[6], copyArticle->mMoleFraction[6], DBL_EPSILON);

    delete copyArticle;
    copyArticle = 0;

    /// @test for throw on source not initialized
    FriendlyGunnsFluidTraceCompounds noInitArticle(tMole);
    CPPUNIT_ASSERT_THROW(new FriendlyGunnsFluidTraceCompounds(noInitArticle,
                                                              copyMole,
                                                              copyName),
                         TsInitializationException);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for initialize method exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testInitializationExceptions()
{
    UT_RESULT;

    {
        /// @test for throw on article constructed with default no-arg constructor.
        FriendlyGunnsFluidTraceCompounds article;
        CPPUNIT_ASSERT_THROW(article.initialize(tConfigData, tInputData, tName),
                             TsInitializationException);
    } {
        /// @test for throw on config data missing
        CPPUNIT_ASSERT_THROW(tArticle->initialize(0, tInputData, tName),
                             TsInitializationException);
    } {
        /// @test for throw on number of config types < 0
        CPPUNIT_ASSERT_THROW(GunnsFluidTraceCompoundsConfigData config(tType, -1, "config"),
                TsInitializationException);
    } {
        /// @test for throw on compound types not defined.
        CPPUNIT_ASSERT_THROW(GunnsFluidTraceCompoundsConfigData config(0, UtGunnsFluidTraceCompounds::NMULTI - 1, "config"),
                TsInitializationException);
    } {
        /// @test for throw on input data given with null mole fractions.
        GunnsFluidTraceCompoundsInputData input(0);
        CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfigData, &input, tName), TsInitializationException);
    } {
        /// @test for throw on an input mole fraction < 0.
        tInputData->mState[UtGunnsFluidTraceCompounds::NMULTI - 1] = -FLT_EPSILON;
        CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfigData, tInputData, tName),
                             TsInitializationException);
        tInputData->mState[UtGunnsFluidTraceCompounds::NMULTI - 1] =
                    tMoleFraction[UtGunnsFluidTraceCompounds::NMULTI - 1];
    } {
        /// @test for throw on a compound type listed twice in the config.
        const ChemicalCompound::Type savedType = tType[UtGunnsFluidTraceCompounds::NMULTI - 2];
        tType[UtGunnsFluidTraceCompounds::NMULTI - 2] = tType[0];
        GunnsFluidTraceCompoundsConfigData config(tType, UtGunnsFluidTraceCompounds::NMULTI-1, "config");
        CPPUNIT_ASSERT_THROW(tArticle->initialize(&config, tInputData, tName),
                             TsInitializationException);
        tType[UtGunnsFluidTraceCompounds::NMULTI - 2] = savedType;
    } {
        /// @test for throw on a compound name listed twice in the config.
        GunnsFluidTraceCompoundsConfigData config(tType, UtGunnsFluidTraceCompounds::NMULTI-1, "config");
        config.addCompound(2.0, config.mCompounds[0]->mName, FluidProperties::NO_FLUID);
        CPPUNIT_ASSERT_THROW(tArticle->initialize(&config, tInputData, tName),
                             TsInitializationException);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testRestart()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    /// @test the restart method updates masses from mole fractions relative to parent moles.
    tMole = 10.0;
    tArticle->restart();

    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI -1; i++) {
        const double moles = tMole * tMoleFraction[i];
        const double MW    = tProperties->getCompound(tType[i])->mMWeight;
        const double mass  = moles * MW;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mass,             tArticle->mMass[i],         1e-6);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[i], tArticle->mMoleFraction[i], 1e-6);
    }
    const double moles = tMole * tMoleFraction[6];
    const double MW    = tProperties->getCompound(ChemicalCompound::CO)->mMWeight;
    const double mass  = moles * MW;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mass,             tArticle->mMass[6],         1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[6], tArticle->mMoleFraction[6], 1e-6);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for getType method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testGetType()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    /// @test the getType method returns the correct chemical compound type
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI - 1; i++) {
        CPPUNIT_ASSERT(tType[i] == tArticle->getType(i));
    }
    CPPUNIT_ASSERT(ChemicalCompound::NO_COMPOUND == tArticle->getType(6));

    /// @test the getType method throws exception on bad indexes
    CPPUNIT_ASSERT_THROW(tArticle->getType(-1),                                 TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(tArticle->getType(UtGunnsFluidTraceCompounds::NMULTI), TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for find methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testFind()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    /// @test the find method returns the correct index for the given chemical compound type
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI - 1; i++) {
        CPPUNIT_ASSERT(i == tArticle->find(tType[i]));
    }
    CPPUNIT_ASSERT(6 == tArticle->find(ChemicalCompound::NO_COMPOUND, "CO"));

    /// @test the findCompound method returns the correct index for the given chemical compound type
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI - 1; i++) {
        CPPUNIT_ASSERT(i == tArticle->findCompound(tType[i]));
    }

    /// @test the findCompound method returns -1 for the a missing chemical compound type
    CPPUNIT_ASSERT(-1 == tArticle->findCompound(ChemicalCompound::NH3));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for getMass method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testGetMass()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    /// @test the getMass method returns the correct mass
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI -1; i++) {
        CPPUNIT_ASSERT(tArticle->mMass[i] == tArticle->getMass(tType[i]));
    }
    CPPUNIT_ASSERT(tArticle->mMass[6] == tArticle->getMass(ChemicalCompound::NO_COMPOUND, "CO"));

    /// @test the getMass method throws exception on bad types
    CPPUNIT_ASSERT_THROW(tArticle->getMass(ChemicalCompound::NO_COMPOUND, "NO_NAME"), TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for getMoleFraction method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testGetMoleFraction()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    /// @test the getMoleFraction method returns the correct mass
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI - 1; i++) {
        CPPUNIT_ASSERT(tArticle->mMoleFraction[i] == tArticle->getMoleFraction(tType[i]));
    }
    CPPUNIT_ASSERT(tArticle->mMoleFraction[6] == tArticle->getMoleFraction(ChemicalCompound::NO_COMPOUND, "CO"));

    /// @test the getMoleFraction method throws exception on bad types
    CPPUNIT_ASSERT_THROW(tArticle->getMoleFraction(ChemicalCompound::NO_COMPOUND, "NO_NAME"), TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for setMasses method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testSetMasses()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    double masses[UtGunnsFluidTraceCompounds::NMULTI];
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        masses[i] = i;
    }

    /// @test the setMasses method sets the article to the given masses and does not update the
    ///       mole fractions
    tArticle->setMasses(masses);
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        CPPUNIT_ASSERT(masses[i] == tArticle->mMass[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[i], tArticle->mMoleFraction[i], DBL_EPSILON);
    }

    /// @test the setMasses method zeroes the article masses when given no argument, and does not
    ///       update the mole fractions
    tArticle->setMasses();
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        CPPUNIT_ASSERT(0.0 == tArticle->mMass[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[i], tArticle->mMoleFraction[i], DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for setMass overloaded methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testSetMass()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    /// @test the setMass method sets the article mass for the given compound and does not update
    ///       the mole fractions
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI - 1; i++) {
        tArticle->setMass(tType[i], static_cast<double>(i));
        CPPUNIT_ASSERT(static_cast<double>(i) == tArticle->mMass[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[i], tArticle->mMoleFraction[i], DBL_EPSILON);
    }
    tArticle->setMass(ChemicalCompound::NO_COMPOUND, static_cast<double>(6), "CO");
    CPPUNIT_ASSERT(static_cast<double>(6) == tArticle->mMass[6]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[6], tArticle->mMoleFraction[6], DBL_EPSILON);

    /// @test the getMoleFraction method throws exception on bad types
    CPPUNIT_ASSERT_THROW(tArticle->setMass(ChemicalCompound::NO_COMPOUND, 1.0, "NO_NAME"), TsOutOfBoundsException);

    /// @test the setMass method sets the article mass for the given index and does not update the
    ///       mole fractions
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        tArticle->setMass(i, static_cast<double>(i));
        CPPUNIT_ASSERT(static_cast<double>(i) == tArticle->mMass[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[i], tArticle->mMoleFraction[i], DBL_EPSILON);
    }

    /// @test the getMoleFraction method throws exception on bad index
    CPPUNIT_ASSERT_THROW(tArticle->setMass(-1, 1.0),                                 TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(tArticle->setMass(UtGunnsFluidTraceCompounds::NMULTI, 1.0), TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for setMoleFraction overloaded methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testSetMoleFraction()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));
    double masses[UtGunnsFluidTraceCompounds::NMULTI];
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        masses[i] = tArticle->getMasses()[i];
    }

    /// @test the setMoleFraction method sets the article mole fraction for the given compound and
    ///       does not update the masses
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI - 1; i++) {
        tArticle->setMoleFraction(tType[i], static_cast<double>(i));
        CPPUNIT_ASSERT(static_cast<double>(i) == tArticle->mMoleFraction[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(masses[i], tArticle->mMass[i], DBL_EPSILON);
    }
    tArticle->setMoleFraction(ChemicalCompound::NO_COMPOUND, static_cast<double>(6), "CO");
    CPPUNIT_ASSERT(static_cast<double>(6) == tArticle->mMoleFraction[6]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(masses[6], tArticle->mMass[6], DBL_EPSILON);

    /// @test the setMoleFraction method throws exception on bad types
    CPPUNIT_ASSERT_THROW(tArticle->setMoleFraction(ChemicalCompound::NO_COMPOUND, 1.0, "NO_NAME"), TsOutOfBoundsException);

    /// @test the setMoleFraction method sets the article mole fraction for the given index and
    ///       does not update the masses
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        tArticle->setMoleFraction(i, static_cast<double>(i));
        CPPUNIT_ASSERT(static_cast<double>(i) == tArticle->mMoleFraction[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(masses[i], tArticle->mMass[i], DBL_EPSILON);
    }

    /// @test the getMoleFraction method throws exception on bad index
    CPPUNIT_ASSERT_THROW(tArticle->setMoleFraction(-1, 1.0),                                 TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(tArticle->setMoleFraction(UtGunnsFluidTraceCompounds::NMULTI, 1.0), TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for flowIn methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testFlowIn()
{
    UT_RESULT;

    // Set up a TC object to flow into the test article.
    double flowConc[UtGunnsFluidTraceCompounds::NMULTI] =
                                           {2.0e-2, 3.0e-3, 4.0e-4, 5.0e-5, 6.0e-6, 7.0e-7, 8.0e-8};
    const double flowMole = 3.0;
    GunnsFluidTraceCompoundsInputData input(flowConc);
    GunnsFluidTraceCompounds          flow(flowMole);
    CPPUNIT_ASSERT_NO_THROW(flow.initialize(tConfigData, &input, "flow"));
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    double oldMasses[UtGunnsFluidTraceCompounds::NMULTI];
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        oldMasses[i] = tArticle->getMasses()[i];
    }

    /// @test the mixing of incoming trace compounds
    tArticle->flowIn(flow, flowMole);

    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI - 1; i++) {
        const double MW   = tProperties->getCompound(tType[i])->mMWeight;
        const double mass = oldMasses[i] + flowMole * flowConc[i] * MW;
        const double conc = mass / tMole / MW;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mass, tArticle->mMass[i],         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(conc, tArticle->mMoleFraction[i], DBL_EPSILON);
    }
    const double MW   = tProperties->getCompound(ChemicalCompound::CO)->mMWeight;
    const double mass = oldMasses[6] + flowMole * flowConc[6] * MW;
    const double conc = mass / tMole / MW;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mass, tArticle->mMass[6],         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(conc, tArticle->mMoleFraction[6], DBL_EPSILON);

    /// @test very low concentrations are zeroed
    flowConc[6] = 0.0;
    GunnsFluidTraceCompoundsInputData input2(flowConc);
    GunnsFluidTraceCompounds          flow2(flowMole);
    CPPUNIT_ASSERT_NO_THROW(flow2.initialize(tConfigData, &input2, "flow2"));

    tArticle->mMass[6] = DBL_EPSILON * DBL_EPSILON;
    tArticle->flowIn(flow2, flowMole);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMass[6],         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMoleFraction[6], DBL_EPSILON);

    /// @test overloaded function that integrates mass flow rates, and negative resulting mass
    ///       is zeroed.
    const double rates[UtGunnsFluidTraceCompounds::NMULTI] =
             {-1.0, 2.0e-1, -3.0e-2, 4.0e-3, -5.0e-4, 6.0e-5, -7.0};
    const double dt = 0.1;
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        oldMasses[i] = tArticle->getMasses()[i];
    }
    tArticle->flowIn(rates, dt);
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI - 1; i++) {
        const double MW   = tProperties->getCompound(tType[i])->mMWeight;
        const double mass = oldMasses[i] + rates[i] * dt;
        const double conc = mass / tMole / MW;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mass, tArticle->mMass[i],         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(conc, tArticle->mMoleFraction[i], DBL_EPSILON);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMass[6],         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMoleFraction[6], DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for flowOut method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testFlowOut()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    double oldMasses[UtGunnsFluidTraceCompounds::NMULTI];
    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        oldMasses[i] = tArticle->getMasses()[i];
    }

    /// @test the reduction of mass of trace compounds due to flow out
    const double flowMole = tMole + 1.0;
    tArticle->flowOut(flowMole);

    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI - 1; i++) {
        const double MW   = tProperties->getCompound(tType[i])->mMWeight;
        const double mass = oldMasses[i] - flowMole * tMoleFraction[i] * MW;
        CPPUNIT_ASSERT_DOUBLES_EQUAL(mass,             tArticle->mMass[i],         1e-6);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[i], tArticle->mMoleFraction[i], 1e-6);
    }
    const double MW   = tProperties->getCompound(ChemicalCompound::CO)->mMWeight;
    const double mass = oldMasses[6] - flowMole * tMoleFraction[6] * MW;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mass,             tArticle->mMass[6],         1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tMoleFraction[6], tArticle->mMoleFraction[6], 1e-6);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for limitPositive method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testLimitPositive()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    /// @test negative masses and mole fractions are zeroed
    const double flowMole = tMole + 1.0;
    tArticle->flowOut(flowMole);
    tArticle->limitPositive();

    for (int i = 0; i < UtGunnsFluidTraceCompounds::NMULTI; i++) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMass[i],         DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMoleFraction[i], DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for state updater method exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidTraceCompounds::testAccessors()
{
    UT_RESULT;

    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tConfigData, tInputData, tName));

    /// @test simple getter methods
    CPPUNIT_ASSERT(tConfigData             == tArticle->getConfig());
    CPPUNIT_ASSERT(tArticle->mMass         == tArticle->getMasses());
    CPPUNIT_ASSERT(tArticle->mMoleFraction == tArticle->getMoleFractions());
    CPPUNIT_ASSERT(true                    == tArticle->isInitialized());

    UT_PASS_FINAL;
}
