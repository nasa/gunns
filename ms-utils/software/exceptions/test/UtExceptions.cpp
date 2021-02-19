/******************************************* TRICK HEADER ******************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (Unit test class for exceptions.)

 REFERENCE:
 (TBD)

 ASSUMPTIONS AND LIMITATIONS:
 ((TBD))

 LIBRARY_DEPENDENCY:
 (
 (TsConfigurationException.o)
 (TsInitializationException.o)
 (TsInvalidStateException.o)
 (TsNumericalException.o)
 (TsOutOfBoundsException.o)
 (TsParseException.o)
 (TsUnknownException.o)
 (simulation/hs/TsHsMngr.o)
 )

 PROGRAMMERS:
 ( ((Jeff Wilson) (L3) (May 2008) (CxTF)) )
 ***************************************************************************************************/

#include "TsConfigurationException.hh"
#include "TsHsException.hh"
#include "TsInitializationException.hh"
#include "TsInvalidStateException.hh"
#include "TsNumericalException.hh"
#include "TsOutOfBoundsException.hh"
#include "TsParseException.hh"
#include "TsUnknownException.hh"
#include "UtExceptions.hh"

/**
 *
 * This method is required by the CPPUNIT framework.
 *
 */
void UtExceptions::setUp()
{
    // nothing to do
}

/**
 *
 * This method is required by the CPPUNIT framework.
 *
 */
void UtExceptions::tearDown()
{
    // nothing to do
}

