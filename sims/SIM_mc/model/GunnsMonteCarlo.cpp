/**
@file     GunnsMonteCarlo.cpp
@brief    GUNNS Monte Carlo Manager implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ()
*/

#include "GunnsMonteCarlo.hh"
#include "sim_services/MonteCarlo/include/montecarlo_c_intf.h"

#include <iostream> //TODO testing

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Monte Carlo Manager Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMonteCarlo::GunnsMonteCarlo()
    :
    mIsMaster(false),
    mIsSlave(false),
    mSlaveId(0),
    mOutDoublesSlave(),
    mOutDoublesMaster(),
    mName(),
    mInitFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Monte Carlo Manager Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMonteCarlo::~GunnsMonteCarlo()
{
    // nothing to do
}

//TODO
void GunnsMonteCarlo::initMaster()
{
    mIsSlave  = mc_is_slave();
    mIsMaster = not mIsSlave;
    if (not mIsMaster) {
        //TODO throw error
    } else {
        mSlaveId = -1; // not a slave
    }
}

//TODO
void GunnsMonteCarlo::initSlave()
{
    mIsSlave  = mc_is_slave();
    mIsMaster = not mIsSlave;
    if (not mIsSlave) {
        //TODO throw error
    } else {
        mSlaveId = mc_get_slave_id();
    }
}

//TODO
void GunnsMonteCarlo::updateMasterPre()
{
    std::cout << "updateMasterPre" << std::endl;
}

//TODO
void GunnsMonteCarlo::updateMasterPost()
{
    // read Slave output values from the MC Master/Slave buffer
    std::cout << "updateMasterPost " << mOutDoublesMaster.size();
    for (unsigned int i=0; i<mOutDoublesMaster.size(); ++i) {
        mc_read((char*) &mOutDoublesMaster.at(i), sizeof(double));
        std::cout << " " << mOutDoublesMaster.at(i);
    }
    std::cout << std::endl;
}

//TODO
void GunnsMonteCarlo::updateMasterShutdown()
{
    std::cout << "updateMasterShutdown" << std::endl;
}

//TODO
void GunnsMonteCarlo::updateSlavePre()
{
    // nothing to do yet
}

//TODO
void GunnsMonteCarlo::updateSlavePost()
{
    // write Slave output values to the MC Master/Slave buffer
    for (unsigned int i=0; i<mOutDoublesSlave.size(); ++i) {
        mc_write((char*) mOutDoublesSlave.at(i), sizeof(double));
    }
}

// TODO in the Slave, store the addresses of the target variables that are to be
//  output to the Master.  On the Master side, size the vector that
//  will receive the values from the Slave.
// This allows the Slave output data to be defined by the input file
void GunnsMonteCarlo::addOutDouble(double* outDouble)
{
    if (mc_is_slave()) {
        mOutDoublesSlave.push_back(outDouble);
    } else {
        mOutDoublesMaster.push_back(0.0);
    }
}
