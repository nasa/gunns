/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsFluidVolumeMonitor.o))
*/

#include "UtGunnsFluidVolumeMonitor.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidVolumeMonitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidVolumeMonitor::UtGunnsFluidVolumeMonitor()
    :
    tArticle(),
    tNodes(),
    tNodeList(),
    tName(""),
    tConfig(),
    tInput(),
    tLinkPort(),
    tFluidProperties(),
    tFluidConfig(),
    tFluidConfig0(),
    tFluidInput(),
    tMassFractions(),
    tLinks(),
    tConductorLink(),
    tConductorLinkConfig(),
    tConductorLinkInput(),
    tMaxConductivity(),
    tExpansionScaleFactor(),
    tPort0(),
    tPort1(),
    tTimeStep(),
    tTcConfig(),
    tTcInput()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidVolumeMonitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidVolumeMonitor::~UtGunnsFluidVolumeMonitor()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidVolumeMonitor::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tConfig;
    delete tInput;
    delete tConductorLinkInput;
    delete tConductorLinkConfig;
    delete tFluidConfig0;
    delete tFluidInput;
    delete tFluidConfig;
    delete tTcInput;
    delete tTcConfig;
    delete tFluidProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidVolumeMonitor::setUp()
{
    /// - Setup some test fluid nodes.
    tFluidProperties = new DefinedFluidProperties();
    FluidProperties::FluidType types[2];
    types[0] = FluidProperties::GUNNS_N2;
    types[1] = FluidProperties::GUNNS_O2;
    tMassFractions[0] = 0.8;
    tMassFractions[1] = 0.2;
    ChemicalCompound::Type     tcTypes[1]    = {ChemicalCompound::LIOH};
    tTcConfig = new GunnsFluidTraceCompoundsConfigData(tcTypes, 1, "tcConfig");
    double state[1];
    state[0] = 0.001;
    tTcInput = new GunnsFluidTraceCompoundsInputData(state);
    tFluidConfig = new PolyFluidConfigData(tFluidProperties, types, 2, tTcConfig);
    tFluidInput  = new PolyFluidInputData(283.15,                   //temperature
                                          700.728,                  //pressure
                                          0.0,                      //flowRate
                                          0.0,                      //mass
                                          tMassFractions,           //massFractions
                                          tTcInput);                 //tcInput

    tFluidConfig0 = new PolyFluidConfigData(tFluidProperties, types, 2);

    /// - Have to initialize the nodes with the fluid configs (normally done by GUNNS)
    tNodes[0].initialize("tNodes_0", tFluidConfig0);
    tNodes[1].initialize("tNodes_1", tFluidConfig);

    tNodes[1].getContent()->initialize(*tFluidConfig, *tFluidInput);
    tNodes[1].setPotential(tFluidInput->mPressure);
    tNodes[1].initVolume(1.0);

    /// - Test fluid conductor initialization.
    tNodeList.mNumNodes   = 2;
    tNodeList.mNodes      = tNodes;
    tMaxConductivity      = 0.5;
    tExpansionScaleFactor = 0.4;
    tPort0                = 0;
    tPort1                = 1;
    tTimeStep             = 0.1;

    /// - Define configuration and input data for the test conductor
    tConductorLinkConfig = new GunnsFluidConductorConfigData(
            "Test Fluid Conductor", &tNodeList, tMaxConductivity, tExpansionScaleFactor);
    tConductorLinkInput  = new GunnsFluidConductorInputData(true, 0.5);

    tConductorLink.initialize(*tConductorLinkConfig, *tConductorLinkInput, tLinks, tPort0, tPort1);

    /// - Test spotter configuration.
    tName     = "tArticle";
    tLinkPort = 1;
    tConfig = new GunnsFluidVolumeMonitorConfigData(tName);
    tInput  = new GunnsFluidVolumeMonitorInputData(tLinkPort);

    /// - Create the test article.
    tArticle = new FriendlyGunnsFluidVolumeMonitor(tNodeList, tConductorLink);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsFluidVolumeMonitorConfigData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidVolumeMonitor::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidVolumeMonitor 01: testConfig ...........................";

    /// - Test nominal config data construction.
    CPPUNIT_ASSERT(tName == tConfig->mName);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the GunnsFluidVolumeMonitorInputData class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidVolumeMonitor::testInput()
{
    std::cout << "\n UtGunnsFluidVolumeMonitor 02: testInput ............................";

    /// - Test nominal input data construction.
    CPPUNIT_ASSERT(tLinkPort == tInput->mLinkPort);

    /// - Test default config data construction.
    GunnsFluidVolumeMonitorInputData defaultInput;
    CPPUNIT_ASSERT(0 == defaultInput.mLinkPort);

    std::cout << "... Pass";
}

#include <iostream>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsFluidVolumeMonitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidVolumeMonitor::testDefaultConstruction()
{
    std::cout << "\n UtGunnsFluidVolumeMonitor 03: testDefaultConstruction ..............";

    /// @test state data
    CPPUNIT_ASSERT(""                       == tArticle->mName);
    CPPUNIT_ASSERT(tNodeList.mNumNodes      == tArticle->mNodeList.mNumNodes);
    CPPUNIT_ASSERT(tConductorLink.getName() == tArticle->mLink.getName());
    CPPUNIT_ASSERT(0                        == tArticle->mLinkPort);
    CPPUNIT_ASSERT(0                        == tArticle->mNumFluidConstituents);
    CPPUNIT_ASSERT(0.0                      == tArticle->mNodeMass);
    CPPUNIT_ASSERT(0                        == tArticle->mNodeMassFractions);
    CPPUNIT_ASSERT(0                        == tArticle->mNodeMoleFractions);
    CPPUNIT_ASSERT(0.0                      == tArticle->mNodeVolume);

    /// @test init flag
    CPPUNIT_ASSERT(false                    ==  tArticle->mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsFluidVolumeMonitor class.  This
///           also tests its stepPostSolver method since it is called by initialize.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidVolumeMonitor::testInitialize()
{
    std::cout << "\n UtGunnsFluidVolumeMonitor 04: testInitialize .......................";

    tArticle->initialize(tConfig, tInput);

    /// - Test nominal initialization.
    CPPUNIT_ASSERT(tName                 == tArticle->mName);
    CPPUNIT_ASSERT(tLinkPort             == tArticle->mLinkPort);
    CPPUNIT_ASSERT(2                     == tArticle->mNumFluidConstituents);
    CPPUNIT_ASSERT(0.0                    < tArticle->mNodeMass);
    CPPUNIT_ASSERT(tNodes[1].getMass()   == tArticle->mNodeMass);
    CPPUNIT_ASSERT(tNodes[1].getContent()->getMassFraction(FluidProperties::GUNNS_N2)
                                         == tArticle->mNodeMassFractions[0]);
    CPPUNIT_ASSERT(tNodes[1].getContent()->getMassFraction(FluidProperties::GUNNS_O2)
                                         == tArticle->mNodeMassFractions[1]);
    CPPUNIT_ASSERT(tNodes[1].getContent()->getMoleFraction(FluidProperties::GUNNS_N2)
                                         == tArticle->mNodeMoleFractions[0]);
    CPPUNIT_ASSERT(tNodes[1].getContent()->getMoleFraction(FluidProperties::GUNNS_O2)
                                         == tArticle->mNodeMoleFractions[1]);
    CPPUNIT_ASSERT(tNodes[1].getVolume() == tArticle->mNodeVolume);
    CPPUNIT_ASSERT(tArticle->mNodeTcMoleFractions[0] == 0.001);

    /// @test init flag
    CPPUNIT_ASSERT(true      == tArticle->mInitFlag);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the initialize method of the GunnsFluidVolumeMonitor with errors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidVolumeMonitor::testInitializeExceptions()
{
    std::cout << "\n UtGunnsFluidVolumeMonitor 05: testInitializeExceptions .............";

    /// - Test exception thrown from missing name.
    tConfig->mName = "";
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfig, tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    tConfig->mName = tName;

    /// - Test exception thrown from null config data.
    const GunnsFluidVolumeMonitorConfigData* nullConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(nullConfig, tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// - Test exception thrown from null input data.
    const GunnsFluidVolumeMonitorInputData* nullInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfig, nullInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);

    /// - Test exception thrown on bad config data pointer type.
    BadGunnsFluidVolumeMonitorConfigData* badConfig = new BadGunnsFluidVolumeMonitorConfigData(tName);
    CPPUNIT_ASSERT_THROW(tArticle->initialize(badConfig, tInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    delete badConfig;

    /// - Test exception thrown on bad input data pointer type.
    BadGunnsFluidVolumeMonitorInputData* badInput = new BadGunnsFluidVolumeMonitorInputData();
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tConfig, badInput), TsInitializationException);
    CPPUNIT_ASSERT(false == tArticle->mInitFlag);
    delete badInput;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPreSolver method of the GunnsFluidVolumeMonitor class.
///           This method has no asserts since the stepPreSolver method has no behavior, but we call
///           it for code coverage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidVolumeMonitor::testPreSolver()
{
    std::cout << "\n UtGunnsFluidVolumeMonitor 06: testPreSolver ........................";

    tArticle->initialize(tConfig, tInput);
    tArticle->stepPreSolver(tTimeStep);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the stepPostSolver method of the GunnsFluidVolumeMonitor class.  It
///           has already been mostly tested by the initialization test, so this method just tests
///           the few remaining behaviors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidVolumeMonitor::testPostSolver()
{
    std::cout << "\n UtGunnsFluidVolumeMonitor 07: testPostSolver .......................";

    tArticle->initialize(tConfig, tInput);

    /// - Test the user's link port selection is limited to the valid link ports range.
    tArticle->mLinkPort = -1;
    tArticle->stepPostSolver(tTimeStep);
    CPPUNIT_ASSERT(0 == tArticle->mLinkPort);

    tArticle->mLinkPort = 2;
    tArticle->stepPostSolver(tTimeStep);
    CPPUNIT_ASSERT(1 == tArticle->mLinkPort);

    tArticle->mLinkPort = 0;
    CPPUNIT_ASSERT_NO_THROW(tArticle->stepPostSolver(tTimeStep));
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the setter & getter methods of the GunnsFluidVolumeMonitor class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidVolumeMonitor::testAccessors()
{
    std::cout << "\n UtGunnsFluidVolumeMonitor 08: testAccessors ........................";

    /// - Set up initial values.
    tArticle->initialize(tConfig, tInput);
    tArticle->mNodeMass             = 5.0;
    tArticle->mNodeMassFractions[0] = 0.1;
    tArticle->mNodeMassFractions[1] = 0.9;
    tArticle->mNodeMoleFractions[0] = 0.2;
    tArticle->mNodeMoleFractions[1] = 0.8;
    tArticle->mNodeVolume           = 7.0;

    /// - Test the getMass method.
    CPPUNIT_ASSERT(tArticle->mNodeMass == tArticle->getMass());

    /// - Test the getVolume method.
    CPPUNIT_ASSERT(tArticle->mNodeVolume == tArticle->getVolume());

    /// - Test the gasMassFraction method.
    CPPUNIT_ASSERT(tArticle->mNodeMassFractions[0] == tArticle->getMassFraction(0));
    CPPUNIT_ASSERT(tArticle->mNodeMassFractions[1] == tArticle->getMassFraction(1));
    CPPUNIT_ASSERT(tArticle->mNodeMassFractions[0] == tArticle->getMassFraction(-1));
    CPPUNIT_ASSERT(tArticle->mNodeMassFractions[1] == tArticle->getMassFraction(2));

    /// - Test the gasMoleFraction method.
    CPPUNIT_ASSERT(tArticle->mNodeMoleFractions[0] == tArticle->getMoleFraction(0));
    CPPUNIT_ASSERT(tArticle->mNodeMoleFractions[1] == tArticle->getMoleFraction(1));
    CPPUNIT_ASSERT(tArticle->mNodeMoleFractions[0] == tArticle->getMoleFraction(-1));
    CPPUNIT_ASSERT(tArticle->mNodeMoleFractions[1] == tArticle->getMoleFraction(2));

    std::cout << "... Pass";
}
