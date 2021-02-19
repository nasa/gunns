#ifndef GunnsThermalPhaseChangeBattery_EXISTS
#define GunnsThermalPhaseChangeBattery_EXISTS

/**
@file
@brief    GUNNS Thermal Phase Change Battery Link declarations

@defgroup  TSM_GUNNS_THERMAL_PHASE_CHANGE_BATTERY    GUNNS Thermal Phase Change Battery Link
@ingroup   TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (This models a Phase Change Thermal Battery for heat storage.  The battery casing contains a mass
   of material with a significant thermal capacity.  This link can model the thermal capacity (as
   its mass times the specific heat) for two different phases of matter of the material (typically
   ice & liquid), and the heat of phase change between the phases.  The specific heats and heat of
   phase change are provided as configuration data, so the link can model any kind of material and
   any two phases.  The link labels these phases as "hot" and "cold" since they can be applied to
   any arbitrary paring of phases.  Liquid (hot) and Ice (cold) are the most typical, but you can
   use any phases.

   When all of the phase-change material is in the same phase, either below (cold phase) or above
   (hot phase) the phase change temperature, this link acts like a regular thermal capacitor,
   applies a capacitance to the Port 0 node, and heat into or out of the node changes its
   temperature.  However while in mixed-phase, the link switches to a potential source to constrain
   the node to the phase change temperature, and any heat into or out of the node goes into phase
   change.

   This is a 1-port link.  We do away with the Port 1 in other capacitor-type links since it's never
   used in the thermal aspect.

   There is a leak malfunction for leaking out the hotter phase when it is present.  The hotter
   phase is usually the less viscous and more prone to escape the battery through cracks in the
   enclosure, etc.)

REFERENCE:
  ()

ASSUMPTIONS AND LIMITATIONS:
  ((Internal fluid properties like pressure & thermal expansion are not modeled)
   (The phase change temperature is constant)
   (The specific heats are constant)
   (The entire battery & phase-change medium always has uniform temperature))

LIBRARY_DEPENDENCY:
  (GunnsThermalPhaseChangeBattery.o)

PROGRAMMERS:
  (
   ((Jason Harvey) (CACI) (Novenber 2016) (Initial))
  )
@{
*/

