#ifndef GunnsBasicExternalDemand_EXISTS
#define GunnsBasicExternalDemand_EXISTS

/**
@file
@brief    GUNNS Basic External Demand Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC_EXTERNAL_DEMAND    GUNNS Basic External Demand Link
@ingroup   TSM_GUNNS_CORE_LINK_BASIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Basic External Demand link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsBasicExternalDemand.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-02) (Initial Prototype)

@{
*/

#include "GunnsBasicPotential.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic External Demand Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic External
///           Demand configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicExternalDemandConfigData : public GunnsBasicPotentialConfigData
{
    public:
        double mFilterMinConductivity; /**< (--) trick_chkpnt_io(**) Minimum filtered effective conductivity */
        double mFilterMinDeltaP;       /**< (--) trick_chkpnt_io(**) Minimum delta-potential for capacitance estimate */
        double mFilterCapacitanceGain; /**< (--) trick_chkpnt_io(**) Gain for estimated capacitance filter (0-1) */

        /// @brief Default constructs this Basic External Demand configuration data.
        GunnsBasicExternalDemandConfigData(
                const std::string&         name                  = "",
                GunnsNodeList*             nodes                 = 0,
                const double               defaultConductivity   = 0.0,
                const double               filterMinConductivity = 1.0,
                const double               filterMinDeltaP       = 1.0E-8,
                const double               filterCapacitanceGain = 0.05);

        /// @brief Default destructs this Basic External Demand configuration data.
        virtual ~GunnsBasicExternalDemandConfigData();

        /// @brief Copy constructs this Basic External Demand configuration data.
        GunnsBasicExternalDemandConfigData(const GunnsBasicExternalDemandConfigData& that);

    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicExternalDemandConfigData& operator =(const GunnsBasicExternalDemandConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic External Demand Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic External
///           Demand input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicExternalDemandInputData : public GunnsBasicPotentialInputData
{
    public:
        /// @brief    Default constructs this Basic External Demand input data.
        GunnsBasicExternalDemandInputData(const bool   malfBlockageFlag     = false,
                                          const double malfBlockageValue    = 0.0,
                                          const double sourcePotential      = 0.0);

        /// @brief    Default destructs this Basic External Demand input data.
        virtual ~GunnsBasicExternalDemandInputData();

        /// @brief    Copy constructs this Basic External Demand input data.
        GunnsBasicExternalDemandInputData(const GunnsBasicExternalDemandInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicExternalDemandInputData& operator =(const GunnsBasicExternalDemandInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basic External Demand class
///
/// @details  This class inherits GunnsBasicPotential, and is used in tandem with the
///           GunnsBasicExternalSupply class to connect two separate Gunns networks together, with
///           this class applying potential from the external supply-side network to our local
///           demand-side network.
///           \verbatim
///
///           Supply-Side Network (external)                             Demand-Side Network (local)
///                                                 |
///
///                                                 |
///            ****                                                                  ****
///         ***    ***                              |                             ***    ***
///        *          *                                                          *          *
///       *   SUPPLY   *                            |                           *   DEMAND   *
///       *            *------------> (Ground)             (Ground) >-----------*            *
///       *    NODE    *   GunnsBasicExternalSupply | GunnsBasicExternalDemand  *    NODE    *
///        *          *              Link                      Link              *          *
///         ***    ***                              |                             ***    ***
///            ****                                                                  ****
///                                                 |
///
///                                                 |
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicExternalDemand : public GunnsBasicPotential
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBasicExternalDemand);

    public:
        /// @brief Default Constructor
        GunnsBasicExternalDemand();

        /// @brief Default Destructor
        virtual ~GunnsBasicExternalDemand();

        /// @brief Initializes the link, with input data
        void initialize(const GunnsBasicExternalDemandConfigData& configData,
                        const GunnsBasicExternalDemandInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        const int                                 port0,
                        const int                                 port1);

        /// @brief Special processing of data inputs to the model prior to the network update
        virtual void processInputs();

        /// @brief Special processing of data outputs from the model after the network update
        virtual void processOutputs();

        /// @brief Updates the state of the link;
        virtual void updateState(const double dt);

    protected:
        double mFilterMinConductivity; /**< (--) trick_chkpnt_io(**) Minimum filtered effective conductivity */
        double mFilterMinDeltaP;       /**< (--) trick_chkpnt_io(**) Minimum delta-potential for capacitance estimate */
        double mAvgDemand;             /**< (--)                     2-frame moving average of past flux demand */
        double mAvgSupplyP;            /**< (--)                     2-frame moving average of past supply potential */
        double mAvgSupplyDeltaP;       /**< (--) trick_chkpnt_io(**) 2-frame moving average of supply delta-potential */
        double mEstimatedCapacitance;  /**< (--)                     Estimate of supply network effective capacitance */
        double mFilterCapacitanceGain; /**< (--) trick_chkpnt_io(**) Gain for estimated capacitance filter (0-1) */
        double mSupplyPotential;       /**< (--)                     Supply potential input from sim bus */
        double mDemandFlux;            /**< (--)                     Demand flux output to sim bus */

        /// @brief Validates the initialization of this Gunns Basic External Demand
        void validate() const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Checks for valid implementation-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsBasicExternalDemand(const GunnsBasicExternalDemand& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicExternalDemand& operator =(const GunnsBasicExternalDemand& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles data read from the external network's supply link, via the sim bus.  Data is
///          moved from the sim bus input members into the link internal members.  If the supply
///          network runs at a different rate than this network, we always want the latest supply
///          potential regardless of how often it runs, therefore we don't use a queue in the sim
///          bus.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicExternalDemand::processInputs()
{
    setSourcePotential(mSupplyPotential);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles data written to the external network's supply link, via the sim bus.  Data is
///          moved from the demand link into the sim bus output members.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicExternalDemand::processOutputs()
{
    mDemandFlux = mFlux;
}

#endif
