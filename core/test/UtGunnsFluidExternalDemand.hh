#ifndef UtGunnsFluidExternalDemand_EXISTS
#define UtGunnsFluidExternalDemand_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_EXTERNAL_DEMAND    Gunns Fluid External Demand Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid External Demand
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidExternalDemand.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidExternalDemand and befriend UtGunnsFluidExternalDemand.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidExternalDemand : public GunnsFluidExternalDemand
{
    public:
        FriendlyGunnsFluidExternalDemand();
        virtual ~FriendlyGunnsFluidExternalDemand();
        friend class UtGunnsFluidExternalDemand;
};
inline FriendlyGunnsFluidExternalDemand::FriendlyGunnsFluidExternalDemand()
    : GunnsFluidExternalDemand() {};
inline FriendlyGunnsFluidExternalDemand::~FriendlyGunnsFluidExternalDemand() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid External Demand unit tests.
////
/// @details  This class provides the unit tests for the GunnsFluidExternalDemand class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidExternalDemand: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidExternalDemand(const UtGunnsFluidExternalDemand& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidExternalDemand& operator =(const UtGunnsFluidExternalDemand& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidExternalDemand);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testIoMethods);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();

        GunnsFluidExternalDemandConfigData* tConfigData;            /**< (--)  Nominal config data */
        GunnsFluidExternalDemandInputData*  tInputData;             /**< (--)  Nominal input data */
        FriendlyGunnsFluidExternalDemand*   tArticle;               /**< (--)  Article under test */
        std::string                         tLinkName;              /**< (--)  Nominal config data */
        double                              tMaxConductivity;       /**< (m2)  Nominal config data */
        double                              tExpansionScaleFactor;  /**< (--)  Nominal config data */
        double                              tBlockage;              /**< (--)  Nominal input data */
        double                              tSourcePressure;        /**< (kPa) Nominal input data */
        FluidProperties::FluidType          tConvertToType;         /**< (--)  Nominal config data */
        double                              tFilterMinConductivity; /**< (m2)  Nominal config data */
        double                              tFilterMinDeltaP;       /**< (kPa) Nominal config data */
        double                              tFilterCapacitanceGain; /**< (--)  Nominal config data */
        double                              tSupplyCapacitance;     /**< (K)   Nominal input data */
        double                              tSupplyTemperature;     /**< (K)   Nominal input data */
        double*                             tSupplyMassFractions;   /**< (--)  Nominal input data */
        double*                             tSupplyTcMoleFractions; /**< (--)  Nominal input data */
        GunnsFluidNode                      tNodes[3];              /**< (--)  Test nodes */
        GunnsNodeList                       tNodeList;              /**< (--)  Test node list */
        std::vector<GunnsBasicLink*>        tLinks;                 /**< (--)  Test links vector */
        int                                 tPort0;                 /**< (--)  Nominal init data */
        int                                 tPort1;                 /**< (--)  Nominal init data */
        double                              tTimeStep;              /**< (s)   Test time step */
        DefinedFluidProperties*             tFluidProperties;       /**< (--)  Test fluid properties */
        DefinedChemicalCompounds*           tTcProperties;          /**< (--)  Pointer to predefined chemical compounds properties. */
        ChemicalCompound::Type              tTcTypes[4];            /**< (--)  Array of trace compound types. */
        GunnsFluidTraceCompoundsConfigData* tFluidTcConfig;         /**< (--)  Pointer to fluid trace compounds config data. */
        PolyFluidConfigData*                tLocalConfig;           /**< (--)  Test local fluid config */
        PolyFluidConfigData*                tExternalConfig;        /**< (--)  Test external fluid config */
        PolyFluidInputData*                 tFluidInput1;           /**< (--)  Test fluid input */
        PolyFluidInputData*                 tFluidInput2;           /**< (--)  Test fluid input */
        PolyFluidInputData*                 tFluidInput3;           /**< (--)  Test fluid input */

    public:
        UtGunnsFluidExternalDemand();
        virtual ~UtGunnsFluidExternalDemand();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testIoMethods();
        void testStep();
        void testRestart();
};

///@}

#endif
