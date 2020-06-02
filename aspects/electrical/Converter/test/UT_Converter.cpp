/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
 (aspects/electrical/Converter/Converter.o)
 )
 **************************************************************************************************/

#include "UT_Converter.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  UtConverter constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtConverter::UtConverter()
    :
    mConfigData(),
    mInputData(),
    mArticle(),
    mStandbyPower(0.0),
    mInputVoltage(0.0),
    mRegulatedVoltage(0.0),
    mEfficiency(0.0),
    mOutputOverCurrentLimit(0.0),
    mOutputOverVoltageLimit(0.0),
    mOutOverCurrentTripActive(false),
    mOutOverVoltageTripActive(false),
    mInputOverVoltageLimit(0.0),
    mInputUnderVoltageLimit(0.0),
    mInOverVoltageTripActive(false),
    mInUnderVoltageTripActive(false),
    mTolerance(0.0) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief UtConverter destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtConverter::~UtConverter() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Method to setup default data for unit tests
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::setUp() {
    mStandbyPower             = 105.0;
    mInputVoltage             = 160.0;
    mRegulatedVoltage         = 124.5;
    mEfficiency               =  0.90;
    mOutputOverCurrentLimit   = 190.0;
    mOutputOverVoltageLimit   = 151.0;
    mOutOverCurrentTripActive =  true;
    mOutOverVoltageTripActive =  true;
    mInputOverVoltageLimit    = 173.0;
    mInputUnderVoltageLimit   = 110.0;
    mInOverVoltageTripActive  =  true;
    mInUnderVoltageTripActive =  true;

    mConfigData       = new ConverterConfigData(mStandbyPower);
    mInputData        = new ConverterInputData(mInputVoltage,
                                              mRegulatedVoltage,
                                              mEfficiency,
                                              mOutputOverCurrentLimit,
                                              mOutputOverVoltageLimit,
                                              mOutOverCurrentTripActive,
                                              mOutOverVoltageTripActive,
                                              mInputOverVoltageLimit,
                                              mInputUnderVoltageLimit,
                                              mInOverVoltageTripActive,
                                              mInUnderVoltageTripActive);
    mArticle    = new FriendlyConverter;

    mTolerance = 1.0e-08;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Method envoked at the end of each unit test that allocates object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::tearDown() {
    delete mConfigData;
    delete mInputData;
    delete mArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test configuration data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testConfig() {
    std::cout << "\n -----------------------------------------------------------------";
    std::cout << "\n UtConverter 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(mStandbyPower == mConfigData->mStandbyPower);

    /// - Check default config construction
    ConverterConfigData  defaultConfig;
    CPPUNIT_ASSERT(0.0 == defaultConfig.mStandbyPower);

    /// - Check copy config construction
    ConverterConfigData  copyConfig(*mConfigData);
    CPPUNIT_ASSERT(mStandbyPower == copyConfig.mStandbyPower);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testInput() {
    std::cout << "\n UtConverter 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(mInputVoltage == mInputData->mInputVoltage);
    CPPUNIT_ASSERT(mRegulatedVoltage == mInputData->mRegulatedVoltage);
    CPPUNIT_ASSERT(mEfficiency == mInputData->mEfficiency);
    CPPUNIT_ASSERT(mOutputOverCurrentLimit == mInputData->mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(mOutputOverVoltageLimit == mInputData->mOutputOverVoltageLimit);
    CPPUNIT_ASSERT(mOutOverCurrentTripActive == mInputData->mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(mOutOverVoltageTripActive == mInputData->mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(mInputOverVoltageLimit == mInputData->mInputOverVoltageLimit);
    CPPUNIT_ASSERT(mInputUnderVoltageLimit == mInputData->mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(mInOverVoltageTripActive == mInputData->mInOverVoltageTripActive);
    CPPUNIT_ASSERT(mInUnderVoltageTripActive == mInputData->mInUnderVoltageTripActive);

    /// - Check default input construction
    ConverterInputData  defaultInput;
    CPPUNIT_ASSERT(0.0 == defaultInput.mInputVoltage);
    CPPUNIT_ASSERT(0.0 == defaultInput.mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0 == defaultInput.mEfficiency);
    CPPUNIT_ASSERT(0.0 == defaultInput.mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(0.0 == defaultInput.mOutputOverVoltageLimit);
    CPPUNIT_ASSERT(false == defaultInput.mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(false == defaultInput.mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(0.0 == defaultInput.mInputOverVoltageLimit);
    CPPUNIT_ASSERT(0.0 == defaultInput.mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(false == defaultInput.mInOverVoltageTripActive);
    CPPUNIT_ASSERT(false == defaultInput.mInUnderVoltageTripActive);

    /// - Check copy input construction
    ConverterInputData   copyInput(*mInputData);
    CPPUNIT_ASSERT(mInputVoltage == copyInput.mInputVoltage);
    CPPUNIT_ASSERT(mRegulatedVoltage == copyInput.mRegulatedVoltage);
    CPPUNIT_ASSERT(mEfficiency == copyInput.mEfficiency);
    CPPUNIT_ASSERT(mOutputOverCurrentLimit == copyInput.mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(mOutputOverVoltageLimit == copyInput.mOutputOverVoltageLimit);
    CPPUNIT_ASSERT(mOutOverCurrentTripActive == copyInput.mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(mOutOverVoltageTripActive == copyInput.mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(mInputOverVoltageLimit == copyInput.mInputOverVoltageLimit);
    CPPUNIT_ASSERT(mInputUnderVoltageLimit == copyInput.mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(mInOverVoltageTripActive == copyInput.mInOverVoltageTripActive);
    CPPUNIT_ASSERT(mInUnderVoltageTripActive == copyInput.mInUnderVoltageTripActive);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test default construction
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testDefaultConstruction() {
    std::cout << "\n UtConverter 03: testDefaultConstruction ...............";

    /// @test default constructed values
    CPPUNIT_ASSERT(0.0 == mArticle->mStandbyPower);
    CPPUNIT_ASSERT(0.0 == mArticle->mInputVoltage);
    CPPUNIT_ASSERT(0.0 == mArticle->mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0 == mArticle->mEfficiency);
    CPPUNIT_ASSERT(0.0 == mArticle->mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(0.0 == mArticle->mOutputOverVoltageLimit);
    CPPUNIT_ASSERT(false == mArticle->mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(false == mArticle->mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(0.0 == mArticle->mInputOverVoltageLimit);
    CPPUNIT_ASSERT(0.0 == mArticle->mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(false == mArticle->mInOverVoltageTripActive);
    CPPUNIT_ASSERT(false == mArticle->mInUnderVoltageTripActive);
    CPPUNIT_ASSERT(false == mArticle->mOutputOverCurrentTrip);
    CPPUNIT_ASSERT(false == mArticle->mOutputOverVoltageTrip);
    CPPUNIT_ASSERT(false == mArticle->mInputOverVoltageTrip);
    CPPUNIT_ASSERT(false == mArticle->mInputUnderVoltageTrip);
    CPPUNIT_ASSERT(false == mArticle->mTripOnSensedValue);
    CPPUNIT_ASSERT(false == mArticle->mConverterOn);
    CPPUNIT_ASSERT(false == mArticle->mBadEfficiency);
    CPPUNIT_ASSERT(0.0 == mArticle->mInputCurrent);
    CPPUNIT_ASSERT(0.0 == mArticle->mOutputCurrent);
    CPPUNIT_ASSERT(0.0 == mArticle->mOutputVoltage);
    CPPUNIT_ASSERT(0.0 == mArticle->mInputPower);
    CPPUNIT_ASSERT(0.0 == mArticle->mOutputPower);
    CPPUNIT_ASSERT(0.0 == mArticle->mHeatDissipation);

    /// @test new/delete for code coverage
    Converter* article = new Converter();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test nominal initialization
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testNominalInitialization() {
    std::cout << "\n UtConverter 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyConverter article;
    CPPUNIT_ASSERT(true == article.initialize(mConfigData, mInputData));

    /// - Check nominal initialization data
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mStandbyPower,  article.mStandbyPower, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputVoltage, article.mInputVoltage, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, article.mRegulatedVoltage, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiency, article.mEfficiency, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputOverCurrentLimit, article.mOutputOverCurrentLimit, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputOverVoltageLimit, article.mOutputOverVoltageLimit, mTolerance);
    CPPUNIT_ASSERT(mOutOverCurrentTripActive == article.mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(mOutOverVoltageTripActive == article.mOutOverVoltageTripActive);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputOverVoltageLimit, article.mInputOverVoltageLimit, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputUnderVoltageLimit, article.mInputUnderVoltageLimit, mTolerance);
    CPPUNIT_ASSERT(mInOverVoltageTripActive == article.mInOverVoltageTripActive);
    CPPUNIT_ASSERT(mInUnderVoltageTripActive == article.mInUnderVoltageTripActive);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test mStandbyPower exception
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testStandbyPowerException() {
    std::cout << "\n UtConverter 05: testStandbyPowerException .............";

    /// - Default construct a test article
    Converter article;

    /// @test initialization exception due to standby power of converter < 0
    mConfigData->mStandbyPower = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(mConfigData, mInputData), TsInitializationException);
    mConfigData->mStandbyPower = mStandbyPower;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test input under voltage exception
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testInputUnderVoltageException() {
    std::cout << "\n UtConverter 06: testInputUnderVoltageException ........";

    /// - Default construct a test article
    Converter article;

    /// @test initialization exception due to input undervoltage > input overvoltage
    mInputData->mInputUnderVoltageLimit = mInputOverVoltageLimit + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(mConfigData, mInputData), TsInitializationException);
    mInputData->mInputUnderVoltageLimit = mInputUnderVoltageLimit;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test regulated voltage exception
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testRegulatedVoltageException() {
    std::cout << "\n UtConverter 07: testRegulatedVoltageException .........";

    /// - Default construct a test article
    Converter article;

    /// @test initialization exception due to regulated voltage > output voltage
    mInputData->mRegulatedVoltage = mOutputOverVoltageLimit + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(mConfigData, mInputData), TsInitializationException);
    mInputData->mRegulatedVoltage = mRegulatedVoltage;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test efficiency exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testEfficiencyException() {
    std::cout << "\n UtConverter 08: testEfficiencyException ...............";

    /// - Default construct a test article
    Converter article;

    /// @test initialization exception due to efficiency < 0
    mInputData->mEfficiency = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(mConfigData, mInputData), TsInitializationException);
    mInputData->mEfficiency = mEfficiency;

    /// @test initialization exception due to efficiency > 1
    mInputData->mEfficiency = 1 + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(mConfigData, mInputData), TsInitializationException);
    mInputData->mEfficiency = mEfficiency;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test getters
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testAccessors() {
    std::cout << "\n UtConverter 09: testAccessors .........................";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(true == mArticle->initialize(mConfigData, mInputData));

    /// @test all the getters
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputVoltage, mArticle->getInputVoltage(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getOutputVoltage(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mStandbyPower, mArticle->mStandbyPower, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getInputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getOutputPower(), mTolerance);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isConverterOn());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isOutputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isInputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isInputUnderVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isTripOnSensedValue());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mBadEfficiency);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test setters
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testModifiers() {
    std::cout << "\n UtConverter 10: testModifiers .........................";

    /// - Default construct a test article
    FriendlyConverter article;

    /// @test all the setters
    article.setConverterState(true);
    CPPUNIT_ASSERT_EQUAL(true, article.mConverterOn);
    CPPUNIT_ASSERT_EQUAL(true, article.isConverterOn());
    article.setInputVoltage(mInputVoltage);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputVoltage, article.mInputVoltage, mTolerance);
    article.setRegulatedVoltage(mRegulatedVoltage);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, article.mRegulatedVoltage, mTolerance);
    article.setEfficiency(mEfficiency);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mEfficiency, article.mEfficiency, mTolerance);
    article.setOutputOverCurrentLimit(mOutputOverCurrentLimit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputOverCurrentLimit, article.mOutputOverCurrentLimit, mTolerance);
    article.setOutputOverVoltageLimit(mOutputOverVoltageLimit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mOutputOverVoltageLimit, article.mOutputOverVoltageLimit, mTolerance);
    article.mInputOverVoltageLimit = mInputOverVoltageLimit;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputOverVoltageLimit, article.mInputOverVoltageLimit, mTolerance);
    article.mInputUnderVoltageLimit = mInputUnderVoltageLimit;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mInputUnderVoltageLimit, article.mInputUnderVoltageLimit, mTolerance);
    article.mOutputVoltage = mRegulatedVoltage;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, article.mOutputVoltage, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, article.getOutputVoltage(), mTolerance);
    article.setOutOverCurrentTripActive(true);
    CPPUNIT_ASSERT_EQUAL(true, article.mOutOverCurrentTripActive);
    article.mOutOverVoltageTripActive = true;
    CPPUNIT_ASSERT_EQUAL(true, article.mOutOverVoltageTripActive);
    article.mInOverVoltageTripActive = true;
    CPPUNIT_ASSERT_EQUAL(true, article.mInOverVoltageTripActive);
    article.mInUnderVoltageTripActive = true;
    CPPUNIT_ASSERT_EQUAL(true, article.mInUnderVoltageTripActive);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test update process
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testUpdate() {
    std::cout << "\n UtConverter 11: testUpdate ............................";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(true == mArticle->initialize(mConfigData, mInputData));

    /// @test Once converter is on and no trip occurs, output voltage will be
    /// same as the regulated voltage
    mArticle->setConverterState(true);
    mArticle->update();

    /// - All the trips will be off and  converter will be on
    CPPUNIT_ASSERT_EQUAL(true,  mArticle->isConverterOn());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->mBadEfficiency);
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isTripOnSensedValue());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isOutputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isInputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, mArticle->isInputUnderVoltageTrip());

    /// - Output voltage same as regulated voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRegulatedVoltage, mArticle->getOutputVoltage(), mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test conversion process
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testConversion() {
    std::cout << "\n UtConverter 12: testConversion ........................";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(true == mArticle->initialize(mConfigData, mInputData));

    /// @test Converter's conversion process
    double outCurrent = 25.0;
    double outPower = mRegulatedVoltage * outCurrent;
    double inPower = outPower / mEfficiency;
    double inCurrent = inPower / mInputVoltage;
    double heat = (inPower - outPower) * 2.0562;

    /// Turn on converter, update and do conversion process
    mArticle->setConverterState(true);
    mArticle->update();
    mArticle->doConversion(outCurrent);

    /// - Check input and output power depending upon the default efficiency
    CPPUNIT_ASSERT_DOUBLES_EQUAL(outPower, mArticle->getOutputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inPower,  mArticle->getInputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inCurrent, mArticle->mInputCurrent, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(heat, mArticle->mHeatDissipation, 1.0);

    /// unloaded case (no load attached), update and do conversion process
    outCurrent = 0.0;
    outPower = 0.0;
    inPower = mStandbyPower;
    inCurrent = inPower / mInputVoltage;
    heat = mStandbyPower * 2.0562;

    mArticle->setConverterState(true);
    mArticle->update();
    mArticle->doConversion(outCurrent);

    /// - Check input and output power depending upon the default efficiency
    CPPUNIT_ASSERT_DOUBLES_EQUAL(outPower, mArticle->getOutputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inPower,  mArticle->getInputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inCurrent, mArticle->mInputCurrent, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(52.5, mArticle->mHeatDissipation, mTolerance);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test converter disable mode
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testConverterOff() {
    std::cout << "\n UtConverter 13: testConverterOff ......................";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(true == mArticle->initialize(mConfigData, mInputData));

    /// @test Once converter is off, output voltage will
    /// be zero
    double outCurrent = 0.0;
    double outPower = 0.0;
    double inPower = 0.0;
    double inCurrent = 0.0;
    double heat = 0.0;

    /// - Set converter On and disable it's output
    mArticle->setConverterState(false);
    mArticle->update();
    mArticle->doConversion(outCurrent);

    /// - Once converter is on and output is disable, power used by the
    /// converter will be standby power, assuming input voltaeg is provided within range
    CPPUNIT_ASSERT_DOUBLES_EQUAL(outPower, mArticle->getOutputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inPower,  mArticle->getInputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inCurrent, mArticle->mInputCurrent, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(heat, mArticle->mHeatDissipation, mTolerance);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test input under voltage trip
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testInputUnderVoltageTrip() {
    std::cout << "\n UtConverter 14: testInputUnderVoltageTrip .............";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(true == mArticle->initialize(mConfigData, mInputData));

    /// @test What happens if input voltage is zero
    double inVoltage = 0.0;
    double outCurrent = 10.0;

    /// - Turn converter on, and provide zaro input voltage to the converer
    mArticle->setInputVoltage(inVoltage);
    mArticle->setConverterState(true);
    mArticle->update();
    mArticle->update();
    mArticle->doConversion(outCurrent);

    /// - input under voltage trip will occur and input current will be zero
    /// and converter will be off
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isInputUnderVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isConverterOn());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inVoltage, mArticle->getInputVoltage(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mInputCurrent, mTolerance);


    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test input over voltage trip
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testInputOverVoltageTrip() {
    std::cout << "\n UtConverter 15: testInputOverVoltageTrip ..............";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(true == mArticle->initialize(mConfigData, mInputData));

    /// @test input over voltaqe trip by setting input voltage greater than the limit
    double inVoltage = mInputOverVoltageLimit + DBL_EPSILON+ 1.0;
    mArticle->setInputVoltage(inVoltage);
    mArticle->setConverterState(true);
    mArticle->update();
    mArticle->update();
    mArticle->update();
    double outCurrent = 10.0;
    mArticle->doConversion(outCurrent);

    /// -- input over voltage trip will occur and input current will be zero
    /// and converter will be on but output will be disabled
    CPPUNIT_ASSERT_EQUAL(true,  mArticle->isInputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isConverterOn());


    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test output over current trip
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testOutputOverCurrentTrip() {
    std::cout << "\n UtConverter 16: testOutputOverCurrentTrip .............";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(true == mArticle->initialize(mConfigData, mInputData));

    /// @test output over current trip by supplying current greated than the limit
    double outCurrent = mOutputOverCurrentLimit + DBL_EPSILON;
    mArticle->setConverterState(true);
    mArticle->doConversion(outCurrent);

    double heat = 52.5;
    /// - output over current trip will occur and input current will be zero
    /// and converter will be off
    CPPUNIT_ASSERT_EQUAL(true,  mArticle->isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isConverterOn());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(heat, mArticle->mHeatDissipation, mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test output over voltage trip
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testOutputOverVoltageTrip() {
    std::cout << "\n UtConverter 17: testOutputOverVoltageTrip .............";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(true == mArticle->initialize(mConfigData, mInputData));

    /// @test output over voltage trip by setting regulated voltage greater than the limit
    double outVoltage = mOutputOverVoltageLimit + DBL_EPSILON;
    double outCurrent = 0.0;

    /// - Turn converter on, update and do conversion
    mArticle->setRegulatedVoltage(outVoltage);
    mArticle->setConverterState(true);
    mArticle->update();
    mArticle->doConversion(outCurrent);

    /// - output over voltage trip will occur and input current will be zero
    /// and converter will be off
    CPPUNIT_ASSERT_EQUAL(true,  mArticle->isOutputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isConverterOn());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testBadEfficiency() {
    std::cout << "\n UtConverter 18: testBadEfficiency .....................";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(true == mArticle->initialize(mConfigData, mInputData));

    /// @test for bad efficiency
    double efficiency = -DBL_EPSILON;
    double outCurrent = 10.0;

    /// - Turn converter on and set efficiency value that is  out of bound
    mArticle->setEfficiency(efficiency);
    mArticle->setConverterState(true);
    mArticle->update();
    mArticle->doConversion(outCurrent);

    /// - Converter will turn off
    CPPUNIT_ASSERT_EQUAL(true,  mArticle->mBadEfficiency);
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isConverterOn());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->getInputPower(), mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, mArticle->mInputCurrent, mTolerance);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Test reset trip functionality
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverter::testResetTrips() {
    std::cout << "\n UtConverter 19: testResetTrips ........................";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(true == mArticle->initialize(mConfigData, mInputData));

    /// - Turn converter on, set all trips and then set resetTrip flag
    mArticle->setConverterState(true);
    mArticle->mOutputOverCurrentTrip = true;
    mArticle->mOutputOverVoltageTrip = true;
    mArticle->mInputOverVoltageTrip = true;
    mArticle->mInputUnderVoltageTrip = true;
    mArticle->setResetTrips(true);
    mArticle->update();

    /// - All trip should have cleared
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isOutputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isInputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false,  mArticle->isInputUnderVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(true,  mArticle->isConverterOn());


    std::cout << "... Pass";
}

