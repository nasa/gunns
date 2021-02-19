/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsFluidShadow.o)
    )
***************************************************************************************************/
#include "UtGunnsFluidShadow.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidShadow class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidShadow::UtGunnsFluidShadow()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tMaxConductivity(),
    tExpansionScaleFactor(),
    tControlTarget(),
    tInputMixtureType(),
    tOutputMixtureType(),
    tBlockage(),
    tSourcePressure(),
    tConvertToType(),
    tNodes(),
    tNodeList(),
    tLinks(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tFluidProperties(),
    tLocalConfig(),
    tExternalConfig(),
    tFluidInput1(),
    tFluidInput2()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidShadow class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidShadow::~UtGunnsFluidShadow()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tExternalConfig;
    delete tFluidInput2;
    delete tFluidInput1;
    delete tLocalConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::setUp()
{
    tLinkName              = "Test Fluid Shadow";
    tNodeList.mNumNodes    = 3;
    tNodeList.mNodes       = tNodes;
    tMaxConductivity       = 1.0E10;
    tExpansionScaleFactor  = 0.0;
    tControlTarget         = 1;
    tInputMixtureType      = GunnsFluidUtils::PARTIAL_PRESSURE;
    tOutputMixtureType     = GunnsFluidUtils::PARTIAL_PRESSURE;
    tBlockage              = 0.0;
    tSourcePressure        = 600.0;
    tConvertToType         = FluidProperties::GUNNS_N2;
    tPort0                 = 2;
    tPort1                 = 0;
    tTimeStep              = 1.0;

    /// - Setup some fluid nodes
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2] = {FluidProperties::GUNNS_N2, FluidProperties::GUNNS_O2};
    tLocalConfig = new PolyFluidConfigData(tFluidProperties, types, 2);
    double fractions[2] = {0.5, 0.5};
    tFluidInput1 = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          fractions);               //massFractions

    FluidProperties::FluidType extTypes[3] = {FluidProperties::GUNNS_O2,
                                              FluidProperties::GUNNS_N2,
                                              FluidProperties::GUNNS_CO2};
    tExternalConfig = new PolyFluidConfigData(tFluidProperties, extTypes, 3);
    double extFractions[3] = {0.2, 0.79, 0.01};
    tFluidInput2 = new PolyFluidInputData(300.0,                    //temperature
                                          689.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          extFractions);            //massFractions

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("UtTestNode0", tLocalConfig);
    tNodes[1].initialize("UtTestNode1", tLocalConfig);
    tNodes[2].initialize("UtTestNode2", tLocalConfig);

    tNodes[0].getContent()->initialize(*tLocalConfig,    *tFluidInput1);
    tNodes[1].getContent()->initialize(*tExternalConfig, *tFluidInput2);
    tNodes[2].getContent()->initialize(*tLocalConfig,    *tFluidInput1);

    tNodes[0].setPotential(tFluidInput1->mPressure);
    tNodes[1].setPotential(tFluidInput2->mPressure);
    tNodes[2].setPotential(0.0);

    /// - Define nominal configuration data
    tConfigData = new GunnsFluidShadowConfigData(tLinkName, &tNodeList, tMaxConductivity,
            tExpansionScaleFactor, tControlTarget, tInputMixtureType, tOutputMixtureType,
            tExternalConfig, tConvertToType);

    tInputData = new GunnsFluidShadowInputData(false, tBlockage, tSourcePressure);

    tArticle = new FriendlyGunnsFluidShadow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidShadow 01: testConfig ..................................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName              == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes                 == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity       == tConfigData->mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor  == tConfigData->mExpansionScaleFactor);
    CPPUNIT_ASSERT(tControlTarget         == tConfigData->mControlTarget);
    CPPUNIT_ASSERT(tInputMixtureType      == tConfigData->mInputMixtureType);
    CPPUNIT_ASSERT(tOutputMixtureType     == tConfigData->mOutputMixtureType);
    CPPUNIT_ASSERT(tExternalConfig        == tConfigData->mExternalConfig);
    CPPUNIT_ASSERT(tConvertToType         == tConfigData->mConvertToType);

    /// - Check default config construction
    GunnsFluidShadowConfigData defaultConfig;
    CPPUNIT_ASSERT(""                             == defaultConfig.mName);
    CPPUNIT_ASSERT(0                              == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mMaxConductivity);
    CPPUNIT_ASSERT(0.0                            == defaultConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(0                              == defaultConfig.mControlTarget);
    CPPUNIT_ASSERT(GunnsFluidUtils::MASS_FRACTION == defaultConfig.mInputMixtureType);
    CPPUNIT_ASSERT(GunnsFluidUtils::MASS_FRACTION == defaultConfig.mOutputMixtureType);
    CPPUNIT_ASSERT(0                              == defaultConfig.mExternalConfig);
    CPPUNIT_ASSERT(FluidProperties::NO_FLUID      == defaultConfig.mConvertToType);

    /// - Check copy config construction
    GunnsFluidShadowConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName              == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes                 == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(tMaxConductivity       == copyConfig.mMaxConductivity);
    CPPUNIT_ASSERT(tExpansionScaleFactor  == copyConfig.mExpansionScaleFactor);
    CPPUNIT_ASSERT(tControlTarget         == copyConfig.mControlTarget);
    CPPUNIT_ASSERT(tInputMixtureType      == copyConfig.mInputMixtureType);
    CPPUNIT_ASSERT(tOutputMixtureType     == copyConfig.mOutputMixtureType);
    CPPUNIT_ASSERT(tExternalConfig        == copyConfig.mExternalConfig);
    CPPUNIT_ASSERT(tConvertToType         == copyConfig.mConvertToType);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::testInput()
{
    std::cout << "\n UtGunnsFluidShadow 02: testInput ...................................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(!tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tBlockage       == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tSourcePressure == tInputData->mSourcePressure);

    /// - Check default config construction
    GunnsFluidShadowInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0             == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0             == defaultInput.mSourcePressure);

    /// - Check copy config construction
    GunnsFluidShadowInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag  == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tSourcePressure == copyInput.mSourcePressure);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidShadow 03: testDefaultConstruction .....................";

    /// @test config data
    CPPUNIT_ASSERT(0.0 == tArticle->mSourcePressure);
    CPPUNIT_ASSERT(0   == tArticle->mTransformMap);
    CPPUNIT_ASSERT(0   == tArticle->mShadowActiveControl);
    CPPUNIT_ASSERT(!tArticle->mShadowIsActive);
    CPPUNIT_ASSERT(0   == tArticle->mControlTarget);
    CPPUNIT_ASSERT(!tArticle->mFirstPassFlag);
    CPPUNIT_ASSERT(0   == tArticle->mExternalConfig);
    CPPUNIT_ASSERT(GunnsFluidUtils::MASS_FRACTION == tArticle->mInputMixtureType);
    CPPUNIT_ASSERT(GunnsFluidUtils::MASS_FRACTION == tArticle->mOutputMixtureType);
    CPPUNIT_ASSERT(0.0 == tArticle->mInputPressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mInputTemperature);
    CPPUNIT_ASSERT(0   == tArticle->mInputMixture);
    CPPUNIT_ASSERT(0.0 == tArticle->mOutputPressure);
    CPPUNIT_ASSERT(0.0 == tArticle->mOutputTemperature);
    CPPUNIT_ASSERT(0   == tArticle->mOutputMixture);

    /// @test init flag
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    /// @test new/delete for code coverage
    GunnsFluidShadow* article = new GunnsFluidShadow();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidShadow 04: testNominalInitialization ...................";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidShadow article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// @test config and input data
    CPPUNIT_ASSERT(tBlockage              == article.mMalfBlockageValue);
    CPPUNIT_ASSERT(tSourcePressure        == article.mSourcePressure);
    CPPUNIT_ASSERT(tControlTarget         == article.mControlTarget);
    CPPUNIT_ASSERT(tExternalConfig        == article.mExternalConfig);
    CPPUNIT_ASSERT(tInputMixtureType      == article.mInputMixtureType);
    CPPUNIT_ASSERT(tOutputMixtureType     == article.mOutputMixtureType);

    /// - Test transform map.
    CPPUNIT_ASSERT_EQUAL(3, article.mTransformMap[0]);
    CPPUNIT_ASSERT_EQUAL(1, article.mTransformMap[1]);
    CPPUNIT_ASSERT_EQUAL(0, article.mTransformMap[2]);
    CPPUNIT_ASSERT_EQUAL(0, article.mTransformMap[3]);

    /// - Test read & write data mass fraction arrays.
    CPPUNIT_ASSERT(article.mInputMixture);
    CPPUNIT_ASSERT(article.mOutputMixture);
    CPPUNIT_ASSERT(1.0 == article.mInputMixture[0]);
    CPPUNIT_ASSERT(0.0 == article.mInputMixture[1]);
    CPPUNIT_ASSERT(0.0 == article.mInputMixture[2]);
    CPPUNIT_ASSERT(1.0 == article.mOutputMixture[0]);
    CPPUNIT_ASSERT(0.0 == article.mOutputMixture[1]);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    /// - Verify a double init works
    article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test initialization exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidShadow 05: testInitializationExceptions ................";

    /// - Check port rules - initialize with port 1 set to the ground node and verify the link fails
    ///   to initialize
    tPort0 = 1;
    tPort1 = 2;
    FriendlyGunnsFluidShadow article;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article.mInitFlag);

    /// - Initialize with port 0 not set to the ground node and verify the link fails to initialize.
    tPort0 = 0;
    tPort1 = 1;
    FriendlyGunnsFluidShadow article2;
    CPPUNIT_ASSERT_THROW(article2.initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1),
                         TsInitializationException);
    CPPUNIT_ASSERT(!article2.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests control of when shadowing is active based on various control combinations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::testShadowModing()
{
    std::cout << "\n UtGunnsFluidShadow 06: testShadowModing ............................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Verify first-pass response.
    tArticle->processInputs();
    CPPUNIT_ASSERT(!tArticle->mShadowIsActive);
    CPPUNIT_ASSERT( tArticle->mFirstPassFlag);

    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);

    /// - Verify shadowing inactive.
    tArticle->mShadowActiveControl = 0;

    tArticle->processInputs();
    CPPUNIT_ASSERT(!tArticle->mShadowIsActive);

    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);

    /// - Verify shadowing active.
    tArticle->mShadowActiveControl = 1;

    tArticle->processInputs();
    CPPUNIT_ASSERT(tArticle->mShadowIsActive);

    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(tMaxConductivity == tArticle->mEffectiveConductivity);

    /// - Verify shadowing inactive.
    tArticle->mControlTarget       = 0;
    tArticle->mShadowActiveControl = 1;

    tArticle->processInputs();
    CPPUNIT_ASSERT(!tArticle->mShadowIsActive);

    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(0.0 == tArticle->mEffectiveConductivity);

    /// - Verify shadowing active.
    tArticle->mShadowActiveControl = 0;

    tArticle->processInputs();
    CPPUNIT_ASSERT(tArticle->mShadowIsActive);

    tArticle->step(tTimeStep);
    CPPUNIT_ASSERT(tMaxConductivity == tArticle->mEffectiveConductivity);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the processInputs method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::testProcessInputs()
{
    std::cout << "\n UtGunnsFluidShadow 07: testProcessInputs ...........................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Set the read data as if the sim bus set it, and verify the read method processes the data.
    ///   This will test the mass fractions input format.
    tArticle->mInputMixtureType = GunnsFluidUtils::MASS_FRACTION;
    tArticle->mInputPressure    = 600.0;
    tArticle->mInputTemperature = 290.0;
    tArticle->mInputMixture[0]  = 0.4;  // External O2
    tArticle->mInputMixture[1]  = 0.5;  // External N2
    tArticle->mInputMixture[2]  = 0.1;  // External CO2

    tArticle->mFirstPassFlag       = true;
    tArticle->mShadowActiveControl = 1;
    tArticle->processInputs();

    /// - The CO2 is converted to N2, so the mass fraction of N2 should be 0.5 + 0.1 = 0.6.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(600.0, tArticle->mSourcePressure, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, tNodes[0].getContent()->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.4,
            tNodes[0].getContent()->getMassFraction(FluidProperties::GUNNS_O2), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.6,
            tNodes[0].getContent()->getMassFraction(FluidProperties::GUNNS_N2), 0.0);

    /// - Test the mole fractions input format.
    tArticle->mInputMixtureType = GunnsFluidUtils::MOLE_FRACTION;
    tArticle->mInputMixture[0]  = 0.4;  // External O2
    tArticle->mInputMixture[1]  = 0.5;  // External N2
    tArticle->mInputMixture[2]  = 0.1;  // External CO2

    double temp[3];
    temp[0] = tArticle->mInputMixture[0] * 31.9988;         // molecular weight of O2
    temp[1] = tArticle->mInputMixture[1] * 28.0134;         // molecular weight of N2
    temp[2] = tArticle->mInputMixture[2] * 44.0095;         // molecular weight of CO2
    double sum = temp[0] + temp[1] + temp[2];

    const double massFractionO2  = temp[0] / sum;
    const double massFractionN2  = temp[1] / sum;
    const double massFractionCO2 = temp[2] / sum;

    tArticle->processInputs();

    /// - The CO2 is converted to N2.
    temp[0] = massFractionO2 / 31.9988;
    temp[1] = (massFractionN2 + massFractionCO2) / 28.0134;
    sum = temp[0] + temp[1];
    const double moleFractionO2  = temp[0] / sum;
    const double moleFractionN2  = temp[1] / sum;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(600.0, tArticle->mSourcePressure, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, tNodes[0].getContent()->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(moleFractionO2,
            tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_O2), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(moleFractionN2,
            tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_N2), 0.0);

    /// - Test the partial pressures input format.  This test makes use of the same mole fractions
    ///   as the test above.
    tArticle->mInputMixtureType = GunnsFluidUtils::PARTIAL_PRESSURE;
    tArticle->mInputMixture[0]  = 0.4 * tArticle->mInputPressure;  // External O2
    tArticle->mInputMixture[1]  = 0.5 * tArticle->mInputPressure;  // External N2
    tArticle->mInputMixture[2]  = 0.1 * tArticle->mInputPressure;  // External CO2

    tArticle->processInputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(600.0, tArticle->mSourcePressure, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(290.0, tNodes[0].getContent()->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(moleFractionO2,
            tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_O2), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(moleFractionN2,
            tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_N2), 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the processOutputs method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::testProcessOutputs()
{
    std::cout << "\n UtGunnsFluidShadow 08: testProcessOutputs ..........................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    /// - Test the mass fractions output format.
    tArticle->mOutputMixtureType = GunnsFluidUtils::MASS_FRACTION;
    tArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getPressure(),
                                 tArticle->mOutputPressure, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),
                                 tArticle->mOutputTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMassFraction(FluidProperties::GUNNS_N2),
                                 tArticle->mOutputMixture[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMassFraction(FluidProperties::GUNNS_O2),
                                 tArticle->mOutputMixture[1], 0.0);

    /// - Test the mole fractions output format.
    tArticle->mOutputMixtureType = GunnsFluidUtils::MOLE_FRACTION;
    tArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getPressure(),
                                 tArticle->mOutputPressure, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),
                                 tArticle->mOutputTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_N2),
                                 tArticle->mOutputMixture[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_O2),
                                 tArticle->mOutputMixture[1], 0.0);

    /// - Test the partial pressure output format.
    tArticle->mOutputMixtureType = GunnsFluidUtils::PARTIAL_PRESSURE;
    tArticle->processOutputs();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getPressure(),
                                 tArticle->mOutputPressure, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getTemperature(),
                                 tArticle->mOutputTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_N2)
                                 * tArticle->mOutputPressure, tArticle->mOutputMixture[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNodes[0].getContent()->getMoleFraction(FluidProperties::GUNNS_O2)
                                 * tArticle->mOutputPressure, tArticle->mOutputMixture[1], 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the setter and getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidShadow::testAccessors()
{
    std::cout << "\n UtGunnsFluidShadow 09: testAccessors ...............................";

    /// - Initialize default test article with nominal initialization data
    tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort0, tPort1);

    const double pressure    = 100.0;
    const double temperature = 270.0;
    const double mixture[3]  = {0.5, 0.0, 0.5};
    double*      mixtureNull = 0;

    /// - Test the setInputs method throws an exception if no mixtures array supplied.
    CPPUNIT_ASSERT_THROW(tArticle->setInputs(pressure, temperature, mixtureNull),
                         TsOutOfBoundsException);

    /// - Test the setInputs method with nominal values.
    tArticle->setInputs(pressure, temperature, mixture);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pressure,    tArticle->mInputPressure,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature, tArticle->mInputTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mixture[0],  tArticle->mInputMixture[0],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mixture[1],  tArticle->mInputMixture[1],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mixture[2],  tArticle->mInputMixture[2],  0.0);

    double pressureOut    = 0.0;
    double temperatureOut = 0.0;
    double mixtureOut[2]  = {0.0, 0.0};

    /// - Test the getOutputs method throws an exception if no mixtures array supplied.
    CPPUNIT_ASSERT_THROW(tArticle->getOutputs(pressureOut, temperatureOut, mixtureNull),
                         TsOutOfBoundsException);

    /// - Test the getOutputs method returns nominal values.
    tArticle->processOutputs();
    tArticle->getOutputs(pressureOut, temperatureOut, mixtureOut);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mOutputPressure,    pressureOut,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mOutputTemperature, temperatureOut, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mOutputMixture[0],  mixtureOut[0],  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mOutputMixture[1],  mixtureOut[1],  0.0);

    std::cout << "... Pass";
}
