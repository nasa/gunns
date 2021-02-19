#ifndef UtGunnsFluidMultiAdsorber_EXISTS
#define UtGunnsFluidMultiAdsorber_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_ADSORBER   Adsorber Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Adsorber link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidMultiAdsorber.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidMultiAdsorber and befriend UtGunnsFluidMultiAdsorber.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidMultiAdsorber : public GunnsFluidMultiAdsorber
{
    public:
        FriendlyGunnsFluidMultiAdsorber();
        virtual ~FriendlyGunnsFluidMultiAdsorber();
        friend class UtGunnsFluidMultiAdsorber;
};
inline FriendlyGunnsFluidMultiAdsorber::FriendlyGunnsFluidMultiAdsorber() : GunnsFluidMultiAdsorber() {};
inline FriendlyGunnsFluidMultiAdsorber::~FriendlyGunnsFluidMultiAdsorber() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Adsorber unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Adsorber link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidMultiAdsorber: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Adsorber unit test.
        UtGunnsFluidMultiAdsorber();
        /// @brief    Default constructs this Adsorber unit test.
        virtual ~UtGunnsFluidMultiAdsorber();
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
        /// @brief    Tests update fluid method (nominal).
        void testUpdateFluidNominal();
        /// @brief    Tests update fluid method (reverse flow).
        void testUpdateFluidReverse();
        /// @brief    Tests specific port mapping rules.
        void testPortMapping();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests restart method.
        void testRestart();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidMultiAdsorber);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateFluidNominal);
        CPPUNIT_TEST(testUpdateFluidReverse);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();
        /// @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 3, N_FLUIDS = 3, N_TC = 3};
        DefinedFluidProperties*             tFluidProperties;                /**< (--)       Defined fluid properties. */
        FluidProperties::FluidType          tTypes[N_FLUIDS];                /**< (--)       Array of Fluid Types. */
        double                              tFractions0[N_FLUIDS];           /**< (--)       Array of fluid mass fractions. */
        double                              tFractions1[N_FLUIDS];           /**< (--)       Array of fluid mass fractions. */
        DefinedChemicalCompounds*           tCompoundProperties;             /**< (--)       Defined chemical compounds. */
        ChemicalCompound::Type              tTcTypes[N_TC];                  /**< (--)       Array of trace compound types. */
        GunnsFluidTraceCompoundsConfigData* tFluidTcConfig;                  /**< (--)       Fluid trace compounds config. */
        double                              tTcFractions[N_TC];              /**< (--)       Trace compounds mole fractions for nodes. */
        GunnsFluidTraceCompoundsInputData*  tFluidTcInput;                   /**< (--)       Trace compounds input data for nodes. */
        PolyFluidConfigData*                tFluidConfig;                    /**< (--)       Fluid config data. */
        PolyFluidInputData*                 tFluidInput0;                    /**< (--)       Fluid 0 input data. */
        PolyFluidInputData*                 tFluidInput1;                    /**< (--)       Fluid 1 input data. */
        std::vector<GunnsBasicLink*>        tLinks;                          /**< (--)       Link vector. */
        std::string                         tName;                           /**< (--)       Nominal name. */
        GunnsFluidNode                      tNodes[N_NODES];                 /**< (--)       Fluid nodes. */
        GunnsNodeList                       tNodeList;                       /**< (--)       Node List. */
        int                                 tPort0;                          /**< (--)       Nominal inlet port index. */
        int                                 tPort1;                          /**< (--)       Nominal outlet port index. */
        double                              tMaxConductivity;                /**< (m2)       Nominal maximum conductivity. */
        double                              tExpansionScaleFactor;           /**< (--)       Nominal scale factor for isentropic gas cooling. */
        double                              tThermalLength;                  /**< (m)        Tube length for thermal convection. */
        double                              tThermalDiameter;                /**< (m)        Tube inner diameter for thermal convection. */
        double                              tSurfaceRoughness;               /**< (m)        Tube wall surface roughness for thermal convection. */
        double                              tThermalSurfaceArea;             /**< (m2)       Tube inner surface area for thermal convection. */
        double                              tThermalROverD;                  /**< (--)       Tube surface roughness over diameter for thermal convection. */
        GunnsFluidMultiAdsorberConfigData*  tConfigData;                     /**< (-)        Pointer to nominal configuration data. */
        bool                                tMalfBlockageFlag;               /**< (--)       Blockage malfunction flag. */
        double                              tMalfBlockageValue;              /**< (--)       Blockage malfunction value. */
        double                              tWallTemperature;                /**< (K)        Tube wall temperature for thermal convection. */
        ChemicalCompound::Type              tCompound1Type;                  /**< (--)       Nominal config data. */
        double                              tCompound1MaxAdsorbedMass;       /**< (kg)       Nominal config data. */
        double                              tCompound1EfficiencyCoeff0;      /**< (--)       Nominal config data. */
        double                              tCompound1EfficiencyCoeff1;      /**< (1/K)      Nominal config data. */
        double                              tCompound1DesorbPartialPressure; /**< (kPa)      Nominal config data. */
        double                              tCompound1DesorbRateFactor;      /**< (kg/s/kPa) Nominal config data. */
        double                              tCompound1HeatOfAdsorption;      /**< (kJ/kg)    Nominal config data. */
        bool                                tCompound1TaperOffFlag;          /**< (--)       Nominal config data. */
        ChemicalCompound::Type              tCompound1DependentType;         /**< (--)       Nominal config data. */
        bool                                tCompound1MalfEfficiencyFlag;    /**< (--)       Nominal config data. */
        double                              tCompound1MalfEfficiencyValue;   /**< (--)       Nominal config data. */
        double                              tCompound1AdsorbedMass;          /**< (kg)       Nominal config data. */
        ChemicalCompound::Type              tCompound2Type;                  /**< (--)       Nominal config data. */
        double                              tCompound2MaxAdsorbedMass;       /**< (kg)       Nominal config data. */
        double                              tCompound2EfficiencyCoeff0;      /**< (--)       Nominal config data. */
        double                              tCompound2EfficiencyCoeff1;      /**< (1/K)      Nominal config data. */
        double                              tCompound2DesorbPartialPressure; /**< (kPa)      Nominal config data. */
        double                              tCompound2DesorbRateFactor;      /**< (kg/s/kPa) Nominal config data. */
        double                              tCompound2HeatOfAdsorption;      /**< (kJ/kg)    Nominal config data. */
        bool                                tCompound2TaperOffFlag;          /**< (--)       Nominal config data. */
        ChemicalCompound::Type              tCompound2DependentType;         /**< (--)       Nominal config data. */
        bool                                tCompound2MalfEfficiencyFlag;    /**< (--)       Nominal config data. */
        double                              tCompound2MalfEfficiencyValue;   /**< (--)       Nominal config data. */
        double                              tCompound2AdsorbedMass;          /**< (kg)       Nominal config data. */
        GunnsFluidMultiAdsorberInputData*   tInputData;                      /**< (--)       Pointer to nominal input data. */
        double                              tFluidTemperature;               /**< (K)        Temperature of the fluid in the reactor. */
        double                              tWallHeatFlux;                   /**< (W)        Convection heat flow from the fluid to the tube wall (simbus output to thermal aspect). */
        double                              tMass;                           /**< (kg)       Adsorbed mass this cycle. */
        double                              tSorbtionFlowRate;               /**< (kg/s)     Sorbtion mass flow rate. */
        FriendlyGunnsFluidMultiAdsorber*    tArticle;                        /**< (--)       Pointer to the friendly adsorber under test. */
        double                              tFlowRate;                       /**< (kg/s)     Nominal mass flow rate. */
        double                              tTimeStep;                       /**< (s)        Nominal time step. */
        static int                          TEST_ID;                         /**< (--)       Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidMultiAdsorber(const UtGunnsFluidMultiAdsorber&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidMultiAdsorber& operator =(const UtGunnsFluidMultiAdsorber&);
};

///@}

#endif
