/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
 (aspects/electrical/Converter/ConverterElect.o)
 )
**************************************************************************************************/

#include <math.h>

#include "UtConverterElect.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  UtConverterElect constructor called before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtConverterElect::UtConverterElect()
    :
    tConfigData(),
    tInputData(),
    tConverter(),
    tArticle(),
    tLinkName(),
    tLinks(),
    tNodes(),
    tNodeList(),
    tPort(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tTolerance(),
    tConverterOnConductance(),
    tConverterOffConductance(),
    tStandbyPower(),
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
    tOutVoltageSensorInput(),
    tOutCurrentSensorInput(),
    tMalfOpOverCurrentFlag(),
    tMalfOpOverVoltageFlag(),
    tMalfRegulatedVoltageFlag() {

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief UtConverterElect destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtConverterElect::~UtConverterElect() {

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Sets up default data to be used for the unit test cases.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::setUp() {
    // config data
    tConverterOnConductance            = 200;
    tConverterOffConductance           = 1.0e-6;
    tStandbyPower                      =  105.0;
    tOutVoltageUpperLimit              =  140.0;
    tOutCurrentUpperLimit              =  125.0;
    tOutVoltageNoiseScale              =   2.80;
    tOutCurrentNoiseScale              =   3.75;
    tTripPriority                      =      3;

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
    tMalfOpOverCurrentFlag =  false;
    tMalfOpOverVoltageFlag   =  false;
    tMalfRegulatedVoltageFlag            =  false;


    tLinkName           = "Test ConverterElect Link";
    tNodeList.mNumNodes = 4;
    tNodeList.mNodes    = tNodes;

    tPort[0] = Node0;
    tPort[1] = VACUUM;
    tPort[2] = Node1;
    tPort[3] = VACUUM;

    tPort0              = 0;
    tPort1              = 1;
    tTimeStep           = 1.0;
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
    tConfigData = new ConverterElectConfigData(
            tLinkName,
            &tNodeList,
            tOutVoltageSensorConfig,
            tOutCurrentSensorConfig,
            tConverterOnConductance,
            tConverterOffConductance,
            tTripPriority,
            tStandbyPower);


    /// - Define nominal input data
    tInputData = new ConverterElectInputData(
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
            tInUnderVoltageTripActive);

//    tLinks.clear();

    tConverter = new Converter();

    tArticle = new FriendlyConverterElect();

    try {
        tNodes[0].initialize(tLinkName + "Node0");
        tNodes[1].initialize(tLinkName + "Node1");
        tNodes[2].initialize(tLinkName + "Node2");
        tNodes[3].initialize(tLinkName + "Node3 (VACUUM)");

        /// - Initialize the Links. catch any exceptions and terminate.
        tLinks.clear();

        tArticle->initialize(*tConfigData, *tInputData, tLinks, Node0, VACUUM, Node1, VACUUM);
//        tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]);

        initPotentialVectorValues();

        } catch (TsInitializationException &e) {
            std::cout << "UtConverterElect setup - Initialization exception caught -- setup failed." << std::endl;
        } catch (...) {
            std::cout << "UtConverterElect setup - unknown exception caught -- setup failed." << std::endl;
        }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Deleted all allocated objects after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::tearDown() {
    delete tConfigData;
    delete tInputData;
    delete tConverter;
    delete tArticle;
    delete tOutVoltageSensorConfig;
    delete tOutCurrentSensorConfig;
    delete tOutVoltageSensorInput;
    delete tOutCurrentSensorInput;

}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initialize Potential Vector values
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::initPotentialVectorValues() {
    // Add these potential vector values
    tArticle->mPotentialVector[0] = 135.0;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->mPotentialVector[2] = 124.4;
    tArticle->mPotentialVector[3] = 0.0;

    tArticle->mDeltaPotentialOutputSide = 124.4;
    tArticle->mDeltaPotentialInputSide   = 135.0;

}



///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Test configuration
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testConfig() {
    std::cout << "\n -----------------------------------------------------------------";
    std::cout << "\n UtConverterElect: testConfig ........................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName           == tConfigData->mName);
    CPPUNIT_ASSERT(tNodes              == tConfigData->mNodeList->mNodes);
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


       /// - Check default config construction
    ConverterElectConfigData defaultConfig;
    CPPUNIT_ASSERT(""    == defaultConfig.mName);
    CPPUNIT_ASSERT(0     == defaultConfig.mNodeList);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mOutputConductance);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mConverterOffConductance);
    CPPUNIT_ASSERT(0     == defaultConfig.mTripPriority);
    CPPUNIT_ASSERT(0.0   == defaultConfig.mConverterConfig.mStandbyPower);


       /// - Check copy config construction
    ConverterElectConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName           == copyConfig.mName);
    CPPUNIT_ASSERT(tNodes              == copyConfig.mNodeList->mNodes);
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

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests input data
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testInput() {
    std::cout << "\n UtConverterElect: testInput .........................";

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

    /// - Check default input construction
    ConverterElectInputData defaultInput;
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


    /// - Check copy input construction
    ConverterElectInputData copyInput(*tInputData);
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

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testDefaultConstruction() {
    std::cout << "\n UtConverterElect: testDefaultConstruction ...........";

    /// @test the default constructed values
    FriendlyConverterElect defaultArticle;

    CPPUNIT_ASSERT(false        == defaultArticle.mMalfOpOverCurrentFlag);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mMalfOpOverCurrentValue);
    CPPUNIT_ASSERT(false        == defaultArticle.mMalfOpOverVoltageFlag);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mMalfOpOverVoltageValue);
    CPPUNIT_ASSERT(false        == defaultArticle.mMalfRegulatedVoltageFlag);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mMalfRegulatedVoltageValue);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mConverterOffConductance);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mOutputConductance);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mInputActiveConductance);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mOutputActiveConductance);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mOpOverCurrentLimit);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mOutputOverVoltageLimit);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mRegulatedVoltage);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mOutputCurrent);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mOutputCurrentSensed);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mOutputVoltageSensed);
    CPPUNIT_ASSERT(false        == defaultArticle.mConverterOnCmd);
    CPPUNIT_ASSERT(false        == defaultArticle.mResetTrips);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mOutputSource);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mDeltaPotentialOutputSide);
    CPPUNIT_ASSERT(0.0          == defaultArticle.mDeltaPotentialInputSide);

    /// @test new/delete for code coverage
    ConverterElect* article = new ConverterElect();
    delete article;

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests nominal initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testNominalInitialization() {
    std::cout << "\n UtConverterElect: testNominalInitialization .........";

    /// - Default construct and initialize (with nominal data) a test article
    FriendlyConverterElect article;
    article.initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]);

    // input data
    CPPUNIT_ASSERT(false        == article.mMalfOpOverCurrentFlag);
    CPPUNIT_ASSERT(0.0          == article.mMalfOpOverCurrentValue);
    CPPUNIT_ASSERT(false        == article.mMalfOpOverVoltageFlag);
    CPPUNIT_ASSERT(0.0          == article.mMalfOpOverVoltageValue);
    CPPUNIT_ASSERT(false        == article.mMalfRegulatedVoltageFlag);
    CPPUNIT_ASSERT(0.0          == article.mMalfRegulatedVoltageValue);
    CPPUNIT_ASSERT(tOpOverCurrentLimit   == article.mOpOverCurrentLimit);
    CPPUNIT_ASSERT(tOutputOverVoltageLimit   == article.mOutputOverVoltageLimit);
    CPPUNIT_ASSERT(tRegulatedVoltage         == article.mRegulatedVoltage);

    // config data
    CPPUNIT_ASSERT(tLinkName    == article.mName);
    CPPUNIT_ASSERT(tConverterOnConductance    == article.mOutputConductance);
    CPPUNIT_ASSERT(tConverterOffConductance   == article.mConverterOffConductance);

    CPPUNIT_ASSERT(0.0          == article.mOutputCurrent);
    CPPUNIT_ASSERT(0.0          == article.mOutputCurrentSensed);
    CPPUNIT_ASSERT(0.0          == article.mOutputVoltageSensed);
    CPPUNIT_ASSERT(false        == article.mConverterOnCmd);
    CPPUNIT_ASSERT(false        == article.mResetTrips);

    /// @test init flag
    CPPUNIT_ASSERT(article.mInitFlag);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests exception due to out of bound converter (on/off) conductance value
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testConductanceException() {
    std::cout << "\n UtConverterElect: testConductanceException ..........";

    /// - Default construct a test article
    ConverterElect article;

    /// @test initialization exception due to converter on/off conductance < 0
    tConfigData->mOutputConductance = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
            TsInitializationException);
    tConfigData->mOutputConductance = tConverterOnConductance;

    tConfigData->mConverterOffConductance = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                TsInitializationException);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests exception due to out of bound trip priority
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testTripPriorityException() {
    std::cout << "\n UtConverterElect: testTripPriorityException .........";

    /// - Default construct a test article
    ConverterElect article;

    /// @test initialization exception due to trip priority <= 0
    tConfigData->mTripPriority = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                TsInitializationException);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests exception due to out of bound converter standby power value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testStandbyPowerException() {
    std::cout << "\n UtConverterElect: testStandbyPowerException .........";

    /// - Default construct a test article
    ConverterElect article;

    /// @test initialization exception due to standby power of converter < 0
    tConfigData->mConverterConfig.mStandbyPower = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                    TsInitializationException);
    tConfigData->mConverterConfig.mStandbyPower = tStandbyPower;

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests exception due to incorrect input under voltage value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testInputUnderVoltageException() {
    std::cout << "\n UtConverterElect: testInputUnderVoltageException ....";

    /// - Default construct a test article
    ConverterElect article;

    /// @test initialization exception due to input undervoltage > input overvoltage
    tInputData->mConverterInput.mInputUnderVoltageLimit = tInputOverVoltageLimit + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                    TsInitializationException);

    std::cout << "... Pass";
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests exception due to incorrect regulated voltage value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testRegulatedVoltageException() {
    std::cout << "\n UtConverterElect: testRegulateVoltageException ......";

    /// - Default construct a test article
    ConverterElect article;

    /// @test initialization exception due to regulated voltage > output voltage
    tInputData->mConverterInput.mRegulatedVoltage = tOutputOverVoltageLimit + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                        TsInitializationException);
    tInputData->mConverterInput.mRegulatedVoltage = tRegulatedVoltage;
    std::cout << "... Pass";
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests exception due to out of bound efficiency value of the converter.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testEfficiencylessThanZeroException() {
    std::cout << "\n UtConverterElect: testEfficiencylessThanZeroException ...........";

    /// - Default construct a test article
    ConverterElect article;

    /// @test initialization exception due to efficiency < 0
    tInputData->mConverterInput.mEfficiency = -DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                        TsInitializationException);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests exception due to out of bound efficiency value of the converter.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testEfficiencyGreaterThanOneException() {
    std::cout << "\n UtConverterElect: testEfficiencyGreaterThanOneException ...........";

    /// - Default construct a test article
    ConverterElect article;

    tInputData->mConverterInput.mEfficiency = tEfficiency;

    /// @test initialization exception due to efficiency > 1
    tInputData->mConverterInput.mEfficiency = 1 + DBL_EPSILON;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(*tConfigData, *tInputData, tLinks, tPort[0], tPort[1], tPort[2], tPort[3]),
                        TsInitializationException);
    tInputData->mConverterInput.mEfficiency = tEfficiency;

    std::cout << "... Pass";
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the setter
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testAccessors() {
    std::cout << "\n UtConverterElect: testAccessors .....................";

    /// - Initialization default test article with nominal data
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    /// @test Testing all the getters
    CPPUNIT_ASSERT_EQUAL(true, tArticle->isNonLinear());
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::CONFIRM, tArticle->confirmSolutionAcceptable(1, 1));
    tArticle->mConverterTripMgr.computeTripState(true);