void UtExceptions::getException(int flag) throw (TsBaseException)
{
    std::string mesg;
    std::string mesg2;
    std::string cause;
    std::string name;
    std::string blank;

    if (flag == 0)
    {

        blank = "";

        mesg = "Number Exception";
        mesg2 = "Calling Set Message";
        cause = "Divide by 0";
        name = "TestExceptions";

        TsNumericalException blankNE;
        CPPUNIT_ASSERT_EQUAL(blank,blankNE.getMessage());

        TsNumericalException ne(mesg);
        ne.setCause("Divide by 0");
        ne.setThrowingEntityName("TestExceptions");

        CPPUNIT_ASSERT_EQUAL(mesg,ne.getMessage());
        CPPUNIT_ASSERT_EQUAL(name,ne.getThrowingEntityName());
        CPPUNIT_ASSERT_EQUAL(cause,ne.getCause());

        TsNumericalException numEx(mesg,name);
        numEx.setCause(cause);
        CPPUNIT_ASSERT_EQUAL(mesg,numEx.getMessage());
        CPPUNIT_ASSERT_EQUAL(name,numEx.getThrowingEntityName());
        CPPUNIT_ASSERT_EQUAL(cause,numEx.getCause());

        numEx.setMessage(mesg2);
        CPPUNIT_ASSERT_EQUAL(mesg2,numEx.getMessage());

        TsNumericalException numerEx(mesg,name,cause);
        CPPUNIT_ASSERT_EQUAL(mesg,numerEx.getMessage());
        CPPUNIT_ASSERT_EQUAL(name,numerEx.getThrowingEntityName());
        CPPUNIT_ASSERT_EQUAL(cause,numerEx.getCause());

        TsNumericalException ne2(numerEx);
        ne2.getMessage();

        std::cout << "\n" << ne.toString();

        throw ne;
    }
    else if (flag == 1)
    {

        mesg = "Out of Bounds Exception";
        cause = "Value > design limit";
        name = "TestExceptions";

        TsOutOfBoundsException blankOE;
        CPPUNIT_ASSERT_EQUAL(blank,blankOE.getMessage());

        TsOutOfBoundsException obe(mesg);
        obe.setCause(cause);
        obe.setThrowingEntityName(name);

        CPPUNIT_ASSERT_EQUAL(mesg,obe.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,obe.getCause());
        CPPUNIT_ASSERT_EQUAL(name,obe.getThrowingEntityName());

        TsOutOfBoundsException obEx(mesg,name);
        obEx.setCause(cause);

        CPPUNIT_ASSERT_EQUAL(mesg,obEx.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,obEx.getCause());
        CPPUNIT_ASSERT_EQUAL(name,obEx.getThrowingEntityName());

        obEx.setMessage(mesg2);
        CPPUNIT_ASSERT_EQUAL(mesg2,obEx.getMessage());

        TsOutOfBoundsException outex(mesg,name,cause);

        CPPUNIT_ASSERT_EQUAL(mesg,outex.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,outex.getCause());
        CPPUNIT_ASSERT_EQUAL(name,outex.getThrowingEntityName());

        std::cout << "\n" + obe.toString();

        throw obe;
    }
    else if (flag == 3)
    {
        mesg = "Configuration Exception";
        cause = "File Not Found.";
        name = "TestExceptions";

        TsConfigurationException blankCE;
        CPPUNIT_ASSERT_EQUAL(blank,blankCE.getMessage());

        TsConfigurationException ce("My Message");
        ce.setMessage(mesg);
        ce.setCause(cause);
        ce.setThrowingEntityName(name);

        CPPUNIT_ASSERT_EQUAL(mesg,ce.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,ce.getCause());
        CPPUNIT_ASSERT_EQUAL(name,ce.getThrowingEntityName());

        ce.setMessage(mesg2);
        CPPUNIT_ASSERT_EQUAL(mesg2,ce.getMessage());

        TsConfigurationException configEx(mesg,name);
        configEx.setCause(cause);

        CPPUNIT_ASSERT_EQUAL(mesg,configEx.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,configEx.getCause());
        CPPUNIT_ASSERT_EQUAL(name,configEx.getThrowingEntityName());

        TsConfigurationException confex(mesg,name,cause);
        CPPUNIT_ASSERT_EQUAL(mesg,confex.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,confex.getCause());
        CPPUNIT_ASSERT_EQUAL(name,confex.getThrowingEntityName());

        std::cout << "\n" + ce.toString();

        throw ce;
    }
    else if (flag == 4)
    {
        mesg = "Initialization Exception";
        cause = "Value > design limit";
        name = "TestExceptions";

        TsInitializationException blankIE;
        CPPUNIT_ASSERT_EQUAL(blank,blankIE.getMessage());

        TsInitializationException ie("My Message");
        ie.setMessage(mesg);
        ie.setCause(cause);
        ie.setThrowingEntityName(name);

        CPPUNIT_ASSERT_EQUAL(mesg,ie.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,ie.getCause());
        CPPUNIT_ASSERT_EQUAL(name,ie.getThrowingEntityName());

        ie.setMessage(mesg2);
        CPPUNIT_ASSERT_EQUAL(mesg2,ie.getMessage());

        TsInitializationException initEx(mesg,name);
        initEx.setCause(cause);

        CPPUNIT_ASSERT_EQUAL(mesg,initEx.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,initEx.getCause());
        CPPUNIT_ASSERT_EQUAL(name,initEx.getThrowingEntityName());

        ie.setMessage(mesg2);
        CPPUNIT_ASSERT_EQUAL(mesg2,ie.getMessage());

        TsInitializationException inEx(mesg,name,cause);
        CPPUNIT_ASSERT_EQUAL(mesg,inEx.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,inEx.getCause());
        CPPUNIT_ASSERT_EQUAL(name,inEx.getThrowingEntityName());

        std::cout << "\n" + ie.toString();
        throw ie;
    }
    else if (flag == 5)
    {
        mesg = "Parse Exception";
        cause = "Ill-formed XML";
        name = "ParseExceptions";

        TsParseException blankPE;
        CPPUNIT_ASSERT_EQUAL(blank,blankPE.getMessage());

        TsParseException pe("My Message");
        pe.setMessage(mesg);
        pe.setCause(cause);
        pe.setThrowingEntityName(name);

        CPPUNIT_ASSERT_EQUAL(mesg,pe.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,pe.getCause());
        CPPUNIT_ASSERT_EQUAL(name,pe.getThrowingEntityName());

        pe.setMessage(mesg2);
        CPPUNIT_ASSERT_EQUAL(mesg2,pe.getMessage());

        TsParseException parseEx(mesg,name);
        parseEx.setCause(cause);

        CPPUNIT_ASSERT_EQUAL(mesg,parseEx.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,parseEx.getCause());
        CPPUNIT_ASSERT_EQUAL(name,parseEx.getThrowingEntityName());

        pe.setMessage(mesg2);
        CPPUNIT_ASSERT_EQUAL(mesg2,pe.getMessage());

        TsParseException pEx(mesg,name,cause);
        CPPUNIT_ASSERT_EQUAL(mesg,pEx.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,pEx.getCause());
        CPPUNIT_ASSERT_EQUAL(name,pEx.getThrowingEntityName());

        std::cout << "\n" + pe.toString();
        throw pe;

    }
    else if (flag == 99)
    {

        mesg = "Unknown Exception";
        cause = "Dont Know";
        name = "TestExceptions";

        TsUnknownException blankUE;
        CPPUNIT_ASSERT_EQUAL(blank,blankUE.getMessage());

        TsUnknownException ue(mesg);
        ue.setCause(cause);
        ue.setThrowingEntityName(name);

        CPPUNIT_ASSERT_EQUAL(mesg,ue.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,ue.getCause());
        CPPUNIT_ASSERT_EQUAL(name,ue.getThrowingEntityName());

        TsUnknownException uEx(mesg,name);
        uEx.setCause(cause);

        CPPUNIT_ASSERT_EQUAL(mesg,uEx.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,uEx.getCause());
        CPPUNIT_ASSERT_EQUAL(name,uEx.getThrowingEntityName());

        uEx.setMessage(mesg2);
        CPPUNIT_ASSERT_EQUAL(mesg2,uEx.getMessage());

        TsUnknownException unEx(mesg,name,cause);
        CPPUNIT_ASSERT_EQUAL(mesg,unEx.getMessage());
        CPPUNIT_ASSERT_EQUAL(cause,unEx.getCause());
        CPPUNIT_ASSERT_EQUAL(name,unEx.getThrowingEntityName());

        std::cout << "\n" + ue.toString() << "\n";

        throw ue;
    }
    else
    {
        std::cout << "\nUnhandled Exception caught...\n";
    }
}

