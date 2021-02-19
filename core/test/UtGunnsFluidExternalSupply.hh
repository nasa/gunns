#ifndef UtGunnsFluidExternalSupply_EXISTS
#define UtGunnsFluidExternalSupply_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_EXTERNAL_SUPPLY    Gunns Fluid External Supply Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid External Supply
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidExternalSupply.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidExternalSupply and befriend UtGunnsFluidExternalSupply.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidExternalSupply : public GunnsFluidExternalSupply
{
    public:
        FriendlyGunnsFluidExternalSupply();
        virtual ~FriendlyGunnsFluidExternalSupply();
        friend class UtGunnsFluidExternalSupply;
};
inline FriendlyGunnsFluidExternalSupply::FriendlyGunnsFluidExternalSupply()
    : GunnsFluidExternalSupply() {};
inline FriendlyGunnsFluidExternalSupply::~FriendlyGunnsFluidExternalSupply() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid External Supply unit tests.
////
/// @details  This class provides the unit tests for the GunnsFluidExternalSupply class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidExternalSupply: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidExternalSupply(const UtGunnsFluidExternalSupply& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidExternalSupply& operator =(const UtGunnsFluidExternalSupply& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidExternalSupply);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testIoMethods);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();

        GunnsFluidExternalSupplyConfigData* tConfigData;            /**< (--)   Nominal config data */
        GunnsFluidExternalSupplyInputData*  tInputData;             /**< (--)   Nominal input data */
        FriendlyGunnsFluidExternalSupply*   tArticle;               /**< (--)   Article under test */
        std::string                         tLinkName;              /**< (--)   Nominal config data */
        double                              tInitialFlowDemand;     /**< (kg/s) Nominal input data */
        FluidProperties::FluidType          tConvertToType;         /**< (--)   Nominal config data */
        bool                                tUseNetworkCapacitance; /**< (--)   Nominal config data */
        double                              tDemandTemperature;     /**< (K)    Nominal input data */
        double*                             tDemandMassFractions;   /**< (--)   Nominal input data */
        double*                             tDemandTcMoleFractions; /**< (--)   Nominal input data */
        GunnsFluidNode                      tNodes[3];              /**< (--)   Test nodes */
        GunnsNodeList                       tNodeList;              /**< (--)   Test node list */
        std::vector<GunnsBasicLink*>        tLinks;                 /**< (--)   Test links vector */
        int                                 tPort0;                 /**< (--)   Nominal init data */
        int                                 tPort1;                 /**< (--)   Nominal init data */
        double                              tTimeStep;              /**< (s)    Test time step */
        DefinedFluidProperties*             tFluidProperties;       /**< (--)   Test fluid properties */
        DefinedChemicalCompounds*           tTcProperties;          /**< (--)   Pointer to predefined chemical compounds properties. */
        ChemicalCompound::Type              tTcTypes[4];            /**< (--)   Array of trace compound types. */
        GunnsFluidTraceCompoundsConfigData* tFluidTcConfig;         /**< (--)   Pointer to fluid trace compounds config data. */
        PolyFluidConfigData*                tLocalConfig;           /**< (--)   Test local fluid config */
        PolyFluidConfigData*                tExternalConfig;        /**< (--)   Test external fluid config */
        PolyFluidInputData*                 tFluidInput1;           /**< (--)   Test fluid input */
        PolyFluidInputData*                 tFluidInput2;           /**< (--)   Test fluid input */

    public:
        UtGunnsFluidExternalSupply();
        virtual ~UtGunnsFluidExternalSupply();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testIoMethods();
        void testStep();
        void testComputeFlows();
        void testRestart();
};

///@}

#endif
