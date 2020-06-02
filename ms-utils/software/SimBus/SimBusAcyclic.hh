#ifndef SimBusAcyclic_EXISTS
#define SimBusAcyclic_EXISTS
/**
@defgroup  TSM_UTILITIES_SOFTWARE_SIMBUS_ACYCLIC    Sim Bus Acyclic Write
@ingroup   TSM_UTILITIES_SOFTWARE_SIMBUS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details

@{
*/

#include <string>

// function pointer type for a sim bus writer function
typedef void (*AcyclicWriteFunc)(double delayTime);

// get a function pointer to an acyclic writer function
AcyclicWriteFunc get_acyclic_write_func(std::string sys, std::string bus, std::string subsys, std::string var);

/// @}

#endif
