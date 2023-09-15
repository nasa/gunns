#ifndef GunnsInfraFunctions_EXISTS
#define GunnsInfraFunctions_EXISTS

/**
 @file
 @brief     GUNNS Infrastructure Integration Function declarations

@defgroup  TSM_GUNNS_CORE_INFRA_FUNCTS Infrastructure Integration Functions
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE: (Defines functions for infrastructure interfaces & dependencies, such as Trick.  These can
          be modified to interface with other simulation environments.)

@details
REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
- ()

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2018-06) (Initial))

@{
*/

#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Infrastructure Integration Functions
///
/// @details  A collection of methods to interface with common simulation environment functions.
///           These default to the Trick environment, with a compiler directive implementing a
///           non-Trick implementation.  This non-Trick implementation is used for our unit testing
///           (a non-Trick environment), and it is intended to be modified by the users for their
///           non-Trick environments as needed.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsInfraFunctions
{
    public:
        static double              mClockTime; /**< ** (s) A clock time value. */
        static bool                mMcIsSlave; /**< ** (1) An indication (true) of the monte carlo slave role. */
        static int                 mMcSlaveId; /**< ** (1) A monte carlo slave run identifier. */
        static std::vector<double> mMcRead;    /**< ** (1) Values to read from the monte carlo slave role to the master role. */
        static std::vector<double> mMcWrite;   /**< ** (1) Values to write to the monte carlo master role from the slave role. */
        /// @brief  Returns a time value from the infrastructure clock function.
        static double clockTime();
        /// @brief  Returns a monte carlo isSlave flag from the infrastructure.
        static bool mcIsSlave();
        /// @brief  Returns a monte carlo slave ID from the infrastructure.
        static int mcSlaveId();
        /// @brief  Reads a value from the infrastructure monte carlo read buffer.
        static void mcRead(double& var);
        /// @brief  Writes a value to the infrastructure monte carlo read buffer.
        static void mcWrite(double& var);
        /// @brief  Although this is never constructed, the destructor is public for some environments that need it.
        virtual ~GunnsInfraFunctions();

    private:
        //TODO
        /// @brief  Constructs and returns an empty vector of doubles, for static vector definition.
        static std::vector<double> makeEmptyVector();
//        {
//        static std::vector<double> makeEmptyVector() {
//            std::vector<double> v;
//            v.clear();
//            return v;
//        }
        /// @brief  Default constructor unavailable since declared private and not implemented.
        GunnsInfraFunctions();
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsInfraFunctions(const GunnsInfraFunctions&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsInfraFunctions& operator =(const GunnsInfraFunctions&);
};

/// @}

#endif
