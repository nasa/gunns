#ifndef GunnsOptimBase_EXISTS
#define GunnsOptimBase_EXISTS

/**
@file     GunnsOptimBase.hh
@brief    GUNNS Optimization Optimizer Base Class declarations

@defgroup  TSM_GUNNS_CORE_OPTIM_BASE    GUNNS Optimization Optimizer Base Class
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Refer to class details below.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsOptimBase.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-05) (Initial))

@{
*/

#include "GunnsOptimMonteCarloTypes.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

//TODO
class GunnsOptimBaseConfigData
{
    public:
        /// @brief Constructs the GUNNS Monte Carlo Optimizer Base Config Data object.
        GunnsOptimBaseConfigData();
        /// @brief Destructs the GUNNS Monte Carlo Optimizer Base Config Data object.
        virtual ~GunnsOptimBaseConfigData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimBaseConfigData(const GunnsOptimBaseConfigData&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsOptimBaseConfigData& operator =(const GunnsOptimBaseConfigData&);
};

//TODO
class GunnsOptimBase
{
    public:
        const std::vector<GunnsOptimMonteCarloInput>* mInStatesMaster;  /**< ** (1) trick_chkpnt_io(**) Pointer to the Master state space description. */
        int                                      mGlobalRunCounter; /**< *o (1) trick_chkpnt_io(**) Count of the total elapsed runs from all epochs. */
        int                                      mRunCounter;      /**< *o (1) trick_chkpnt_io(**) Count of the elapsed runs in the current epoch. */
        int                                      mEpoch;           /**< *o (1) trick_chkpnt_io(**) The current epoch number. */
        /// @brief Constructs the GUNNS Monte Carlo Optimizer Base object.
        GunnsOptimBase();
        /// @brief Destructs the GUNNS Monte Carlo Optimizer Base object.
        virtual ~GunnsOptimBase();
        //TODO
        virtual void setConfigData(const GunnsOptimBaseConfigData* configData);
        /// @brief TODO replace args with polymorphic config data, similar to solver-links.
        //TODO pure virtual base
        virtual void initialize(const std::vector<GunnsOptimMonteCarloInput>* inStatesMaster) = 0;
        //TODO pure virtual base
        virtual void update() = 0;
        /// @brief TODO DEAD CODE but useful?
        double uniformRand() { return std::rand() / RAND_MAX; }
        /// @brief TODO this can be called from input file before init
        //TODO pure virtual base
        virtual unsigned int getNumRuns() const = 0;
        /// @brief TODO returns the MC var states from the active particle to teh MC manager to send to teh slave
        //TODO pure virtual base
        virtual const std::vector<double>* getState() const = 0;
        /// @brief Returns the RSS magnitude of the given vector's components.
        double computeVectorMagnitude(const std::vector<double>& vec) const;
        //TODO
        void normalizeVector(std::vector<double>& vec, const double magnitude) const;
        //TODO pure virtual base
        virtual void assignCost(const double cost, double runID, double runIdReturned) = 0;
        //TODO pure vurtial base
        virtual void shutdown() const = 0;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimBase(const GunnsOptimBase&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsOptimBase& operator =(const GunnsOptimBase&);
};

/// @}

#endif