//    tArticle->mWaitToTrip = true;
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::DELAY, tArticle->confirmSolutionAcceptable(1, 1));
    tArticle->mConverterTripMgr.computeTripState(true);
//    tArticle->mWaitToTrip = true;
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::REJECT, tArticle->confirmSolutionAcceptable(tTripPriority, 1));

    std::cout << "... Pass";
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the getter
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testModifiers() {
    std::cout << "\n UtConverterElect: testModifiers .....................";

       /// - Default construct a test article
       FriendlyConverterElect article;

       /// @test Testing all the setters
       article.mConverter.setInputVoltage(tInputVoltage);
       CPPUNIT_ASSERT_DOUBLES_EQUAL(tInputVoltage, article.mConverter.getInputVoltage(), tTolerance);
       article.mConverter.setOutputOverVoltageLimit(tOutputOverVoltageLimit);
       CPPUNIT_ASSERT_DOUBLES_EQUAL(tOutputOverVoltageLimit, article.mConverter.getOutputOverVoltageLimit(), tTolerance);
       article.setConverterOnCmd(true);
       CPPUNIT_ASSERT_EQUAL(true, article.mConverterOnCmd);

    std::cout << "... Pass";
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  method to step the article. This utility method is called when the article needs to be stepped through.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::stepTheModel() {
    tArticle->step(tTimeStep);  // for minorStep == 1 run regular step

    for (int i = 1; i<=10; i++) {  // worst case, run until EPS minor step limit
        tArticle->minorStep(tTimeStep, i);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the link step routin.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testStep() {
    std::cout << "\n UtConverterElect: testStep ..........................";

    /// - Initialization default test article with nominal data
    /// @test Once converter is on and no trip occurs, output voltage will be same as
    /// regulated voltage
    tArticle->mConverterOnCmd = true;
    tArticle->mDeltaPotentialInputSide = 135.0;
    stepTheModel();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConverterOnConductance, tArticle->mOutputActiveConductance, tTolerance);

    /// - all the trip will be off and converter will be on
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isConverterOn());
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mConverter.isInputUnderVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mConverter.isInputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mConverter.isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mConverter.isOutputOverVoltageTrip());

    /// - Output voltage same as regulated voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tRegulatedVoltage, tArticle->mConverter.getOutputVoltage(), tTolerance);

    /// @test Once converter is off and no trip occurs, output voltage will be 0.0
    tArticle->mConverterOnCmd = false;
    stepTheModel();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConverterOffConductance, tArticle->mOutputActiveConductance, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConverterOffConductance, tArticle->mOutputActiveConductance, tTolerance);

    /// - all the trip will be off and converter will be off
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverter.isConverterOn());
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mConverter.isInputUnderVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mConverter.isInputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mConverter.isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mConverter.isOutputOverVoltageTrip());

    /// - Output voltage same as regulated voltage
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mConverter.getOutputVoltage(), tTolerance);

    std::cout << "... Pass";
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the link step routin.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testBuildConductance() {
    std::cout << "\n UtConverterElect: testBuildConductance .........................." << std::endl;

    tArticle->mConverterOnCmd = true;
    stepTheModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mOutputActiveConductance, tArticle->mOutputConductance,
            tTolerance);

    double inActConductance = tArticle->mConverter.getInputPower() /
            (tArticle->mDeltaPotentialInputSide * tArticle->mDeltaPotentialInputSide);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInputActiveConductance, inActConductance,
            tTolerance);

    // make the input delta potential equal zero.
    tArticle->mDeltaPotentialInputSide = 0.0;
    tArticle->buildConductance();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInputActiveConductance, 1.0e-5, tTolerance);

    // make tolerance less than zero
    inActConductance = 1.0e-5;
    tArticle->buildConductance();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInputActiveConductance, inActConductance,
            tTolerance);

    std::cout << "... Pass" << std::endl;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the link step routin.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testBuildAdmittance() {
    std::cout << "\n UtConverterElect: testBuildAdmittance ..........................";
    // set converter to On
    tArticle->mConverterOnCmd = true;
    stepTheModel();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mOutputActiveConductance, tArticle->mOutputConductance,
            tTolerance);

    double inActConductance = tArticle->mConverter.getInputPower() /
            (tArticle->mDeltaPotentialInputSide * tArticle->mDeltaPotentialInputSide);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInputActiveConductance, inActConductance,
            tTolerance);
    stepTheModel();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mOutputActiveConductance, tArticle->mOutputConductance,
                tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mAdmittanceMatrix[0], tArticle->mInputActiveConductance,
                    tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mAdmittanceMatrix[10], tArticle->mOutputActiveConductance,
                    tTolerance);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests link's compute flow routine.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testComputeFlow() {
    std::cout << "\n UtConverterElect: testComputeFlow ...................";

    /// - Initialization default test article with nominal data
    /// @test Output current calculated by GUNNS if potential vector is providedconfirmSolutionAcceptable

    double outCurrent = 25.0;
    double outPower = tRegulatedVoltage * outCurrent;

    double inPower = (outPower / tArticle->mConverter.getEfficiency());
    double inCurrent = inPower / tInputVoltage;

    /// - Turn converter on, step and then compute flow
    tArticle->mConverterOnCmd = true;
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    std::cout <<"\n testComputeFlow - after step - Converter Output Current = " << tArticle->mConverter.getOutputCurrent() << std::endl;
    std::cout <<"\n testComputeFlow - after step - Output Current = " << tArticle->mOutputCurrent << std::endl;
    std::cout <<"\n testComputeFlow - after step -  mDeltaPotentialOutputSide = " << tArticle->mDeltaPotentialOutputSide << std::endl;
    std::cout <<"\n testComputeFlow - after step -  mAdmittanceMatrix[10] = " << tArticle->mAdmittanceMatrix[10] << std::endl;
    std::cout <<"\n testComputeFlow - after step -  mSourceVector[2] = " << tArticle->mSourceVector[2] << std::endl;

    tArticle->mDeltaPotentialOutputSide = 124.375;
    tArticle->mPotentialVector[2] = 124.375;
    tArticle->mPotentialVector[3] = 0.0;
    tArticle->mConverterOnCmd = true;
    tArticle->updateConverterState();
    stepTheModel();

     /// - Once converter is On, output power and input power depends on output
     /// current of the converter
     CPPUNIT_ASSERT_DOUBLES_EQUAL(outCurrent, tArticle->mOutputCurrent, tTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(inPower,  tArticle->mConverter.getInputPower(), tTolerance);
     CPPUNIT_ASSERT_DOUBLES_EQUAL(inCurrent, tArticle->mConverter.getInputCurrent(), tTolerance);

    /// @test Test what happens if input voltage is 0.0
    tArticle->mConverter.setInputVoltage(0.0);
    tArticle->mConverter.setTripOnSensedValue(false);
    outCurrent = 0.0;
    outPower = 0.0;
    inPower = 0.0;
    inCurrent = 0.0;

    tArticle->mPotentialVector[0] = 0.0;
    tArticle->mPotentialVector[1] = 0.0;
    tArticle->mPotentialVector[2] = 0.0;
    tArticle->mPotentialVector[3] = 0.0;

    /// - Turn converter off, step and then compute flow
    tArticle->mConverterOnCmd = false;
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    /// - The output voltage will be 0.0, no current will flow and converter will be
    /// off and no power will be consumbed by it.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,  tArticle->mConverter.getOutputVoltage(), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInputVoltage,  tArticle->mConverter.getInputVoltage(), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(outCurrent, tArticle->mOutputCurrent, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inPower,  tArticle->mConverter.getInputPower(), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inCurrent, tArticle->mConverter.getInputCurrent(), tTolerance);
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverter.isConverterOn());

    /// - Set the potential difference across the link like Gunns would do.
    tArticle->mPotentialVector[0] = 0.0;
    tArticle->mPotentialVector[1] = 0.0;
    outCurrent = 0.0;

    /// @test Once converter is off, no power will be consumed by the converter
    tArticle->mConverterOnCmd = false;
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(outCurrent, tArticle->mOutputCurrent, tTolerance);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests confirm solution when time to trip is false
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testConfirmSolutionAcceptable() {
    std::cout << "\n UtConverterElect: testConverterOff ..................";

    /// plan - create a trip in compute flows, run minor step and confirm soultion with a lesser priority.
    /// increment priority until time to trip is true and solution will be set to confirm

    /// - Initialization default test article with nominal data
    /// @test Output current calculated by GUNNS if potential vector is providedconfirmSolutionAcceptable
    /// - Turn converter on, step and then compute flow
    tArticle->mConverterOnCmd = true;
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    GunnsBasicLink::SolutionResult result = tArticle->confirmSolutionAcceptable(1, 1);
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverterTripMgr.getTimeToTrip());
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverterTripMgr.getWaitToTrip());
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::CONFIRM,  result);

    tArticle->mPotentialVector[2] = 124.3; // yield an output current of 40 amp
    tArticle->mPotentialVector[3] = 0.0;
    tArticle->mConverterOnCmd = true;
    tArticle->updateConverterState();
    tArticle->computeFlows(tTimeStep);

    result = tArticle->confirmSolutionAcceptable(1, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mDeltaPotentialOutputSide, tArticle->getDeltaPotential(2, 3), tTolerance);
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverterTripMgr.getTimeToTrip());
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverterTripMgr.getWaitToTrip());
//    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mTimeToTrip);
//    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::DELAY,  result);

    tArticle->computeFlows(tTimeStep);
    result = tArticle->confirmSolutionAcceptable(2, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mDeltaPotentialOutputSide, tArticle->getDeltaPotential(2, 3), tTolerance);
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverterTripMgr.getTimeToTrip());
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverterTripMgr.getWaitToTrip());
//    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mTimeToTrip);
//    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::DELAY,  result);

    tArticle->computeFlows(tTimeStep);
    result = tArticle->confirmSolutionAcceptable(3, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mDeltaPotentialOutputSide, tArticle->getDeltaPotential(2, 3), tTolerance);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverterTripMgr.getTimeToTrip());
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverterTripMgr.getWaitToTrip());
//    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mTimeToTrip);
//    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mWaitToTrip);
    CPPUNIT_ASSERT_EQUAL(GunnsBasicLink::REJECT,  result);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests converter disable output logic ( or turn off converter)
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testConverterOff() {
    std::cout << "\n UtConverterElect: testConverterOff ..................";

    /// @test Once converter is on and disable output, output voltage will
    /// be zero
    double outCurrent = 0.0;
    double outPower = 0.0;
    double inPower = 0.0;
    double inCurrent = 0.0;

    /// - Set the potential difference across the link like Gunns would do.
    tArticle->mPotentialVector[0] = 0.0;
    tArticle->mPotentialVector[2] = 0.0;

    tArticle->mConverterOnCmd = false;
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);

    /// - Once converter is on and output is disable, power used by the
    /// converter will be standby power, provided that input voltage is within range
    CPPUNIT_ASSERT_DOUBLES_EQUAL(outCurrent, tArticle->mOutputCurrent, tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(outPower, tArticle->mConverter.getOutputPower(), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inPower,  tArticle->mConverter.getInputPower(), tTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inCurrent, tArticle->mConverter.getInputCurrent(), tTolerance);

    std::cout << "... Pass";
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests output over current trip logic
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testOutputOverCurrentTrip() {
    std::cout << "\n UtConverterElect: testOutputOverCurrentTrip .........";

    /// @test Test output over current trip by providing current more than the limit
    tArticle->mConverterOnCmd = true;

    /// @test set current to be above over current trip limit
    double outCurrent = tOpOverCurrentLimit + DBL_EPSILON;

    /// - Trip on computed values instead of sensor values
    tArticle->mConverter.setTripOnSensedValue(false);
    tArticle->step(1.0);
    tArticle->mConverter.doConversion(outCurrent);

    /// - Output over current trip will occur and converter will be off
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverter.isConverterOn());

    /// - Trip on computed values instead of sensor values
    tArticle->mConverter.setTripOnSensedValue(false);
    /// @test reset trip functionality
    tArticle->mResetTrips = true;
    tArticle->mConverterOnCmd = true;
    tArticle->step(1.0);
    tArticle->step(1.0);
    tArticle->mConverter.setOutOverCurrentTripActive(true);
    tArticle->mConverter.doConversion(outCurrent);


    /// - Output over current trip will occur and converter will be off
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverter.isConverterOn());

    /// @test rest trip functionality
    tArticle->mResetTrips = true;
    tArticle->mConverterOnCmd = true;
    tArticle->step(1.0);
    tArticle->mConverter.setOutOverCurrentTripActive(false);

    /// @test over current trip when trip is inhibited
    outCurrent = tOpOverCurrentLimit + 1.0;
    tArticle->mConverter.doConversion(outCurrent);

    /// - Output over current trip will occur and converter will be off
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverter.isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isConverterOn());

    std::cout << "... Pass";
}