void UtExceptions::validateNumericalException(const TsBaseException* be)
{

    const TsNumericalException* ne =
            dynamic_cast<const TsNumericalException*>(be);

    try
    {
        std::string name = typeid(*ne).name();
        std::string name2 = typeid(TsNumericalException).name();

        CPPUNIT_ASSERT(name == name2);
    }
    catch (const std::exception& ex)
    {
        std::cout << "\nWrong Exception type!!\n";
        CPPUNIT_ASSERT(false);
    }
}

void UtExceptions::validateUnknownException(const TsBaseException* be)
{

    const TsUnknownException* ue = dynamic_cast<const TsUnknownException*>(be);

    try
    {
        std::string name = typeid(*ue).name();
        std::string name2 = typeid(TsUnknownException).name();

        CPPUNIT_ASSERT(name == name2);
    }
    catch (const std::exception& ex)
    {
        std::cout << "\nWrong Exception type!!";
        CPPUNIT_ASSERT(false);
    }
}

void UtExceptions::validateOutOfBoundsException(const TsBaseException* be)
{

    const TsOutOfBoundsException* obe =
            dynamic_cast<const TsOutOfBoundsException*>(be);

    try
    {
        std::string name = typeid(*obe).name();
        std::string name2 = typeid(TsOutOfBoundsException).name();

        CPPUNIT_ASSERT(name == name2);
    }
    catch (const std::exception& ex)
    {
        std::cout << "\nWrong Exception type!!";
        CPPUNIT_ASSERT(false);
    }
}

void UtExceptions::validateConfigurationException(const TsBaseException* be)
{

    const TsConfigurationException* ce =
            dynamic_cast<const TsConfigurationException*>(be);

    try
    {
        std::string name = typeid(*ce).name();
        std::string name2 = typeid(TsConfigurationException).name();

        CPPUNIT_ASSERT(name == name2);
    }
    catch (const std::exception& ex)
    {
        std::cout << "\nWrong Exception type!!";
        CPPUNIT_ASSERT(false);
    }
}

void UtExceptions::validateInitializationException(const TsBaseException* be)
{

    const TsInitializationException* ie =
            dynamic_cast<const TsInitializationException*>(be);

    try
    {
        std::string name = typeid(*ie).name();
        std::string name2 = typeid(TsInitializationException).name();

        CPPUNIT_ASSERT(name == name2);
    }
    catch (const std::exception& ex)
    {
        std::cout << "\nWrong Exception type!!";
        CPPUNIT_ASSERT(false);
    }
}

