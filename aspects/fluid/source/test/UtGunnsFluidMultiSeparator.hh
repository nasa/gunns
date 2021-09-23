#ifndef UtGunnsFluidMultiSeparator_EXISTS
#define UtGunnsFluidMultiSeparator_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_MULTI_SEPARATOR    Gunns Fluid Multi-Separator Unit Test
/// @ingroup  UT_GUNNS_FLUID
///
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Multi-Separator
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/source/GunnsFluidMultiSeparator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidMultiSeparator and befriend UtGunnsFluidMultiSeparator.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidMultiSeparator : public GunnsFluidMultiSeparator
{
    public:
        FriendlyGunnsFluidMultiSeparator();
        virtual ~FriendlyGunnsFluidMultiSeparator();
        friend class UtGunnsFluidMultiSeparator;
};
inline FriendlyGunnsFluidMultiSeparator::FriendlyGunnsFluidMultiSeparator() : GunnsFluidMultiSeparator() {};
inline FriendlyGunnsFluidMultiSeparator::~FriendlyGunnsFluidMultiSeparator() {}

class FriendlyGunnsFluidMultiSeparatorNode : public GunnsFluidNode
{
    public:
        FriendlyGunnsFluidMultiSeparatorNode();
        virtual ~FriendlyGunnsFluidMultiSeparatorNode();
        friend class UtGunnsFluidMultiSeparator;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns fluid multi-separator link unit tests.
///
/// @details  This class provides unit tests within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidMultiSeparator: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidMultiSeparator(const UtGunnsFluidMultiSeparator& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidMultiSeparator& operator =(const UtGunnsFluidMultiSeparator& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidMultiSeparator);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testInitializationExceptionsTc);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testTransportFlows);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST_SUITE_END();

        std::string                             tLinkName;          /**< (--) Nominal config data */
        double                                  tMaxConductance;    /**< (m2) Nominal config data */
        std::vector<FluidProperties::FluidType> tFluidTypes;        /**< (--) Nominal config data */
        std::vector<int>                        tFluidPorts;        /**< (--) Nominal config data */
        std::vector<ChemicalCompound::Type>     tTcTypes;           /**< (--) Nominal config data */
        std::vector<int>                        tTcPorts;           /**< (--) Nominal config data */
        GunnsFluidMultiSeparatorConfigData*     tConfigData;        /**< (--) Nominal config data */
        bool                                    tMalfBlockageFlag;  /**< (--) Nominal input data */
        double                                  tMalfBlockageValue; /**< (--) Nominal input data */
        std::vector<double>                     tFluidFractions;    /**< (--) Nominal input data */
        std::vector<double>                     tTcFractions;       /**< (--) Nominal input data */
        GunnsFluidMultiSeparatorInputData*      tInputData;         /**< (--) Nominal input data */
        FriendlyGunnsFluidMultiSeparator*       tArticle;           /**< (--) Article under test */
        GunnsFluidNode                          tNodes[5];          /**< (--) Network nodes */
        GunnsNodeList                           tNodeList;          /**< (--) Network node list */
        std::vector<GunnsBasicLink*>            tLinks;             /**< (--) Network links vector */
        int                                     tPort0;             /**< (--) Nominal init data */
        int                                     tPort1;             /**< (--) Nominal init data */
        int                                     tPort2;             /**< (--) Nominal init data */
        int                                     tPort3;             /**< (--) Nominal init data */
        double                                  tTimeStep;          /**< (s)  Nominal time step */
        DefinedFluidProperties*                 tFluidProperties;   /**< (--) Nominal config data */
        GunnsFluidTraceCompoundsConfigData*     tTcConfig;          /**< (--) Nominal config data */
        PolyFluidConfigData*                    tFluidConfig;       /**< (--) Nominal config data */
        PolyFluidInputData*                     tFluidInput0;       /**< (--) Nominal input data */
        PolyFluidInputData*                     tFluidInput1;       /**< (--) Nominal input data */
        PolyFluidInputData*                     tFluidInput2;       /**< (--) Nominal input data */
        PolyFluidInputData*                     tFluidInput3;       /**< (--) Nominal input data */
        double*                                 fractions;          /**< (--) Nominal input data */
        static int                              TEST_ID;            /**< (--) Test identification number. */

    public:
        UtGunnsFluidMultiSeparator();
        virtual ~UtGunnsFluidMultiSeparator();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testInitializationExceptionsTc();
        void testRestart();
        void testStep();
        void testComputeFlows();
        void testTransportFlows();
        void testAccessMethods();
};

///@}

#endif
