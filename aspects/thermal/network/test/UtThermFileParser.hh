#ifndef UtThermFileParser_EXISTS
#define UtThermFileParser_EXISTS
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_PTCS_THERMAL_NETWORK    Thermal Network: ThermFileParser Unit Test
/// @ingroup  UT_PTCS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Thermal Network: ThermFileParser
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "aspects/thermal/network/ThermFileParser.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from ThermFileParser and befriend UtThermFileParser.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members. This class also befriends
///           the UtThermalNetwork class, which contains a FriendlyThermFileParser. That
///           way, UtThermalNetwork can access the protected members of a parser just as a regular
///           ThermalNetwork can.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyThermFileParser : public ThermFileParser
{
    public:
        FriendlyThermFileParser();
        FriendlyThermFileParser(const ThermFileParser& that);
        virtual ~FriendlyThermFileParser();
        friend class UtThermFileParser;
        friend class UtThermalNetwork;
};
inline FriendlyThermFileParser::FriendlyThermFileParser() :
        ThermFileParser() {};
inline FriendlyThermFileParser::~FriendlyThermFileParser() {}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ThermFileParser unit tests.
////
/// @details  Provides the unit tests for the ThermFileParser within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtThermFileParser: public CppUnit::TestFixture
{
    public:
        /// @brief  Default Constructor.
        UtThermFileParser();
        /// @brief  Default Destructor.
        virtual ~UtThermFileParser();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Tests for appropriate construction.
        void testConstruction();
        /// @brief  Tests for appropriate handling of off-nominal file parsing.
        void testParseErrorHandling();
        /// @brief  Tests for correct Node data from node-file.
        void testNode();
        /// @brief  Tests for correct Capacitance link data from node-file.
        void testCap();
        /// @brief  Tests for correct Conduction link data from cond-file.
        void testCond();
        /// @brief  Tests for correct Radiation link data from rad-file.
        void testRad();
        /// @brief  Tests for correct Heater data from htr-file.
        void testHtr();
        /// @brief  Tests for correct Panel data from pan-file.
        void testPan();
        /// @brief  Tests for correct Potential link data from pot-file.
        void testPot();
        /// @brief  Tests for correct Source link data from pot-file.
        void testSrc();
        /// @brief  Tests for correct edit of data by reading of ThermInput file.
        void testThermInput();

    private:
        CPPUNIT_TEST_SUITE(UtThermFileParser);
        CPPUNIT_TEST(testConstruction);
        CPPUNIT_TEST(testParseErrorHandling);
        CPPUNIT_TEST(testNode);
        CPPUNIT_TEST(testCap);
        CPPUNIT_TEST(testCond);
        CPPUNIT_TEST(testRad);
        CPPUNIT_TEST(testHtr);
        CPPUNIT_TEST(testPan);
        CPPUNIT_TEST(testPot);
        CPPUNIT_TEST(testSrc);
        CPPUNIT_TEST(testThermInput);
        CPPUNIT_TEST_SUITE_END();

        /// @brief  (s)  Nominal time step
        double tTimeStep;
        /// @brief  (--) Nominal tolerance for comparison of expected and returned values
        double tTol;
        /// @brief  (--) Default string for link name
        std::string tNameError;
        /// @brief  (--) Test Article
        FriendlyThermFileParser*  tArticle;

        std::string tNodeFile; /**< (--)  name of node config-file to parse */
        std::string tCondFile; /**< (--)  name of conduction config-file to parse */
        std::string tRadFile;  /**< (--)  name of radiation config-file to parse */
        std::string tHtrFile;  /**< (--)  name of heater config-file to parse */
        std::string tPanFile;  /**< (--)  name of panel config-file to parse */
        std::string tEtcFile;  /**< (--)  name of et.cetera config-file to parse */
        std::string tThermInputFile;   /**< (--)  name of thermal input-file to parse */

        /// - Node Attributes
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief  (--) Num of nodes listed in test case.
        int tNumNodes;
        /// @brief  (--) Index of the test node to which data will be compared.
        int tNode;
        /// @brief  (--) Name of the test node.
        std::string tNodeName;
        /// @brief  (--) Name of the space node.
        std::string tSpaceNode;

        /// - Capacitance Link Attributes
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief  (--) Num of capacitance links listed in test case.
        int tNumLinksCap;
        /// @brief  (--) Index of the test capacitance link to which data will be compared.
        int tCap;
        /// @brief  (--) Name of the test capacitance link.
        std::string tCapName;
        /// @brief  (K)  Initial Temperature of the test capacitance link.
        double tCapTemperature;
        /// @brief (J/K) Thermal Capacitance of the test capacitance link.
        double tCapCapacitance;
        /// @brief (--) Identifier for capacitance edit grouping.
        int tCapGroup;

        /// - Conduction Link Attributes
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief  (--) Num of conduction links listed in test case.
        int tNumLinksCond;
        /// @brief  (--) Index of the test conduction link to which data will be compared.
        int tCond;
        /// @brief  (--) Name of the test conduction link.
        std::string tCondName;
        /// @brief  (--) Name of the node at Port0 of the test conduction link.
        std::string tCondNode0;
        /// @brief  (--) Name of the node at Port1 of the test conduction link.
        std::string tCondNode1;
        /// @brief (W/K) Conduction conductivity of the test conduction link.
        double tCondConductivity;
        /// @brief  (--) Index of the test conduction link 2 to which data will be compared.
        int tCond2;
        /// @brief  (--) Name of the test conduction link 2.
        std::string tCond2Name;

        /// - Radiation Link Attributes
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief  (--) Num of radiation links listed in test case.
        int tNumLinksRad;
        /// @brief  (--) Index of the test radiation link to which data will be compared.
        int tRad;
        /// @brief  (--) Name of the test radiation link.
        std::string tRadName;
        /// @brief  (--) Name of the node at Port0 of the test radiation link.
        std::string tRadNode0;
        /// @brief  (--) Name of the node at Port1 of the test radiation link.
        std::string tRadNode1;
        /// @brief  (m2) Radiation coefficient of the test radiation link.
        double tRadCoefficient;

        /// - Heater A Link Attributes
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief  (--) Num of heater links listed in Heater/Source test case.
        int tNumLinksHtr;
        /// @brief  (--) Index of the test heater A link to which data will be compared.
        int tHtrA;
        /// @brief  (--) Name of the test heater A link.
        std::string tHtrAName;
        /// @brief  (--) Tuning Scalar of the test heater A link.
        double tHtrAScalar;
        /// @brief  (--) Number of ports for the test heater A link.
        int tHtrAPorts;
        /// @brief  (--) Name of the nodes of the test heater A link.
        std::string tHtrANode0;
        std::string tHtrANode1;
        std::string tHtrANode2;
        std::string tHtrANode3;
        std::string tHtrANode4;
        std::string tHtrANode5;
        /// @brief  (--) The flux-distribution fractions of the test heater A link.
        double tHtrAFrac0;
        double tHtrAFrac1;
        double tHtrAFrac2;
        double tHtrAFrac3;
        double tHtrAFrac4;
        double tHtrAFrac5;

        /// - Heater B Link Attributes
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief  (--)  Index of the test heater B link to which data will be compared.
        int tHtrB;
        /// @brief  (--) Name of the test heater B link.
        std::string tHtrBName;
        /// @brief  (--) Tuning Scalar of the test heater B link.
        double tHtrBScalar;
        /// @brief  (--) Number of ports for the test heater B link.
        int tHtrBPorts;
        /// @brief  (--)  Name of the nodes of the test heater B link.
        std::string tHtrBNode0;
        std::string tHtrBNode1;
        std::string tHtrBNode2;
        std::string tHtrBNode3;
        std::string tHtrBNode4;
        std::string tHtrBNode5;
        /// @brief  (--) The flux-distribution fractions of the test heater B link.
        double tHtrBFrac0;
        double tHtrBFrac1;
        double tHtrBFrac2;
        double tHtrBFrac3;
        double tHtrBFrac4;
        double tHtrBFrac5;

        /// - Panel Link Attributes
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief  (--) Num of thermal panel links listed in test case.
        int tNumLinksPan;
        /// @brief  (--) Index of the test thermal panel link to which data will be compared.
        int tPan;
        /// @brief  (--) Name of the test thermal panel link.
        std::string tPanName;
        /// @brief  (--) Number of ports of the test thermal panel link.
        int tPanPorts;
        /// @brief  (--) Name of the node at Port0 of the test thermal panel link.
        std::string tPanNode0;
        /// @brief  (--) Name of the node at Port1 of the test thermal panel link.
        std::string tPanNode1;
        /// @brief  (m2) Surface area of the test thermal panel link.
        double tPanArea;
        /// @brief  (--) Absorptivity of the test thermal panel link.
        double tPanAbsorptivity;

        /// - Potential Link Attributes
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief  (--) Num of potential links listed in test case.
        int tNumLinksPot;
        /// @brief  (--) Index of the test potential link to which data will be compared.
        int tPot;
        /// @brief  (--) Name of the test potential link.
        std::string tPotName;
        /// @brief  (--) Name of the node at Port1 of the test potential link.
        std::string tPotNode;
        /// @brief  (K) Temperature of the test potential link.
        double tPotTemperature;
        /// @brief  (W/K) Conductivity of the test potential link.
        double tPotConductivity;

        /// - Source Link Attributes
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief  (--) Num of source links listed in test case.
        int tNumLinksSrc;
        /// @brief  (--) Index of the test source link to which data will be compared.
        int tSrc;
        /// @brief  (--) Name of the test source link.
        std::string tSrcName;
        /// @brief  (--) Name of the nodes at the ports of the test source link.
        std::string tSrcNode0;
        std::string tSrcNode1;
        std::string tSrcNode2;
        std::string tSrcNode3;
        /// @brief  (--) The flux-distribution fractions of the test source link.
        double tSrcFrac0;
        double tSrcFrac1;
        double tSrcFrac2;
        double tSrcFrac3;
        /// @brief  (W)  Initial source flux (heat rate) of the test source link.
        double tSrcFlux;

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtThermFileParser(const UtThermFileParser& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtThermFileParser& operator =(const UtThermFileParser& that);
};
///@}
#endif /* UtThermFileParser_EXISTS */
