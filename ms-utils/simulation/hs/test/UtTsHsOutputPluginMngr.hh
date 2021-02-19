#ifndef UtTsHsOutputPluginMngr_EXISTS
#define UtTsHsOutputPluginMngr_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class TsHsStringPlugin;
class TsHsOutputPluginMngr;

/// @brief Unit test for TsHsOutputPluginMngr.
class UtTsHsOutputPluginMngr: public CppUnit::TestFixture
{
public:
    // Ctor not really needed here, but gets rid of some compiler warnings
    UtTsHsOutputPluginMngr() : CppUnit::TestFixture(), plugin0(0), plugin1(0), plugin2(0), mngr(0) {}

    void setUp();
    void tearDown();

    void addPlugin();

    void initPlugins();
    void updatePlugins();
    void msgPlugins();
    void shutdownPlugins();

private:

    CPPUNIT_TEST_SUITE(UtTsHsOutputPluginMngr);

    CPPUNIT_TEST(addPlugin);
    CPPUNIT_TEST(initPlugins);
    CPPUNIT_TEST(updatePlugins);
    CPPUNIT_TEST(msgPlugins);
    CPPUNIT_TEST(shutdownPlugins);

    CPPUNIT_TEST_SUITE_END();

    // Data Members
    TsHsStringPlugin*     plugin0;
    TsHsStringPlugin*     plugin1;
    TsHsStringPlugin*     plugin2;
    TsHsOutputPluginMngr* mngr;

    // Disable these to prevent compiler warnings about them being not implemented
    UtTsHsOutputPluginMngr(const UtTsHsOutputPluginMngr&);
    const UtTsHsOutputPluginMngr& operator=(const UtTsHsOutputPluginMngr&);
};

#endif /* UtTsHsOutputPluginMngr_EXISTS */
