#ifndef UtGunnsThermalRadiation_EXISTS
#define UtGunnsThermalRadiation_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup    UT_GUNNS_THERMAL_RADIATION  Gunns Thermal Radiation Unit test
/// @ingroup     UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details     Unit Tests for Gunns Thermal Radiation
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "aspects/thermal/GunnsThermalRadiation.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsThermalRadiation and befriend UtGunnsThermalRadiation.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermalRadiation : public GunnsThermalRadiation
{
    public:
        FriendlyGunnsThermalRadiation();
        virtual ~FriendlyGunnsThermalRadiation();
        friend class UtGunnsThermalRadiation;
};
inline FriendlyGunnsThermalRadiation::FriendlyGunnsThermalRadiation() : GunnsThermalRadiation() {};
inline FriendlyGunnsThermalRadiation::~FriendlyGunnsThermalRadiation() {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Thermal Radiation unit tests.
///
/// @details  This class provides unit tests for the Thermal Radiation within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermalRadiation: public CppUnit::TestFixture
{

    public:
        /// @brief    Default Constructor.
        UtGunnsThermalRadiation();

        /// @brief    Default Destructor.
        virtual ~UtGunnsThermalRadiation();

        /// @brief    Executes before each test.
        void setUp();

        /// @brief    Executes after each test.
        void tearDown();

        /// @brief    Tests ConfigData construction
        void testConfig();

        /// @brief    Tests InputData  construction
        void testInput();

        /// @brief    Tests Default Construction.
        void testDefaultConstruction();

        /// @brief    Tests initialize method without exceptions
        void testInitialization();

        /// @brief    Tests restart method.
        void testRestart();

        /// @brief    Tests getter and setter methods.
        void testAccessors();

        /// @brief    Tests method for updating the link
        void testUpdateState();

    private:

        CPPUNIT_TEST_SUITE(UtGunnsThermalRadiation);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST_SUITE_END();

        /// @brief  (--)  Pointer to nominal configuration data
        GunnsThermalRadiationConfigData* tConfigData; /**< (--)  Pointer to nominal configuration
                                                                 data */
        GunnsThermalRadiationInputData*  tInputData;  /**< (--)  Pointer to nominal input data */
        FriendlyGunnsThermalRadiation    tArticle;    /**< (--)  Test Article */
        std::string    tLinkName;                     /**< (--)  Conductor Name */
        GunnsBasicNode tNodes[2];                     /**< (--)  Network Nodes */
        GunnsNodeList  tNodeList;                     /**< (--)  Node List */
        std::vector<GunnsBasicLink*>  tNetworkLinks;  /**< (--)  Network Links  */
        double  tDefaultConductivity;                 /**< (W/K) Link Conductivity */
        double  tViewScalar;                          /**< (--)  View Scalar */
        double  tBlockage;                            /**< (--)  Link Blockage */
        int     tPort0;                               /**< (--)  Nominal inlet port index */
        int     tPort1;                               /**< (--)  Nominal outlet port index */
        double  tTimeStep;                            /**< (s)   Nominal time step */
        double  tTolerance;                           /**< (--)  Nominal tolerance for comparison
                                                                 of expected and returned values */

        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermalRadiation(const UtGunnsThermalRadiation& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermalRadiation& operator =(const UtGunnsThermalRadiation& that);
};

///@}

#endif
