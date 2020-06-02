/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
     (TsHsStringMngr.o)
     (TsHsMsg.o)
 )
 *******************************************************************************/
#include <iostream>
#include <iomanip>  // For stream insertion operator testing
#include <sstream>  // Debug Matt's message

#include "TsHsStringMngr.hh"
#include "TsHsMsg.hh"
#include "UtTsHsMsg.hh"

void UtTsHsMsg::setUp()
{
    mStringMngr = new TsHsStringMngr();
    mStringMngr->registerSelf();
}

void UtTsHsMsg::tearDown()
{
    delete mStringMngr;
}

void UtTsHsMsg::testSend()
{
    // Test send(file,line)
    // The send method uses the test_message's message field
    TsHsMsg test_message(TS_HS_INFO,"TEST","message_field");
    test_message.send(__FILE__,123, "function");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__"|123|function|1|TEST|00000|0|0|message_field"), mStringMngr->str());

    // Test send(file,line,msg)
    // The send method ignores the test_message's message field and uses the message field supplied as an argument
    TsHsMsg test_message2(TS_HS_INFO,"TEST","ignored_message_field");
    test_message2.send(__FILE__,123,"function", "message_field");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__"|123|function|1|TEST|00000|0|0|message_field"), mStringMngr->str());
}

void UtTsHsMsg::testSetters()
{
    // Test setMessage and setSeverity methods.
    // Create a test_message without a message field. Then set the message field and change it severity.
    TsHsMsg test_message(TS_HS_INFO,"replace_subsys"); // Info=1
    test_message.setSubsys("TEST");
    test_message.setMessageText("message_field");
    test_message.setSeverity(TS_HS_DEBUG);   // Debug=0
    test_message.send(__FILE__, 123, "function");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|function|0|TEST|00000|0|0|message_field"), mStringMngr->str());
}

void UtTsHsMsg::testClear()
{
    // Clear should reset the message field part of the message.
    // Create a message with a message field, clear it, then send the message.
    // If the clear worked, the message that was sent should not contain the message field.

    TsHsMsg test_message(TS_HS_INFO,"TEST","message_field");
    test_message.clear();
    test_message.send(__FILE__,123, "function");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|function|1|TEST|00000|0|0|"), mStringMngr->str());
}

void UtTsHsMsg::testStreamInsertionOperator()
{
    // Use the SIO to build the entire message field
    TsHsMsg test_message(TS_HS_INFO,"TEST");
    test_message << "message_field" << 999;
    test_message.send(__FILE__, 123, "function");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|function|1|TEST|00000|0|0|message_field999"), mStringMngr->str());

    // Use the SIO to append to an existing message field
    TsHsMsg test_message2(TS_HS_INFO,"TEST","message_field");
    test_message2 << "_appended_text_" << 999;
    test_message2 << "_some_more_" << 888;
    test_message2.send(__FILE__, 123, "function");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|function|1|TEST|00000|0|0|message_field_appended_text_999_some_more_888"), mStringMngr->str());

    // Use the SIO with iomanip
    TsHsMsg test_message3(TS_HS_INFO,"TEST");
    test_message3 << "_appended_text_[" << std::setw(2) << std::setfill('0') << 8 << "]_some_more_text";
    test_message3.send(__FILE__, 123, "function");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|function|1|TEST|00000|0|0|_appended_text_[08]_some_more_text"), mStringMngr->str());

    // Use the SIO with iomanip
    TsHsMsg test_message4(TS_HS_ERROR,"TEST");
    test_message4 << "Null pointer detected\n" << tsStackTrace();
    test_message4.send(__FILE__, 123, "function");
}

void UtTsHsMsg::testDisable()
{
    TsHsMsg::disable();

    // Try to send a message when disabled. For code coverage.
    std::cerr << "\nShould not see an message between the dashed lines." << std::endl;
    std::cerr << "\n----------------------------------" << std::endl;
    hsSendMsg(TS_HS_DEBUG, "TEST", "new_message_field");
    std::cerr << "\n----------------------------------" << std::endl;

    bool status = TsHsMsg::isEnabled();
    CPPUNIT_ASSERT_EQUAL(status, false);

    TsHsMsg::enable();
    status = TsHsMsg::isEnabled();
    CPPUNIT_ASSERT_EQUAL(status, true);
}

void UtTsHsMsg::testMacroFunctions()
{
    // Test tsHsSendMsgFileLine(...TsHsMsg...)
    TsHsMsg test_message(TS_HS_INFO,"TEST","message_field");
    tsHsSendMsgFileLine(__FILE__, 123, "function", test_message);
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|function|1|TEST|00000|0|0|message_field"), mStringMngr->str());

    // Test tsHsSendMsgFileLine(...string...)
    tsHsSendMsgFileLine(__FILE__, 123, "function", TS_HS_WARNING, "TEST", "message_field");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|function|2|TEST|00000|0|0|message_field"), mStringMngr->str());

    // Test really big messages. Reportedly, messages bigger than 1000 bytes were getting
    // truncated when sent with the macro.

    std::string really_big_message =
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 100
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 200
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 300
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 400
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 500
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 600
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 700
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 800
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 900
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 1000
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 1100
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 1200
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 1300
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 1400
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 1500
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 1600
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 1700
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 1800
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."  // 1900
            "123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789."; // 2000

    std::string expected_result_string = std::string(__FILE__ "|123|testMacroFunctions|0|TEST|00000|0|0|") + really_big_message;

#line 123
    hsSendMsg(TS_HS_DEBUG, "TEST", really_big_message);
    CPPUNIT_ASSERT_EQUAL(expected_result_string, mStringMngr->str());
    CPPUNIT_ASSERT(((mStringMngr->str()).length() > 2000));

    // Test the hsSendMsg(...) macro.
    // This test has been placed at the bottom to avoid the #line directives
    // affecting anything else, since cppunit uses __LINE__ to report lines numbers
    // for test failures.

    TsHsMsg test_message2(TS_HS_INFO, "TEST", "message_field");

    // Test hsSendMsg(TsHsMsg) form of macro call
#line 123
    hsSendMsg(test_message2);
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|testMacroFunctions|1|TEST|00000|0|0|message_field"), mStringMngr->str());

    // Test hsSendMsg(severity,subsys,mtext) form of macro call
#line 123
    hsSendMsg(TS_HS_DEBUG, "TEST", "new_message_field");
    CPPUNIT_ASSERT_EQUAL(std::string(__FILE__ "|123|testMacroFunctions|0|TEST|00000|0|0|new_message_field"), mStringMngr->str());
}

void UtTsHsMsg::testHsRegisterSubsys()
{
    // ensure no problem with negative index
    tsHsRegisterSubsys(-1, "foo");

    // nominal register
    tsHsRegisterSubsys(0, "foo");

    // ensure no problem when at current size
    tsHsRegisterSubsys(20, "bar"); // 20: the intial max size

    // force a resize
    tsHsRegisterSubsys(21, "bar");
}
