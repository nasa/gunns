#ifndef GunnsFluidBalloon_EXISTS
#define GunnsFluidBalloon_EXISTS

/**
@file
@brief    GUNNS Fluid Balloon Link declarations

@defgroup  TSM_GUNNS_FLUID_CAPACITOR_BALLOON    GUNNS Fluid Balloon Link
@ingroup   TSM_GUNNS_FLUID_CAPACITOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Balloon Link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- ((Only ideal gas should be used inside the balloon volume, Port 0 node.)
   (Assumes a constant inflatability ratio between volume and pressure.)
   (Cannot model internal displacement of volume by contained objects, such as nested balloons.)
   (Ambient external pressure is assumed constant.))

LIBRARY DEPENDENCY:
- ((GunnsFluidBalloon.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2016-11) (Initial))

@{
*/

#include "aspects/fluid/capacitor/GunnsFluidTank.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Balloon Configuration Data
///
/// @details  This class provides a data structure for the Fluid Balloon configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidBalloonConfigData : public GunnsFluidTankConfigData
{
    public:
        double mInflatability; /**< (m3/kPa) trick_chkpnt_io(**) Inflatability of balloon. */
        double mMaxVolume;     /**< (m3)     trick_chkpnt_io(**) Maximum balloon volume, at which wall becomes rigid. */
        /// @brief Default constructs this Fluid Balloon configuration data.
        GunnsFluidBalloonConfigData(const std::string& name                 = "",
                                    GunnsNodeList*     nodes                = 0,
                                    const double       expansionScaleFactor = 0.0,
                                    const double       dpdtFilterGain       = 0.0,
                                    const double       thermalDampingMass   = 0.0,
                                    const double       editFluxTarget       = 1.0E-6,
                                    const float        surfaceArea          = 0.0,
                                    const float        shellRadius          = 0.0,
                                    const double       inflatability        = 0.0,
                                    const double       maxVolume            = 0.0);
        /// @brief Default destructs this Fluid Balloon configuration data.
        virtual ~GunnsFluidBalloonConfigData();
        /// @brief Copy constructs this Fluid Balloon configuration data.
        GunnsFluidBalloonConfigData(const GunnsFluidBalloonConfigData& that);

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidBalloonConfigData& operator =(const GunnsFluidBalloonConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Balloon Input Data
///
/// @details  This class provides a data structure for the Fluid Balloon input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidBalloonInputData : public GunnsFluidTankInputData
{
    public:
        bool   mMalfStuckFlag;               /**< (--) trick_chkpnt_io(**) Initial activation state of the stuck wall malfunction. */
        bool   mMalfInflatabilityScaleFlag;  /**< (--) trick_chkpnt_io(**) Initial activation state of the inflatability scale malfunction. */
        double mMalfInflatabilityScaleValue; /**< (--) trick_chkpnt_io(**) Initial value of the inflatability scale malfunction. */
        /// @brief Default constructs this Fluid Balloon input data.
        GunnsFluidBalloonInputData(const bool          malfStuckFlag               = false,
                                   const bool          malfInflatabilityScaleFlag  = false,
                                   const double        malfInflatabilityScaleValue = 0.0,
                                   PolyFluidInputData* initialFluidState           = 0,
                                   const float         initialShellTemperature     = 0.0,
                                   const float         biasHeatFlux                = 0.0);
        /// @brief Default destructs this Fluid Balloon input data.
        virtual ~GunnsFluidBalloonInputData();
        /// @brief Copy constructs this Fluid Balloon input data.
        GunnsFluidBalloonInputData(const GunnsFluidBalloonInputData& that);

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidBalloonInputData& operator =(const GunnsFluidBalloonInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   GUNNS Fluid Balloon Link Class
///
/// @details This link models a gas-filled bag or balloon with an elastic membrane.  It can be used
///          to model vehicle landing or uprighting bags (non-nested) and inflatable habitats.
///
///          See above for a list of assumptions and limitations.
///
///          The balloon's internal volume is the node on Port 0, and Port 1 is the balloon's
///          ambient environment.  We assume the ambient pressure is constant, so we recommend only
///          connecting Port 1 to either the network Ground node or a pressure-constrained boundary
///          or ambient node.  The link can handle slow changes to the ambient pressure.  This can't
///          be used nested inside another balloon node.
///
///          It assumes ideal gas, so we only recommend using the ideal gas fluid types.  Port 0 can
///          only be attached to a gas node, and it can't be the network Ground node.
///
///          Port 1 can be any node, Ground or normal, and gas or liquid.  This link doesn't affect
///          the Port 1 node volume and doesn't model displacement of the ambient volume.
///
///          This inherits GunnsFluidTank and all of its edits & bias capability.  It doesn't model
///          leaks or the balloon popping.  Those effects can be modeled with leak or "burst-valve"
///          relief valves in parallel to this link.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidBalloon : public GunnsFluidTank
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluidBalloon);

    public:
        /// @brief Enumeration of the inflation states.
        enum InflationStates {
            DEFLATED           = 0, ///< Balloon is at minimum volume.
            DEFLATING          = 1, ///< Balloon volume decreased this pass.
            PARTIALLY_INFLATED = 2, ///< Balloon volume is between min-max and unchanged this pass.
            INFLATING          = 3, ///< Balloon volume increased this pass.
            INFLATED           = 4  ///< Balloon is at maximum volume.
        };
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfStuckFlag;               /**< (--) Activation state of the stuck wall malfunction. */
        bool   mMalfInflatabilityScaleFlag;  /**< (--) Activation state of the inflatability scale malfunction. */
        double mMalfInflatabilityScaleValue; /**< (--) Value of the inflatability scale malfunction. */
        /// @}
        /// @brief Constructs the Fluid Balloon.
        GunnsFluidBalloon();
        /// @brief Destructs the Fluid Balloon.
        virtual ~GunnsFluidBalloon();
        /// @brief Initialize Method
        void initialize(const GunnsFluidBalloonConfigData& configData,
                        const GunnsFluidBalloonInputData&  inputData,
                        std::vector<GunnsBasicLink*>&      networkLinks,
                        const int                          port0,
                        const int                          port1);
        /// @brief Step through a frame.
        virtual void    step(const double dt);
        /// @brief Updates the derived class object internal fluid.
        virtual void    updateFluid(const double dt, const double flowrate);
        /// @brief Returns the current inflation fraction of this GUNNS Fluid Balloon.
        double          getInflation() const;
        /// @brief Returns the current inflation state enumeration of this GUNNS Fluid Balloon.
        InflationStates getInflationState() const;
        /// @brief Sets and resets the wall stuck malfunction.
        void            setMalfStuck(const double flag = false);
        /// @brief Sets and resets the inflatability scale malfunction.
        void            setMalfInflatabilityScale(const double flag  = false,
                                                  const double value = 0.0);

    protected:
        double              mInflatability;          /**< (m3/kPa)     trick_chkpnt_io(**) Nominal inflatability of balloon. */
        double              mMaxVolume;              /**< (m3)         trick_chkpnt_io(**) Maximum balloon volume, at which wall becomes rigid. */
        double              mInflation;              /**< (--)                             Current balloon fraction (0-1) of full inflation. */
        InflationStates     mInflationState;         /**< (--)         trick_chkpnt_io(**) Current balloon inflation state. */
        double              mActualInflatability;    /**< (m3/kPa)     trick_chkpnt_io(**) Current inflatability including malfunctions. */
        double              mPressureCorrection;     /**< (kPa)        trick_chkpnt_io(**) Node pressure correction for state error. */
        double              mPressureCorrectionGain; /**< (--)         trick_chkpnt_io(**) Gain on node pressure correction. */
        double              mInflatableCapacitance;  /**< (kg*mol/kPa) trick_chkpnt_io(**) Contribution of inflatability to molar capacitance. */
        static const double mMinVolume;              /**< (m3)         trick_chkpnt_io(**) Minimum volume for initial inflation stability. */
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Builds the source vector for the link.
        virtual void buildSourceVector(const double dt);
        /// @brief Validates the initialization of this GUNNS Fluid Balloon.
        void         validate(const GunnsFluidBalloonConfigData& configData) const;
        /// @brief Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;
        /// @brief Computes this GUNNS Fluid Balloon's current actual inflatability.
        void         updateInflatability();
        /// @brief Updates and returns this GUNNS Fluid Balloon's volume and inflation state.
        double       updateInflation();
        /// @brief Returns the balloon volume corresponding to the given internal pressure.
        double       computeVolumeFromPressure(const double pressure) const;
        /// @brief Updates and returns the node pressure correction.
        double       computePressureCorrection();
        /// @brief Process a volume edit and synchronizes with pressure edits.
        void         processVolumeEdit();
        /// @brief Corrects partial pressure rate edit flows for flexible volume.
        void         correctPpRateEdits(const double dt);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidBalloon(const GunnsFluidBalloon& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidBalloon& operator =(const GunnsFluidBalloon& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (--) Inflation fraction (0-1).
///
/// @details Returns the current inflation fraction of this GUNNS Fluid Balloon.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidBalloon::getInflation() const
{
    return mInflation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns InflationStates (--) Inflation state enumeration.
///
/// @details Returns the current inflation state enumeration of this GUNNS Fluid Balloon.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsFluidBalloon::InflationStates GunnsFluidBalloon::getInflationState() const
{
    return mInflationState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) Malfunction activation flag, true activates.
///
/// @details Sets the wall stuck malf flag to the given state.  Calling this method with default
///          arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidBalloon::setMalfStuck(const double flag)
{
    mMalfStuckFlag = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (--) Malfunction scale factor value.
///
/// @details Sets the inflatability scale malf to the given state.  Calling this method with default
///          arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidBalloon::setMalfInflatabilityScale(const double flag, const double value)
{
    mMalfInflatabilityScaleFlag  = flag;
    mMalfInflatabilityScaleValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] pressure (kPa) Gas pressure inside the balloon.
///
/// @returns double (m3) Computed balloon internal volume.
///
/// @details Compute & returns what the internal balloon volume must be at the given internal
///          pressure, based on its inflatability and exterior pressure.  The returned value is
///          limited between the minimum & maximum volumes of this balloon.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidBalloon::computeVolumeFromPressure(const double pressure) const
{
    return MsMath::limitRange(mMinVolume,
                            mActualInflatability * (pressure - mPotentialVector[1]),
                            mMaxVolume);
}

#endif
