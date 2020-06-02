/*
 * @copyright Copyright 2019 United States Government as represented by the Administrator of the
 *            National Aeronautics and Space Administration.  All Rights Reserved.
 */

#include <iostream>
//#include "../networks/fluid/test/TestFluidNetwork.hh"
#include "TestFluidNetworkWrapper.hh"

int main(int argc, char** argv) {

    std::cout << std::endl;

    std::cout << "Constructing...";
    TestFluidNetworkWrapper fluid("fluid");
    std::cout << " success." << std::endl;

    std::cout << "Initializing...";
    fluid.initialize("fluid");
    std::cout << " success." << std::endl;

    std::cout << "Initial network state:" << std::endl;
    fluid.printState();

    for (int step=0; step<30; ++step) {
        std::cout << "Stepping: " << step << "...";
        fluid.update(0.1);
        std::cout << " success." << std::endl;
    }

    std::cout << "Final network state:" << std::endl;
    fluid.printState();

    std::cout << "Normal termination." << std::endl << std::endl;
    return 0;
}
