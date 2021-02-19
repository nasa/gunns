/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
(
    (TsHsStringPlugin.o)
    (TsHsOutputPlugin.o)
    (TsHsPluginConfig.o)
)
*******************************************************************************/

#include "TsHsOutputPlugin.hh"
#include "TsHsStringPlugin.hh"
#include "simulation/hs/TsHsPluginConfig.hh"

#include "UtTsHsOutputPluginMngr.hh"

void UtTsHsOutputPluginMngr::setUp()
{
    mngr = new TsHsOutputPluginMngr();

    plugin0 = new TsHsStringPlugin(0);
    plugin1 = new TsHsStringPlugin(1);
    plugin2 = new TsHsStringPlugin(2);

    TsHsPluginConfig configData;

    configData.mBlocking  = false;
    configData.mEnabled   = true;
    configData.mOverwrite = true;
    configData.mPath      = "";

    plugin0->configure(configData);
    plugin1->configure(configData);
    plugin2->configure(configData);

    mngr->add(plugin0);
    mngr->add(plugin1);
    mngr->add(plugin2);
}

void UtTsHsOutputPluginMngr::tearDown()
{
    delete mngr;
}

void UtTsHsOutputPluginMngr::addPlugin()
{
    // Create a new plugin with ID=3
    mngr->add(new TsHsStringPlugin(3));

    // There should now be 4 plugins total
    CPPUNIT_ASSERT_EQUAL(4U, mngr->numPlugins());
}

void UtTsHsOutputPluginMngr::initPlugins()
{
    mngr->init();
    CPPUNIT_ASSERT_EQUAL(std::string("initialized"), plugin0->data);
    CPPUNIT_ASSERT_EQUAL(std::string("initialized"), plugin1->data);
    CPPUNIT_ASSERT_EQUAL(std::string("initialized"), plugin2->data);
}

void UtTsHsOutputPluginMngr::updatePlugins()
{
    mngr->update();
    CPPUNIT_ASSERT_EQUAL(std::string("updated"), plugin0->data);
    CPPUNIT_ASSERT_EQUAL(std::string("updated"), plugin1->data);
    CPPUNIT_ASSERT_EQUAL(std::string("updated"), plugin2->data);
}

// Test basic message output, test the getPlugin method, test the setEnabled method.
void UtTsHsOutputPluginMngr::msgPlugins()
{
    plugin0->data = "msg test";
    plugin1->data = "msg test";
    plugin2->data = "msg test";

    TsHsOutputPlugin* plugin_ptr = mngr->getPlugin(1);

    plugin_ptr->setEnabled(false);

    TS_TIMER_TYPE met = { 1, 2, 3, 4, 5, 6.7, 8 };
    mngr->msg("filename", 999, "function", TS_HS_INFO, "test-sys", met, 9L, "test-message");
    CPPUNIT_ASSERT_EQUAL(std::string("filename|999|function|1|test-sys|12345|6.7|8|test-message"), plugin0->data);
    CPPUNIT_ASSERT_EQUAL(std::string("msg test"), plugin1->data);
    CPPUNIT_ASSERT_EQUAL(std::string("filename|999|function|1|test-sys|12345|6.7|8|test-message"), plugin2->data);
}


void UtTsHsOutputPluginMngr::shutdownPlugins()
{
    mngr->shutdown();
    CPPUNIT_ASSERT_EQUAL(std::string("shut down"), plugin0->data);
    CPPUNIT_ASSERT_EQUAL(std::string("shut down"), plugin1->data);
    CPPUNIT_ASSERT_EQUAL(std::string("shut down"), plugin2->data);
}