void UtExceptions::validateParseException(const TsBaseException* be)
{

    const TsParseException* ne = dynamic_cast<const TsParseException*>(be);

    try
    {
        std::string name = typeid(*ne).name();
        std::string name2 = typeid(TsParseException).name();

        CPPUNIT_ASSERT(name == name2);
    }
    catch (const std::exception& ex)
    {
        std::cout << "\nWrong Exception type!!\n";
        CPPUNIT_ASSERT(false);
    }
}

/**
 *
 * This is a test method used to test the TsOutOfBoundsException
 *
 */

void UtExceptions::testOutOfBoundsException()
{

    std::cout << "\nTesting the OutOfBounds exception....";

    UtExceptions te;
    int flag;

    flag = 1;
    try
    {
        te.getException(flag);
    }
    catch (const TsOutOfBoundsException& obe)
    {
        const TsBaseException* be = dynamic_cast<const TsBaseException*>(&obe);
        validateOutOfBoundsException(be);

    }
    catch (...)
    {
        std::cout << "Unexpected Exception raised";
        CPPUNIT_ASSERT(false);
    }

    TsBaseException *tbe = new TsOutOfBoundsException("","");
    tbe->setCause("MyCause");
    std::string expected = "MyCause";
    CPPUNIT_ASSERT_EQUAL(tbe->getCause(),expected);
    delete tbe;

}

/**
 *
 * This is a test method used to test the TsUnknownException
 *
 */

void UtExceptions::testUnknownException()
{

    std::cout << "\nTesting the Unknown exception....";

    UtExceptions te;
    int flag;

    flag = 99;
    try
    {
        te.getException(flag);
    }
    catch (const TsUnknownException& ue)
    {
        const TsBaseException* be = dynamic_cast<const TsBaseException*>(&ue);
        validateUnknownException(be);

    }
    catch (...)
    {
        std::cout << "Unexpected Exception raised";
        CPPUNIT_ASSERT(false);
    }

    TsBaseException *tbe = new TsUnknownException("","");
    tbe->setCause("MyCause");
    std::string expected = "MyCause";
    CPPUNIT_ASSERT_EQUAL(tbe->getCause(),expected);
    delete tbe;

}

/**
 *
 * This is a test method used to test the TsNumericalException
 *
 */

void UtExceptions::testNumericalException()
{

    std::cout << "\nTesting the Numerical exception....";

    UtExceptions te;
    int flag;

    flag = 0;
    try
    {
        te.getException(flag);
    }
    catch (const TsNumericalException& ne)
    {
        const TsBaseException* be = dynamic_cast<const TsBaseException*>(&ne);
        validateNumericalException(be);

    }
    catch (...)
    {
        std::cout << "Unexpected Exception raised";
        CPPUNIT_ASSERT(false);
    }

    TsBaseException *tbe = new TsNumericalException("","");
    tbe->setCause("MyCause");
    std::string expected = "MyCause";
    CPPUNIT_ASSERT_EQUAL(tbe->getCause(),expected);
    delete tbe;

}

/**
 *
 * This is a test method used to test the TsConfigurationException
 *
 */

void UtExceptions::testConfigurationException()
{

    std::cout << "\nTesting the Configuration exception....";

    UtExceptions te;
    int flag;

    flag = 3;
    try
    {
        te.getException(flag);
    }
    catch (const TsConfigurationException& ce)
    {
        const TsBaseException* be = dynamic_cast<const TsBaseException*>(&ce);
        validateConfigurationException(be);

    }
    catch (...)
    {
        std::cout << "Unexpected Exception raised";
        CPPUNIT_ASSERT(false);
    }

    TsBaseException *tbe = new TsConfigurationException("","");
    tbe->setCause("MyCause");
    std::string expected = "MyCause";
    CPPUNIT_ASSERT_EQUAL(tbe->getCause(),expected);
    delete tbe;

}

/**
 *
 * This is a test method used to test the TsConfigurationException
 *
 */

void UtExceptions::testInitializationException()
{

    std::cout << "\nTesting the Initialization exception....";

    UtExceptions te;
    int flag;

    flag = 4;
    try
    {
        te.getException(flag);
    }
    catch (const TsInitializationException& ie)
    {
        const TsBaseException* be = dynamic_cast<const TsBaseException*>(&ie);
        validateInitializationException(be);

    }
    catch (...)
    {
        std::cout << "Unexpected Exception raised";
        CPPUNIT_ASSERT(false);
    }

    TsBaseException *tbe = new TsInitializationException("","");
    tbe->setCause("MyCause");
    std::string expected = "MyCause";
    CPPUNIT_ASSERT_EQUAL(tbe->getCause(),expected);
    delete tbe;

}

