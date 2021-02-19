#ifndef UTGUNNSTHERMALPOTENTIAL_EXISTS
#define UTGUNNSTHERMALPOTENTIAL_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup    UT_GUNNS_THERMAL_POTENTIAL  Gunns Thermal Potential Unit test
/// @ingroup     UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details     Unit Tests for Gunns Thermal Potential
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "aspects/thermal/GunnsThermalPotential.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsThermalPotential and befriend UtGunnsThermalPotential
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermalPotential : public GunnsThermalPotential
{
    public:
        FriendlyGunnsThermalPotential();
        virtual ~FriendlyGunnsThermalPotential();
        friend class UtGunnsThermalPotential;
};
inline FriendlyGunnsThermalPotential::FriendlyGunnsThermalPotential() : GunnsThermalPotential() {};
inline FriendlyGunnsThermalPotential::~FriendlyGunnsThermalPotential() {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Thermal Potential unit tests.
///
/// @details  This class provides unit tests for the Thermal Potential within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermalPotential: public CppUnit::TestFixture
{

    public:
        /// @brief    Default Constructor.
        UtGunnsThermalPotential();

        /// @brief    Default Destructor.
        virtual ~UtGunnsThermalPotential();

        /// @brief    Executes before each test.
        void setUp();

        /// @brief    Executes after each test.
        void tearDown();

        /// @brief    Tests ConfigData construction
        void testConfig();

        /// @brief    Tests InputData construction
        void testInput();

        /// @brief    Tests Default Construction
        void testDefaultConstruction();

        /// @brief    Tests initialize method without exceptions
        void testInitialization();

        /// @brief    Tests processOutputs method
        void testProcessOutputs();

        /// @brief    Tests checkSpecificPortRules method
        void testSpecificPortRules();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsThermalPotential);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testProcessOutputs);
        CPPUNIT_TEST(testSpecificPortRules);
        CPPUNIT_TEST_SUITE_END();


        GunnsThermalPotentialConfigData* tConfigData; /**< (--)  Pointer to nominal configuration
                                                                 data */
        GunnsThermalPotentialInputData*  tInputData;  /**< (--)  Pointer to nominal input data */
        FriendlyGunnsThermalPotential    tArticle;    /**< (--)  Test Article */
        std::string     tLinkName;                    /**< (--)  Conductor Name */
        GunnsBasicNode  tNodes[2];                    /**< (--)  Network Nodes */
        GunnsNodeList   tNodeList;                    /**< (--)  Node List */
        std::vector<GunnsBasicLink*>  tNetworkLinks;  /**< (--)  Network Links */
        double  tInitialConductivity;                 /**< (W/K) Link Default Conductance */
        double  tInitialPotential;                    /**< (K)   Link Initial Potential */
        double  tMalfBlockageValue;                   /**< (--)  Blockage Malfunction fractional
                                                                 value (0-1) */
        double  tMalfBlockageFlag;                    /**< (--)  Blockage Malfunction boolean */
        int     tPort0;                               /**< (--)  Nominal inlet port index */
        int     tPort1;                               /**< (--)  Nominal outlet port index */
        double  tTimeStep;                            /**< (s)   Nominal time step */
        double  tTolerance;                           /**< (--)  Nominal tolerance for comparison
                                                                 of expected and returned values */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermalPotential(const UtGunnsThermalPotential& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermalPotential& operator =(const UtGunnsThermalPotential& that);
};

/// @}

#endif
