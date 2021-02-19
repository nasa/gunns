#ifndef UtPowerBusElect_EXISTS
#define UtPowerBusElect_EXISTS

/*!******************************** TRICK HEADER ***************************

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
(
(This is the unit test class for the PowerBus model)
)

REFERENCE:
()

ASSUMPTIONS AND LIMITATIONS:
()
 *
LIBRARY DEPENDENCY:
(
  (UtPowerBusElect.o)
  (core/GunnsBasicJumper.o)
  (core/GunnsBasicSocket.o)
)

PROGRAMMERS:
    (
     ((Carlo Bocatto) (L-3 Comm) (5/20/2013) (TS21) (Initial Prototype))
    )

 ***************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "aspects/electrical/PowerBus/PowerBusElect.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "aspects/electrical/UserLoad/ResistiveLoad.hh"
#include "aspects/electrical/UserLoad/ConstantPowerLoad.hh"
 #include "core/GunnsBasicJumperPlug.hh"
 #include "core/GunnsBasicSocket.hh"

//test specific includes
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Create class to contain the test article loads.
///////////////////////////////////////////////////////////////////////////////////////////////////
class EpsTestLoads {
public:
    TS_MAKE_SIM_COMPATIBLE(EpsTestLoads);

    EpsTestLoads();
    virtual ~EpsTestLoads();

private:
    EpsTestLoads(const EpsTestLoads&);
    EpsTestLoads &operator = (const EpsTestLoads&);

public:
    ResistiveLoad testLoad1;
    ConstantPowerLoad testLoad2;
    ResistiveLoad testLoad3;

};

class EpsTestLoadsConfig{
    EpsTestLoadsConfig (const EpsTestLoadsConfig&);
    EpsTestLoadsConfig& operator = (const EpsTestLoadsConfig&);

public:
    EpsTestLoadsConfig();
    virtual ~EpsTestLoadsConfig();

    ResistiveLoadConfigData testLoad1Config;
    ConstantPowerLoadConfigData testLoad2Config;
    ResistiveLoadConfigData testLoad3Config;

};

class EpsTestLoadsInput {
    EpsTestLoadsInput(const EpsTestLoadsInput&);
    EpsTestLoadsInput& operator =(const EpsTestLoadsInput&);

public:
    TS_MAKE_SIM_COMPATIBLE(EpsTestLoadsInput);

    EpsTestLoadsInput();
    virtual ~EpsTestLoadsInput();

    ResistiveLoadInputData testLoad1Input;
    ConstantPowerLoadInputData testLoad2Input;
    ResistiveLoadInputData testLoad3Input;

};

class FriendlyPowerBus : public PowerBusElect {
public:
    FriendlyPowerBus();
    virtual ~FriendlyPowerBus();

    friend class UtPowerBusElect;
};

inline FriendlyPowerBus::FriendlyPowerBus() : PowerBusElect(){}
inline FriendlyPowerBus::~FriendlyPowerBus(){}



class TestPoweBusSocketList : public GunnsBasicJumperPlug {
    TS_MAKE_SIM_COMPATIBLE(TestPoweBusSocketList);
        public:
            /// @brief  Enumeration of the socket options for this plug.
            enum TestPoweBusSocketListEnum{
                testPB1Socket = 0,
                NONE = 1
            };


            TestPoweBusSocketListEnum mActiveConnection;       /**< (--) Currently connected socket */
            TestPoweBusSocketListEnum mConnectionRequest;      /**< (--) Connect to socket command  */
            TestPoweBusSocketListEnum mDisconnectionRequest;   /**< (--) Disconnect form socket command */
            /// @brief  Default constructs this jumper plug.
            TestPoweBusSocketList(const std::string name)
                :
                GunnsBasicJumperPlug(name, static_cast <int> (NONE)),
                mActiveConnection    (NONE),
                mConnectionRequest   (NONE),
                mDisconnectionRequest(NONE) {}
            /// @brief Default desctructs this jumper plug.
            virtual ~TestPoweBusSocketList() {}
            /// @brief Sets the active connection of this plug to the given socket index.
            /// @param[in] index (--) Index to the socket list enumeration.
            virtual void setActiveConnection    (const int index) {mActiveConnection     = static_cast <TestPoweBusSocketListEnum> (index);}
            /// @brief Sets the command to connected to the given socket index.
            /// @param[in] index (--) Index to the socket list enumeration.
            virtual void setConnectionRequest   (const int index) {mConnectionRequest    = static_cast <TestPoweBusSocketListEnum> (index);}
            /// @brief Sets the command to disconnect to the given socket index.
            /// @param[in] index (--) Index to the socket list enumeration.
            virtual void setDisconnectionRequest(const int index) {mDisconnectionRequest = static_cast <TestPoweBusSocketListEnum> (index);}
            /// @brief Returns the index of the currently connected socket.
            /// @return int (--) Index to the socket list enumeration.
            virtual int  getActiveConnection()     {return static_cast <int> (mActiveConnection);}
            /// @brief Returns the index of the socket the plug is commanded to connect to.
            /// @return int (--) Index to the socket list enumeration.
            virtual int  getConnectionRequest()    {return static_cast <int> (mConnectionRequest);}
            /// @brief Returns the index of the socket the plug is commanded to disconnect from.
            /// @return int (--) Index to the socket list enumeration.
            virtual int  getDisconnectionRequest() {return static_cast <int> (mDisconnectionRequest);}
            /// @brief Returns the index of no socket, the last index in the list.
            /// @return int (--) Index to the socket list enumeration.
            virtual int  getNoConnection()         {return static_cast <int> (NONE);}
};



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Electric Power Bus Link unit tests.
////
/// @details  This class provides the unit tests for the PowerBusElect within the CPPUnit framework.
///////////////////////////////////////////////////////////////////////////////////////////////////
class UtPowerBusElect: public CppUnit::TestFixture {
    UtPowerBusElect(const UtPowerBusElect&);
    UtPowerBusElect& operator = (const UtPowerBusElect&);

