#ifndef GunnsThermalPanel_EXISTS
#define GunnsThermalPanel_EXISTS
/************************** TRICK HEADER ***********************************************************
@defgroup  TSM_GUNNS_THERMAL_PANEL    Gunns ThermalPanel Link
@ingroup   TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (The GunnsThermalPanel is designed to be the interface between a thermal network and the
    Environment ephemeris model. It is a medium to distribute a heatflux across multiple nodes
    in a Gunns ThermalNetwork. It is a multi-port link, and the proportion of the overall flux
    that is applied to each specific port is given in a flux-distribution-fraction array.

    GunnsThermalPanel is derived from GunnsThermalSource. In a standard GunnsThermalSource,
    mDemandedFlux is set directly (by the SimBus). In GunnsThermalPanel, mIncidentHeatFlux is
    set directly, and mDemandedFlux is derived as the product of the following four members:

    1. mIncidentHeatFlux (W/m2) :: Set externally by the SimBus. This member is calculated by
                                   the Environment model, taking into account such factors
                                   as solar, albedo, and infrared radiation. This orbital data is
                                   applied against configuration data specific to each panel,
                                   such as position and surface normal vector, which should be
                                   read by ENV at initialization. ENV factors in all this
                                   information and publishes a single W/m2 value to the SimBus.

    2. mAbsorptivity (--) :: Fraction (0-1) of panel's absorption of environmental radiative heat
                             fluxes (the rest is reflected). This value is assumed to be a summary
                             of the optical absorptivity of solar, earth albedo, and infrared
                             radiation.

    3. mSurfaceArea (m2) :: Surface area of this specific panel. Assumed not to change.
   )

REQUIREMENTS:
     (R.TS228-0013: TS21 thermal models shall receive ephemeris data from the Environment model.)

REFERENCE:
     ()

ASSUMPTIONS AND LIMITATIONS:
     (
     (GunnsThermalPanel assumes an absorptivity that accounts for the entire optical spectrum:
      solar, infrared, and earth albedo.)
     (GunnsThermalPanel assumes that its surface area is constant. Any disruption of line-of-sight
      is assumed to be incorporated into the incident heatflux value provided by Environment.)
     )

LIBRARY DEPENDENCY:
     (GunnsThermalPanel.o)

PROGRAMMERS:
     (
      (Joe Valerioti) (L3) (May 2011) (derived from GunnsBasicSource)
                           (Feb 2012) (re-design to allow W/m2 read from ENV)
                           (Jan 2013) (updated to allow for multi-port capability)
     )
@{
***************************************************************************************************/
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsThermalSource.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ThermalPanel Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the ThermalPanel
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalPanelConfigData : public GunnsThermalSourceConfigData
{
    public:
        double cAbsorptivity; /**< (--)  trick_chkpnt_io(**) absorptivity (0-1) of exterior panel */
        double cSurfaceArea;  /**< (m2)  trick_chkpnt_io(**) surface area of exterior panel */

        /// @brief  Default constructs this ThermalPanel configuration data.
        GunnsThermalPanelConfigData(const std::string& name   = "unnamed panel",
                                    GunnsNodeList* nodes      = 0,
                                    const double tuningScalar = 1.0,
               std::vector<double>* fluxDistributionFractions = 0,
                                    const double absorptivity = 0.5,
                                    const double area         = 1.0);

        /// @brief  Default destructs this ThermalPanel configuration data.
        virtual ~GunnsThermalPanelConfigData();
        /// @brief  Copy constructs this ThermalPanel configuration data.
        GunnsThermalPanelConfigData(const GunnsThermalPanelConfigData& that);

    protected:
        // Nothing
    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalPanelConfigData& operator =(const GunnsThermalPanelConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ThermalPanel Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the ThermalPanel
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalPanelInputData : public GunnsThermalSourceInputData
{
    public:
        /// @brief   Default constructs this ThermalSource InputData.
        GunnsThermalPanelInputData(const bool malfBlockageFlag        = false,
                                   const double malfBlockageValue     = 0.0,
                                   const double demandedFlux          = 0.0,
                                   const bool malfFluxOverrideFlag    = false,
                                   const double malfFluxOverrideValue = 0.0);

        /// @brief  Default destructs this ThermalPanel input data.
        virtual ~GunnsThermalPanelInputData();
        /// @brief  Copy constructs this ThermalPanel input data.
        GunnsThermalPanelInputData(const GunnsThermalPanelInputData& that);

    protected:
        // Nothing
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalPanelInputData& operator =(const GunnsThermalPanelInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ThermalPanel class compatible in a Gunns thermal network.
///
/// @details  The GunnsThermalPanel inherits from GunnsThermalSource, which distributes a heat flux
///           across multiple nodes in a Gunns ThermalNetwork. The proportion of the overall flux
///           that's applied to each specific port is given by the flux-distribution-fraction array.
///           The functionality of the GunnsThermalPanel child-class is in the computation of the
///           heatflux. The magnitude is the product of scalars, representing surface area and
///           optical absorptivity, times the incident radiation flux, computed by the Environment
///           model and set via the SimBus. The flux applied to the ThermalNetwork is then scaled
///           by a tuning scalar and, if active, a blockage malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalPanel : public GunnsThermalSource
{
    TS_MAKE_SIM_COMPATIBLE(GunnsThermalPanel);

    public:
        /// @brief  Default Constructor
        GunnsThermalPanel();
        /// @brief  Default Destructor
        virtual ~GunnsThermalPanel();

        /// @brief  Initializes the link
        void initialize(const GunnsThermalPanelConfigData& configData,
                        const GunnsThermalPanelInputData&  inputData,
                        std::vector<GunnsBasicLink*>&      networkLinks,
                        std::vector<int>*                  portsVector);

        /// @brief  Updates the link during a frame
        virtual void updateState(const double dt);

        /// @brief  Returns the incident flux
        double getIncidentFlux() const;

    protected:
        double mIncidentHeatFluxPerArea; /**< (W/m2)                     incident flux, calculated by ENV, set by bus */
        double mAbsorptivity;            /**< (--)   trick_chkpnt_io(**) absorptivity (0-1) of exterior panel */
        double mSurfaceArea;             /**< (m2)   trick_chkpnt_io(**) surface area of exterior panel */

        /// @brief  Validates the ThermalPanel initial state.
        void  validate(const GunnsThermalPanelConfigData& configData) const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsThermalPanel(const GunnsThermalPanel& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalPanel& operator =(const GunnsThermalPanel& that);
};
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W/m2) The incident flux.
///
/// @details  Getter method returns the incident flux.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermalPanel::getIncidentFlux() const
{
    return mIncidentHeatFluxPerArea;
}

#endif
