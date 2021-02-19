////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_GUNNS_DYNAMICS   Dynamics Unit tests
/// @ingroup   UT_GUNNS
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @details   Unit test classes for GUNNS dynamics models.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/ui/text/TestRunner.h>

#include "UtGunnsDynSingleRigidMassProps.hh"
#include "UtGunnsDynSingleRigidStateDeriv.hh"
#include "UtGunnsDynSingleRigidEom.hh"
#include "UtGunnsDynUtils.hh"
#include "UtGunnsDynEuler.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param     int     --  not used
/// @param     char**  --  not used
///
/// @return    --  status (always 0)
///
/// @details  Main for GUNNS dynamics model unit tests in the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int, char**)
{
    CppUnit::TextTestRunner runner;

    runner.addTest(UtGunnsDynSingleRigidMassProps::suite());
    runner.addTest(UtGunnsDynSingleRigidStateDeriv::suite());
    runner.addTest(UtGunnsDynSingleRigidEom::suite());
    runner.addTest(UtGunnsDynUtils::suite());
    runner.addTest(UtGunnsDynEuler::suite());

    runner.run();

    return 0;
}
