#ifndef GunnsMonteCarlo_EXISTS
#define GunnsMonteCarlo_EXISTS

/**
@file     GunnsMonteCarlo.hh
@brief    GUNNS Monte Carlo Manager declarations

@defgroup  TSM_GUNNS_CORE_MONTE_CARLO    GUNNS Monte Carlo Manager
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- TODO

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsMonteCarlo.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-01) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Monte Carlo Manager
///
/// @details  This is the base link class for any connection between two nodes in a network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMonteCarlo
{
    TS_MAKE_SIM_COMPATIBLE(GunnsMonteCarlo);

    public:
    //TODO embed all calls to Trick in an included Macro file (similar to but separate from SimCompatibility
    // for ease of porting outside of Trick)
    //TODO don't where this will end up.  Since you have to modify a Trick sim object to add the
    // monte carlo jobs, then might as well add this object directly to the sim object.  It's going to have
    // vectors of pointers to all the model input & output terms,
    // so this doesn't need to be direclty associated with a network.
    //TODO should have a polymorphic interface to the optimziation object
    // the optijmizer object will have its own config data, like PSO swarm size, weights, etc.
    //TODO #slaves corresponds to # parallel runs (CPU's) in the compute farm, not PSO swarm size
//        GunnsOptimizer*      mOptimizer; //TODO pointer to the optimizer object
        bool                 mIsMaster;         /**< *o (1) trick_chkpnt_io(**) This instance is in the Monte Carlo Master role. */
        bool                 mIsSlave;          /**< *o (1) trick_chkpnt_io(**) This instance is in the Monte Carlo Slave role. */
        int                  mSlaveId;          /**< *o (1) trick_chkpnt_io(**) The Slave role identifier of this instance. */
        std::vector<double*> mOutDoublesSlave;  /**< ** (1) trick_chkpnt_io(**) Pointers to doubles for output from Slave. */
        std::vector<double>  mOutDoublesMaster; /**< ** (1) trick_chkpnt_io(**) Doubles values in Master read from Slave. */
        std::string          mName;             /**< *o (1) trick_chkpnt_io(**) Name of this instance for output messages. */
        bool                 mInitFlag;         /**< *o (1) trick_chkpnt_io(**) This instance has been initialized. */
        /// @brief Constructs the GUNNS Monte Carlo Manager object.
        GunnsMonteCarlo();
        /// @brief Destructs the GUNNS Monte Carlo Manager object.
        virtual ~GunnsMonteCarlo();
        /// @brief Initializes in the Master role.
        void initMaster();
        /// @brief Initializes in the Slave role.
        void initSlave();
        /// @brief Updates the Master role before each Slave run.
        void updateMasterPre();
        /// @brief Updates the Master role after each Slave run.
        void updateMasterPost();
        /// @brief Updates the Master role after all Slave runs.
        void updateMasterShutdown();
        /// @brief Updates the Slave role prior to the Slave run.
        void updateSlavePre();
        /// @brief Updates the Slave role after to the Slave run.
        void updateSlavePost();
        /// @brief Returns the object name.
        const char* getName() const;
        /// @brief Returns the initialization status.
        bool        isInitialized() const;
        /// @brief Adds a model variable to the Slave-to-Master data.
        void addOutDouble(double* outDouble);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsMonteCarlo(const GunnsMonteCarlo& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsMonteCarlo& operator =(const GunnsMonteCarlo& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   char* (--) The instance name.
///
/// @details  Returns the value of mName as a char*.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* GunnsMonteCarlo::getName() const
{
    return mName.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool (--) The initialization flag.
///
/// @details  Returns the initialization status.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsMonteCarlo::isInitialized() const
{
    return mInitFlag;
}

#endif