    CPPUNIT_TEST_SUITE(UtPowerBusElect);
    CPPUNIT_TEST(testCDName);
    CPPUNIT_TEST(testCDNumLoads);
    CPPUNIT_TEST(testCDCopyConstructor);
    CPPUNIT_TEST(testIDBlockFlag);
    CPPUNIT_TEST(testIDBlockValue);
    CPPUNIT_TEST(testIDMaxConductance);
    CPPUNIT_TEST(testIDCopyConstructor);
    CPPUNIT_TEST(testGoodDataInitialize);
    CPPUNIT_TEST(testZeroMinConductance);
    CPPUNIT_TEST(testBadNumLoadsInitialize);
    CPPUNIT_TEST(testBadMaxConductanceInitialize);
    CPPUNIT_TEST(testNullLoadsPointer);
    CPPUNIT_TEST(testInitializeAddedVector);
    CPPUNIT_TEST(testInitFlag);
    CPPUNIT_TEST(testDefConstBusVoltage);
    CPPUNIT_TEST(testDefConstInputCurrent);
    CPPUNIT_TEST(testDefConstResLoadConductance);
    CPPUNIT_TEST(testDefConstPwrLoadConductance);
    CPPUNIT_TEST(testDefConstActiveResLoadConductance);
    CPPUNIT_TEST(testDefConstActivePwrLoadconductance);
    CPPUNIT_TEST(testDefConstNonLinear);
    CPPUNIT_TEST(testZeroLoads);
    CPPUNIT_TEST(testOneLoad);
    CPPUNIT_TEST(testLargeLoadNumber);
    CPPUNIT_TEST(testNegativeNumberOfLoads);
    CPPUNIT_TEST(testSteppedBusVoltage);
    CPPUNIT_TEST(testOverMaxResLoadConductance);
    CPPUNIT_TEST(testUnderMinResLoadConductamce);
    CPPUNIT_TEST(testOverMaxPwrConductance);
    CPPUNIT_TEST(testBusLoadsHaveExpectedValues);
    CPPUNIT_TEST(testSteppedLoadVoltage);
    CPPUNIT_TEST(testSteppedBusInputCurrentFlowsToLoadsCorrectly);
    CPPUNIT_TEST(testPositveFluxComputeFlows);
    CPPUNIT_TEST(testSteppedIsNonLinear);
    CPPUNIT_TEST(testDisconnectionRequestToSocket);
    CPPUNIT_TEST(testConnectionRequestToSocket);

