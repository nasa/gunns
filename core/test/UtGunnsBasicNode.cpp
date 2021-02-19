/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (core/GunnsBasicNode.o)
    )
***************************************************************************************************/
#include <iostream>
#include <cmath>
#include <cfloat>

#include "UtGunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsBasicNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicNode::UtGunnsBasicNode()
    :
    tNode(),
    tNodeList()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsBasicNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsBasicNode::~UtGunnsBasicNode()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicNode::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicNode::setUp()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the default constructor of the GunnsBasicNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicNode::testDefaultConstruction()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n UtGunnsBasicNode ....... 01: testDefaultConstruction ...............";

    /// - Test the GunnsNodeList default construction.
    CPPUNIT_ASSERT(0      == tNodeList.mNumNodes);
    CPPUNIT_ASSERT(0      == tNodeList.mNodes);

    /// - Test the GunnsNodeList nominal construction.
    GunnsNodeList nodeList(1, &tNode);
    CPPUNIT_ASSERT(1      == nodeList.mNumNodes);
    CPPUNIT_ASSERT(&tNode == nodeList.mNodes);

    /// - Test the GunnsBasicNode constructor
    CPPUNIT_ASSERT(""     == tNode.mName);
    CPPUNIT_ASSERT(0.0    == tNode.mPotential);
    CPPUNIT_ASSERT(0.0    == tNode.mInfluxRate);
    CPPUNIT_ASSERT(0.0    == tNode.mOutfluxRate);
    CPPUNIT_ASSERT(0.0    == tNode.mNetFlux);
    CPPUNIT_ASSERT(0.0    == tNode.mFluxThrough);
    CPPUNIT_ASSERT(0.0    == tNode.mNetworkCapacitance);
    CPPUNIT_ASSERT(0.0    == tNode.mNetworkCapacitanceRequest);
    CPPUNIT_ASSERT(0      == tNode.mIslandVector);
    CPPUNIT_ASSERT(0      == tNode.mNetCapDeltaPotential);
    CPPUNIT_ASSERT(false  == tNode.mInitFlag);

    GunnsBasicNode* article = new GunnsBasicNode;
    delete article;

    std::cout << "... Pass";
}

