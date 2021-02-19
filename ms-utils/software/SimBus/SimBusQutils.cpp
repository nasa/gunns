/***************************************** TRICK HEADER ********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Utility methods commonly used by TS21 models to extract data from a Sim Bus queue.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
     ((simulation/hs/TsHsMsg.o)
      (software/exceptions/TsOutOfBoundsException.o))

 PROGRAMMERS:
     ((Jason Harvey) (L-3 Communications) (2011-11) (Initial Delivery))
 **************************************************************************************************/

#include "SimBusQutils.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    queueValues  --  Input pointer to the queue values array
///
/// @returns  double       --  The most recent value in the queue
///
/// @details  Returns the most recent value in the queue.
////////////////////////////////////////////////////////////////////////////////////////////////////
double SimBusQutils::getLatest(const double* queueValues)
{
    return queueValues[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    queueValues  --  Input A pointer to the queue values array
/// @param    queueSize    --  Input The most recent number of values to sum
///
/// @returns  double       --  The sum of the most recent values in the queue
///
/// @details  Returns the sum of the queueSize'd most recent values in the queue array.
////////////////////////////////////////////////////////////////////////////////////////////////////
double SimBusQutils::getSum(const double* queueValues, const int queueSize)
{
    double returnValue = 0.0;
    for (int i = 0; i < queueSize; ++i) {
        returnValue += queueValues[i];
    }
    return returnValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    queueValues  --  Input A pointer to the queue values array
/// @param    queueSize    --  Input The most recent number of values to sum
///
/// @returns  double       --  The average of the most recent values in the queue
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the average of the queueSize'd most recent values in the queue array.  Throws
///           an exception if the queue size is not > 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
double SimBusQutils::getAverage(const double* queueValues, const int queueSize)
{
    if (queueSize < 1) {
        TsHsMsg(TS_HS_ERROR, "Utilities", "queueSize argument is not > 0.");
        throw TsOutOfBoundsException("Invalid Calling Arguments", "SimBusQutils::getAverage",
                "queueSize argument is not > 0.");
    } else {
        return SimBusQutils::getSum(queueValues, queueSize) / queueSize;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    queueBools   --  Input A pointer to the queue boolean array
/// @param    queueSize    --  Input The most recent number of values to sum
///
/// @returns  bool         --  Indicates if the queue of booleans had atleast one true value
///
/// @details  Returns booleans if any of the queue array had atleast one true boolean.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool SimBusQutils::isPulseHigh(const bool* queueBools, const int queueSize)
{
    for (int i = 0; i < queueSize; ++i) {
        if (queueBools[i]) {
            return true;
        }
    }
    return false;
}

