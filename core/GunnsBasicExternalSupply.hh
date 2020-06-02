#ifndef GunnsBasicExternalSupply_EXISTS
#define GunnsBasicExternalSupply_EXISTS

/**
@file
@brief    GUNNS Basic External Supply Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC_EXTERNAL_SUPPLY    GUNNS Basic External Supply Link
@ingroup   TSM_GUNNS_CORE_LINK_BASIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Gunns Basic External Supply link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsBasicExternalSupply.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-02) (Initial Prototype)

@{
*/

#include "GunnsBasicSource.hh"
#include "software/SimBus/SimBusQutils.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic External Supply Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic External
///           Supply configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicExternalSupplyConfigData: public GunnsBasicSourceConfigData
{
    public:
        /// @brief Default constructs this Basic External Supply configuration data.
        GunnsBasicExternalSupplyConfigData(const std::string& name  = "",
                                           GunnsNodeList*     nodes = 0);

        /// @brief Default destructs this Basic External Supply configuration data.
        virtual ~GunnsBasicExternalSupplyConfigData();

        /// @brief Copy constructs this Basic External Supply configuration data.
        GunnsBasicExternalSupplyConfigData(const GunnsBasicExternalSupplyConfigData& that);

    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicExternalSupplyConfigData& operator =(const GunnsBasicExternalSupplyConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic External Supply Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic External
///           Supply input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicExternalSupplyInputData : public GunnsBasicSourceInputData
{
    public:
        /// @brief    Default constructs this Basic External Supply input data.
        GunnsBasicExternalSupplyInputData(const bool   malfBlockageFlag  = false,
                                          const double malfBlockageValue = 0.0,
                                          const double sourceFlux        = 0.0);

        /// @brief    Default destructs this Basic External Supply input data.
        virtual ~GunnsBasicExternalSupplyInputData();

        /// @brief    Copy constructs this Basic External Supply input data.
        GunnsBasicExternalSupplyInputData(const GunnsBasicExternalSupplyInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicExternalSupplyInputData& operator =(const GunnsBasicExternalSupplyInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basic External Supply class
///
/// @details  This class inherits GunnsBasicSource, and is used in tandem with the
///           GunnsBasicExternalDemand class to connect two separate Gunns networks together, with
///           this class applying demand from the external demand-side network to our local
///           supply-side network.
///           \verbatim
///
///           Supply-Side Network (local)                             Demand-Side Network (external)
///                                                 |
///
///                                                 |
///            ****                                                                  ****
///         ***    ***                              |                             ***    ***
///        *          *                                                          *          *
///       *   SUPPLY   *                            |                           *   DEMAND   *
///       *            *------------> (Ground)             (Ground) >-----------*            *
///       *    NODE    *   GunnsBasicExternalSupply | GunnsBasicExternalDemand  *    NODE    *
///        *          *              Link                       Link             *          *
///         ***    ***                              |                             ***    ***
///            ****                                                                  ****
///                                                 |
///
///                                                 |
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicExternalSupply : public GunnsBasicSource
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBasicExternalSupply);

    public:
        /// @brief Default Constructor
        GunnsBasicExternalSupply();

        /// @brief Default Destructor
        virtual ~GunnsBasicExternalSupply();

        /// @brief Initialize the link with config data and port assignments
        void initialize(const GunnsBasicExternalSupplyConfigData& configData,
                        const GunnsBasicExternalSupplyInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        const int                                 port0,
                        const int                                 port1);

        /// @brief Special processing of data inputs to the model prior to the network update
        virtual void   processInputs();

        /// @brief Special processing of data outputs from the model after the network update
        virtual void   processOutputs();

    protected:
        double  mSupplyPotential;       /**< (--)                     Supply potential output to sim bus */
        double* mDemandFlux_queue;      /**< (--) trick_chkpnt_io(**) Demand flux queue values input from sim bus */
        int     mDemandFlux_queue_size; /**< (--) trick_chkpnt_io(**) Demand flux queue size input from sim bus */

        /// @brief Validates the initialization of this Gunns Basic External Supply
        void validate() const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Checks for valid implementation-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsBasicExternalSupply(const GunnsBasicExternalSupply& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicExternalSupply& operator =(const GunnsBasicExternalSupply& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles data written to the external network's demand link, via the sim bus.  Data is
///          moved from the supply node into the sim bus output members.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicExternalSupply::processOutputs()
{
    mSupplyPotential = mNodes[0]->getPotential();
}

#endif