/**
 *
 * This is a test method used to test the TsParseException
 *
 */
void UtExceptions::testParseException()
{

    std::cout << "\nTesting the Parse exception....";

    UtExceptions te;
    int flag;

    flag = 5;
    try
    {
        te.getException(flag);
    }
    catch (const TsParseException& ie)
    {
        const TsBaseException* be = dynamic_cast<const TsBaseException*>(&ie);
        validateParseException(be);

    }
    catch (...)
    {
        std::cout << "Unexpected Exception raised";
        CPPUNIT_ASSERT(false);
    }

    TsBaseException *tbe = new TsParseException("","");
    tbe->setCause("MyCause");
    std::string expected = "MyCause";
    CPPUNIT_ASSERT_EQUAL(tbe->getCause(),expected);
    delete tbe;

}

void UtExceptions::testInitializationOperatorEqual()
{
    TsInitializationException te("","");
    te.setCause("MyCause");
    te.setMessage("MyMessage");
    te.setThrowingEntityName("UtExceptions");

    TsInitializationException te2;
    te2 = te;
    std::string expected = "MyCause";
    std::string cause = te2.getCause();
    CPPUNIT_ASSERT_EQUAL(expected,cause);
}

void UtExceptions::testConfigurationOperatorEqual()
{
    TsConfigurationException ex("","");
    ex.setCause("MyCause");
    ex.setMessage("MyMessage");
    ex.setThrowingEntityName("UtExceptions");

    TsConfigurationException ex2;
    ex2 = ex;
    std::string expected = "MyCause";
    std::string cause = ex2.getCause();
    CPPUNIT_ASSERT_EQUAL(expected,cause);
}

void UtExceptions::testUnknownOperatorEqual()
{
    TsUnknownException ex("","");
    ex.setCause("MyCause");
    ex.setMessage("MyMessage");
    ex.setThrowingEntityName("UtExceptions");

    TsUnknownException ex2;
    ex2 = ex;
    std::string expected = "MyCause";
    std::string cause = ex2.getCause();
    CPPUNIT_ASSERT_EQUAL(expected,cause);
}

void UtExceptions::testParseOperatorEqual()
{
    TsParseException ex("","");
    ex.setCause("MyCause");
    ex.setMessage("MyMessage");
    ex.setThrowingEntityName("UtExceptions");

    TsParseException ex2;
    ex2 = ex;
    std::string expected = "MyCause";
    std::string cause = ex2.getCause();
    CPPUNIT_ASSERT_EQUAL(expected,cause);

}

void UtExceptions::testNumericalOperatorEqual()
{
    TsNumericalException ex("","");
    ex.setCause("MyCause");
    ex.setMessage("MyMessage");
    ex.setThrowingEntityName("UtExceptions");

    TsNumericalException ex2;
    ex2 = ex;
    std::string expected = "MyCause";
    std::string cause = ex2.getCause();
    CPPUNIT_ASSERT_EQUAL(expected,cause);
}

void UtExceptions::testOutOfBoundsOperatorEqual()
{
    TsOutOfBoundsException ex("","");
    ex.setCause("MyCause");
    ex.setMessage("MyMessage");
    ex.setThrowingEntityName("UtExceptions");

    TsOutOfBoundsException ex2;
    ex2 = ex;
    std::string expected = "MyCause";
    std::string cause = ex2.getCause();
    CPPUNIT_ASSERT_EQUAL(expected,cause);
}

void UtExceptions::testInvalidStateOperatorEqual()
{
    TsInvalidStateException be("");
    be.setCause("MyCause");
    be.setMessage("MyMessage");
    be.setThrowingEntityName("UtExceptions");

    TsInvalidStateException be2;
    be2 = be;
    std::string expected = "MyCause";
    std::string cause = be.getCause();
    CPPUNIT_ASSERT_EQUAL(expected,cause);
}