    CPPUNIT_TEST(stepTheModel);

    CPPUNIT_TEST_SUITE_END();

public:
    enum epsnodeNames{
            testNumNodes = 3
          };
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief    Gunns Electric Power Bus Initial Setup for unit tests.
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    /// @brief Nominal constructor
    UtPowerBusElect();

    /// @brief Nominal destructor
    virtual ~UtPowerBusElect();

    /// @brief Executes before each test.
    void setUp();

    /// @brief Executes after each test.
    void tearDown();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief    Gunns Electric Power Bus unit tests.
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    //Config Data tests
    void testCDName();

    void testCDNumLoads();

    void testCDCopyConstructor();

    //input Data tests
    void  testIDBlockFlag();

    void  testIDBlockValue();

    void  testIDMaxConductance();

    void testIDCopyConstructor();

    //test Initialize Method
    void testGoodDataInitialize();

    void testZeroMinConductance();

    void testBadNumLoadsInitialize();

    void testBadMaxConductanceInitialize();

    void testInitializeAddedVector();

    void testNullLoadsPointer();

    void testBusLoadsAreInitialized();

    void testInitFlag();

    //Default Construction tests
    void testDefConstBusVoltage();

    void testDefConstInputCurrent();

    void testDefConstResLoadConductance();

    void testDefConstPwrLoadConductance();

    void testDefConstActiveResLoadConductance();

    void testDefConstActivePwrLoadconductance();

    void testDefConstNonLinear();

    //Load number test
    void testZeroLoads();

    void testOneLoad();

    void testLargeLoadNumber();

    void testNegativeNumberOfLoads();


    //Update tests
    void testSteppedBusVoltage();

    void testOverMaxResLoadConductance();

    void testUnderMinResLoadConductamce();

    void testOverMaxPwrConductance();

    void testBusLoadsHaveExpectedValues();

    void testSteppedLoadVoltage();

    void testSteppedBusInputCurrentFlowsToLoadsCorrectly();

    void testPositveFluxComputeFlows();

    void testSteppedIsNonLinear();

    void testDisconnectionRequestToSocket();

    void testConnectionRequestToSocket();

    //step test for the model
    void stepTheModel();

    ///@brief Sets up the suite of tests for the PowerBusElect unit testing.

        FriendlyPowerBus* tObject;
        PowerBusElectConfigData* tConfigData;
        PowerBusElectInputData* tInputData;


        // Network Jumper Plugs
        TestPoweBusSocketList            tObjectPlug0; /**< (--) Node1IsolationJumper instance. */

        // create some test sockets
        GunnsBasicSocket    testPB1Socket; // (--) Network link
        GunnsBasicSocketConfigData   testPB1SocketConfig; // ** (--) Network link config data
        GunnsBasicSocketInputData    testPB1SocketInput; // ** (--) Network link input data

        EpsTestLoads tLoads;
        EpsTestLoadsConfig tLoadsConfig;
        EpsTestLoadsInput tLoadsInput;
        std::vector<UserLoadBase*> tNetworkLoads;
        UserLoadBase* tBusLoads[1][3];

        std::vector<GunnsBasicLink*> tLinks;

        GunnsNodeList tNodeList;
        GunnsBasicNode tNodes[testNumNodes];

        int* tPortMap;
        int tNumPorts;

        // class variable go here
        // config data
        std::string tName;
        int tNumLoads;

        //input data
        bool tMalfBlockageFlag;
        double tMalfBlockageValue;

        double tMaxConductance;
        double tMinConductance;

        // test tolerance for doubles
        double tTolerance;
        double tTimeStep;
};
///@}

#endif /*UtPowerBusElect_EXISTS*/