////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests output over voltage trip logic
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testOutputOverVoltageTrip() {
    std::cout << "\n UtConverterElect: testOutputOverVoltageTrip .........";

    /// @test Test output over voltage trip by providing regulated voltage that is more
    /// than the limit
    double outVoltage = tOutputOverVoltageLimit + DBL_EPSILON;
    double outCurrent = 25.0;
    tArticle->mConverter.setRegulatedVoltage(outVoltage);
    tArticle->mConverterOnCmd = true;
    tArticle->step(tTimeStep);
    tArticle->mConverter.handleOutputTrips(outVoltage, outCurrent);
    tArticle->mConverter.handleOutputTrips(outVoltage, outCurrent);

    /// - Output over voltage trip will occur and converter will be off
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isOutputOverVoltageTrip());
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverter.isConverterOn());
    std::cout << "... Pass";
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests bad efficiency logic
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testBadEfficiency() {
    std::cout << "\n UtConverterElect: testBadEfficiency .................";

    /// @test Test for bad efficiency
    double efficiency = -DBL_EPSILON;
    tArticle->mConverter.setEfficiency(efficiency);
    tArticle->mConverterOnCmd = true;
    tArticle->step(tTimeStep);

    /// - Converter will turn off if badEfficiency flag will be set
    CPPUNIT_ASSERT_EQUAL(false,  tArticle->mConverter.isConverterOn());

    std::cout << "... Pass";
}