#include "core/GunnsBasicLink.hh"
#include "aspects/thermal/GunnsThermalCapacitor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermal Phase Change Battery link Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermal Phase
///           Change Battery link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalPhaseChangeBatteryConfigData : public GunnsBasicLinkConfigData
{
    public:
        double mPhaseChangeTemperature; /**< (K)     trick_chkpnt_io(**) Temperature at which the thermal battery medium changes phase. */
        double mPhaseChangeHeat;        /**< (J/g)   trick_chkpnt_io(**) Heat of phase change of the thermal battery medium. */
        double mHotPhaseSpecificHeat;   /**< (J/g/K) trick_chkpnt_io(**) Specific heat of the thermal battery medium in the hotter phase. */
        double mColdPhaseSpecificHeat;  /**< (J/g/K) trick_chkpnt_io(**) Specific heat of the thermal battery medium in the colder phase. */
        double mStructureCapacitance;   /**< (J/K)   trick_chkpnt_io(**) Thermal capacitance of non-phase changing structure. */
        /// @brief Default constructs this Thermal Phase Change Battery link configuration data.
        GunnsThermalPhaseChangeBatteryConfigData(const std::string& name                   = "",
                                                 GunnsNodeList*     nodes                  = 0,
                                                 const double       phaseChangeTemperature = 0.0,
                                                 const double       phaseChangeHeat        = 0.0,
                                                 const double       hotPhaseSpecificHeat   = 0.0,
                                                 const double       coldPhaseSpecificHeat  = 0.0,
                                                 const double       structureCapacitance   = 0.0);
        /// @brief Default destructs this Thermal Phase Change Battery link configuration data.
        virtual ~GunnsThermalPhaseChangeBatteryConfigData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsThermalPhaseChangeBatteryConfigData(const GunnsThermalPhaseChangeBatteryConfigData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsThermalPhaseChangeBatteryConfigData& operator =(const GunnsThermalPhaseChangeBatteryConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermal Phase Change Battery link Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermal Phase
///           Change Battery link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalPhaseChangeBatteryInputData : public GunnsBasicLinkInputData
{
    public:
        double mMass;                 /**< (kg)   trick_chkpnt_io(**) Initial mass of the phase-changing thermal battery medium. */
        double mTemperature;          /**< (K)    trick_chkpnt_io(**) Initial temperature of the thermal battery medium. */
        double mHotPhaseFraction;     /**< (--)   trick_chkpnt_io(**) Initial mass fraction (0-1) of the phase-changing thermal battery medium in the hotter phase. */
        bool   mMalfHotPhaseLeakFlag; /**< (--)   trick_chkpnt_io(**) Initial hot phase mass leak malfunction activation flag. */
        double mMalfHotPhaseLeakRate; /**< (kg/s) trick_chkpnt_io(**) Initial hot phase mass leak malfunction leak rate. */
        /// @brief    Default constructs this Thermal Phase Change Battery link input data.
        GunnsThermalPhaseChangeBatteryInputData(const double mass                 = 0.0,
                                                const double temperature          = 0.0,
                                                const double hotPhaseFraction     = 0.0,
                                                const bool   malfHotPhaseLeakFlag = false,
                                                const double malfHotPhaseLeakRate = 0.0);
        /// @brief    Default destructs this Thermal Phase Change Battery link input data.
        virtual ~GunnsThermalPhaseChangeBatteryInputData();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        GunnsThermalPhaseChangeBatteryInputData(const GunnsThermalPhaseChangeBatteryInputData& that);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalPhaseChangeBatteryInputData& operator =(const GunnsThermalPhaseChangeBatteryInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Thermal Phase Change Battery Link
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalPhaseChangeBattery : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsThermalPhaseChangeBattery);

    public:
        /// @name    Malfunctions.
        /// @{
        /// @details Malfunctions are public to allow access from the Trick events processor.
        bool   mMalfHotPhaseLeakFlag;   /**< (--)                        Hot phase mass leak malfunction activation flag. */
        double mMalfHotPhaseLeakRate;   /**< (kg/s)                      Hot phase mass leak malfunction leak rate. */
        /// @}
        /// @brief Default Constructor
        GunnsThermalPhaseChangeBattery();
        /// @brief Default Destructor
        virtual ~GunnsThermalPhaseChangeBattery();
        /// @brief Initialize method
        void           initialize(const GunnsThermalPhaseChangeBatteryConfigData& configData,
                                  const GunnsThermalPhaseChangeBatteryInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&                   networkLinks,
                                  const int                                       port0);
        /// @brief Updates the link contributions to the network system of equations.
        virtual void   step(const double dt);
        /// @brief Computes & transports flows resulting from the network solution.
        virtual void   computeFlows(const double dt);
        /// @brief Updates the link admittance for inclusion in the network solution.
        virtual void   updateState(const double dt);
        /// @brief Updates the battery mass & phase in response to flows.
        virtual void   updateFlux(const double dt, const double flux);
        /// @brief Sets and resets the hot phase leak malfunction.
        void           setMalfHotPhaseLeak(const bool flag = false, const double rate = 0.0);
        /// @brief Returns the uniform temperature of the battery material.
        double         getTemperature() const;
        /// @brief Returns the mass fraction of the hot phase of the phase-change material.
        double         getHotPhaseFraction() const;
        /// @brief Returns the actual leak rate of the hot phase out of the battery.
        double         getActualLeakRate() const;

    protected:
        double mPhaseChangeTemperature; /**< (K)     trick_chkpnt_io(**) Temperature at which the thermal battery medium changes phase. */
        double mPhaseChangeHeat;        /**< (J/g)   trick_chkpnt_io(**) Heat of phase change of the thermal battery medium. */
        double mHotPhaseSpecificHeat;   /**< (J/g/K) trick_chkpnt_io(**) Specific heat of the thermal battery medium in the hotter phase. */
        double mColdPhaseSpecificHeat;  /**< (J/g/K) trick_chkpnt_io(**) Specific heat of the thermal battery medium in the colder phase. */
        double mStructureCapacitance;   /**< (J/K)   trick_chkpnt_io(**) Thermal capacitance of non-phase changing structure. */
        double mMass;                   /**< (kg)                        Mass of the phase-changing thermal battery medium. */
        double mTemperature;            /**< (K)                         Temperature of the thermal battery medium. */
        double mHotPhaseFraction;       /**< (--)                        Mass fraction (0-1) of the phase-changing thermal battery medium in the hotter phase. */
        double mActualLeakRate;         /**< (kg/s)                      Actual leak rate of hot phase out of the battery. */
        double mExternalHeatFlux[GunnsThermalCapacitor::NUM_EXT_HEATFLUXES]; /**< (W) trick_chkpnt_io(**) Array of external heat fluxes into the battery. */
        double mSumExternalHeatFluxes;  /**< (W)     trick_chkpnt_io(**) Sum of the external heat fluxes. */
        double mAdmittance;             /**< (W/K)   trick_chkpnt_io(**) Current value of link admittance. */
        static const double mIdealAdmittance; /**< ** (W/K) trick_chkpnt_io(**) Admittance constant for mixed-phase operation. */
        /// @brief Validates the given link configuration and input data.
        void           validate(const GunnsThermalPhaseChangeBatteryConfigData& configData,
                                const GunnsThermalPhaseChangeBatteryInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void   restartModel();
        /// @brief Computes and returns the capacitance effect value.
        virtual double computeCapacitance() const;
        /// @brief Computes flux across the link.
        virtual void   computeFlux();
        /// @brief Computes power across the link.
        virtual void   computePower();
        /// @brief Adds fluxes to nodes.
        virtual void   transportFlux(const int fromPort = 0, const int toPort = 1);
        /// @brief Updates the hot phase mass fraction.
        virtual void   updatePhaseFraction(const double dt);
        /// @brief Models the hot phase leak malfunction.
        virtual void   updateMassLeak(const double dt);
        /// @brief Clears the external heat flux terms.
        void           zeroExternalFluxes();
        /// @brief Sum the external heat fluxes.
        void           sumExternalFluxes();
        /// @brief Sets various terms in the link and node to the given temperature.
        void           setTemperature(const double temperature);

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 1};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsThermalPhaseChangeBattery(const GunnsThermalPhaseChangeBattery& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsThermalPhaseChangeBattery& operator =(const GunnsThermalPhaseChangeBattery& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--)   Malfunction activation flag, true activates.
/// @param[in] rate (kg/s) Malfunction leak rate value.
///
/// @details Sets the hot-phase leak rate malfunction to the given state.  Calling this method with
///          default arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsThermalPhaseChangeBattery::setMalfHotPhaseLeak(const bool   flag,
                                                                const double rate)
{
    mMalfHotPhaseLeakFlag = flag;
    mMalfHotPhaseLeakRate = rate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (K) Uniform temperature of the thermal battery.
///
/// @details  Returns mTemperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermalPhaseChangeBattery::getTemperature() const
{
    return mTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (--) Mass fraction (0-1) of the phase-changing thermal battery medium in the
///                       hotter phase.
///
/// @details  Returns mHotPhaseFraction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermalPhaseChangeBattery::getHotPhaseFraction() const
{
    return mHotPhaseFraction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (kg/s) Actual leak rate of hot phase out of the battery.
///
/// @details  Returns mActualLeakRate.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermalPhaseChangeBattery::getActualLeakRate() const
{
    return mActualLeakRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes flux across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsThermalPhaseChangeBattery::computeFlux()
{
    mFlux = mPotentialDrop * mAdmittanceMatrix[0] - mSourceVector[0];
}

#endif
