#ifndef GunnsFluidAccumGas_EXISTS
#define GunnsFluidAccumGas_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CAPACITOR_ACCUMULATOR_GAS  Gas Accumulator Model
@ingroup   TSM_GUNNS_FLUID_CAPACITOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the class for the generic gas pressurized accumulator.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidAccumGas.o))

 PROGRAMMERS:
- ((Tim Caldwell) (GHG) (Initial) (2012-03))

@{
*/
#include "aspects/fluid/capacitor/GunnsFluidAccum.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Gas Accumulator Configuration Data
///
/// @details  The purpose of this class is to provide a data structure for the Fluid
///           Gas Accumulator configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidAccumGasConfigData : public GunnsFluidAccumConfigData
{
    public:
        double mGasMaxConductivity;    /**< (m2)     trick_chkpnt_io(**) Max effective conductivity - gas side only. */
        double mGasMinConductivity;    /**< (m2)     trick_chkpnt_io(**) Min effective conductivity - gas side only. */
        double mMaxForcePressureRate;  /**< (kPa/s)  trick_chkpnt_io(**) Max rate for overriding pressure. */

        /// @brief Default constructs this gas accumulator configuration data.
        GunnsFluidAccumGasConfigData(const std::string& name = "",
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
                                     const double gasMaxConductivity = 0.0,
                                     const double gasMinConductivity = 0.0,
                                     const double maxForcePressureRate = 0.0,
                                     const double fillModePressureThreshold = 0.0,
                                     const double effCondScaleOneWayRate = 0.0);
        /// @brief Copy constructs this gas accumulator configuration data.
        GunnsFluidAccumGasConfigData(const GunnsFluidAccumGasConfigData& that);
        /// @brief Default destructs this gas accumulator configuration data.
        virtual ~GunnsFluidAccumGasConfigData();

    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidAccumGasConfigData& operator =(const GunnsFluidAccumGasConfigData&);
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Gas Accumulator Input Data
///
/// @details  The purpose of this class is to provide a data structure for the Fluid
///           Gas Accumulator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidAccumGasInputData : public GunnsFluidAccumInputData
{
    public:
        PolyFluidInputData* mGasFluidInputData;  /**< (--)   Input data for gas fluid */

        /// @brief Default constructs this accumulator input data.
        GunnsFluidAccumGasInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0,
                                     const double initialBellowsPosition = 0.0,
                                     PolyFluidInputData* liquidFluidInputData = 0,
                                     PolyFluidInputData* gasFluidInputData = 0);
        /// @brief Copy constructs this accumulator input data.
        GunnsFluidAccumGasInputData(const GunnsFluidAccumGasInputData& that);
        /// @brief Default destucts this accumulator input data.
        virtual ~GunnsFluidAccumGasInputData();

    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidAccumGasInputData& operator =(const GunnsFluidAccumGasInputData&);
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Accumulator.
///
/// @details  Provides the gas pressurized accumulator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidAccumGas : public GunnsFluidAccum {
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidAccumGas);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfBellowsRuptureFlag;                      /**< (--) Flag to turn on/off bellows rupture malfunction.*/
        double mMalfBellowsRuptureBellowsPos;                /**< (--) Fraction, Forced bellows position value for bellows rupture malfunction.*/
        double mMalfBellowsRuptureBellowsRate;               /**< (--) Fraction/sec, Rate for bellows movement during bellows rupture malfunction.*/
        double mMalfBellowsRupturePress;                     /**< (kPa) Forced pressure for base class bellows rupture malfunction. */
        double mMalfBellowsRupturePressRate;                 /**< (kPa/s) Forced pressure rate for base class bellows rupture malfunction. */
        /// @}
        /// @brief Default constructs this gas accumulator.
        GunnsFluidAccumGas();
        /// @brief Default destructs this gas accumulator.
        virtual ~GunnsFluidAccumGas();
        /// @brief Computes the flows in and out of accumulator.
        virtual void computeFlows(const double dt);
        /// @brief Initializes this gas accumulator.
        void initialize(const GunnsFluidAccumGasConfigData& configData,
                        const GunnsFluidAccumGasInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0,       // Gas Node
                        const int                            port1);      // Liquid Node
        /// @brief Sets and resets the bellows rupture malfunction parameters.
        void setMalfBellowsRupture(const bool   flag      = false,
                                   const double pos       = 0.0,
                                   const double posRate   = 0.0,
                                   const double press     = 0.0,
                                   const double pressRate = 0.0);
        /// @brief Returns a pointer to the internal gas fluid.
        PolyFluid* getGasInternalFluid() const;

    protected:
        static const int GAS_PORT;        /**< (--)                        Gas port number. */

        double mGasMaxConductivity;       /**< (m2)    trick_chkpnt_io(**) Gas link Maximum Conductivity. */
        double mGasMinConductivity;       /**< (m2)    trick_chkpnt_io(**) Gas link Minimum Conductivity. */
        double mGasEffectiveConductivity; /**< (m2)                        Gas link Effective Conductivity. */
        double mGasCapacitance;           /**< (kg*mol/kPa)                Fluid capacitance of gas chamber. */
        double mGasPressureReading;       /**< (kPa)                       Gas pressure truth value for pressure sensor. */
        double mGasFlowRate;              /**< (kg/s)                      Flow rate into and out of gas chamber. */
        double mGasFlux;                  /**< (kg*mol/s)                  Gas molar flow rate into the gas chamber. */
        double mGasHousingQ;              /**< (W)                         Heat input from metal housing (from thermal aspect). */
        bool mEditPressureFlag;           /**< (--)                        Flag to turn on/off pressure edit feature. */
        double mEditPressureValue;        /**< (kPa)                       Desired pressure for pressure edit. */
        double mEditPressureRate;         /**< (kPa/s)                     Desired pressure edit rate. */
        double mEditPressureTimer;        /**< (s)                         Timer for holding edit once pressure is achieved, Used with mEditHoldTime. */
        PolyFluid* mGasInternalFluid;     /**< (--)    trick_chkpnt_io(**) Gas PolyFluid. */
        double mMaxForcePressureRate;     /**< (kPa/s) trick_chkpnt_io(**) Maximum rate at which pressure can be forced by edit or malfunction. */
        bool mPressureMalfSet;            /**< (--)                        Flag to signal requested malfunction pressure was reached. */
        double mEditHoldPressure;         /**< (kPa)                       Last updated gas pressure to remain at during certain edit scenarios. */

        /// @brief Updates the admittance matrix.
        void buildGasConductance();
        /// @brief Updates source vector of the link.
        void buildGasPotential();
        /// @brief Forces gas chamber temperature, based on base class temperature edit terms.
        void editPressurizerTemperature();
        /// @brief Forces gas chamber due to edit or malfunction.
        bool forceGasChamber(const double dt,
                              double &forcedPressure,
                              double &forcedPressureRate,
                              bool   updateTemperature);
        /// @brief Returns gas pressure.
        double getPressurizerPressure();
        /// @brief Override the bellows position in base class.
        void pressurizerOrideBellows(const double dt);
        /// @brief Reset edit timer due to malfunction activation.
        void resetPressurizerEditFlagsAndTimers();
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Update capacitance for gas and liquid sides.
        void updateCapacitance();
        /// @brief Update gas chamber nominally.
        void updateGasChamber(const double dt);
        /// @brief Update gas side effective conductivity.
        void updatePressurizerEffCond(const double dt);
        /// @brief Updates the fluid state of the gas chamber (mass, temperature, pressure).
        virtual void updatePressurizerFluid(const double dt);
        /// @brief Updates the state of the gas chamber (link conductivity, admittance, source pressure).
        virtual void updatePressurizerState(const double dt);
        /// @brief Validates the initialization of this gas accumulator.
        void validate(const GunnsFluidAccumGasConfigData& configData,
                      const GunnsFluidAccumGasInputData& inputData) const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidAccumGas(const GunnsFluidAccumGas& that);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidAccumGas& operator=(const GunnsFluidAccumGas&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     void
///
/// @details    Updates source vector for the accumulator gas chamber. Source vector [0]
///             is for port 0, which is the gas side. Refer to buildPotential inside the
///             GunnsFluidAccum class..
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidAccumGas::buildGasPotential()
{
    /// - Build the system source vector contribution.  The source pressure term is defined as the
    ///   pressure rise created by the potential source.

    /// - Set mSourceVector[0] to be pressure of the gas chamber.
    mSourceVector[0] = mGasInternalFluid->getPressure() * mAdmittanceMatrix[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  PolyFluid (--) Pointer to the internal gas fluid.
///
/// @details  Returns a pointer to the internal gas fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline PolyFluid* GunnsFluidAccumGas::getGasInternalFluid() const
{
    return mGasInternalFluid;
}

#endif  /* GunnsFluidAccumGas_EXISTS */
