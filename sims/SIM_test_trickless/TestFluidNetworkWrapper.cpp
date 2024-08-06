/**
@file
@brief TestFluidNetworkWrapper implementation.
@copyright Copyright 2019 United States Government as represented by the Administrator of the
              National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "TestFluidNetworkWrapper.hh"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  Name of the network for H&S messages.
///
/// @details  Default constructs the TestFluidNetworkWrapper.
/////////////////////////////////////////////////////////////////////////////////////////////////
TestFluidNetworkWrapper::TestFluidNetworkWrapper(const std::string& name)
    :
    TestFluidNetwork(name)
{
    // Nothing to do

}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the TestFluidNetworkWrapper.
/////////////////////////////////////////////////////////////////////////////////////////////////
TestFluidNetworkWrapper::~TestFluidNetworkWrapper()
{
    // Nothing to do
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Prints network node properties to cout.
/////////////////////////////////////////////////////////////////////////////////////////////////
void TestFluidNetworkWrapper::printState()
{
    std::cout << "  Nodes P: " << netNodes[0].getPotential()
                       << ", " << netNodes[1].getPotential() << std::endl;
    std::cout << "  Nodes T: " << netNodes[0].getContent()->getTemperature()
                       << ", " << netNodes[1].getContent()->getTemperature() << std::endl;
    std::cout << "  Nodes X: " << netNodes[0].getContent()->getMassFraction(0)
                       << ", " << netNodes[1].getContent()->getMassFraction(0) << std::endl;
    std::cout << "           " << netNodes[0].getContent()->getMassFraction(1)
                       << ", " << netNodes[1].getContent()->getMassFraction(1) << std::endl;
    std::cout << "           " << netNodes[0].getContent()->getMassFraction(2)
                       << ", " << netNodes[1].getContent()->getMassFraction(2) << std::endl;
    std::cout << "           " << netNodes[0].getContent()->getMassFraction(3)
                       << ", " << netNodes[1].getContent()->getMassFraction(3) << std::endl;
    std::cout << "           " << netNodes[0].getContent()->getMassFraction(4)
                       << ", " << netNodes[1].getContent()->getMassFraction(4) << std::endl;
    std::cout << "           " << netNodes[0].getContent()->getMassFraction(5)
                       << ", " << netNodes[1].getContent()->getMassFraction(5) << std::endl;
    std::cout << "  Leak mDot: " << leak.getFlowRate() << std::endl;
}