void UtExceptions::testTsHsMsgClassAndException()
{
    CPPUNIT_ASSERT_THROW(
            TS_HS_EXCEPTION(TS_HS_WARNING, "Subsystem", "In the event of a real emergency ...", TsInitializationException, "HsExceptionSim::hsAndException", "This is only a test."),
            TsInitializationException);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for thowing an exception and logging simulataneously.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtExceptions::testHsAndException()
{
    std::cout
            << "\n TsHsException Test: Both Health & Status Message and Exception Test ";

    std::ostringstream msg;
    msg << "In the event of a real emergency ...";
    CPPUNIT_ASSERT_THROW(
            TS_HS_EXCEPTION(TS_HS_ERROR, "EXAMPLE", msg.str(), TsInitializationException, "HsExceptionSim::hsAndException", "This is only a test."),
            TsInitializationException);

    CPPUNIT_ASSERT_THROW(
            TS_HS_EXCEPTION(TS_HS_ERROR, "EXAMPLE", std::string("In the event of a real emergency ..."), TsConfigurationException, "HsExceptionSim::hsAndException", "This is only a test."),
            TsConfigurationException);

    CPPUNIT_ASSERT_THROW(
            TS_HS_EXCEPTION(TS_HS_ERROR, "EXAMPLE", std::string("In the event of a real emergency ..."), TsInvalidStateException, "HsExceptionSim::hsAndException", "This is only a test."),
            TsInvalidStateException);

    CPPUNIT_ASSERT_THROW(
            TS_HS_EXCEPTION(TS_HS_ERROR, "EXAMPLE", std::string("In the event of a real emergency ..."), TsNumericalException, "HsExceptionSim::hsAndException", "This is only a test."),
            TsNumericalException);

    CPPUNIT_ASSERT_THROW(
            TS_HS_EXCEPTION(TS_HS_ERROR, "EXAMPLE", std::string("In the event of a real emergency ..."), TsParseException, "HsExceptionSim::hsAndException", "This is only a test."),
            TsParseException);

    CPPUNIT_ASSERT_THROW(
            TS_HS_EXCEPTION(TS_HS_ERROR, "EXAMPLE", std::string("In the event of a real emergency ..."), TsOutOfBoundsException, "HsExceptionSim::hsAndException", "This is only a test."),
            TsOutOfBoundsException);

    CPPUNIT_ASSERT_THROW(
            TS_HS_EXCEPTION(TS_HS_ERROR, "EXAMPLE", std::string("In the event of a real emergency ..."), TsUnknownException, "HsExceptionSim::hsAndException", "This is only a test."),
            TsUnknownException);

    // The inside assertion should fail: I'm passing in a TsOutOfBounds exception
    // and expecting a TsInitilization exception as a test to ensure the macro
    // can fail. The outer assertion will pass if the inner test fails. IOW, I'm
    // expecting a failed test, so if it does, my test should pass. :-)
    CPPUNIT_ASSERT_ASSERTION_FAIL(
            CPPUNIT_ASSERT_THROW(TS_HS_EXCEPTION(TS_HS_ERROR, "EXAMPLE", std::string("In the event of a real emergency ..."), TsOutOfBoundsException, "HsExceptionSim::hsAndException", "This is only a test."), TsInitializationException));

    std::cout << "\n... Pass\n";

}

void UtExceptions::testTsInitializationExceptionWhatMethod()
{
    std::cout << "\n TsBaseException Test: what() method Test ";

    TsInitializationException e;
    e.setMessage("MyMessage");
    e.setThrowingEntityName("UtExceptions");
    e.setCause("MyCause");

    std::string expected = std::string("TsInitializationException, MyMessage thrown by UtExceptions because MyCause");
    std::string cause = e.what();
    CPPUNIT_ASSERT_EQUAL(expected,cause);

    std::cout << "\n... Pass\n";
}

void UtExceptions::testTsNumericalExceptionWhatMethod()
{
    std::cout << "\n TsNumericalException Test: NumericalException what() method Test ";

    TsNumericalException ne("","");
    ne.setMessage("MyMessage");
    ne.setThrowingEntityName("UtExceptions");
    ne.setCause("MyCause");

    std::string expected = std::string("TsNumericalException, MyMessage thrown by UtExceptions because MyCause");
    std::string cause = ne.what();
    CPPUNIT_ASSERT_EQUAL(expected,cause);

    std::cout << "\n... Pass\n";
}


