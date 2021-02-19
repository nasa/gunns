#ifndef GunnsThermalMultiPanel_EXISTS
#define GunnsThermalMultiPanel_EXISTS

/**
@file
@brief    GUNNS Orchestrator declarations

@defgroup  TSM_GUNNS_THERMAL_MULTI_PANEL    GUNNS Thermal Multiple Sources Panel Link
@ingroup   TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (The GunnsThermalMultiPanel is a variant of GunnsThermalPanel that allows for the separate
    absorption of multiple radiant sources with different absorptivity values for each.  We also add
    terms to receive a separate view scalar for each source from an external view factor/occultation
    model.  We allow for up to 5 sources, which might be:

    Sun
    Planetary albedo
    Planetary infrared
    Other body albedo (such as a moon or other vehicle)
    Other body infrared

    ...though you may use the 5 inputs for any source -- the above list is just a suggestion.

    GunnsThermalMultiPanel derives from GunnsThermalSource.
   )

REQUIREMENTS:
     ()

REFERENCE:
     ()

ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
     (GunnsThermalMultiPanel.o)

PROGRAMMERS:
     (
     ((Jason Harvey) (L-3 Communications) (July 2015) (Initial))
     )
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsThermalSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermal Multi-Panel Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermal Multi-
///           Panel configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalMultiPanelConfigData : public GunnsThermalSourceConfigData
{
    public:
        double cAbsorptivity[5]; /**< (--) trick_chkpnt_io(**) Absorptivity (0-1) of exterior panel to each radiant source */
        double cSurfaceArea;     /**< (m2) trick_chkpnt_io(**) Surface area of exterior panel */
        /// @brief  Default constructs this Thermal Multi-Panel configuration data.
        GunnsThermalMultiPanelConfigData(const std::string&   name                      = "",
                                         GunnsNodeList*       nodes                     = 0,
                                         const double         tuningScalar              = 1.0,
                                         std::vector<double>* fluxDistributionFractions = 0,
                                         const double         absorptivity0             = 0.0,
                                         const double         absorptivity1             = 0.0,
                                         const double         absorptivity2             = 0.0,
                                         const double         absorptivity3             = 0.0,
                                         const double         absorptivity4             = 0.0,
                                         const double         surfaceArea               = 1.0);
        /// @brief  Default destructs this Thermal Multi-Panel configuration data.
        virtual ~GunnsThermalMultiPanelConfigData();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        GunnsThermalMultiPanelConfigData(const GunnsThermalMultiPanelConfigData& that);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalMultiPanelConfigData& operator =(const GunnsThermalMultiPanelConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermal Multi-Panel Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermal Multi-
///           Panel input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalMultiPanelInputData : public GunnsThermalSourceInputData
{
    public:
        double iViewScalar[5];   /**< (--)   trick_chkpnt_io(**) Initial view scalar (0-1) to account for occultation or facing, etc */
        double iIncidentFlux[5]; /**< (W/m2) trick_chkpnt_io(**) Initial flux of each radiant source */
        /// @brief   Default constructs this ThermalSource InputData.
        GunnsThermalMultiPanelInputData(const bool           malfBlockageFlag      = false,
                                        const double         malfBlockageValue     = 0.0,
                                        const double         demandedFlux          = 0.0,
                                        const bool           malfFluxOverrideFlag  = false,
                                        const double         malfFluxOverrideValue = 0.0,
                                        const double         viewScalar0           = 0.0,
                                        const double         viewScalar1           = 0.0,
                                        const double         viewScalar2           = 0.0,
                                        const double         viewScalar3           = 0.0,
                                        const double         viewScalar4           = 0.0,
                                        const double         incidentFlux0         = 0.0,
                                        const double         incidentFlux1         = 0.0,
                                        const double         incidentFlux2         = 0.0,
                                        const double         incidentFlux3         = 0.0,
                                        const double         incidentFlux4         = 0.0);
        /// @brief  Default destructs this Thermal Multi-Panel input data.
        virtual ~GunnsThermalMultiPanelInputData();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        GunnsThermalMultiPanelInputData(const GunnsThermalMultiPanelInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalMultiPanelInputData& operator =(const GunnsThermalMultiPanelInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermal Multi-Panel class compatible in a Gunns thermal network.
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalMultiPanel : public GunnsThermalSource
{
    TS_MAKE_SIM_COMPATIBLE(GunnsThermalMultiPanel);

    public:
        /// @brief  Default Constructor.
        GunnsThermalMultiPanel();
        /// @brief  Default Destructor.
        virtual ~GunnsThermalMultiPanel();
        /// @brief  Initializes the link
        void initialize(const GunnsThermalMultiPanelConfigData& configData,
                        const GunnsThermalMultiPanelInputData&  inputData,
                        std::vector<GunnsBasicLink*>&           networkLinks,
                        std::vector<int>*                       portsVector);
        /// @brief  Updates the link during a frame.
        virtual void updateState(const double dt);

        /// @brief  Get absorptivity with index.
        virtual double getAbsorptivity(const int index) const;

        /// @brief  Get surface area.
        virtual double getSurfaceArea() const;

        /// @brief  Get view scalar with index.
        virtual double getViewScalar(const int index) const;

        /// @brief  Get incident flux with index.
        virtual double getIncidentFlux(const int index) const;


    protected:
        double mAbsorptivity[5]; /**< (--)   trick_chkpnt_io(**) Absorptivity (0-1) of exterior panel to each radiant source */
        double mSurfaceArea;     /**< (m2)   trick_chkpnt_io(**) Surface area of exterior panel */
        double mViewScalar[5];   /**< (--)                       View scalar (0-1) of exterior panel to each radiant source */
        double mIncidentFlux[5]; /**< (W/m2)                     Incident flux from each radiant source */
        /// @brief  Validates the Thermal Multi-Panel initial state.
        void  validate(const GunnsThermalMultiPanelConfigData& configData,
                       const GunnsThermalMultiPanelInputData&  inputData) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsThermalMultiPanel(const GunnsThermalMultiPanel& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalMultiPanel& operator =(const GunnsThermalMultiPanel& that);
};
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (m2)  Surface area.
///
/// @details  Returns the mSurfaceArea value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermalMultiPanel::getSurfaceArea() const
{
    return mSurfaceArea;
}

#endif
