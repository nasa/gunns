/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsFluidLink.o)
    )
***************************************************************************************************/
#include "UtGunnsFluidLink.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsFluidLink class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidLink::UtGunnsFluidLink()
    :
    tConfigData(),
    tInputData(),
    tArticle(),
    tLinkName(),
    tNetworkNodes(),
    tNodeList(),
    tNetworkLinks(),
    tPortMap(),
    tFluidConfig(),
    tFluidProperties()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsFluidLink class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsFluidLink::~UtGunnsFluidLink()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::tearDown()
{
    /// - Deletes for news in setUp
    delete tFluidConfig;
    delete tArticle;
    delete tInputData;
    delete tConfigData;
    delete [] tPortMap;
    delete [] tNetworkNodes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::setUp()
{
    tLinkName = "Test Fluid Link";

    /// - Setup some fluid nodes
    tNetworkNodes = new FriendlyGunnsFluidNode[3];

    tPortMap = new int[2];
    tPortMap[0] = 0;
    tPortMap[1] = 1;
    tNodeList.mNumNodes = 3;
    tNodeList.mNodes    = tNetworkNodes;

    /// - Define nominal configuration data
    tConfigData   = new GunnsFluidLinkConfigData(tLinkName, &tNodeList);

    /// - Define nominal input data
    tInputData    = new GunnsFluidLinkInputData(true, 0.5);

    /// - Define nominal test article with two ports
    tArticle      = new FriendlyGunnsFluidLink(2);

    /// - Define fluid configuration
    static const int NumLiquidTypes = 1;
    static FluidProperties::FluidType LiquidTypesList[NumLiquidTypes] = {
            FluidProperties::GUNNS_O2
    };
    tFluidConfig = new PolyFluidConfigData(&tFluidProperties, LiquidTypesList, NumLiquidTypes);

    /// - Initialize fluids in the network nodes.
    tNetworkNodes[0].initialize("UtTestNode1", tFluidConfig);
    tNetworkNodes[1].initialize("UtTestNode2", tFluidConfig);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testConfig()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsFluidLink ....... 01: testConfig ............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tLinkName == tConfigData->mName);

    /// - Check default config construction
    GunnsFluidLinkConfigData defaultConfig;
    CPPUNIT_ASSERT("" == defaultConfig.mName);

    /// - Check copy config construction
    GunnsFluidLinkConfigData copyConfig(*tConfigData);
    CPPUNIT_ASSERT(tLinkName == copyConfig.mName);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for construction of input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testInput()
{
    std::cout << "\n UtGunnsFluidLink ....... 02: testInput .............................";

    /// - Check nominal config construction
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tInputData->mMalfBlockageValue, 0.0);

    /// - Check default config construction
    GunnsFluidLinkInputData defaultInput;
    CPPUNIT_ASSERT(!defaultInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultInput.mMalfBlockageValue, 0.0);

    /// - Check copy config construction
    GunnsFluidLinkInputData copyInput(*tInputData);
    CPPUNIT_ASSERT(tInputData->mMalfBlockageFlag == copyInput.mMalfBlockageFlag);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tInputData->mMalfBlockageValue, copyInput.mMalfBlockageValue, 0.0);

    std::cout << "... Pass";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsFluidLink class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testNominalConstruction()
{
    std::cout << "\n UtGunnsFluidLink ....... 03: testNominalConstruction ...............";

    /// - Verify initial member values.
    CPPUNIT_ASSERT(2   == tArticle->mNumPorts);
    CPPUNIT_ASSERT(0   == tArticle->mInternalFluid);
    CPPUNIT_ASSERT(0.0 == tArticle->mFlowRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mVolFlowRate);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for nominal initialization without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testNominalInitialization()
{
    std::cout << "\n UtGunnsFluidLink ....... 04: testNominalInitialization .............";

    /// - Construct and initialize (with nominal data) a test article
    FriendlyGunnsFluidLink* article = new FriendlyGunnsFluidLink(2);
    article->initialize(*tConfigData, *tInputData, tNetworkLinks, tPortMap);

    /// - Verify parent initialize method is called
    CPPUNIT_ASSERT(tLinkName == article->getName());
    CPPUNIT_ASSERT(tInputData->mMalfBlockageValue == article->mMalfBlockageValue);

    /// - Verify initial member values.
    CPPUNIT_ASSERT(0.0 == tArticle->mFlowRate);
    CPPUNIT_ASSERT(0.0 == tArticle->mVolFlowRate);

    /// @test init flag
    CPPUNIT_ASSERT(article->mInitFlag);

    delete article;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for initialization exceptions on invalid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testInitializationExceptions()
{
    std::cout << "\n UtGunnsFluidLink ....... 05: testInitializationExceptions ..........";

    /// - There is currently nothing to test

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests registration of the fluid node pointers.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testNodeRegistration()
{
    std::cout << "\n UtGunnsFluidLink ....... 06: testNodeRegistration ..................";

    const int portMap[2] = {0, 1};
    tArticle->initialize(*tConfigData, *tInputData, tNetworkLinks, &portMap[0]);

    CPPUNIT_ASSERT(&tNetworkNodes[0] == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNetworkNodes[1] == tArticle->mNodes[1]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the getNodeContent method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testGetNodeContent()
{
    std::cout << "\n UtGunnsFluidLink ....... 07: testGetNodeContent ....................";

    /// - Initialize the link.
    const int portMap[2] = {0, 1};
    tArticle->initialize(*tConfigData, *tInputData, tNetworkLinks, &portMap[0]);

    /// - Verify return of the port nodes content's address.
    CPPUNIT_ASSERT(tNetworkNodes[0].getContent() == tArticle->getNodeContent(0));
    CPPUNIT_ASSERT(tNetworkNodes[1].getContent() == tArticle->getNodeContent(1));

    /// - Verify exception if the port argument is out of range.
    CPPUNIT_ASSERT_THROW(tArticle->getNodeContent( 2), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(tArticle->getNodeContent(-1), TsOutOfBoundsException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the createInternalFluid method copying the internal fluid from node 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testCreateInternalFluidFromDefault()
{
    std::cout << "\n UtGunnsFluidLink ....... 08: testCreateInternalFluidFromDefault ....";

    /// - Initialize the link.
    const int portMap[2] = {0, 1};
    tArticle->initialize(*tConfigData, *tInputData, tNetworkLinks, &portMap[0]);

    /// - Verify creation of the internal fluid from the port0 node content.
    tNetworkNodes[0].setPotential(200.0);
    tNetworkNodes[0].resetFlows();
    tArticle->createInternalFluid();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNetworkNodes[0].getContent()->getDensity(),
                                 tArticle->mInternalFluid->getDensity(), 0.0);

    /// - Verify re-creation of the internal fluid.
    tNetworkNodes[0].setPotential(300.0);
    tNetworkNodes[0].resetFlows();
    tArticle->createInternalFluid();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNetworkNodes[0].getContent()->getDensity(),
                                 tArticle->mInternalFluid->getDensity(), 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the creation of the internal fluid with given input fluid properties data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testCreateInternalFluidFromInput()
{
    std::cout << "\n UtGunnsFluidLink ....... 09: testCreateInternalFluidFromInput ......";

    /// - Initialize the link.
    const int portMap[2] = {0, 1};
    tArticle->initialize(*tConfigData, *tInputData, tNetworkLinks, &portMap[0]);

    /// - Set up initial fluid properties data and verify the internal fluid receives it.
    double fractions[FluidProperties::NO_FLUID] = {1.0};
    PolyFluidInputData tFluidInit(   270.0,                // temperature
                                      1.0,                 // pressure
                                      0.0,                 // flowrate
                                      0.0,                 // mass
                                 fractions);               // massFraction
    tArticle->createInternalFluid(tFluidInit);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(270.0, tArticle->mInternalFluid->getTemperature(), 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the createInternalFluid method throwing an exception if neither port node has a
///           fluid that can be copied.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testCreateInternalFluidException()
{
    std::cout << "\n UtGunnsFluidLink ....... 10: testCreateInternalFluidException ......";

    /// - Initialize the link with both ports mapped to the vacuum node.
    const int portMap[2] = {2, 2};
    tArticle->initialize(*tConfigData, *tInputData, tNetworkLinks, &portMap[0]);

    /// - Verify an exception is thrown when createInternalFluid is called and no ports have a
    ///   valid node for copying.
    CPPUNIT_ASSERT_THROW(tArticle->createInternalFluid(), TsInitializationException);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Various tests of the transportFluid method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testTransportFluid()
{
    std::cout << "\n UtGunnsFluidLink ....... 11: testTransportFluid ....................";

    /// - Set up link port pressures and densities to test values.
    double pressure0 = 210.0;
    double pressure1 = 200.0;
    double temperature0 = 310.0;
    double temperature1 = 300.0;
    tNetworkNodes[0].setPotential(pressure0);
    tNetworkNodes[1].setPotential(pressure1);
    tNetworkNodes[0].getContent()->setTemperature(temperature0);
    tNetworkNodes[1].getContent()->setTemperature(temperature1);

    /// - Initialize the link.
    const int portMap[2] = {0, 1};
    tArticle->initialize(*tConfigData, *tInputData, tNetworkLinks, &portMap[0]);

    /// - Test positive flow, no internal fluid.
    tNetworkNodes[0].resetFlows();
    tNetworkNodes[1].resetFlows();
    tArticle->mFlowRate = 1.0;
    tArticle->transportFluid(false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tArticle->mFlowRate), tNetworkNodes[0].mOutfluxRate, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tArticle->mFlowRate), tNetworkNodes[1].mInfluxRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNetworkNodes[0].getContent()->getTemperature(),
                                 tNetworkNodes[1].getInflow()->getTemperature(), FLT_EPSILON);

    /// - Test negative flow, no internal fluid.
    tNetworkNodes[0].resetFlows();
    tNetworkNodes[1].resetFlows();
    tArticle->mFlowRate = -1.0;
    tArticle->transportFluid(false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tArticle->mFlowRate), tNetworkNodes[0].mInfluxRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tArticle->mFlowRate), tNetworkNodes[1].mOutfluxRate, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tNetworkNodes[1].getContent()->getTemperature(),
                                 tNetworkNodes[0].getInflow()->getTemperature(), FLT_EPSILON);

    /// - Test insufficient flow for transport, no internal fluid.
    tNetworkNodes[0].resetFlows();
    tNetworkNodes[1].resetFlows();
    tArticle->mFlowRate = 0.9 * DBL_EPSILON;
    tArticle->transportFluid(false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[0].mInfluxRate,                   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[1].mInfluxRate,                   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[0].mOutfluxRate,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[1].mOutfluxRate,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[0].getInflow()->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[1].getInflow()->getTemperature(), 0.0);

    /// - Now instantiate the internal fluid and give it values to test.
    tArticle->createInternalFluid();
    tArticle->mInternalFluid->setTemperature(320.0);

    /// - Test positive flow, internal fluid, no forced outflow.
    tNetworkNodes[0].resetFlows();
    tNetworkNodes[1].resetFlows();
    tArticle->mFlowRate = 1.0;
    tArticle->transportFluid(false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tArticle->mFlowRate), tNetworkNodes[0].mOutfluxRate, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tArticle->mFlowRate), tNetworkNodes[1].mInfluxRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInternalFluid->getTemperature(),
                                 tNetworkNodes[1].getInflow()->getTemperature(), FLT_EPSILON);

    /// - Test negative flow, internal fluid, no forced outflow.
    tNetworkNodes[0].resetFlows();
    tNetworkNodes[1].resetFlows();
    tArticle->mFlowRate = -1.0;
    tArticle->transportFluid(false);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tArticle->mFlowRate), tNetworkNodes[0].mInfluxRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(tArticle->mFlowRate), tNetworkNodes[1].mOutfluxRate, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInternalFluid->getTemperature(),
                                 tNetworkNodes[0].getInflow()->getTemperature(), FLT_EPSILON);

    /// - Test positive flow, internal fluid, forced outflow.
    tNetworkNodes[0].resetFlows();
    tNetworkNodes[1].resetFlows();
    tArticle->mFlowRate = 1.0;
    tArticle->transportFluid(true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(   -(tArticle->mFlowRate), tNetworkNodes[0].mInfluxRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(    (tArticle->mFlowRate), tNetworkNodes[1].mInfluxRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(                      0.0, tNetworkNodes[0].mOutfluxRate, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(                      0.0, tNetworkNodes[1].mOutfluxRate, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInternalFluid->getTemperature(),
                                 tNetworkNodes[0].getInflow()->getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInternalFluid->getTemperature(),
                                 tNetworkNodes[1].getInflow()->getTemperature(), FLT_EPSILON);

    /// - Test negative flow, internal fluid, forced outflow.
    tNetworkNodes[0].resetFlows();
    tNetworkNodes[1].resetFlows();
    tArticle->mFlowRate = -1.0;
    tArticle->transportFluid(true);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(   -(tArticle->mFlowRate), tNetworkNodes[0].mInfluxRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(    (tArticle->mFlowRate), tNetworkNodes[1].mInfluxRate,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(                      0.0, tNetworkNodes[0].mOutfluxRate, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(                      0.0, tNetworkNodes[1].mOutfluxRate, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInternalFluid->getTemperature(),
                                 tNetworkNodes[0].getInflow()->getTemperature(), FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mInternalFluid->getTemperature(),
                                 tNetworkNodes[1].getInflow()->getTemperature(), FLT_EPSILON);

    /// - Test insufficient flow for transport, internal fluid.
    tNetworkNodes[0].resetFlows();
    tNetworkNodes[1].resetFlows();
    tArticle->mFlowRate = 0.9 * DBL_EPSILON;
    tArticle->transportFluid(true, 0, 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[0].mInfluxRate,                   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[1].mInfluxRate,                   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[0].mOutfluxRate,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[1].mOutfluxRate,                  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[0].getInflow()->getTemperature(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNetworkNodes[1].getInflow()->getTemperature(), 0.0);

    /// - Call the empty updateFluid method for code coverage.
    tArticle->updateFluid(0.0, 0.0);

    /// - Test calls to transportFluid with invalid port arguments.
    tNetworkNodes[0].resetFlows();
    tNetworkNodes[1].resetFlows();
    tArticle->mFlowRate = 1.0;
    tArticle->transportFluid(false, -1, 1);
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[0].getOutflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[1].getOutflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[0].getInflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[1].getInflux());

    tArticle->transportFluid(false, 2, 1);
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[0].getOutflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[1].getOutflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[0].getInflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[1].getInflux());

    tArticle->transportFluid(false, 0, -1);
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[0].getOutflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[1].getOutflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[0].getInflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[1].getInflux());

    tArticle->transportFluid(false, 0, 2);
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[0].getOutflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[1].getOutflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[0].getInflux());
    CPPUNIT_ASSERT(0.0 == tNetworkNodes[1].getInflux());

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the getFlowRate method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testAccessMethods()
{
    std::cout << "\n UtGunnsFluidLink ....... 12: testAccessMethods .....................";
    /// - Initialize the link.
    const int portMap[2] = {0, 1};
    tArticle->initialize(*tConfigData, *tInputData, tNetworkLinks, &portMap[0]);

    /// - Set up initial fluid properties data.
    double fractions[FluidProperties::NO_FLUID] = {1.0};
    PolyFluidInputData tFluidInit(   270.0,                // temperature
                                      1.0,                 // pressure
                                      0.0,                 // flowrate
                                      0.0,                 // mass
                                 fractions);               // massFraction
    tArticle->createInternalFluid(tFluidInit);
    PolyFluid* tFluid = tArticle->mInternalFluid;
    CPPUNIT_ASSERT(tFluid==tArticle->getInternalFluid());
    tArticle->mFlowRate = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->getFlowRate(), 0.0);
    tArticle->mVolFlowRate = 1.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->getVolFlowRate(), 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the setPorts method for dynamic link mapping.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsFluidLink::testSetPort()
{
    std::cout << "\n UtGunnsBasicLink ....... 13: testSetPort ...........................";

    /// - Initialize the link and set its port potentials.
    const int portMap[2] = {0, 1};
    tArticle->initialize(*tConfigData, *tInputData, tNetworkLinks, &portMap[0]);

    /// - Re-map the link ports and verify.
    tArticle->setPort(0, 2);
    tArticle->setPort(1, 0);

    CPPUNIT_ASSERT(2 == tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(0 == tArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(&tNetworkNodes[2] == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tNetworkNodes[0] == tArticle->mNodes[1]);

    /// - Attempt a map that will violate port rules and verify it isn't changed.
    tArticle->setPort(0, 0);
    CPPUNIT_ASSERT(2 == tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(&tNetworkNodes[2] == tArticle->mNodes[0]);

    std::cout << "... Pass";
}
