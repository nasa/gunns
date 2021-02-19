/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/thermal/network/ThermFileParser.o))
***************************************************************************************************/
#include "UtThermFileParser.hh"
#include "software/exceptions/TsParseException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include <iostream>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtThermFileParser class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtThermFileParser::UtThermFileParser()
    :
    tTimeStep(),
    tTol(),
    tNameError(),
    tArticle(),
    tNodeFile(),
    tCondFile(),
    tRadFile(),
    tHtrFile(),
    tPanFile(),
    tEtcFile(),
    tThermInputFile(),
    tNumNodes(),
    tNode(),
    tNodeName(),
    tSpaceNode(),
    tNumLinksCap(),
    tCap(),
    tCapName(),
    tCapTemperature(),
    tCapCapacitance(),
    tCapGroup(),
    tNumLinksCond(),
    tCond(),
    tCondName(),
    tCondNode0(),
    tCondNode1(),
    tCondConductivity(),
    tCond2(),
    tCond2Name(),
    tNumLinksRad(),
    tRad(),
    tRadName(),
    tRadNode0(),
    tRadNode1(),
    tRadCoefficient(),
    tNumLinksHtr(),
    tHtrA(),
    tHtrAName(),
    tHtrAScalar(),
    tHtrAPorts(),
    tHtrANode0(),
    tHtrANode1(),
    tHtrANode2(),
    tHtrANode3(),
    tHtrANode4(),
    tHtrANode5(),
    tHtrAFrac0(),
    tHtrAFrac1(),
    tHtrAFrac2(),
    tHtrAFrac3(),
    tHtrAFrac4(),
    tHtrAFrac5(),
    tHtrB(),
    tHtrBName(),
    tHtrBScalar(),
    tHtrBPorts(),
    tHtrBNode0(),
    tHtrBNode1(),
    tHtrBNode2(),
    tHtrBNode3(),
    tHtrBNode4(),
    tHtrBNode5(),
    tHtrBFrac0(),
    tHtrBFrac1(),
    tHtrBFrac2(),
    tHtrBFrac3(),
    tHtrBFrac4(),
    tHtrBFrac5(),
    tNumLinksPan(),
    tPan(),
    tPanName(),
    tPanPorts(),
    tPanNode0(),
    tPanNode1(),
    tPanArea(),
    tPanAbsorptivity(),
    tNumLinksPot(),
    tPot(),
    tPotName(),
    tPotNode(),
    tPotTemperature(),
    tPotConductivity(),
    tNumLinksSrc(),
    tSrc(),
    tSrcName(),
    tSrcNode0(),
    tSrcNode1(),
    tSrcNode2(),
    tSrcNode3(),
    tSrcFrac0(),
    tSrcFrac1(),
    tSrcFrac2(),
    tSrcFrac3(),
    tSrcFlux()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtThermFileParser class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtThermFileParser::~UtThermFileParser()
{
    //do nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::setUp()
{
    /// - test technical data
    tTol = 0.0001;
    tTimeStep = 0.1;
    tNameError = "(error setting name)";

    /// - Declare the config-files.
    tNodeFile = "ThermNodes_base.xml";
    tCondFile = "ThermLinksCond_base.xml";
    tRadFile = "ThermLinksRad_base.xml";
    tHtrFile = "HtrRegistry_base.xml";
    tPanFile = "ThermLinksPan_base.xml";
    tEtcFile = "ThermLinksEtc_base.xml";

    /// - Declare the input-files.
    tThermInputFile = "ThermInput_base.xml";

    /// - nominal node data from the test case
    tNumNodes = 22;
    tNode = 15;
    tNodeName = "CMRCSNR1_20";
    tSpaceNode = "SPACE_1";

    /// - nominal capacitance link data from the test case
    tNumLinksCap = 21;
    tCap = 6;
    tCapName = "cap_CMBCKSHL_13330";
    tCapTemperature = 455;
    tCapCapacitance = 4722.05;
    tCapGroup = 1;

    /// - nominal conduction link data from the test case
    tNumLinksCond = 8;
    tCond = 5;
    tCondNode0 = "CMPRPHYT_2";
    tCondNode1 = "CMPRPHYT_20";
    tCondName = "cond " + tCondNode0 + " to " + tCondNode1;
    tCondConductivity = 0.40;
    tCond2 = 6;
    tCond2Name = "hydrazine tank bracket";

    /// - nominal radiation link data from the test case
    tNumLinksRad = 17;
    tRad = 7;
    tRadNode0 = "CMBCKSHL_13330";
    tRadNode1 = "CMHS_62803";
    tRadName = "rad " + tRadNode0 + " to " + tRadNode1;
    tRadCoefficient = 53.1;

    /// - nominal ThermalPanel data from the test case
    tNumLinksPan = 3;
    tPan = 0;
    tPanName = "shell";
    tPanPorts = 2;
    tPanNode0 = "CMBCKSHL_13330";
    tPanNode1 = "CMHS_48233";
    tPanArea = 14.531;
    tPanAbsorptivity = 0.43;

    /// - nominal heater A data from the test case
    tNumLinksHtr = 3;
    tHtrA = 0;
    tHtrAName = "eclss heater";
    tHtrAScalar = 0.93;
    tHtrAPorts = 3;
    tHtrANode0 = "CECLSS_46";
    tHtrANode1 = "CECLSS_26";
    tHtrANode2 = "CECLSS_161";
    tHtrAFrac0 = 0.46;
    tHtrAFrac1 = 0.46;
    tHtrAFrac2 = 0.08;

    /// - nominal heater B data from the test case
    tHtrB = 2;
    tHtrBName = "tank heater";
    tHtrBScalar = 0.99;
    tHtrBPorts = 2;
    tHtrBNode0 = "CMPRPHET_1";
    tHtrBNode1 = "CMPRPHET_100";
    tHtrBFrac0 = 0.5;
    tHtrBFrac1 = 0.5;

    /// - nominal potential link data from the test case
    tNumLinksPot = 2;
    tPot = 1;
    tPotName = "pressure vessel";
    tPotNode = "CPV_5380";
    tPotTemperature = 318;
    tPotConductivity = 1e12;

    /// - nominal source link data from the test case
    tNumLinksSrc = 2;
    tSrc = 1;
    tSrcName = "multi-node source";
    tSrcNode0 = "CMAFTGUS_30";
    tSrcNode1 = "CMAFTGUS_31";
    tSrcNode2 = "CRRMECH_200";
    tSrcNode3 = "CRRMECH_300";
    tSrcFrac0 = 0.3;
    tSrcFrac1 = 0.3;
    tSrcFrac2 = 0.2;
    tSrcFrac3 = 0.2;
    tSrcFlux = 100;

    /// - Create the test article.
    tArticle = new FriendlyThermFileParser();

    /// - Simulate the setting of config-files by ThermalNetwork.
    tArticle->mNodeFile = tNodeFile;
    tArticle->mCondFile = tCondFile;
    tArticle->mRadFile = tRadFile;
    tArticle->mHtrFile = tHtrFile;
    tArticle->mPanFile = tPanFile;
    tArticle->mEtcFile = tEtcFile;

    /// - Read files!
    tArticle->initialize("article_nominal");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for appropriate construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testConstruction()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n ThermFileParser 01: Testing default construction ..............";

    /// - Default construct an un-initialized test article.
    FriendlyThermFileParser article;

    /// @test  Logistic data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("NOT_FOUND", -99, article.NOT_FOUND );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("areNodesRegistered", false, article.areNodesRegistered );
    CPPUNIT_ASSERT_MESSAGE("mName", "" == article.mName );

    /// @test  Node data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("numNodes",   0, article.numNodes );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vNodeNames", 0, static_cast<int>(article.vNodeNames.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("mNodeMap",   0, static_cast<int>(article.mNodeMap.size()) );

    /// @test  Capacitance link data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("numLinksCap", 0, article.numLinksCap );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vCapNames",   0, static_cast<int>(article.vCapNames.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vCapTemperatures", 0, static_cast<int>(article.vCapTemperatures.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vCapCapacitances", 0, static_cast<int>(article.vCapCapacitances.size()) );

    /// @test  Conduction link data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("numLinksCond", 0, article.numLinksCond );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vCondNames",   0, static_cast<int>(article.vCondNames.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vCondPorts0",  0, static_cast<int>(article.vCondPorts0.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vCondPorts1",  0, static_cast<int>(article.vCondPorts1.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vCondConductivities", 0, static_cast<int>(article.vCondConductivities.size()) );

    /// @test  Radiation link data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("numLinksRad", 0, article.numLinksRad );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vRadNames",   0, static_cast<int>(article.vRadNames.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vRadPorts0",  0, static_cast<int>(article.vRadPorts0.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vRadPorts1",  0, static_cast<int>(article.vRadPorts1.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vRadCoefficients", 0, static_cast<int>(article.vRadCoefficients.size()) );

    /// @test  Heater link data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("numLinksHtr", 0, article.numLinksHtr );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vHtrNames",   0, static_cast<int>(article.vHtrNames.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vHtrScalars", 0, static_cast<int>(article.vHtrScalars.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vHtrPorts",   0, static_cast<int>(article.vHtrPorts.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vHtrFracs",   0, static_cast<int>(article.vHtrFracs.size()) );

    /// @test  Panel data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("numLinksPan", 0, article.numLinksPan );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vPanNames", 0, static_cast<int>(article.vPanNames.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vPanAbsorptivities", 0, static_cast<int>(article.vPanAbsorptivities.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vPanPorts", 0, static_cast<int>(article.vPanPorts.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vPanFracs", 0, static_cast<int>(article.vPanFracs.size()) );

    /// @test  Potential link data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("numLinksPot", 0, article.numLinksPot );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vPotNames", 0, static_cast<int>(article.vPotNames.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vPotPorts", 0, static_cast<int>(article.vPotPorts.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vPotTemperatures", 0, static_cast<int>(article.vPotTemperatures.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vPotConductivities", 0, static_cast<int>(article.vPotConductivities.size()) );

    /// @test  Source link data
    CPPUNIT_ASSERT_EQUAL_MESSAGE("numLinksSrc", 0, article.numLinksSrc );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vSrcNames", 0, static_cast<int>(article.vSrcNames.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vSrcInits", 0, static_cast<int>(article.vSrcInits.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vSrcPorts", 0, static_cast<int>(article.vSrcPorts.size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("vSrcFracs", 0, static_cast<int>(article.vSrcFracs.size()) );

    std::cout << " Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for appropriate handling of off-nominal file parsing.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testParseErrorHandling()
{
    std::cout << "\n ThermFileParser 02: Testing off-nominal file parsing...........";

    /// - Default construct an un-initialized test article.
    FriendlyThermFileParser article;

    /// - Create a dummy TiXmlDocument.
    TiXmlDocument doc;

    std::string noFile = "no_file.net";
    std::string nonXmlFile = "main.cpp";
    std::string illFormedXml = "ThermNodes_illformed.xml";

    /// @test  Exception thrown on attempt to build links before nodes.
    CPPUNIT_ASSERT_THROW_MESSAGE("file doesn't exist", article.openFile(doc, noFile), TsParseException);
    /// @test  Exception thrown on article given a random file that has no thermal link info.
    CPPUNIT_ASSERT_THROW_MESSAGE("non-XML file",  article.openFile(doc, nonXmlFile), TsParseException);
    /// @test  Exception thrown on article with ill-formed XML
    CPPUNIT_ASSERT_THROW_MESSAGE("ill-formed XML", article.openFile(doc, illFormedXml), TsParseException);

    /// @test  Exception thrown on article with invalid tags
    article.mNodeFile = "ThermNodes_nolist.xml";
    CPPUNIT_ASSERT_THROW_MESSAGE("no <list>", article.readNodeFile(), TsParseException);
    article.mNodeFile = "ThermLinksCond_base.xml";
    CPPUNIT_ASSERT_THROW_MESSAGE("no <node>", article.readNodeFile(), TsParseException);

    /// @test  Exception NOT thrown on article with empty <name> tag
    article.mNodeFile = "ThermNodes_blankname.xml";
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("empty <name> tag", article.readNodeFile() );
    /// @test  Exception NOT thrown on article with no space node (just a warning).
    article.mNodeFile = "ThermNodes_nospace.xml";
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("no space node", article.readNodeFile() );


    /// @test  Exception NOT thrown on article with non-numerical info where needed (just warning).
    article.mNodeFile = "ThermNodes_nonnumeric.xml";
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("non-numerical info", article.readNodeFile() );

    /// @test  Capacitance link #0 had a string in the <capacitance> tag, so its
    ///        value should default to 0.0.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.vCapCapacitances.at(0), tTol);

    /// - Successfully read node-file first before executing remaining tests.
    article.mNodeFile = tNodeFile;
    article.readNodeFile();

    /// @test   Exception thrown on article given the wrong file by accident.
    article.mCondFile = "ThermLinksRad_base.xml";
    CPPUNIT_ASSERT_THROW_MESSAGE("cond-file with no links", article.readCondFile(), TsParseException);
    article.mRadFile = "ThermLinksCond_base.xml";
    CPPUNIT_ASSERT_THROW_MESSAGE("rad-file with no links", article.readRadFile(), TsParseException);
    article.mHtrFile = "ThermLinksPan_base.xml";
    CPPUNIT_ASSERT_THROW_MESSAGE("htr-file with no links", article.readHtrFile(), TsParseException);
    article.mPanFile = "ThermLinksHtr_base.xml";
    CPPUNIT_ASSERT_THROW_MESSAGE("pan-file with no links", article.readPanFile(), TsParseException);
    article.mEtcFile = "ThermLinksHtr_base.xml";
    CPPUNIT_ASSERT_THROW_MESSAGE("etc-file with no links", article.readEtcFile(), TsParseException);

    /// @test  Exception thrown on article initialized without config set in default data.
    CPPUNIT_ASSERT_THROW_MESSAGE("initialize without config set", article.initialize(tNameError), TsParseException);

    /// - Default construct an un-initialized test article.
    FriendlyThermFileParser article1;
    /// - Set NodeFile only.
    article1.mNodeFile = tNodeFile;
    /// @test  Exception NOT thrown on article with all files null except for the NodeFile.
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("all files null except nodes", article1.initialize(tNameError) );

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct build of Node data from node-file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testNode()
{
    std::cout << "\n ThermFileParser 03: Testing node data read from config-file....";

    /// @test  Node data from the test case was built correctly.
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Node amount", tNumNodes, tArticle->numNodes);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Node name", tNodeName, tArticle->vNodeNames.at(tNode) );

    /// @test  Node map was constructed correctly.
    CPPUNIT_ASSERT_EQUAL( 0, tArticle->getMapLocation(tHtrANode1) );
    CPPUNIT_ASSERT_EQUAL( 6, tArticle->getMapLocation(tPanNode0) );
    CPPUNIT_ASSERT_EQUAL(21, tArticle->getMapLocation(tSpaceNode) );

    /// - Declare an invalid node string.
    std::string fakeName = "FAKE_123";

    /// @test  An invalid node name should return NOT_FOUND.
    CPPUNIT_ASSERT_EQUAL(ThermFileParser::NOT_FOUND, tArticle->getMapLocation(fakeName));

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct build of Capacitance link data from node-file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testCap()
{
    std::cout << "\n ThermFileParser 04: Testing capacitance link, index " << tCap
            << " .........";

    /// @test  capacitance link data from the test case
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cap amount", tNumLinksCap, tArticle->numLinksCap);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cap name", tCapName, tArticle->vCapNames.at(tCap) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cap Port", tCap, tArticle->vCapPorts.at(tCap));
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cap capacitance", tCapCapacitance, tArticle->vCapCapacitances.at(tCap), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cap temperature", tCapTemperature, tArticle->vCapTemperatures.at(tCap), tTol);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cap group id", tCapGroup, tArticle->vCapEditGroupIdentifiers.at(tCap) );

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct build of Conduction link data from cond-file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testCond()
{
    std::cout << "\n ThermFileParser 05: Testing conduction link, index " << tCond
            << " ..........";

    /// @test  conduction link data from the test case
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Conduction amount", tNumLinksCond, tArticle->numLinksCond);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cond given name", tCondName, tArticle->vCondNames.at(tCond) );
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Cond conductivity", tCondConductivity, tArticle->vCondConductivities.at(tCond), tTol);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cond Port 0", tArticle->getMapLocation(tCondNode0), tArticle->vCondPorts0.at(tCond));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cond Port 1", tArticle->getMapLocation(tCondNode1), tArticle->vCondPorts1.at(tCond));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Cond generic name", tCond2Name, tArticle->vCondNames.at(tCond2) );

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct build of Radiation link data from rad-file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testRad()
{
    std::cout << "\n ThermFileParser 06: Testing radiation link, index " << tRad
            << " ...........";

    /// @test  radiation link data from the test case
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Rad amount", tNumLinksRad, tArticle->numLinksRad);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Rad name", tRadName, tArticle->vRadNames.at(tRad) );
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Rad coefficient", tRadCoefficient, tArticle->vRadCoefficients.at(tRad), tTol);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Rad port 0", tArticle->getMapLocation(tRadNode0), tArticle->vRadPorts0.at(tRad));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Rad port 1", tArticle->getMapLocation(tRadNode1), tArticle->vRadPorts1.at(tRad));

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct build of Heater data from htr-file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testHtr()
{
    std::cout << "\n ThermFileParser 07: Testing heaters, index "
            << tHtrA << " and "<< tHtrB << " ............";

    /// @test  heater data from the test case
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Htr amount", tNumLinksHtr, tArticle->numLinksHtr);

    /// @test  heater A name
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrA name", tHtrAName, tArticle->vHtrNames.at(tHtrA) );
    /// @test  heater A tuning scalar
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("HtrA scalar", tHtrAScalar, tArticle->vHtrScalars[tHtrA], tTol);
    /// @test  heater A ports
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrA ports", tHtrAPorts, static_cast<int>(tArticle->vHtrFracs.at(tHtrA).size()) );
    /// @test  heater A port numbers
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrA port0 node", tArticle->getMapLocation(tHtrANode0), tArticle->vHtrPorts.at(tHtrA).at(0) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrA port1 node", tArticle->getMapLocation(tHtrANode1), tArticle->vHtrPorts.at(tHtrA).at(1) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrA port2 node", tArticle->getMapLocation(tHtrANode2), tArticle->vHtrPorts.at(tHtrA).at(2) );
    /// @test  heater A power-draw fractions
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrA port0 frac", tHtrAFrac0, tArticle->vHtrFracs.at(tHtrA).at(0) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrA port1 frac", tHtrAFrac1, tArticle->vHtrFracs.at(tHtrA).at(1) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrA port2 frac", tHtrAFrac2, tArticle->vHtrFracs.at(tHtrA).at(2) );
    CPPUNIT_ASSERT_THROW_MESSAGE("HtrA port3 frac", tArticle->vHtrFracs.at(tHtrA).at(3), std::out_of_range );

    /// @test  heater B name
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrB name", tHtrBName,    tArticle->vHtrNames.at(tHtrB) );
    /// @test  heater B tuning scalar
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("HtrB scalar", tHtrBScalar, tArticle->vHtrScalars[tHtrB], tTol);
    /// @test  heater B ports
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrB ports", tHtrBPorts, static_cast<int>(tArticle->vHtrFracs.at(tHtrB).size()) );
    /// @test  heater B port numbers
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrB port0 node", tArticle->getMapLocation(tHtrBNode0), tArticle->vHtrPorts.at(tHtrB).at(0) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrB port1 node", tArticle->getMapLocation(tHtrBNode1), tArticle->vHtrPorts.at(tHtrB).at(1) );
    /// @test  heater B power-draw fractions
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrB port0 frac", tHtrBFrac0, tArticle->vHtrFracs.at(tHtrB).at(0) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("HtrB port1 frac", tHtrBFrac1, tArticle->vHtrFracs.at(tHtrB).at(1) );
    CPPUNIT_ASSERT_THROW_MESSAGE("HtrB port2 frac", tArticle->vHtrFracs.at(tHtrB).at(2), std::out_of_range );

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct build of ThermalPanel data from pan-file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testPan()
{
    std::cout << "\n ThermFileParser 08: Testing thermal panel, index " << tPan
            << " ............";

    /// @test  heater data from the test case
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Pan amount", tNumLinksPan, tArticle->numLinksPan);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Pan name", tNameError, tArticle->vPanNames.at(2) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Pan name", tPanName, tArticle->vPanNames.at(tPan) );
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Pan absorptivity", tPanAbsorptivity, tArticle->vPanAbsorptivities.at(tPan),  tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Pan area", tPanArea,  tArticle->vPanAreas.at(tPan),    tTol);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Pan ports", tPanPorts, static_cast<int>(tArticle->vPanFracs.at(tPan).size()) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Pan port0", tArticle->getMapLocation(tPanNode0), tArticle->vPanPorts.at(tPan).at(0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Pan port1", tArticle->getMapLocation(tPanNode1), tArticle->vPanPorts.at(tPan).at(1));

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct build of Potential link data from etc-file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testPot()
{
    std::cout << "\n ThermFileParser 10: Testing potential link, index " << tPot
            << " ...........";

    /// @test  Potential link data from the test case
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Pot amount", tNumLinksPot, tArticle->numLinksPot);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Pot name", tPotName, tArticle->vPotNames.at(tPot) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Pot node", tArticle->getMapLocation(tPotNode), tArticle->vPotPorts.at(tPot));
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Pot temperature", tPotTemperature, tArticle->vPotTemperatures.at(tPot), tTol);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Pot conductivity", tPotConductivity, tArticle->vPotConductivities.at(tPot) );

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct build of Source link data from etc-file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testSrc()
{
    std::cout << "\n ThermFileParser 11: Testing source link, index " << tSrc
            << " ..............";

    /// @test  Sources data from the test case
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Src amount", tNumLinksSrc, tArticle->numLinksSrc);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Src port0", tArticle->getMapLocation(tSrcNode0), tArticle->vSrcPorts.at(tSrc).at(0));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Src port1", tArticle->getMapLocation(tSrcNode1), tArticle->vSrcPorts.at(tSrc).at(1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Src port2", tArticle->getMapLocation(tSrcNode2), tArticle->vSrcPorts.at(tSrc).at(2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Src port3", tArticle->getMapLocation(tSrcNode3), tArticle->vSrcPorts.at(tSrc).at(3));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Src port0 frac", tSrcFrac0, tArticle->vSrcFracs.at(tSrc).at(0) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Src port1 frac", tSrcFrac1, tArticle->vSrcFracs.at(tSrc).at(1) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Src port2 frac", tSrcFrac2, tArticle->vSrcFracs.at(tSrc).at(2) );
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Src port3 frac", tSrcFrac3, tArticle->vSrcFracs.at(tSrc).at(3) );
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Src flux", tSrcFlux, tArticle->vSrcInits.at(tSrc), tTol);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Src name", tSrcName, tArticle->vSrcNames.at(tSrc) );

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for correct edit of data by reading of ThermInput file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtThermFileParser::testThermInput()
{
    std::cout << "\n ThermFileParser 12: Testing read of ThermInput file............";

    /// @test  Exception NOT thrown on article with null input file.
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("null input file", tArticle->readThermInputFile() );

    /// - Set input file to invalid path.
    tArticle->mThermInputFile = "calabria.xml";

    /// @test   Read invalid input file.
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("invalid input file", tArticle->readThermInputFile() );

    /// - Set input file to valid path.
    tArticle->mThermInputFile = tThermInputFile;

    /// @test   Read valid input file.
    CPPUNIT_ASSERT_NO_THROW_MESSAGE("read valid input file", tArticle->readThermInputFile() );

    /// @test  Temperature overwrites.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Temp override 0",  12.34, tArticle->vCapTemperatures.at(0), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Temp override 1",  56.78, tArticle->vCapTemperatures.at(1), tTol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Temp override 2", 910.0,  tArticle->vCapTemperatures.at(2), tTol);

    /// @test  Do nothing if no override provided.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Should maintain original temperature",
            tCapTemperature, tArticle->vCapTemperatures.at(tCap), tTol);

    std::cout << " Pass";
}
