#ifndef TestFluidNetworkWrapper_EXISTS
#define TestFluidNetworkWrapper_EXISTS

/**
@file
@brief TestFluidNetworkWrapper declarations.

@defgroup  ISS_ORCH_FLUID_TESTFLUIDNETWORK TestFluidNetworkWrapper GUNNS Network
@ingroup   ISS_ORCH_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
              National Aeronautics and Space Administration.  All Rights Reserved.

@{
*/

#include "../networks/fluid/test/TestFluidNetwork.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TestFluidNetworkWrapper
///
/// @details  Extends TestFluidNetwork to output node properties values to cout for testing.
/////////////////////////////////////////////////////////////////////////////////////////////////
class TestFluidNetworkWrapper : public TestFluidNetwork
{
    public:
        /// @brief    Default constructor
        TestFluidNetworkWrapper(const std::string& name = "");
        /// @brief    Default destructor
        virtual ~TestFluidNetworkWrapper();
        /// @brief    Print node properties
        void printState();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        TestFluidNetworkWrapper(const TestFluidNetworkWrapper& rhs);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        TestFluidNetworkWrapper& operator =(const TestFluidNetworkWrapper& rhs);
};

/// @}
#endif
