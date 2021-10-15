#ifndef UtGunnsFluidTypeChangeConductor_EXISTS
#define UtGunnsFluidTypeChangeConductor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_CONDUCTOR_TYPE_CHANGE    GUNNS Fluid Type Change Converter Unit Test
/// @ingroup  UT_GUNNS_FLUID
///
/// @copyright Copyright 2021 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Type Change Converter
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/conductor/GunnsFluidTypeChangeConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidTypeChangeConductor and befriend UtGunnsFluidTypeChangeConductor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidTypeChangeConductor : public GunnsFluidTypeChangeConductor
{
    public:
        FriendlyGunnsFluidTypeChangeConductor();
        virtual ~FriendlyGunnsFluidTypeChangeConductor();
        friend class UtGunnsFluidTypeChangeConductor;
};
inline FriendlyGunnsFluidTypeChangeConductor::FriendlyGunnsFluidTypeChangeConductor() : GunnsFluidTypeChangeConductor() {};
inline FriendlyGunnsFluidTypeChangeConductor::~FriendlyGunnsFluidTypeChangeConductor() {}

class FriendlyGunnsFluidTypeChangeConductorNode : public GunnsFluidNode
{
    public:
        FriendlyGunnsFluidTypeChangeConductorNode();
        virtual ~FriendlyGunnsFluidTypeChangeConductorNode();
        friend class UtGunnsFluidTypeChangeConductor;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns fluid type change converter link unit tests.
///
/// @details  This class provides unit tests within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidTypeChangeConductor: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidTypeChangeConductor(const UtGunnsFluidTypeChangeConductor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidTypeChangeConductor& operator =(const UtGunnsFluidTypeChangeConductor& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidTypeChangeConductor);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testTransportFlows);
        CPPUNIT_TEST_SUITE_END();

        std::string                              tLinkName;          /**< (--) Nominal config data */
        double                                   tMaxConductivity;   /**< (m2) Nominal config data */
        std::vector<FluidProperties::FluidType>  tFluidTypesPort0;   /**< (--) Nominal config data */
        std::vector<FluidProperties::FluidType>  tFluidTypesPort1;   /**< (--) Nominal config data */
        GunnsFluidTypeChangeConductorConfigData* tConfigData;        /**< (--) Nominal config data */
        bool                                     tMalfBlockageFlag;  /**< (--) Nominal input data */
        double                                   tMalfBlockageValue; /**< (--) Nominal input data */
        GunnsFluidTypeChangeConductorInputData*  tInputData;         /**< (--) Nominal input data */
        FriendlyGunnsFluidTypeChangeConductor*   tArticle;           /**< (--) Article under test */
        GunnsFluidNode                           tNodes[3];          /**< (--) Network nodes */
        GunnsNodeList                            tNodeList;          /**< (--) Network node list */
        std::vector<GunnsBasicLink*>             tLinks;             /**< (--) Network links vector */
        int                                      tPort0;             /**< (--) Nominal init data */
        int                                      tPort1;             /**< (--) Nominal init data */
        double                                   tTimeStep;          /**< (s)  Nominal time step */
        DefinedFluidProperties*                  tFluidProperties;   /**< (--) Nominal config data */
        PolyFluidConfigData*                     tFluidConfig;       /**< (--) Nominal config data */
        PolyFluidInputData*                      tFluidInput0;       /**< (--) Nominal input data */
        PolyFluidInputData*                      tFluidInput1;       /**< (--) Nominal input data */
        double*                                  fractions;          /**< (--) Nominal input data */
        static int                               TEST_ID;            /**< (--) Test identification number. */

    public:
        UtGunnsFluidTypeChangeConductor();
        virtual ~UtGunnsFluidTypeChangeConductor();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRestart();
        void testStep();
        void testTransportFlows();
};

///@}

#endif
