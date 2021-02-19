/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsNetworkSpotter.o))
***************************************************************************************************/

#include "UtGunnsNetworkSpotter.hh"
#include "software/exceptions/TsInitializationException.hh"

ExampleDerivedSpotterConfigData::ExampleDerivedSpotterConfigData(const std::string& name,
                                                                 const int          preStepCounter)
    :
    GunnsNetworkSpotterConfigData(name),
    mPreStepCounter              (preStepCounter)
{
    // nothing to do
}

ExampleDerivedSpotterInputData::ExampleDerivedSpotterInputData(const int postStepCounter)
    :
    mPostStepCounter(postStepCounter)
{
    // nothing to do
}

ExampleDerivedSpotter::ExampleDerivedSpotter(GunnsBasicConductor& linkRef)
    :
    GunnsNetworkSpotter(),
    mLinkRef(linkRef),
    mPreStepCounter(0),
    mPostStepCounter(0)
{
    // nothing to do
}

void ExampleDerivedSpotter::initialize(const GunnsNetworkSpotterConfigData* configData,
                                       const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate & type-cast config & input data.
    const ExampleDerivedSpotterConfigData* config = validateConfig(configData);
    const ExampleDerivedSpotterInputData*  input  = validateInput(inputData);

    /// - Initialize with validated config & input data.
    mPreStepCounter  = config->mPreStepCounter;
    mPostStepCounter = input->mPostStepCounter;

    /// - Set the init flag.
    mInitFlag = true;
}

const ExampleDerivedSpotterConfigData* ExampleDerivedSpotter::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const ExampleDerivedSpotterConfigData* result = dynamic_cast<const ExampleDerivedSpotterConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }

    /// - Do your other data validation as appropriate.

    return result;
}

const ExampleDerivedSpotterInputData* ExampleDerivedSpotter::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const ExampleDerivedSpotterInputData* result = dynamic_cast<const ExampleDerivedSpotterInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }

    /// - Do your other data validation as appropriate.

    return result;
}

void ExampleDerivedSpotter::stepPreSolver(const double dt __attribute__((unused))) {
    mPreStepCounter++;
}

void ExampleDerivedSpotter::stepPostSolver(const double dt __attribute__((unused))) {
    mPostStepCounter++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsNetworkSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsNetworkSpotter::UtGunnsNetworkSpotter()
    :
    tArticle(tConductor),
    tConductor(),
    tName("test article"),
    tConfig(tName),
    tInput()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsNetworkSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsNetworkSpotter::~UtGunnsNetworkSpotter()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkSpotter::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkSpotter::setUp()
{
    tConfig.mName           = tName;
    tConfig.mPreStepCounter = 1;
    tInput.mPostStepCounter = 5;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsNetworkSpotterConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkSpotter::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsNetworkSpotter .. 01: testConfig ............................";

    /// - Test nominal config data construction.
    CPPUNIT_ASSERT(tName == tConfig.mName);
    CPPUNIT_ASSERT(1     == tConfig.mPreStepCounter);

    /// - Test default config data construction.
    ExampleDerivedSpotterConfigData article(tName);
    CPPUNIT_ASSERT(tName == article.mName);
    CPPUNIT_ASSERT(0     == article.mPreStepCounter);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsNetworkSpotterInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkSpotter::testInput()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsNetworkSpotter .. 02: testInput .............................";

    /// - Test nominal input data construction.
    CPPUNIT_ASSERT(5 == tInput.mPostStepCounter);

    /// - Test default input data construction.
    ExampleDerivedSpotterInputData article;
    CPPUNIT_ASSERT(0 == article.mPostStepCounter);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsNetworkSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkSpotter::testDefaultConstruction()
{
    std::cout << "\n UtGunnsNetworkSpotter .. 03: testDefaultConstruction ...............";

    CPPUNIT_ASSERT(""          ==  tArticle.mName);
    CPPUNIT_ASSERT(false       ==  tArticle.mInitFlag);
    CPPUNIT_ASSERT(&tConductor == &tArticle.mLinkRef);
    CPPUNIT_ASSERT(0           ==  tArticle.mPreStepCounter);
    CPPUNIT_ASSERT(0           ==  tArticle.mPostStepCounter);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsNetworkSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkSpotter::testInitialize()
{
    std::cout << "\n UtGunnsNetworkSpotter .. 04: testInitialize ........................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Test nominal initialization.
    CPPUNIT_ASSERT(tName       ==  tArticle.mName);
    CPPUNIT_ASSERT(true        ==  tArticle.mInitFlag);
    CPPUNIT_ASSERT(&tConductor == &tArticle.mLinkRef);
    CPPUNIT_ASSERT(1           ==  tArticle.mPreStepCounter);
    CPPUNIT_ASSERT(5           ==  tArticle.mPostStepCounter);

    /// - Test exception thrown from missing name.
    tConfig.mName = "";
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    tConfig.mName = tName;

    /// - Test exception thrown from null config data.
    const GunnsNetworkSpotterConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(nullConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown from null input data.
    const GunnsNetworkSpotterInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);

    /// - Test exception thrown on bad config data pointer type.
    BadSpotterConfigData* badConfig = new BadSpotterConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle.initialize(badConfig, &tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badConfig;

    /// - Test exception thrown on bad input data pointer type.
    BadSpotterInputData* badInput = new BadSpotterInputData();
    CPPUNIT_ASSERT_THROW(tArticle.initialize(&tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle.mInitFlag);
    delete badInput;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsNetworkSpotter class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkSpotter::testAccessors()
{
    std::cout << "\n UtGunnsNetworkSpotter .. 05: testAccessors .........................";

    tArticle.initialize(&tConfig, &tInput);

    /// - Test getName method.
    CPPUNIT_ASSERT(tName == tArticle.getName());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsNetworkSpotter class.  That
///           method doesn't actually do anything so there are no asserts, but this is done for
///           complete code coverage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkSpotter::testPreSolver()
{
    std::cout << "\n UtGunnsNetworkSpotter .. 06: testPreSolver .........................";

    tArticle.initialize(&tConfig, &tInput);
    tArticle.stepPreSolver(0.0);
    CPPUNIT_ASSERT(2 == tArticle.mPreStepCounter);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsNetworkSpotter class.  That
///           method doesn't actually do anything so there are no asserts, but this is done for
///           complete code coverage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsNetworkSpotter::testPostSolver()
{
    std::cout << "\n UtGunnsNetworkSpotter .. 07: testPostSolver ........................";

    tArticle.initialize(&tConfig, &tInput);
    tArticle.stepPostSolver(0.0);
    CPPUNIT_ASSERT(6 == tArticle.mPostStepCounter);

    std::cout << "... Pass";
}
