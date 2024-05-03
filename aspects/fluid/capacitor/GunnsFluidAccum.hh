#ifndef GunnsFluidAccum_EXISTS
#define GunnsFluidAccum_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CAPACITOR_ACCUMULATOR  Accumulator Model
@ingroup   TSM_GUNNS_FLUID_CAPACITOR

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the class for the generic accumulator. This class can be
   used with either a spring pressurizer or gas pressurizer or both.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidAccum.o))

 PROGRAMMERS:
- ((Tim Caldwell) (GHG) (Initial) (2012-03))

@{
*/
#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Accumulator Configuration Data
///
/// @details  The purpose of this class is to provide a data structure for the Fluid
///           Accumulator configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidAccumConfigData : public GunnsFluidLinkConfigData
{
    public:
        double mMaxConductivity;                /**< (m2)   trick_chkpnt_io(**) Max effective conductivity - liquid side only. */
        /// @details
        ///    When liquid or pressurizer volume is within the deadband, minConductivityScale is used to
        ///    gradually close off the liquid side conductivity. This adds stability and also creates a smooth
        ///    transition to the point where the conductivity is closed off completely. The conductivity is closed
        ///    off when the bellows is at a limit (0 or 1). In this condition, if the pressure is keeping the
        ///    bellows at the limit, then the pressurizer pressure can no longer longer affect the
        ///    liquid side, and that is communicated to the Gunns network by closing off the conductivity. When
        ///    the pressure condition is such that the bellows will be forced off of the limit, the pipe (conductivity)
        ///    is cracked open to allow this to happen. The minConductivityScale is used to accomplish this. Refer to
        ///    updateEffConductivityScale.
        double mMinConductivityScale;           /**< (--)   trick_chkpnt_io(**) Min scale to crack open liquid side at bellows limit (see details) */
        double mAccumVolume;                    /**< (m3)   trick_chkpnt_io(**) Documented real world accumulator volume.  */
        double mMinChamberVolPercent;           /**< (--)   trick_chkpnt_io(**) Percent accum volume added for minimum chamber volume. */
        double mMinChamberVolDeadBandPercent;   /**< (--)   trick_chkpnt_io(**) Percent of accum volume for dead band. */
        double mForceBellowsMaxRate;            /**< (--)   trick_chkpnt_io(**) Fraction/sec, Max rate for overriding bellows movement. */
        double mEditHoldTime;                   /**< (s)    trick_chkpnt_io(**) Hold time for edit to achieve system stability. */
        double mMinTemperature;                 /**< (K)    trick_chkpnt_io(**) Min temperature - all applicable chambers. */
        double mMaxTemperature;                 /**< (K)    trick_chkpnt_io(**) Max temperature - all applicable chambers. */
        double mMaxPressure;                    /**< (kPa)  trick_chkpnt_io(**) Max pressure accumulator will compute. */
        /// @details
        ///    mSpringCoeff0, mSpringCoeff1, mSpringCoeff2 are part of the built in spring feature.
        ///    to turn off the feature, just supply 0.0 for each of the terms. This feature can be used to simulate
        ///    the small "springyness" of the bellows itself, or an actual spring pressurizer.
        ///    An example of the small spring effect of the bellows: mSpringCoeff0=-10.0,
        ///    mSpringCoeff1=20.0,  mSpringCoeff2=0.0. An example of a full spring pressurizer:
        ///    mSpringCoeff0=-350.0, mSpringCoeff1=350.0, mSpringCoeff2 = 0.0. Coefficients are entered in the form
        ///    y = ax^2 + bx + c; where a = mSpringCoeff2, b = mSpringCoeff1, c = mSpringCoeff0, x = bellows position
        ///    and y = Spring pressure.
        double mSpringCoeff0;                  /**< (kPa)  trick_chkpnt_io(**) Spring pressure at bellows zero position, spring coefficient 0. */
        double mSpringCoeff1;                  /**< (--)   trick_chkpnt_io(**) Spring coefficient 1, or spring constant, k (kPa/bellows position), in a linear spring system. */
        double mSpringCoeff2;                  /**< (--)   trick_chkpnt_io(**) Spring coefficient 2 (kPa/bellows position squared). */
        double mFillModePressureThreshold;     /**< (kPa) trick_chkpnt_io(**) Threshold for computing Fill Mode. Non-zero activates "one way" mEffConductivityScale ramping. */
        double mEffCondScaleOneWayRate;        /**< (--)  trick_chkpnt_io(**) Fraction/sec, "One way" mEffConductivityScale ramping. Used if mFillModePressureThreshold > 0 */
        /// @brief Default constructs this accumulator configuration data.
        GunnsFluidAccumConfigData(const std::string& name = "",
                                      GunnsNodeList* nodeList = 0,
                                      const double maxConductivity = 0.0,
                                      const double minConductivityScale = 0.0,
                                      const double accumVolume = 0.0,
                                      const double minChamberVolPercent = 0.0,
                                      const double minChamberVolDeadBandPercent = 0.0,
                                      const double forceBellowsMaxRate = 0.0,
                                      const double editHoldtime = 0.0,
                                      const double minTemperature = 0.0,
                                      const double maxTemperature = 0.0,
                                      const double maxPressure = 0.0,
                                      const double springCoeff0 = 0.0,
                                      const double springCoeff1 = 0.0,
                                      const double springCoeff2 = 0.0,
                                      const double fillModePressureThreshold = 0.0,
                                      const double effCondScaleOneWayRate = 0.0);
        /// @brief Copy constructs this accumulator configuration data.
        GunnsFluidAccumConfigData(const GunnsFluidAccumConfigData& that);
        /// @brief Default destructs this accumulator configuration data.
        virtual ~GunnsFluidAccumConfigData();

    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidAccumConfigData& operator =(const GunnsFluidAccumConfigData&);
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Accumulator Input Data
///
/// @details  The purpose of this class is to provide a data structure for the Fluid
///           Accumulator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidAccumInputData : public GunnsFluidLinkInputData
{
    public:
        double mInitialBellowsPosition;                 /**< (--)   Fraction of liquid chamber that is filled (0-1). */
        PolyFluidInputData* mLiquidFluidInputData;      /**< (--)   Input data for liquid fluid */

        /// @brief    Default constructs this accumulator input data.
        GunnsFluidAccumInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0,
                                     const double initialBellowsPosition = 0.0,
                                     PolyFluidInputData* liquidFluidInputData = 0);
        /// @brief    Copy constructs this accumulator input data.
        GunnsFluidAccumInputData(const GunnsFluidAccumInputData& that);
        /// @brief    Default destructs this accumulator input data.
        virtual ~GunnsFluidAccumInputData();

    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidAccumInputData& operator =(const GunnsFluidAccumInputData&);
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Accumulator.
///
/// @details  Provides the base accumulator class for an accumulator that is pressurized by either
///           gas or spring.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidAccum : public GunnsFluidLink {
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidAccum);
    public:
        /// @brief Enumeration of the accumulator fill mode based on fill mode pressure threshold value.
        enum fillModeType {
            EQUALIZED      = 0,              ///< Liquid inside and ouside pressures are equalized.
            FILLING        = 1,              ///< Liquid outside pressure is greater than inside pressure.
            DRAINING       = 2               ///< Liquid inside pressure is greaterthan outside pressure.
        };

        /// @brief Enumeration of the bellows zone relative to dead bands and hard stops.
        enum bellowsZoneType {
            EMPTY          = 0,              ///< Empty. Liquid hardtop.
            EMPTY_DEADBAND = 1,              ///< Almost empty. Within liquid deadband.
            MIDDLE         = 2,              ///< Outside of both deadbands. In the middle.
            FULL_DEADBAND  = 3,              ///< Almost full. Within pressurizer deadband.
            FULL           = 4               ///< Full. Pressurizer hardtop.
        };

        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfBellowsStickFlag;                /**< (--) Forces bellows not to move, bellows position does not change. */
        bool   mMalfBellowsStickToPosFlag;           /**< (--) Flag to turn on/off stick bellows to position malfunction. */
        double mMalfBellowsStickToPosValue;          /**< (--) Forced bellows position value for stick bellows to position malfunction. */
        double mMalfBellowsStickToPosRate;           /**< (--) Fraction/sec, Rate for bellows movement during stick bellows to position malfunction. */
        /// @}
        /// @brief  Default constructs this accumulator.
        GunnsFluidAccum();
        /// @brief  Default destructs this accumulator.
        virtual ~GunnsFluidAccum();
        /// @brief Computes the flows in and out of accumulator, Solves accumulator state.
        virtual void computeFlows(const double dt);
        /// @brief Transports the flows through the link between the nodes.
        virtual void transportFlows(const double dt);
        /// @brief Initializes this accumulator.
        void initialize(const GunnsFluidAccumConfigData& configData,
                        const GunnsFluidAccumInputData&  inputData,
                        std::vector<GunnsBasicLink*>&    networkLinks,
                        const int                        port0,       // Gas Node or ground
                        const int                        port1);      // Liquid Node
        /// @brief Step through a frame, Solve capacitance, conductivity, and call updateState.
        virtual void step(const double dt);
        /// @brief Updates the fluid state of the accumulator (bellows pos, temperature, pressure)
        virtual void updateFluid(const double dt, const double);
        /// @brief Updates the state of the accumulator (admittance, conductance, source pressure)
        virtual void updateState(const double dt);
        /// @brief Returns the bellows position of the accumulator
        double getBellowsPosition() const;
        /// @brief Returns the usable liquid mass in the accumulator.
        double getUsableMass() const;
        /// @brief Sets and resets the bellows stuck in place malfunction
        void setMalfBellowsStick(const bool flag = false);
        /// @brief Sets and resets the bellows stuck to position malfunction
        void setMalfBellowsStickToPos(const bool flag = false, const double value = 0.0, const double rate = 0.0);
        /// @brief Sets accumulator liquid and pressurizer temperature.
        void setTemperature(double temperature);
        /// @brief Sets accumulator spring coefficients.
        void setSpringCoeffs(const double coeff0 = 0.0, const double coeff1 = 0.0, const double coeff2 = 0.0);
        /// @brief Sets extra pressure at exit due to liquid column under acceleration.
        void setAccelPressureHead(const double pressure);

    protected:
        static const int LIQUID_PORT;        /**< (--)                      Liquid port number. */

        double mMaxConductivity;             /**< (m2)  trick_chkpnt_io(**) Link Maximum Conductivity. */
        double mMinConductivityScale;        /**< (--)  trick_chkpnt_io(**) Min scale to crack open liquid side at bellows limit. */
        double mEffectiveConductivity;       /**< (m2)                      Link Effective Conductivity. */
        double mEffConductivityScale;        /**< (m2)                      Link Effective Conductivity scale factor. */
        double mLiqCapacitance;              /**< (kg*mol/kPa)              Fluid capacitance of liquid chamber. */
        double mBellowsPosition;             /**< (--)                      Fraction of liquid chamber that is filled (0-1). */
        double mLiquidPressureReading;       /**< (kPa)                     Liquid pressure truth value for pressure sensor. */
        double mLiquidHousingQ;              /**< (W)                       Heat input from metal housing (from thermal aspect). */
        double mTotalVolume;                 /**< (m3)  trick_chkpnt_io(**) total volume of accumulator (both chambers), Config accum vol + minChamberVol * 2. */
        double mMinChamberVol;               /**< (m3)  trick_chkpnt_io(**) minimum volume of either chamber, when bellows position is 0% or 100%. */
        double mMaxChamberVol;               /**< (m3)  trick_chkpnt_io(**) maximum volume of either chamber, when bellows position is 0% or 100%. */
        double mMinDeadBandVol;              /**< (m3)  trick_chkpnt_io(**) Volume range where liquid conductivity is scaled. */
        double mActiveVolRange;              /**< (m3)  trick_chkpnt_io(**) Volume that represents bellows position range = mMaxChamberVol - mMinChamberVol. */
        double mLiquidVolume;                /**< (m3)                      Current volume of liquid chamber. */
        double mPressurizerVolume;           /**< (m3)                      Current volume of pressurizer chamber. */
        double mSpringCapacitance;           /**< (kg*mol/kPa)              Capacitance of liquid chamber based on spring. */
        double mSpringCoeff0;                /**< (kPa) trick_chkpnt_io(**) Spring pressure at bellows position of 0.0, spring coefficient 0. */
        double mSpringCoeff1;                /**< (--)  trick_chkpnt_io(**) Spring coefficient 1, or spring constant, k (kPa/bellows position), in a linear spring system. */
        double mSpringCoeff2;                /**< (--)  trick_chkpnt_io(**) Spring coefficient 2 (kPa/bellows position squared). */
        double mSpringPressure;              /**< (kPa)                     Current spring pressure. */
        double mEditHoldTime;                /**< (s)                       Hold time for edit function to achieve system stability. */
        bool   mEditBellowsFlag;             /**< (--)                      Flag to turn on/off Bellows edit feature. */
        double mEditBellowsPosition;         /**< (--)                      Desired bellows position for Bellows edit. */
        double mEditBellowsRate;             /**< (--)                      Fraction/sec, Desired Bellows edit rate for bellows position change. */
        double mEditBellowsTimer;            /**< (s)                       Timer for holding edit once Bellows is achieved. Used with mEditHoldTime. */
        bool   mEditTemperatureFlag;         /**< (--)                      Flag to turn on/off temperature edit feature. */
        double mEditTemperatureValue;        /**< (K)                       Desired temperature value for temperature edit. */
        bool   mHoldAccumFlag;               /**< (--)                      Forces accumulator not to update state. */
        bool   mHoldTemperatureFlag;         /**< (--)                      Forces accumulator not to update temperature. */
        bool   mPressurizerOrideBellowsFlag; /**< (--)                      Flag used to call the pressurizerOrideBellows method if implemented. */
        bool   mBellowsStuck;                /**< (--)                      Flag bellows as being stuck due to malfunctions. */
        double mForceBellowsMaxRate;         /**< (--)  trick_chkpnt_io(**) Percent/sec, maximum rate at which bellows position can be forced by edit or malfunction. */
        double mMinTemperature;              /**< (K)   trick_chkpnt_io(**) Minimum temperature for both chambers of accumulator. */
        double mMaxTemperature;              /**< (K)   trick_chkpnt_io(**) Maximum temperature for both chambers of accumulator. */
        double mMaxPressure;                 /**< (kPa) trick_chkpnt_io(**) Maximum pressure the accumulator can achieve. */
        bool   mBellowsMalfSet;              /**< (--)                      Flag to signal requested malfunction bellows position was reached. */
        bool   mEditsAreReactive;            /**< (--)                      Flag to for reactive edits. If true, other actors will react like normal. */
        bool   mSimultaneousEditsInProgress; /**< (--)                      Flag to indicate pressurizer and bellows edit at the same time. MUST BE ACTIVATED BY DERIVED CLASS. */
        fillModeType    mFillMode;           /**< (--)  trick_chkpnt_io(**) Mode for computing effective conductivity scale. */
        bellowsZoneType mBellowsZone;        /**< (--)  trick_chkpnt_io(**) Bellows zone for computing effective conductivity scale. */
        double mFillModePressureThreshold;   /**< (kPa) trick_chkpnt_io(**) Threshold for computing Fill Mode. Non-zero activates "one way" mEffConductivityScale ramping. */
        double mEffCondScaleOneWayRate;      /**< (--)  trick_chkpnt_io(**) Fraction/sec, "One way" mEffConductivityScale ramping. Used if mFillModePressureThreshold > 0 */
        double mAccelPressureHead;           /**< (kPa)                     Extra pressure head at exit due to liquid column under acceleration. */

        /// @brief Updates the admittance matrix.
        void buildConductance();
        /// @brief Updates source vector of the link.
        void buildPotential();
        /// @brief Checks for valid accumulator port node assignment
        bool checkSpecificPortRules(const int port, const int node) const;
        /// @brief Computes conductivity for accumulator link, Can also be used by derived class.
        double computeConductivity(const double capacitance,
                                   const double maxConductivity,
                                   const double currentConductivity,
                                   const double minConductivity,
                                   const double dt) const;
        /// @brief Computes flow rate for accumulator link, Can also be used by derived class.
        double computeFlowRate(const double admittance,
                               GunnsBasicNode* node,
                               const PolyFluid* fluid) const;
        /// @brief Computes molar flow rate into the accumulator.
        double computeFlux(const double admittance,
                           GunnsBasicNode* node,
                           const PolyFluid* fluid) const;
        /// @brief Computes volumetric flow rate for accumulator link, Can also be used by derived class.
        double computeVolFlowRate(const double     flowRate,
                                  GunnsBasicNode*  sourceNode,
                                  const PolyFluid* internalFluid) const;
        /// @brief Computes mass for either accumulator chamber using flowrate and dt, Can also be used by derived class.
        double computeMass(const double dt, const double flowRate, const double currentMass) const;
        /// @brief Computes mass for either accumulator chamber using density and volume, Can also be used by derived
        ///        class.
        double computeMass(const double volume, const double density) const;
        /// @brief Computes temperature for either accumulator chamber, Can also be used by derived class.
        double computeTemperature(const double dt,
                                  const double flowRate,
                                  const double inSpecificEnthalpy,
                                  const PolyFluid* accumFluid,
                                  const double accumHousingQ) const;
        /// @brief Computes and updates volumes after initialize or when accumulator state is forced.
        void deriveChamberVolumes();
        /// @brief Forces accumulator state, based on Bellows edit terms.
        void editLiqChamber(const double dt);
        /// @brief Forces pressurizer chamber temperature (if present), based on base class temperature edit terms.
        virtual void editPressurizerTemperature();
        /// @brief Forces accumulator temperature, based on temperature edit terms.
        void editTemperature(const double volume, PolyFluid* accumFluid);
        /// @brief Forces accumulator state, based on malfunction terms.
        void failLiqChamber(const double dt);
        /// @brief Forces accumulator state due to edit or malfunction.
        bool forceLiqChamber(const double dt,
                             double& forcedBellowsPos,
                             double& forceBellowsRate,
                             bool    updateTemperature);
        /// @brief Get pressure from pressurizer - place holder for derived class.
        virtual double getPressurizerPressure();
        /// @brief Place holder for pressurizer to override liquid chamber.
        virtual void pressurizerOrideBellows(const double dt);
        /// @brief Processes edit timer for an edit function, Can also be used by derived class.
        bool processTimer(const double dt, double& timer, const double timerLimit);
        /// @brief Ramp applicable value for edits or malfunctions, Can also be used by derived class.
        bool rampValue(const double dt,
                       double& currentValue,
                       double& targetValue,
                       double& rate,
                       const double targetLowerLimit,
                       const double targetUpperLimit,
                       const double rateLowerLimit,
                       const double rateUpperLimit);
        /// @brief Reset edit timers due to malfunction activation.
        void resetEditFlagsAndTimers();
        /// @brief Reset pressurizer edit timers due to malfunction activation.
        virtual void resetPressurizerEditFlagsAndTimers();
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Transport fluid to and from accumulator and adjacent node, Can also be used by derived class.
        void transportAccumFluid(const double flowRate,
                                 GunnsBasicNode* node,
                                 PolyFluid* fluid);
        /// @brief Update capacitance of liquid chamber, Place holder for derived class.
        virtual void updateCapacitance();
        /// @brief Update liquid chamber state nominally.
        void updateLiqChamber(const double dt);
        /// @brief Update effective conductivity scale.
        void updateEffConductivityScale(const double dt);
        /// @brief Update effective conductivity for liquid side.
        void updateEffectiveConductivity(const double dt);
        /// @brief Update pressurizer effective conductivity, Place holder for derived class.
        virtual void updatePressurizerEffCond(const double dt);
        /// @brief Update pressurizer fluid - mass, temperature, pressure, Place holder for derived class.
        virtual void updatePressurizerFluid(const double dt);
        /// @brief Update pressurizer state - admittance, conductance, potential, Place holder for derived class.
        virtual void updatePressurizerState(const double dt);
        /// @brief Update pressure of liquid side.
        void updatePressure(const double dt);
        /// @brief Update volume of pressurizer chamber.
        void updatePressurizerVolume();
        /// @brief Validates the initialization of this accumulator.
        void validate(const GunnsFluidAccumConfigData& configData,
                      const GunnsFluidAccumInputData& inputData) const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidAccum(const GunnsFluidAccum& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidAccum& operator=(const GunnsFluidAccum&);
};
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     void
///
/// @details    Updates source vector for the accumulator liquid chamber. Source vector [1]
///             is for port 1, which is the liquid side. Source vector [0] is for the pressurizer
///             if applicable and is zeroed out. If a gas pressurizer is used, then this source
///             vector must be set by the pressurizer after this function executes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidAccum::buildPotential()
{
    /// - Build the system source vector contribution.  The source pressure term is defined as the
    ///   pressure rise created by the potential source.

    /// - Zero out mSourceVector[0]. To be overriden by pressurizer if applicable.
    mSourceVector[0] = 0.0;
    /// - Set mSourceVector[1] to be pressure of the liquid chamber.
    mSourceVector[1] = mInternalFluid->getPressure() * mAdmittanceMatrix[3];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The bellows position.
///
/// @details Returns the fraction of the liquid chamber that is filled.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidAccum::getBellowsPosition() const
{
    return mBellowsPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg) The usable liquid mass.
///
/// @details  The usable mass is that which can be removed from the accumulator before it reaches
///           its minimum chamber volume.  So this is the internal liquid fluid mass (the total
///           liquid mass) minus the amount trapped in the minimum chamber volume.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidAccum::getUsableMass() const
{
    // Divide by zero protected from initial validation of mMinChamberVol, which effectively limits
    // mLiquidVolume.
    return mInternalFluid->getMass() * (1.0 - mMinChamberVol / mLiquidVolume);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  temperature  (K)  Desired temperature.
///
/// @details  Set accumulator temperature including pressurizer via the temperature edit.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidAccum::setTemperature(double temperature)
{
    mEditTemperatureFlag  = true;
    mEditTemperatureValue = temperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  coeff0  (kPa)  The 0th order coefficient for the spring pressure polynomial.
/// @param[in]  coeff1  (kPa)  The 1th order coefficient for the spring pressure polynomial.
/// @param[in]  coeff2  (kPa)  The 2th order coefficient for the spring pressure polynomial.
///
/// @details  Sets the mSpringCoeff0, 1, 2 terms to the given values.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidAccum::setSpringCoeffs(const double coeff0, const double coeff1, const double coeff2)
{
    mSpringCoeff0 = coeff0;
    mSpringCoeff1 = coeff1;
    mSpringCoeff2 = coeff2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] pressure (kPa) Extra pressure head at exit due to liquid column under acceleration.
///
/// @details  Sets the mAccelPressureHead attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidAccum::setAccelPressureHead(const double pressure)
{
    mAccelPressureHead = pressure;
}

#endif  /* GunnsFluidAccum_EXISTS */
