#ifndef SimBusQutils_EXISTS
#define SimBusQutils_EXISTS
/**
@defgroup  TSM_UTILITIES_SOFTWARE_SIMBUS_QUEUE    Sim Bus Queue Utilities
@ingroup   TSM_UTILITIES_SOFTWARE_SIMBUS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (This class contains utility methods that are commonly used by TS21 models to extract data from a
  Sim Bus queue.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((SimBusQutils.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-11) (Initial Delivery))

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sim Bus Queue Utilities
///
/// @details  A collection of methods commonly used to extract data from a sim bus data queue.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SimBusQutils
{
    public:
        /// @brief    Returns the most recent value in the queue.
        static double getLatest (const double* queueValues);

        /// @brief    Returns the sum of the queueSize most recent values in the queue.
        static double getSum    (const double* queueValues, const int queueSize);

        /// @brief    Returns the average of the queueSize most recent values in the queue.
        static double getAverage(const double* queueValues, const int queueSize);

        /// @brief    Returns the average of the queueSize most recent values in the queue.
        static bool   isPulseHigh(const bool* queueValues, const int queueSize);

        /// @brief    Although the class is never instantiated, Trick 10 requires a public
        ///           destructor.
        virtual ~SimBusQutils();

    protected:

    private:
        /// @brief    Default constructor unavailable since declared private and not implemented.
        SimBusQutils();

        /// @brief    Copy constructor unavailable since declared private and not implemented.
        SimBusQutils(const SimBusQutils& that);

        /// @brief    Assignment operator unavailable since declared private and not implemented.
        SimBusQutils& operator =(const SimBusQutils& that);
};

/// @}

#endif
