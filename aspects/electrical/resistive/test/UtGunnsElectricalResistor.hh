#ifndef UtGunnsElectricalResistor_EXISTS
#define UtGunnsElectricalResistor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_ELECTRICAL_RESISTOR    Gunns Electrical Resistor Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Electrical Resistor
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include "aspects/electrical/resistive/GunnsElectricalResistor.hh"
#include "core/GunnsBasicNode.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsElectricalResistor and befriend UtGunnsElectricalResistor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectricalResistor : public GunnsElectricalResistor
{
    public:
        FriendlyGunnsElectricalResistor();
        virtual ~FriendlyGunnsElectricalResistor();
        friend class UtGunnsElectricalResistor;
};
inline FriendlyGunnsElectricalResistor::FriendlyGunnsElectricalResistor() : GunnsElectricalResistor() {};
inline FriendlyGunnsElectricalResistor::~FriendlyGunnsElectricalResistor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Electrical Resistor  unit tests.
////
/// @details  This class provides the unit tests for the Gunns Electrical Resistor within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectricalResistor: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsElectricalResistor(const UtGunnsElectricalResistor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsElectricalResistor& operator =(const UtGunnsElectricalResistor& that);

        CPPUNIT_TEST_SUITE(UtGunnsElectricalResistor);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testUpdateState);

        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsElectricalResistorConfigData* mConfigData;

        /// --     Pointer to nominal input data
        GunnsElectricalResistorInputData*  mInputData;

        /// --     Test Article
        FriendlyGunnsElectricalResistor*   mArticle;

        /// --     Resistor Name
        std::string                    mLinkName;

        /// --     Link Resistance
        double                         mResistance;

        /// --     Input voltage to the electrical resistor link
        double                         mVoltageIn;

        /// --     Output voltage from the electrical resistor link
        double                         mVoltageOut;

        /// --     Electrical resistor voltage drop across link.
        double                         mVoltageDrop;

        /// --     Electrical link current
        double                         mCurrent;

        /// --     Electrical link efficiency
        double                         mElectricalEfficiency;

        /// --     Electrical link waste heat
        double                         mWasteHeat;

        /// --     Link Blockage
        double                         mBlockage;

        /// --     Network Nodes
        GunnsBasicNode                 mNodes[2];

        /// --     Node List
        GunnsNodeList                  mNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*>   mLinks;

        /// --     Nominal inlet port index
        int                            mPort0;

        /// --     Nominal outlet port index
        int                            mPort1;

        /// (s)    Nominal time step
        double                         mTimeStep;

        /// --     Nominal tolerance for comparison of expected and returned values
        double                         mTolerance;

    public:
        UtGunnsElectricalResistor();
        virtual ~UtGunnsElectricalResistor();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testUpdateState();
        void testComputeFlows();
};

///@}

#endif
