#ifndef GunnsFluidExternalDemand_EXISTS
#define GunnsFluidExternalDemand_EXISTS

/**
@file
@brief    GUNNS Fluid External Demand Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_EXTERNAL_DEMAND    GUNNS Fluid External Demand Link
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid External Demand link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidExternalDemand.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-02) (Initial Prototype)

@{
*/

#include "GunnsFluidPotential.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid External Demand Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid External
///           Demand configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidExternalDemandConfigData : public GunnsFluidPotentialConfigData
{
    public:
        double mFilterMinConductivity; /**< (m2)  trick_chkpnt_io(**) Minimum filtered effective conductivity */
        double mFilterMinDeltaP;       /**< (kPa) trick_chkpnt_io(**) Minimum delta-pressure for capacitance estimate */
        double mFilterCapacitanceGain; /**< (--)  trick_chkpnt_io(**) Gain for estimated capacitance filter (0-1) */
        PolyFluidConfigData*       mExternalConfig; /**< (--) trick_chkpnt_io(**) Pointer to external network fluid config */
        FluidProperties::FluidType mConvertToType;  /**< (--) trick_chkpnt_io(**) Fluid to convert extra constituents to */

        /// @brief Default constructs this Fluid External Demand configuration data.
        GunnsFluidExternalDemandConfigData(
                const std::string&         name                  = "",
                GunnsNodeList*             nodes                 = 0,
                const double               maxConductivity       = 0.0,
                const double               expansionScaleFactor  = 0.0,
                const double               filterMinConductivity = 1.0,
                const double               filterMinDeltaP       = 1.0E-8,
                const double               filterCapacitanceGain = 0.05,
                PolyFluidConfigData*       externalConfig        = 0,
                FluidProperties::FluidType convertToType         = FluidProperties::NO_FLUID);

        /// @brief Default destructs this Fluid External Demand configuration data.
        virtual ~GunnsFluidExternalDemandConfigData();

        /// @brief Copy constructs this Fluid External Demand configuration data.
        GunnsFluidExternalDemandConfigData(const GunnsFluidExternalDemandConfigData& that);

    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidExternalDemandConfigData& operator =(const GunnsFluidExternalDemandConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid External Demand Input Data
///
/// @details  This provides a data structure for the Fluid External Demand input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidExternalDemandInputData : public GunnsFluidPotentialInputData
{
    public:
        double  mSupplyCapacitance;     /**< (kg*mol/kPa) trick_chkpnt_io(**) Initial supply capacitance */
        double  mSupplyTemperature;     /**< (K)          trick_chkpnt_io(**) Initial supply temperature */
        double* mSupplyMassFractions;   /**< (--)         trick_chkpnt_io(**) Initial supply mass fractions */
        double* mSupplyTcMoleFractions; /**< (--)         trick_chkpnt_io(**) Initial supply trace compounds mole fractions */
        /// @brief Default constructs this Fluid External Demand input data.
        GunnsFluidExternalDemandInputData(const bool    malfBlockageFlag      = false,
                                          const double  malfBlockageValue     = 0.0,
                                          const double  sourcePressure        = 0.0,
                                          const double  supplyCapacitance     = 0.0,
                                          const double  supplyTemperature     = 0.0,
                                                double* supplyMassFractions   = 0,
                                                double* supplyTcMoleFractions = 0);

        /// @brief Copy constructs this Fluid External Demand input data.
        GunnsFluidExternalDemandInputData(const GunnsFluidExternalDemandInputData& that);

        /// @brief Default destructs this Fluid External Demand input data.
        virtual ~GunnsFluidExternalDemandInputData();

    protected:

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidExternalDemandInputData& operator =(const GunnsFluidExternalDemandInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid External Demand class
///
/// @details  This class inherits GunnsFluidPotential, and is used in tandem with the
///           GunnsFluidExternalSupply class to connect two separate Gunns networks together, with
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
///       *    NODE    *   GunnsFluidExternalSupply | GunnsFluidExternalDemand  *    NODE    *
///        *          *              Link                      Link              *          *
///         ***    ***                              |                             ***    ***
///            ****                                                                  ****
///                                                 |
///
///                                                 |
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidExternalDemand : public GunnsFluidPotential
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidExternalDemand);

    public:
        /// @brief Default Constructor
        GunnsFluidExternalDemand();

        /// @brief Default Destructor
        virtual ~GunnsFluidExternalDemand();

        /// @brief Initializes the link, with input data
        void initialize(const GunnsFluidExternalDemandConfigData& configData,
                        const GunnsFluidExternalDemandInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        const int                                 port0,
                        const int                                 port1);

        /// @brief Special processing of data inputs to the model prior to the network update
        virtual void processInputs();

        /// @brief Special processing of data outputs from the model after the network update
        virtual void processOutputs();

        /// @brief Updates link for the frame
        virtual void updateState(const double dt);

    protected:
        double  mFilterMinConductivity; /**<    (m2)         trick_chkpnt_io(**) Minimum filtered effective conductivity */
        double  mFilterMinDeltaP;       /**<    (kPa)        trick_chkpnt_io(**) Minimum delta-pressure for capacitance estimate */
        int*    mTransformMap;          /**< ** (--)         trick_chkpnt_io(**) Map to convert the external fluid to this config */
        double  mAvgDemand;             /**<    (kg*mol/s)                       2-frame moving average of past flux demand */
        double  mAvgSupplyP;            /**<    (kPa)                            2-frame moving average of past supply potential */
        double  mAvgSupplyDeltaP;       /**<    (kPa)        trick_chkpnt_io(**) 2-frame moving average of supply delta-potential */
        double  mEstimatedCapacitance;  /**<    (kg*mol/kPa)                     Estimate of supply effective capacitance */
        double  mFilterCapacitanceGain; /**<    (--)         trick_chkpnt_io(**) Gain for estimated capacitance filter (0-1) */
        double  mSupplyCapacitance;     /**<    (kg*mol/kPa)                     Supply network capacitance input from sim bus */
        double  mSupplyPressure;        /**<    (kPa)                            Supply pressure input from sim bus */
        double  mSupplyTemperature;     /**<    (K)                              Supply temperature input from sim bus */
        double* mSupplyMassFractions;   /**<    (--)         trick_chkpnt_io(**) Supply mass fractions input from sim bus */
        double* mSupplyTcMoleFractions; /**<    (--)         trick_chkpnt_io(**) Supply trace compounds mole fractions input from sim bus */
        double  mDemandFlux;            /**<    (kg*mol/s)                       Demand molar flux output to sim bus */
        double  mDemandTemperature;     /**<    (K)                              Demand temperature output to sim bus */
        double* mDemandMassFractions;   /**<    (--)         trick_chkpnt_io(**) Demand mass fractions output to sim bus */
        double* mDemandTcMoleFractions; /**<    (--)         trick_chkpnt_io(**) Demand trace compounds mole fractions output to sim bus */

        /// @brief Validates the initialization of this Gunns Fluid External Demand
        void validate() const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief This implementation skips the optional linearization for the admittance matrix.
        virtual double linearizeConductance();

        /// @brief Checks for valid implementation-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidExternalDemand(const GunnsFluidExternalDemand& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidExternalDemand& operator =(const GunnsFluidExternalDemand& that);
};

/// @}

#endif