void UtGunnsBasicNode::testInitialize()
{
    std::cout << "\n UtGunnsBasicNode ....... 02: testInitialize ........................";

    //Initialize Basic Node
    tNode.initialize("TestNode", 1.0);
    CPPUNIT_ASSERT(std::string("TestNode") == tNode.getName());
    CPPUNIT_ASSERT(1.0                     == tNode.mPotential);
    CPPUNIT_ASSERT(0.0                     == tNode.mInfluxRate);
    CPPUNIT_ASSERT(0.0                     == tNode.mOutfluxRate);
    CPPUNIT_ASSERT(0.0                     == tNode.mNetFlux);
    CPPUNIT_ASSERT(0.0                     == tNode.mFluxThrough);
    CPPUNIT_ASSERT(0.0                     == tNode.mNetworkCapacitance);
    CPPUNIT_ASSERT(0.0                     == tNode.mNetworkCapacitanceRequest);
    CPPUNIT_ASSERT(0                       == tNode.mIslandVector);
    CPPUNIT_ASSERT(0                       == tNode.mNetCapDeltaPotential);
    CPPUNIT_ASSERT(false                   == tNode.mInitFlag);

    std::cout << "... Pass";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the validate method of the GunnsBasicNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicNode::testValidate()
{
    std::cout << "\n UtGunnsBasicNode ....... 03: testValidate ..........................";

    //Initialize Basic Node
    tNode.initialize("TestNode");
    tNode.validate();
    CPPUNIT_ASSERT(tNode.mInitFlag == true);

    //Check to ensure an exception is thrown when the node name is not set during initialization
    tNode.mInitFlag = false;
    CPPUNIT_ASSERT_THROW(tNode.initialize(), TsInitializationException);
    CPPUNIT_ASSERT(tNode.mInitFlag       == false);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the access methods of GunnsBasicNode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicNode::testAccessMethods()
{
    std::cout << "\n UtGunnsBasicNode ....... 04: testAccessMethods .....................";

    tNode.initialize("TestNode");
    tNode.validate();
    CPPUNIT_ASSERT(true == tNode.isInitialized());

    tNode.setPotential(300.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(300.0, tNode.getPotential(),                 0.0);

    tNode.collectInflux(1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,   tNode.getInflux(),                    0.0);

    tNode.collectOutflux(2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,   tNode.mOutfluxRate,                   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,   tNode.getOutflux(),                   0.0);

    tNode.scheduleOutflux(3.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0,   tNode.mScheduledOutflux,              0.0);
    tNode.scheduleOutflux(3.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0,   tNode.getScheduledOutflux(),          0.0);

    tNode.mFluxThrough = 5.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0,   tNode.getFluxThrough(),               0.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   tNode.getNetworkCapacitanceRequest(), 0.0);
    tNode.setNetworkCapacitanceRequest();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,   tNode.mNetworkCapacitanceRequest,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,   tNode.getNetworkCapacitanceRequest(), 0.0);
    tNode.setNetworkCapacitanceRequest(2.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,   tNode.mNetworkCapacitanceRequest,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,   tNode.getNetworkCapacitanceRequest(), 0.0);

    tNode.setNetworkCapacitance(3.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0,   tNode.mNetworkCapacitance,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0,   tNode.getNetworkCapacitance(),        0.0);
    tNode.setNetworkCapacitance(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   tNode.mNetworkCapacitance,            0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,   tNode.getNetworkCapacitance(),        0.0);

    /// @test the node's island vector setter & getter methods.
    std::vector<int> tIsland;
    tIsland.push_back(2);
    tIsland.push_back(7);
    tNode.setIslandVector(&tIsland);
    CPPUNIT_ASSERT(&tIsland == tNode.mIslandVector);
    CPPUNIT_ASSERT(&tIsland == tNode.getIslandVector());
    const std::vector<int>* tPtr = tNode.getIslandVector();
    CPPUNIT_ASSERT(&tIsland == tPtr);
    CPPUNIT_ASSERT(7        == (*tPtr)[1]);

    /// @test the network capacitance delta potential array setter & getter methods.
    const double netCapDp[4] = {0.1, 0.2, 0.3, 0.4};
    tNode.setNetCapDeltaPotential(netCapDp);
    CPPUNIT_ASSERT_EQUAL(netCapDp[0], tNode.getNetCapDeltaPotential()[0]);
    CPPUNIT_ASSERT_EQUAL(netCapDp[1], tNode.getNetCapDeltaPotential()[1]);
    CPPUNIT_ASSERT_EQUAL(netCapDp[2], tNode.getNetCapDeltaPotential()[2]);
    CPPUNIT_ASSERT_EQUAL(netCapDp[3], tNode.getNetCapDeltaPotential()[3]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the resetFlows method of GunnsBasicNode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicNode::testResetFlows()
{
    std::cout << "\n UtGunnsBasicNode ....... 05: testResetFlows ........................";

    tNode.collectInflux (1.0);
    tNode.collectOutflux(2.0);
    tNode.resetFlows();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.mInfluxRate,                0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.mOutfluxRate,               0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the integrateFlows method of the GunnsBasicNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicNode::testIntegrateFlows()
{
    std::cout << "\n UtGunnsBasicNode ....... 06: testIntegrateFlows ....................";

    /// - Test case where inflow is greater than outflow
    double inflow  = 2.0;
    double outflow = 1.0;
    tNode.collectInflux (inflow);
    tNode.collectOutflux(outflow);
    tNode.integrateFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL((inflow - outflow), tNode.getNetFlux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(outflow,            tNode.mFluxThrough, DBL_EPSILON);
    tNode.resetFlows();

    /// - Test case where outflow is greater than inflow
    inflow  = 1.0;
    outflow = 2.0;
    tNode.collectInflux (inflow);
    tNode.collectOutflux(outflow);
    tNode.integrateFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL((inflow - outflow), tNode.getNetFlux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(inflow,             tNode.mFluxThrough, DBL_EPSILON);
    tNode.resetFlows();

    /// - Test case where inflow and outflow are equal
    inflow  = 2.0;
    outflow = 2.0;
    tNode.collectInflux (inflow);
    tNode.collectOutflux(outflow);
    tNode.integrateFlows(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL((inflow - outflow), tNode.getNetFlux(), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(outflow,            tNode.mFluxThrough, DBL_EPSILON);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the placeholder methods for derived classes in GunnsBasicNode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicNode::testPlaceholderMethods()
{
    std::cout << "\n UtGunnsBasicNode ....... 07: testPlaceholderMethods ................";

    const PolyFluid tFluid;
    const PolyFluidConfigData tFluidConfig;

    /// - Call all the dummy methods that don't do or return anything to satisfy code coverage.
    tNode.updateMass();
    tNode.prepareForStart();
    tNode.resetContentState();
    tNode.setVolume(0.0);
    tNode.initVolume(0.0);
    tNode.collectHeatFlux(0.0);
    tNode.setThermalDampingMass(0.0);
    tNode.setExpansionScaleFactor(0.0);
    tNode.setUndampedHeatFlux(0.0);
    tNode.collectInflux(0.0, &tFluid );

    /// - Call the dummy methods that return zero.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.computeThermalCapacitance(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.computeCompression(),        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.getVolume(),                 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.getMass(),                   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tNode.computePressureCorrection(), 0.0);
    CPPUNIT_ASSERT(0 == tNode.getContent());
    CPPUNIT_ASSERT(0 == tNode.getInflow());
    CPPUNIT_ASSERT(0 == tNode.getFluidConfig());
    CPPUNIT_ASSERT(0 == tNode.getOutflow());
    CPPUNIT_ASSERT(false == tNode.isOverflowing(0.0));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method tests the restart method of the GunnsBasicNode class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsBasicNode::testRestart()
{
    std::cout << "\n UtGunnsFluidNode ....... 08: testRestart ...........................";

    /// - Load up the node with some non-zero values, then restart it and verify stuff got reset.
    tNode.initialize("TestNode");

    std::vector<int> tIsland;
    tIsland.push_back(2);
    tIsland.push_back(7);
    tNode.setIslandVector(&tIsland);
    tNode.mInfluxRate                = 1.0;
    tNode.mOutfluxRate               = 2.0;
    tNode.mNetFlux                   = 3.0;
    tNode.mFluxThrough               = 4.0;
    tNode.mNetworkCapacitanceRequest = 5.0;

    tNode.restart();

    CPPUNIT_ASSERT(0.0 == tNode.mInfluxRate);
    CPPUNIT_ASSERT(0.0 == tNode.mOutfluxRate);
    CPPUNIT_ASSERT(0.0 == tNode.mNetFlux);
    CPPUNIT_ASSERT(0.0 == tNode.mFluxThrough);
    CPPUNIT_ASSERT(0.0 == tNode.mNetworkCapacitanceRequest);
    CPPUNIT_ASSERT(0   == tNode.mIslandVector);

    std::cout << "... Pass";
}
