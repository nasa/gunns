#ifndef GunnsThermalCapacitor_EXISTS
#define GunnsThermalCapacitor_EXISTS

/************************** TRICK HEADER ***********************************************************
@defgroup  TSM_GUNNS_THERMAL_CAPACITOR    GUNNS Thermal Capacitor Link
@ingroup   TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
     (The GunnsThermalCapacitor will model almost all thermal aspects and be compatible within a
      GUNNS Thermal Network. The basic function of the class is to apply a thermal capacitance to a
      GUNNS node. The higher the capacitance value, the more heat energy it will take to raise the
      potential, or temperature, of the node. The capacitance is determined by multiplying the mass
      of an object by its specific heat (based on a weighted average of the specific heat values of
      its material make-up). GunnsThermalCapacitor is derived from GunnsBasicCapacitor, with the
      added capability of allowing an external heat flux, from another aspect, to be written to its
      mExternalHeatFlux[] array. The members of this array are summed and then applied into the
      Gunns network.

      Note: The "potential" that is solved for in GUNNS is the same as "temperature" for a thermal
      network. The two are used interchangeably in the commenting below.)

REQUIREMENTS:
     (R.TS228-0023: Spacecraft thermal models shall simulate temperatures of passively
                    cooled components.
      R.TS228-0024: Spacecraft thermal models shall provide thermal data to passively cooled
                    components.
      R.TS228-0025: Spacecraft thermal models shall receive thermal data from passively
                    cooled components.
      R.TS228-0026: Spacecraft thermal models shall provide thermal data to thermally
                    protected components.)

REFERENCE:
     ()

ASSUMPTIONS AND LIMITATIONS:
     (GunnsThermalCapacitor assumes a constant thermal capacitance.)

LIBRARY DEPENDENCY:
    (
     (GunnsThermalCapacitor.o)
    )

PROGRAMMERS:
    (
     (Jason Harvey)  (L3) (2011-02) (Initial Prototype)
     (Kevin Supak)   (L3) (2011-02) (Updated to Coding Standards)
     (Joe Valerioti) (L3) (2011-11) (Significant updates, unit-test and code review)
    )
@{
 **************************************************************************************************/
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicCapacitor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsThermalCapacitor Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermal
///           Capacitor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalCapacitorConfigData : public GunnsBasicCapacitorConfigData
{
    public:
        int mEditCapacitanceGroup;  /**< (--) trick_chkpnt_io(**) Identifier for capacitance edit grouping */
        /// @brief   Default constructs this GunnsThermalCapacitor configuration data.
        GunnsThermalCapacitorConfigData(const std::string& name        = "unnamed capacitor",
                                        GunnsNodeList* nodes           = 0,
                                        const int editCapacitanceGroup = -1);

        /// @brief   Default destructs this GunnsThermalCapacitor configuration data.
        virtual ~GunnsThermalCapacitorConfigData();
        /// @brief   Copy constructs this GunnsThermalCapacitor configuration data.
        GunnsThermalCapacitorConfigData(const GunnsThermalCapacitorConfigData& that);

    protected:
        // Nothing
    private:
        /// @brief   Assignment operator unavailable since declared private and not implemented.
        GunnsThermalCapacitorConfigData& operator =(const GunnsThermalCapacitorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsThermalCapacitor Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermal
///           Capacitor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalCapacitorInputData : public GunnsBasicCapacitorInputData
{
    public:
        static const double DEFAULT_TEMPERATURE;       /**< (K)  trick_chkpnt_io(**) Standard default temperature */
        bool                iTemperatureOverrideFlag;  /**< (--) trick_chkpnt_io(**) Flag to activate Temperature override */
        double              iTemperatureOverrideValue; /**< (K)  trick_chkpnt_io(**) Value at which to override Temperature */

        /// @brief   Default constructs this Thermal Capacitor input data.
        GunnsThermalCapacitorInputData(const bool malfBlockageFlag           = false,
                                       const double malfBlockageValue        = 0.0,
                                       const double capacitance              = 0.0,
                                       const double potential                = DEFAULT_TEMPERATURE,
                                       const bool   temperatureOverrideFlag  = false,
                                       const double temperatureOverrideValue = DEFAULT_TEMPERATURE);

        /// @brief   Default destructs this Thermal Capacitor input data.
        virtual ~GunnsThermalCapacitorInputData();
        /// @brief   Copy constructs this Thermal Capacitor input data.
        GunnsThermalCapacitorInputData(const GunnsThermalCapacitorInputData& that);

    protected:
        // Nothing
    private:
        /// @details   Assignment operator unavailable since declared private and not implemented.
        GunnsThermalCapacitorInputData& operator =(const GunnsThermalCapacitorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsThermalCapacitor Class
///
/// @details  The purpose of this class is to model thermal capacitance in a GUNNS network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalCapacitor : public GunnsBasicCapacitor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsThermalCapacitor);

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief    Enumeration of the number of heat fluxes that can be written to this capacitor
        ///           from external sources.
        ///
        /// @details  This enumeration is used to specify the size of the mExternalHeatFlux array.
        ///           It represents the maximum amount of heat fluxes that can be given to a
        ///           capacitor from external aspects. This value can be freely increased without
        ///           consequence.
        ////////////////////////////////////////////////////////////////////////////////////////////
        enum NumExtHeatFluxes {
            NUM_EXT_HEATFLUXES = 15,  ///< Amount of heat fluxes from external aspects.
        };

        /// @brief   Default Constructor
        GunnsThermalCapacitor();
        /// @brief   Default Destructor
        virtual ~GunnsThermalCapacitor();

        /// @brief   ThermalCapacitor initialization method
        void initialize(const GunnsThermalCapacitorConfigData& configData,
                        const GunnsThermalCapacitorInputData& inputData,
                        std::vector<GunnsBasicLink*>& networkLinks,
                        const int port0,
                        const int port1);

        /// @brief   Returns potential of non-vacuum node.
        double getTemperature() const;

        /// @brief   Returns the edit capacitance group number.
        int    getEditCapacitanceGroup() const;

        /// @brief   Sets an external heat flux.
        void   setExternalHeatFlux(const unsigned int index, const double flux);

        /// @brief   Overwrites a base-class method that executes right after GUNNS updates every
        ///          link's mPotentialVector. Used here to update mTemperature.
        virtual void processOutputs();

    protected:
        int     mEditCapacitanceGroup;            /**< (--) trick_chkpnt_io(**) Identifier for capacitance edit grouping */
        double  mTemperature;                     /**< (K)                      Temperature of link's non-vacuum node (port0) */
        double  mExternalHeatFlux[GunnsThermalCapacitor::NUM_EXT_HEATFLUXES]; /**< (W)  trick_chkpnt_io(**) Array of external heat fluxes */
        double  mSumExternalHeatFluxes;           /**< (W)  trick_chkpnt_io(**) Sum of the external heat fluxes */

        /// @brief   Updates the state.
        virtual void updateState(const double dt);

        /// @brief  Validates the ThermalCapacitor initial state.
        void  validate(const GunnsThermalCapacitorInputData& inputData) const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @brief   Copy constructor unavailable since declared private and not implemented.
        GunnsThermalCapacitor(const GunnsThermalCapacitor&);
        /// @brief   Assignment operator unavailable since declared private and not implemented.
        GunnsThermalCapacitor& operator =(const GunnsThermalCapacitor&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  mPotentialVector[0]    (K)  potential of link's non-vacuum node
///
/// @details  A getter for the value of the temperature at the link's non-vacuum node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermalCapacitor::getTemperature() const
{
    /** returns the temperature at the link's non-vacuum node */
    return mPotentialVector[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int  (--)  Number of the link's edit capacitance group.
///
/// @details  A getter for the number of the link's edit capacitance group.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsThermalCapacitor::getEditCapacitanceGroup() const
{
    return mEditCapacitanceGroup;
}

#endif
