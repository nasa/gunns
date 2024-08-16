/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (aspects/electrical/sar/GunnsSolarArrayRegulator.o)
        (core/GunnsBasicPotential.o)
    )
***************************************************************************************************/
#include "UtGunnsSolarArrayRegulator.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsSolarArrayRegulator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsSolarArrayRegulator::UtGunnsSolarArrayRegulator()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tLinks(),
    tNodes(),
    tNodeList(),
    tPort(),
    tTimeStep(),
    tTolerance(),
    tConverterOnConductance(),
    tConverterOffConductance(),
    tStandbyPower(),
    tNominalTrickleChargeRate(),
    tRegulatedVoltageLowLimit(),
    tRegulatedVoltageHighLimit(),
    tOutVoltageSensorConfig(),
    tOutCurrentSensorConfig(),
    tOutVoltageUpperLimit(),
    tOutCurrentUpperLimit(),
    tOutVoltageNoiseScale(),
    tOutCurrentNoiseScale(),
    tTripPriority(),
    tmalfBlockageFlag(),
    tmalfBlockageValue(),
    tInputVoltage(),
    tRegulatedVoltage(),
    tEfficiency(),
    tOpOverCurrentLimit(),
    tOutputOverVoltageLimit(),
    tOpOverCurrentTripActive(),
    tOpOverVoltageTripActive(),
    tInputOverVoltageLimit(),
    tInputUnderVoltageLimit(),
    tInOverVoltageTripActive(),
    tInUnderVoltageTripActive(),
    tKp(),
    tKd(),
    tOutVoltageSensorInput(),
    tOutCurrentSensorInput(),
    tMalfOpOverCurrentFlag(),
    tMalfOpOverVoltageFlag(),
    tMalfRegulatedVoltageFlag(),
    tBatteryConfig(),
    tBatteryInput(),
    tBatteryLink()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsSolarArrayRegulator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsSolarArrayRegulator::~UtGunnsSolarArrayRegulator()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSolarArrayRegulator::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete tOutCurrentSensorInput;
    delete tOutVoltageSensorInput;
    delete tOutCurrentSensorConfig;
    delete tOutVoltageSensorConfig;
    delete tBatteryInput;
    delete tBatteryConfig;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initialize Potential Vector values
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSolarArrayRegulator::initPotentialVectorValues() {
    // Add these potential vector values
    tArticle->mPotentialVector[0] = 135.0;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->mPotentialVector[2] = 124.4;
    tArticle->mPotentialVector[3] = 0.0;

    tArticle->mDeltaPotentialOutputSide = 124.4;
    tArticle->mDeltaPotentialInputSide   = 135.0;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSolarArrayRegulator::setUp()
{
    // test battery setup
    batterySetup();

    // config data
    tConverterOnConductance            = 200;
    tConverterOffConductance           = 1.0e-6;
    tTripPriority                      =  3;
    tStandbyPower                      =  50.0;
    tNominalTrickleChargeRate          =  5.0;
    tRegulatedVoltageLowLimit          =  126.0;
    tRegulatedVoltageHighLimit         =  131.2;

    // sensor configuration/input
    tOutVoltageUpperLimit              =  140.0;
    tOutCurrentUpperLimit              =  125.0;
    tOutVoltageNoiseScale              =   2.80;
    tOutCurrentNoiseScale              =   3.75;

    // input data
    tmalfBlockageFlag                  =  false;
    tmalfBlockageValue                 =    0.0;
    tInputVoltage                      =  135.0;
    tRegulatedVoltage                  =  124.5;
    tEfficiency                        =   0.90;
    tOpOverCurrentLimit                =   30.0;
    tOutputOverVoltageLimit            =  151.0;
    tOpOverCurrentTripActive           =  true;
    tOpOverVoltageTripActive           =  true;
    tInputOverVoltageLimit             =  173.0;
    tInputUnderVoltageLimit            =  110.0;
    tInOverVoltageTripActive           =  true;
    tInUnderVoltageTripActive          =  true;
    tKp                                =  0.1;
    tKd                                =  0.01;
    tMalfOpOverCurrentFlag             =  false;
    tMalfOpOverVoltageFlag             =  false;
    tMalfRegulatedVoltageFlag          =  false;

    tLinkName           = "Test SAR Link";
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNodes;

    tPort[0] = Node0;
    tPort[1] = VACUUM;
    tPort[2] = Node1;
    tPort[3] = VACUUM;

    tTimeStep           = 0.1;
    tTolerance          = 1.0e-08;

    // sensor config data
    tOutVoltageSensorConfig = new SensorAnalogConfigData(0.0, tOutVoltageUpperLimit, 0.0, 0.0, 1.0, tOutVoltageNoiseScale,
                                                         0.001, 0, UnitConversion::NO_CONVERSION);
    tOutCurrentSensorConfig = new SensorAnalogConfigData(-125.0, tOutCurrentUpperLimit, 0.0, 0.0, 1.0, tOutCurrentNoiseScale,
                                                         0.001, 0, UnitConversion::NO_CONVERSION);

    // sensor input data
    tOutVoltageSensorInput  = new SensorAnalogInputData(true, 0.0);
    tOutCurrentSensorInput  = new SensorAnalogInputData(true, 0.0);

    /// - Define nominal configuration data
    tConfigData = new GunnsSolarArrayRegulatorConfigData(
            tLinkName,
            &tNodeList,
            &tBatteryLink,
            tOutVoltageSensorConfig,
            tOutCurrentSensorConfig,
            tConverterOnConductance,
            tConverterOffConductance,
            tTripPriority,
            tStandbyPower,
            tNominalTrickleChargeRate,
            tRegulatedVoltageLowLimit,
            tRegulatedVoltageHighLimit);

    /// - Define nominal input data
    tInputData = new GunnsSolarArrayRegulatorInputData(
            tmalfBlockageFlag,
            tmalfBlockageValue,
            tOutVoltageSensorInput,
            tOutCurrentSensorInput,
            tMalfOpOverCurrentFlag,
            tMalfOpOverVoltageFlag,
            tMalfRegulatedVoltageFlag,
            tInputVoltage,
            tRegulatedVoltage,
            tEfficiency,
            tOpOverCurrentLimit,
            tOutputOverVoltageLimit,
            tOpOverCurrentTripActive,
            tOpOverVoltageTripActive,
            tInputOverVoltageLimit,
            tInputUnderVoltageLimit,
            tInOverVoltageTripActive,
            tInUnderVoltageTripActive,
            tKp,
            tKd);

    tArticle = new FriendlyGunnsSolarArrayRegulator();

    try {
            tNodes[0].initialize(tLinkName + "Node0");
            tNodes[1].initialize(tLinkName + "Node1");
            tNodes[2].initialize(tLinkName + "Node2 (VACUUM)");

            /// - Initialize the Links. catch any exceptions and terminate.
            tLinks.clear();

            // Initiliaze the test battery link.
            tBatteryLink.initialize(*tBatteryConfig, *tBatteryInput, tLinks, Node1, VACUUM);

            // Initiliaze the test solar array regulator link.
            tArticle->initialize(*tConfigData, *tInputData, tLinks, Node0, VACUUM, Node1, VACUUM);

            initPotentialVectorValues();

        } catch (TsInitializationException &e) {
            std::cout << "UtGunnsSolarArrayRegulator setup - Initialization exception caught -- setup failed." << std::endl;
        } catch (...) {
            std::cout << "UtGunnsSolarArrayRegulator setup - unknown exception caught -- setup failed." << std::endl;
        }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Setup the battery. Executed before each test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSolarArrayRegulator::batterySetup()
{
    tBatteryConfig = new GunnsBasicPotentialConfigData("Test Battery",
                                                       &tNodeList,
                                                       156.25);        // defaultConductivity

    tBatteryInput = new GunnsBasicPotentialInputData(false,            // malfBlockageFlag
                                                     0.0,              // malfBlockageValue
                                                     120.0);           // sourcePotential
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSolarArrayRegulator::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsSolarArrayRegulator .. 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName           == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes              == tConfigData->mNodeList->mNodes);
    CPPUNIT_ASSERT(&tBatteryLink == tConfigData->mBattery);
    CPPUNIT_ASSERT(tConverterOnConductance == tConfigData->mOutputConductance);
    CPPUNIT_ASSERT(tConverterOffConductance == tConfigData->mConverterOffConductance);
    CPPUNIT_ASSERT(tOutVoltageUpperLimit  == tConfigData->mOutVoltageSensorConfig->mMaxRange);
    CPPUNIT_ASSERT(tOutCurrentUpperLimit  == tConfigData->mOutCurrentSensorConfig->mMaxRange);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOutVoltageNoiseScale,
            tConfigData->mOutVoltageSensorConfig->mNominalNoiseScale, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOutCurrentNoiseScale,
            tConfigData->mOutCurrentSensorConfig->mNominalNoiseScale, FLT_EPSILON);
    CPPUNIT_ASSERT(tTripPriority          == tConfigData->mTripPriority);
    CPPUNIT_ASSERT(tStandbyPower       == tConfigData->mConverterConfig.mStandbyPower);
    CPPUNIT_ASSERT(tNominalTrickleChargeRate  == tConfigData->mNominalTrickleChargeRate);
    CPPUNIT_ASSERT(tRegulatedVoltageLowLimit  == tConfigData->mRegulatedVoltageLowLimit);
    CPPUNIT_ASSERT(tRegulatedVoltageHighLimit  == tConfigData->mRegulatedVoltageHighLimit);

    /// - Check default config construction
    GunnsSolarArrayRegulatorConfigData defaultConfig;
    CPPUNIT_ASSERT(""    == defaultConfig.mName);
    CPPUNIT_ASSERT(0     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0     == defaultConfig.mBattery);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mOutputConductance);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mConverterOffConductance);
    CPPUNIT_ASSERT(0     == defaultConfig.mTripPriority);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mConverterConfig.mStandbyPower);
    CPPUNIT_ASSERT(0     == defaultConfig.mNominalTrickleChargeRate);
    CPPUNIT_ASSERT(0     == defaultConfig.mRegulatedVoltageLowLimit);
    CPPUNIT_ASSERT(0     == defaultConfig.mRegulatedVoltageHighLimit);

    /// - Check copy config construction
    GunnsSolarArrayRegulatorConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName           == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes              == copyConfig.mNodeList->mNodes);
    CPPUNIT_ASSERT(&tBatteryLink       == copyConfig.mBattery);
    CPPUNIT_ASSERT(tConverterOnConductance == copyConfig.mOutputConductance);
    CPPUNIT_ASSERT(tConverterOffConductance == copyConfig.mConverterOffConductance);
    CPPUNIT_ASSERT(tOutVoltageUpperLimit  == copyConfig.mOutVoltageSensorConfig->mMaxRange);
    CPPUNIT_ASSERT(tOutCurrentUpperLimit  == copyConfig.mOutCurrentSensorConfig->mMaxRange);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOutVoltageNoiseScale,
            copyConfig.mOutVoltageSensorConfig->mNominalNoiseScale, FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOutCurrentNoiseScale,
            copyConfig.mOutCurrentSensorConfig->mNominalNoiseScale, FLT_EPSILON);
    CPPUNIT_ASSERT(tTripPriority          == copyConfig.mTripPriority);
    CPPUNIT_ASSERT(tStandbyPower       == copyConfig.mConverterConfig.mStandbyPower);
    CPPUNIT_ASSERT(tNominalTrickleChargeRate       == copyConfig.mNominalTrickleChargeRate);
    CPPUNIT_ASSERT(tRegulatedVoltageLowLimit       == copyConfig.mRegulatedVoltageLowLimit);
    CPPUNIT_ASSERT(tRegulatedVoltageHighLimit      == copyConfig.mRegulatedVoltageHighLimit);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSolarArrayRegulator::testInput()
{
    std::cout << "\n UtGunnsSolarArrayRegulator .. 02: testInput .............................";

    /// - Check nominal input construction
    CPPUNIT_ASSERT(tmalfBlockageFlag           == tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT(tmalfBlockageValue          == tInputData->mMalfBlockageValue);
    CPPUNIT_ASSERT(tMalfOpOverCurrentFlag == tInputData->mMalfOpOverCurrentFlag);
    CPPUNIT_ASSERT(tMalfOpOverVoltageFlag  == tInputData->mMalfOpOverVoltageFlag);
    CPPUNIT_ASSERT(tMalfRegulatedVoltageFlag    == tInputData->mMalfRegulatedVoltageFlag);
    CPPUNIT_ASSERT(tOpOverCurrentLimit    == tInputData->mOpOverCurrentLimit);
    CPPUNIT_ASSERT(tInputVoltage              == tInputData->mConverterInput.mInputVoltage);
    CPPUNIT_ASSERT(tRegulatedVoltage          == tInputData->mConverterInput.mRegulatedVoltage);
    CPPUNIT_ASSERT(tEfficiency                == tInputData->mConverterInput.mEfficiency);
    CPPUNIT_ASSERT(tOpOverCurrentLimit  == tInputData->mConverterInput.mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(tOpOverCurrentTripActive  == tInputData->mConverterInput.mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(tOpOverVoltageTripActive  == tInputData->mConverterInput.mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(tInputOverVoltageLimit     == tInputData->mConverterInput.mInputOverVoltageLimit);
    CPPUNIT_ASSERT(tInputUnderVoltageLimit    == tInputData->mConverterInput.mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(tInOverVoltageTripActive   == tInputData->mConverterInput.mInOverVoltageTripActive);
    CPPUNIT_ASSERT(tInUnderVoltageTripActive  == tInputData->mConverterInput.mInUnderVoltageTripActive);
    CPPUNIT_ASSERT(tKp     == tInputData->mProportionalGain);
    CPPUNIT_ASSERT(tKd     == tInputData->mDerivativeGain);

    /// - Check default input construction
    GunnsSolarArrayRegulatorInputData defaultInput;
    CPPUNIT_ASSERT(false        == defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(0.0          == defaultInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(0.0          == defaultInput.mMalfOpOverCurrentFlag);
    CPPUNIT_ASSERT(0.0          == defaultInput.mMalfOpOverVoltageFlag);
    CPPUNIT_ASSERT(false        == defaultInput.mMalfRegulatedVoltageFlag);
    CPPUNIT_ASSERT(0.0          == defaultInput.mOpOverCurrentLimit);
    CPPUNIT_ASSERT(0.0          == defaultInput.mConverterInput.mInputVoltage);
    CPPUNIT_ASSERT(0.0          == defaultInput.mConverterInput.mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0          == defaultInput.mConverterInput.mEfficiency);
    CPPUNIT_ASSERT(0.0          == defaultInput.mConverterInput.mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(false        == defaultInput.mConverterInput.mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(false        == defaultInput.mConverterInput.mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(0.0          == defaultInput.mConverterInput.mInputOverVoltageLimit);
    CPPUNIT_ASSERT(0.0          == defaultInput.mConverterInput.mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(false        == defaultInput.mConverterInput.mInOverVoltageTripActive);
    CPPUNIT_ASSERT(false        == defaultInput.mConverterInput.mInUnderVoltageTripActive);
    CPPUNIT_ASSERT(0.0          == defaultInput.mProportionalGain);
    CPPUNIT_ASSERT(0.0          == defaultInput.mDerivativeGain);


    /// - Check copy input construction
    GunnsSolarArrayRegulatorInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tmalfBlockageFlag           == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT(tmalfBlockageValue          == copyInput.mMalfBlockageValue);
    CPPUNIT_ASSERT(tMalfOpOverCurrentFlag      == copyInput.mMalfOpOverCurrentFlag);
    CPPUNIT_ASSERT(tMalfOpOverVoltageFlag      == copyInput.mMalfOpOverVoltageFlag);
    CPPUNIT_ASSERT(tOpOverCurrentLimit         == copyInput.mOpOverCurrentLimit);
    CPPUNIT_ASSERT(tMalfRegulatedVoltageFlag   == copyInput.mMalfRegulatedVoltageFlag);
    CPPUNIT_ASSERT(tInputVoltage               == copyInput.mConverterInput.mInputVoltage);
    CPPUNIT_ASSERT(tRegulatedVoltage           == copyInput.mConverterInput.mRegulatedVoltage);
    CPPUNIT_ASSERT(tEfficiency                 == copyInput.mConverterInput.mEfficiency);
    CPPUNIT_ASSERT(tOpOverCurrentLimit         == copyInput.mConverterInput.mOutputOverCurrentLimit);
    CPPUNIT_ASSERT(tOpOverCurrentTripActive    == copyInput.mConverterInput.mOutOverCurrentTripActive);
    CPPUNIT_ASSERT(tOpOverVoltageTripActive    == copyInput.mConverterInput.mOutOverVoltageTripActive);
    CPPUNIT_ASSERT(tInputOverVoltageLimit      == copyInput.mConverterInput.mInputOverVoltageLimit);
    CPPUNIT_ASSERT(tInputUnderVoltageLimit     == copyInput.mConverterInput.mInputUnderVoltageLimit);
    CPPUNIT_ASSERT(tInOverVoltageTripActive    == copyInput.mConverterInput.mInOverVoltageTripActive);
    CPPUNIT_ASSERT(tInUnderVoltageTripActive   == copyInput.mConverterInput.mInUnderVoltageTripActive);
    CPPUNIT_ASSERT(tKp      == copyInput.mProportionalGain);
    CPPUNIT_ASSERT(tKd      == copyInput.mDerivativeGain);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSolarArrayRegulator::testDefaultConstruction()
{
    std::cout << "\n UtGunnsSolarArrayRegulator .. 03: testDefaultConstruction ...............";

    /// @test the default constructed values
    FriendlyGunnsSolarArrayRegulator defaultArticle;

    CPPUNIT_ASSERT(false         == defaultArticle.mInitFlag);
    CPPUNIT_ASSERT(0             == defaultArticle.mBattery);
    CPPUNIT_ASSERT(0.0           == defaultArticle.mDesiredChargeCurrent);
    CPPUNIT_ASSERT(0.0           == defaultArticle.mPreviousChargeCurrentError);
    CPPUNIT_ASSERT(0.0           == defaultArticle.mKp);
    CPPUNIT_ASSERT(0.0           == defaultArticle.mKd);
    CPPUNIT_ASSERT(0.0           == defaultArticle.mRegulatedVoltageLowLimit);
    CPPUNIT_ASSERT(0.0           == defaultArticle.mRegulatedVoltageHighLimit);
    CPPUNIT_ASSERT(0.0           == defaultArticle.mControlledVoltage);

    /// @test new/delete for code coverage
    GunnsSolarArrayRegulator* article = new GunnsSolarArrayRegulator();
    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSolarArrayRegulator::testNominalInitialization()
{
    std::cout << "\n UtGunnsSolarArrayRegulator .. 04: testNominalInitialization .............";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyGunnsSolarArrayRegulator article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]);

    /// @test config data loaded into model correctly
    CPPUNIT_ASSERT(&tBatteryLink == article.mBattery);
    CPPUNIT_ASSERT(tNominalTrickleChargeRate == article.mDesiredChargeCurrent);
    CPPUNIT_ASSERT(tRegulatedVoltageLowLimit == article.mRegulatedVoltageLowLimit);
    CPPUNIT_ASSERT(tRegulatedVoltageHighLimit == article.mRegulatedVoltageHighLimit);

    /// @test input data loaded into model correctly
    CPPUNIT_ASSERT(tKp == article.mKp);
    CPPUNIT_ASSERT(tKd == article.mKd);

    /// @test Converter is defaulted on
    CPPUNIT_ASSERT(article.mConverterOnCmd);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSolarArrayRegulator::testInitializationExceptions()
{
    std::cout << "\n UtGunnsSolarArrayRegulator .. 05: testInitializationExceptions ..........";

    /// - Default construct a test article
    GunnsSolarArrayRegulator article;

    /// @test for exception on null battery reference
    tConfigData->mBattery = 0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                         TsInitializationException);

    /// @test for exception on invalid config data: charge current < 0
    tConfigData->mNominalTrickleChargeRate = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                         TsInitializationException);

    /// @test for exception on invalid config data: low voltage limit < 0
    tConfigData->mRegulatedVoltageLowLimit = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                         TsInitializationException);

    /// @test for exception on invalid config data: high voltage limit < 0
    tConfigData->mRegulatedVoltageHighLimit = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                         TsInitializationException);

    /// @test for exception on invalid config data: high voltage limit < low voltage limit
    tConfigData->mRegulatedVoltageLowLimit  = 25.0;
    tConfigData->mRegulatedVoltageHighLimit = 12.0;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                         TsInitializationException);

    /// @test for exception on invalid input data: proportional gain < 0
    tInputData->mProportionalGain = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                         TsInitializationException);

    /// @test for exception on invalid input data: derivative gain < 0
    tInputData->mDerivativeGain = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(article.initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                         TsInitializationException);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the SAR's step method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsSolarArrayRegulator::testStep()
{
    std::cout << "\n UtGunnsSolarArrayRegulator .. 06: testStep ..............................";

    /// @test Step the SAR under nominal circumstances
    tArticle->step(tTimeStep);

    /// - Verify the SAR regulates the voltage
    CPPUNIT_ASSERT_EQUAL(126.0,  tArticle->mConverter.getOutputVoltage());

    std::cout << "... Pass.";

    std::cout << "\n -----------------------------------------------------------------------------";
}
