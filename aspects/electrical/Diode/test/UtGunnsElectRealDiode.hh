#ifndef UtGunnsElectRealDiode_EXISTS
#define UtGunnsElectRealDiode_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_DIODE    Real Diode Link Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Real Diode Link
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/electrical/Diode/GunnsElectRealDiode.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectRealDiode and befriend UtGunnsElectRealDiode.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectRealDiode : public GunnsElectRealDiode
{
    public:
        FriendlyGunnsElectRealDiode();
        virtual ~FriendlyGunnsElectRealDiode();
        friend class UtGunnsElectRealDiode;
};
inline FriendlyGunnsElectRealDiode::FriendlyGunnsElectRealDiode() : GunnsElectRealDiode() {};
inline FriendlyGunnsElectRealDiode::~FriendlyGunnsElectRealDiode() {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electric Diode Link  unit tests.
////
/// @details  This class provides the unit tests for the GunnsElectRealDiode within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectRealDiode: public CppUnit::TestFixture
{
    public:
        /// @brief  Nominal constructor
        UtGunnsElectRealDiode();
        /// @brief  Nominal destructs
        virtual ~UtGunnsElectRealDiode();
        /// @brief  Executes before each test.
        void setUp();
        /// @brief  Executes after each test.
        void tearDown();
        /// @brief  Tests config and input data.
        void testConfigAndInput();
        /// @brief  Tests default construction.
        void testDefaultConstruction();
        /// @brief  Tests the initialization method with nominal values.
        void testNominalInitialization();
        /// @brief  Tests the initialization method with errors.
        void testInitializationErrors();
        /// @brief  Tests the restart method.
        void testRestart();
        /// @brief  Tests the step and updateState methods.
        void testStep();
        /// @brief  Tests the minorStep and updateState methods.
        void testMinorStep();
        /// @brief  Tests the getter and setter methods.
        void testAccessors();
        /// @brief  Tests the confirmSolutionAcceptable method.
        void testConfirmSolutionAcceptable();

    private:
        /// @brief  Sets up the suite of tests for the GunnsElectRealDioderic unit testing.
        CPPUNIT_TEST_SUITE(UtGunnsElectRealDiode);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationErrors);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testMinorStep);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testConfirmSolutionAcceptable);
        CPPUNIT_TEST_SUITE_END();
        /// @brief  Enumeration for the number of nodes.
        enum {N_NODES = 2};
        std::vector<GunnsBasicLink*>   tLinks;              /**< (--)    Network links vector. */
        std::string                    tName;               /**< (--)    Test article name. */
        GunnsBasicNode                 tNodes[N_NODES];     /**< (--)    Network nodes. */
        GunnsNodeList                  tNodeList;           /**< (--)    Network nodes list. */
        int                            tPort0;              /**< (--)    Port 0 node mapping. */
        int                            tPort1;              /**< (--)    Port 1 node mapping. */
        GunnsElectRealDiodeConfigData* tConfigData;         /**< (--)    Pointer to config data. */
        GunnsElectRealDiodeInputData*  tInputData;          /**< (--)    Pointer to input data. */
        FriendlyGunnsElectRealDiode*   tArticle;            /**< (--)    Pointer to article under test. */
        double                         tForwardConductance; /**< (1/ohm) Nominal config data. */
        double                         tReverseConductance; /**< (1/ohm) Nominal config data. */
        double                         tVoltageDrop;        /**< (v)     Nominal config data. */
        bool                           tMalfBlockageFlag;   /**< (--)    Nominal input data. */
        double                         tMalfBlockageValue;  /**< (--)    Nominal input data. */
        bool                           tReverseBias;        /**< (--)    Nominal input data. */
        static int                     TEST_ID;             /**< (--)    Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectRealDiode(const UtGunnsElectRealDiode& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsElectRealDiode& operator =(const UtGunnsElectRealDiode& that);
};

///@}

#endif
