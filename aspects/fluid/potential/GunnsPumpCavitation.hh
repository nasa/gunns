#ifndef GunnsPumpCavitation_EXISTS
#define GunnsPumpCavitation_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_POTENTIAL_PUMP_CAVITATION      Pump Cavitation Model
@ingroup   TSM_GUNNS_FLUID_POTENTIAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Class for the GUNNS Pump Cavitation model, used to model cavitation effects in liquid pumps.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsPumpCavitation.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Install) (2012-07))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "properties/FluidProperties.hh"
class GunnsBasicNode;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Pump Cavitation Model
///
/// @details  This class calculates the effect of cavitation on the flow produced by a liquid pump.
///           Depending on the type of pump, the flow parameter affected may be pressure or flow
///           rate, so this model treats it as a dimensionless parameter.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsPumpCavitation
{
    TS_MAKE_SIM_COMPATIBLE(GunnsPumpCavitation);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfFullCavitationFlag;         /**< (--) Full cavitation malf activation flag    */
        double mMalfFullCavitationDuration;     /**< (s)  Duration of full cavitation malf effect */
        bool   mMalfPartialCavitationFlag;      /**< (--) Partial cavitation malf activation flag */
        double mMalfPartialCavitationDuration;  /**< (s)  Duration of partial cavitation wash-out */
        double mMalfPartialCavitationAmplitude; /**< (--) Amplitude of partial cavitation noise   */
        /// @}

        /// @brief  Default constructs this Pump Cavitation object.
        GunnsPumpCavitation();
        /// @brief  Default destructs this Pump Cavitation object.
        virtual ~GunnsPumpCavitation();
        /// @brief  Initializes this Pump Cavitation object.
        void initialize(const double autoCavitationDuration,
                        const double autoCavitationAmplitude,
                        const bool   enableAutoCavitation);
        /// @brief  Computes the effects of pump cavitation.
        virtual void update(double&                           pumpSource,
                            const double                      dt,
                            GunnsBasicNode*                   inletNode,
                            const FluidProperties::FluidType& liquidType);
        /// @brief  Sets and resets the full cavitation malfunction.
        void setMalfFullCavitation(const bool flag = false, const double duration = 0.0);
        /// @brief  Sets and resets the full cavitation malfunction.
        void setMalfPartialCavitation(const bool   flag      = false,
                                      const double duration  = 0.0,
                                      const double amplitude = 0.0);

    protected:
        double mAutoCavitationDuration;         /**< (s)  trick_chkpnt_io(**) Duration of auto-cavitation effect      */
        double mAutoCavitationAmplitude;        /**< (--) trick_chkpnt_io(**) Amplitude of autocavitation noise       */
        bool   mEnableAutoCavitation;           /**< (--)                     Enables the automatic cavitation model  */
        double mCavitationElapsedTime;          /**< (s)                      Elapsed time of active cavitation event */
        double mInletVaporPressure;             /**< (--)                     Vapor pressure of fluid at pump inlet   */
        double mDuration;                       /**< (--)                     Duration of active cavitation           */
        double mCavitationFraction;             /**< (--)                     Fraction of cavitation pump source loss */
        /// @brief  Calculates the pump inlet vapor pressure.
        virtual void computeVaporPressure(const FluidProperties::FluidType& liquidType,
                                          GunnsBasicNode*                   inletNode);

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsPumpCavitation(const GunnsPumpCavitation&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsPumpCavitation& operator =(const GunnsPumpCavitation&);
};

/// @}

#endif
