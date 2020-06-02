#ifndef UtGunnsFluidSeparatorGas_EXISTS
#define UtGunnsFluidSeparatorGas_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_SEPARATOR_GAS   Separator Gas Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Separator Liquid link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidSeparatorGas.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSeparatorGas and befriend UtGunnsFluidSeparatorGas.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSeparatorGas : public GunnsFluidSeparatorGas
{
    public:
        FriendlyGunnsFluidSeparatorGas();
        virtual ~FriendlyGunnsFluidSeparatorGas();
        friend class UtGunnsFluidSeparatorGas;
};
inline FriendlyGunnsFluidSeparatorGas::FriendlyGunnsFluidSeparatorGas() : GunnsFluidSeparatorGas() {};
inline FriendlyGunnsFluidSeparatorGas::~FriendlyGunnsFluidSeparatorGas() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hatch unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Separator Gas link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSeparatorGas: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Separator Gas unit test.
        UtGunnsFluidSeparatorGas();
        /// @brief    Default constructs this Separator Gas unit test.
        virtual ~UtGunnsFluidSeparatorGas();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config and input data.
        void testConfigAndInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update fluid method (empty).
        void testUpdateFluidEmpty();
        /// @brief    Tests update fluid method (no gas).
        void testUpdateFluidNoGas();
        /// @brief    Tests update fluid method (dt = 0).
        void testUpdateFluidZeroTimeStep();
        /// @brief    Tests update fluid method (full).
        void testUpdateFluidFull();
        /// @brief    Tests update fluid method (dry air).
        void testUpdateFluidDryAir();
        /// @brief    Tests update fluid method (nominal).
        void testUpdateFluidNominal();
        /// @brief    Tests process outputs method.
        void testProcessOutputs();
        /// @brief    Tests specific port mapping rules.
        void testPortMapping();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests update fluid method (reverse liquid flow).
        void testUpdateFluidReverseFlow();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidSeparatorGas);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateFluidEmpty);
        CPPUNIT_TEST(testUpdateFluidNoGas);
        CPPUNIT_TEST(testUpdateFluidZeroTimeStep);
        CPPUNIT_TEST(testUpdateFluidFull);
        CPPUNIT_TEST(testUpdateFluidDryAir);
        CPPUNIT_TEST(testUpdateFluidNominal);
        CPPUNIT_TEST(testProcessOutputs);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST(testUpdateFluidReverseFlow);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        FriendlyGunnsFluidSeparatorGas*   tArticle;                /**< (--) Test article. */
        double                            tTimeStep;               /**< (s)  Nominal time step. */
        DefinedFluidProperties*           tFluidProperties;        /**< (--) Predefined fluid properties. */
        PolyFluidConfigData*              tFluidConfig;            /**< (--) Fluid config data. */
        PolyFluidInputData*               tFluidInput0;            /**< (--) Fluid input data for node 0. */
        PolyFluidInputData*               tFluidInput1;            /**< (--) Fluid input data for node 1. */
        std::vector<GunnsBasicLink*>      tLinks;                  /**< (--) Link vector. */
        GunnsFluidNode                    tNodes[2];               /**< (--) Nominal connected nodes. */
        int                               tPort0;                  /**< (--) Nominal inlet port index. */
        int                               tPort1;                  /**< (--) Nominal outlet port index. */
        std::string                       tName;                   /**< (--) Nominal config data. */
        GunnsNodeList                     tNodeList;               /**< (--) Nominal config data. */
        double                            tMaxConductivity;        /**< (m2) Nominal config data. */
        FluidProperties::FluidType        tGasType;                /**< (--) Nominal config data. */
        double                            tMassExponent;           /**< (--) Nominal config data. */
        double                            tMaxLiquidMass;          /**< (kg) Nominal config data. */
        double                            tReferenceSpeed;         /**< (rev/min) Nominal config data. */
        double                            tReferencePressure;      /**< (kPa) Nominal config data. */
        double                            tReferenceRemovalRate;   /**< (kg*min/rev) Nominal config data. */
        double                            tPowerCurveCoefficient;  /**< (kPa*min/rev) Config-derived data. */
        GunnsFluidSeparatorGasConfigData* tConfigData;             /**< (--) Nominal config data. */
        bool                              tMalfBlockageFlag;       /**< (--) Nominal input data. */
        double                            tMalfBlockageValue;      /**< (--) Nominal input data. */
        double                            tSeparatorSpeed;         /**< (rev/min) Nominal input data. */
        double                            tTransferFlowRate;       /**< (kg/s) Nominal input data. */
        double                            tLiquidMass;             /**< (kg) Nominal input data. */
        GunnsFluidSeparatorGasInputData*  tInputData;              /**< (--) Nominal input data. */
        static int                        TEST_ID;                 /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSeparatorGas(const UtGunnsFluidSeparatorGas&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSeparatorGas& operator =(const UtGunnsFluidSeparatorGas&);
};

///@}

#endif
