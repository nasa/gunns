#ifndef GunnsFluidExternalSupply_EXISTS
#define GunnsFluidExternalSupply_EXISTS

/**
@file
@brief    GUNNS Fluid External Supply Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_EXTERNAL_SUPPLY    GUNNS Fluid External Supply Link
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Gunns Fluid External Supply link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidExternalSupply.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-02) (Initial Prototype)

@{
*/

#include "GunnsFluidSource.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid External Supply Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid External
///           Supply configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidExternalSupplyConfigData : public GunnsFluidSourceConfigData
{
    public:
        PolyFluidConfigData*       mExternalConfig;        /**< (--) trick_chkpnt_io(**) Pointer to external network fluid config */
        FluidProperties::FluidType mConvertToType;         /**< (--) trick_chkpnt_io(**) Fluid to convert extra constituents to */
        bool                       mUseNetworkCapacitance; /**< (--) trick_chkpnt_io(**) Causes mSupplyCapacitance to be available for external demands */

        /// @brief Default constructs this Fluid External Supply configuration data.
        GunnsFluidExternalSupplyConfigData(
                const std::string&         name                  = "",
                GunnsNodeList*             nodes                 = 0,
                PolyFluidConfigData*       externalConfig        = 0,
                FluidProperties::FluidType convertToType         = FluidProperties::NO_FLUID,
                const bool                 useNetworkCapacitance = false);

        /// @brief Default destructs this Fluid External Supply configuration data.
        virtual ~GunnsFluidExternalSupplyConfigData();

        /// @brief Copy constructs this Fluid External Supply configuration data.
        GunnsFluidExternalSupplyConfigData(const GunnsFluidExternalSupplyConfigData& that);

    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidExternalSupplyConfigData& operator =(const GunnsFluidExternalSupplyConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid External Supply Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid External
///           Supply input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidExternalSupplyInputData : public GunnsFluidSourceInputData
{
    public:
        double  mDemandTemperature;     /**< (K)  trick_chkpnt_io(**) Initial demand temperature */
        double* mDemandMassFractions;   /**< (--) trick_chkpnt_io(**) Initial demand mass fractions */
        double* mDemandTcMoleFractions; /**< (--) trick_chkpnt_io(**) Initial demand trace compounds mole fractions */
        /// @brief    Default constructs this Fluid External Supply input data.
        GunnsFluidExternalSupplyInputData(const bool    malfBlockageFlag      = false,
                                          const double  malfBlockageValue     = 0.0,
                                          const double  flowDemand            = 0.0,
                                          const double  demandTemperature     = 0.0,
                                                double* demandMassFractions   = 0,
                                                double* demandTcMoleFractions = 0);

        /// @brief    Default destructs this Fluid External Supply input data.
        virtual ~GunnsFluidExternalSupplyInputData();

        /// @brief    Copy constructs this Fluid External Supply input data.
        GunnsFluidExternalSupplyInputData(const GunnsFluidExternalSupplyInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidExternalSupplyInputData& operator =(const GunnsFluidExternalSupplyInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid External Supply class
///
/// @details  This class inherits GunnsFluidSource, and is used in tandem with the
///           GunnsFluidExternalDemand class to connect two separate Gunns networks together, with
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
///       *            *------------< (Ground)             (Ground) <-----------*            *
///       *    NODE    *   GunnsFluidExternalSupply | GunnsFluidExternalDemand  *    NODE    *
///        *          *              Link                       Link             *          *
///         ***    ***                              |                             ***    ***
///            ****                                                                  ****
///                                                 |
///
///                                                 |
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidExternalSupply : public GunnsFluidSource
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidExternalSupply);

    public:
        /// @brief Default Constructor
        GunnsFluidExternalSupply();

        /// @brief Default Destructor
        virtual ~GunnsFluidExternalSupply();

        /// @brief Initialize the link with config data and port assignments
        void initialize(const GunnsFluidExternalSupplyConfigData& configData,
                        const GunnsFluidExternalSupplyInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        const int                                 port0,
                        const int                                 port1);

        /// @brief Special processing of data inputs to the model prior to the network update
        virtual void processInputs();

        /// @brief Special processing of data outputs from the model after the network update
        virtual void processOutputs();

        /// @brief Step method for updating the link
        virtual void step(const double dt);

        /// @brief Method for transporting flows across the link between nodes.
        virtual void transportFlows(const double dt);

    protected:
        bool    mUseNetworkCapacitance; /**<    (--)         trick_chkpnt_io(**) Causes mSupplyCapacitance to be available for external demands */
        int*    mTransformMap;          /**< ** (--)         trick_chkpnt_io(**) Map to convert the external fluid to this config */
        double  mSupplyCapacitance;     /**<    (kg*mol/kPa)                     Local network effective capacitance output to sim bus */
        double  mSupplyPressure;        /**<    (kPa)                            Supply pressure output to sim bus */
        double  mSupplyTemperature;     /**<    (K)                              Supply temperature output to sim bus */
        double* mSupplyMassFractions;   /**<    (--)         trick_chkpnt_io(**) Supply mass fractions output to sim bus */
        double* mSupplyTcMoleFractions; /**<    (--)         trick_chkpnt_io(**) Supply trace compounds mole fractions output to sim bus */
        double  mDemandFlux;            /**<    (kg*mol/s)                       Demand molar flux input from sim bus */
        double  mDemandTemperature;     /**<    (K)                              Demand temperature input from sim bus */
        double* mDemandMassFractions;   /**<    (--)         trick_chkpnt_io(**) Demand mass fractions input from sim bus */
        double* mDemandTcMoleFractions; /**<    (--)         trick_chkpnt_io(**) Demand trace compounds mole fractions input from sim bus */
        static const double mNetworkCapacitanceFlux; /**< ** (kg*mol/s) Flux value to use in network node capacitance calculations */

        /// @brief Validates the initialization of this Gunns Fluid External Supply
        void validate() const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Checks for valid implementation-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidExternalSupply(const GunnsFluidExternalSupply& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidExternalSupply& operator =(const GunnsFluidExternalSupply& that);
};

/// @}

#endif