////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Tests biasing output load malfunction
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testBiasOutputLoad() {
    std::cout << "\n UtConverterElect: testBiasOutputLoad ................";

    /// @test Test biasing load by adding dummy load to the converter

    double outCurrent = 15.0;
    /// - Set the potential difference across the link like Gunns would do.
    tArticle->mPotentialVector[0] = (outCurrent / tConverterOnConductance);
    tArticle->mPotentialVector[2] = tRegulatedVoltage;

    tArticle->mConverterOnCmd = true;
    tArticle->step(tTimeStep);

    /// - The conductance of the link will change according to the amount of load added
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isConverterOn());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConverterOnConductance, tArticle->mAdmittanceMatrix[10], tTolerance);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests inhibit ouput over current function (125% and 150% over current)
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testInhibitOutOverCurrent() {
    std::cout << "\n UtConverterElect: testInhibitOutOverCurrent .........";

    /// @test Test inhibit output over current functionality
    double outCurrent = tOpOverCurrentLimit + DBL_EPSILON;

    /// - Set the potential difference across the link like Gunns would do.
    tArticle->mPotentialVector[0] = (outCurrent / tConverterOnConductance);
    tArticle->mPotentialVector[1] = tRegulatedVoltage;

    /// - Turn converter on, inhibit over current trip
    tArticle->mConverterOnCmd = true;
    tArticle->mConverter.setOutOverCurrentTripActive(false);
    tArticle->step(tTimeStep);
    tArticle->computeFlows(tTimeStep);
    tArticle->mConverter.handleOutputTrips(tRegulatedVoltage, outCurrent);

    /// - Since inhibited, 125% over current trip will not occur, unless hardware limit is reached
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isConverterOn());
    CPPUNIT_ASSERT_EQUAL(false, tArticle->mConverter.isOutputOverCurrentTrip());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tOpOverCurrentLimit, tArticle->mConverter.getOutputOverCurrentLimit(), tTolerance);

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests malfunctions
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtConverterElect::testMalfunctions() {
    std::cout << "\n UtConverterElect: testMalfunctions ..................";

    /// @test Test 125% over current limit malfunction (override the limit)
    double overCurrentValue = 10.0;
    tArticle->mMalfOpOverCurrentFlag = true;
    tArticle->mMalfOpOverCurrentValue = overCurrentValue;
    tArticle->mConverterOnCmd = true;
    tArticle->step(tTimeStep);

    /// - New limit is set for 125% over current limit
    CPPUNIT_ASSERT_DOUBLES_EQUAL(overCurrentValue, tArticle->mConverter.getOutputOverCurrentLimit(), tTolerance);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isConverterOn());
    tArticle->mMalfOpOverCurrentFlag = false;

    /// @test Test over current limit malfunction (override the limit)
    tArticle->mConverterOnCmd = true;
    tArticle->step(tTimeStep);

    /// - Limit set for over current limit is the config value
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInputData->mOpOverCurrentLimit, tArticle->mOpOverCurrentLimit, tTolerance);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isConverterOn());

    /// @test Test hardware over voltage limit malfunction (override the limit)
    double overVoltageValue = 130.0;
    tArticle->mMalfOpOverVoltageFlag = true;
    tArticle->mMalfOpOverVoltageValue = overVoltageValue;
    tArticle->mConverterOnCmd = true;
    tArticle->step(tTimeStep);

    /// - New limit is set for hardware over voltage limit
    CPPUNIT_ASSERT_DOUBLES_EQUAL(overVoltageValue, tArticle->mConverter.getOutputOverVoltageLimit(), tTolerance);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isConverterOn());
    tArticle->mMalfOpOverVoltageFlag = false;

    /// @test Test output voltage malfunction (override output voltage value)
    double voltageValue = 120.0;
    tArticle->mMalfRegulatedVoltageFlag = true;
    tArticle->mMalfRegulatedVoltageValue = voltageValue;
    tArticle->mConverterOnCmd = true;
    tArticle->step(tTimeStep);

    /// - New output voltage is regulated
    CPPUNIT_ASSERT_DOUBLES_EQUAL(voltageValue, tArticle->mConverter.getOutputVoltage(), tTolerance);
    CPPUNIT_ASSERT_EQUAL(true,  tArticle->mConverter.isConverterOn());
    tArticle->mMalfRegulatedVoltageFlag = false;

    std::cout << "... Pass";
    std::cout << "\n -----------------------------------------------------------------";
}

