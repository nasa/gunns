#ifndef GunnsFluidCondensingHx_EXISTS
#define GunnsFluidCondensingHx_EXISTS

/**
@file
@brief    GUNNS Fluid Condensing Heat Exchanger declarations

@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_CONDENSING_HX_SEPARATOR    Condensing Heat Exchanger
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Condensing Heat Exchanger fluid aspect.)

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
-((GunnsFluidCondensingHx.o))

PROGRAMMERS:
-((Jason Harvey) (CACI) (December 2018) (Initial))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Condensing Heat Exchanger Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Condensing Heat Exchanger link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCondensingHxConfigData: public GunnsFluidConductorConfigData
{
    public:
        int    mNumSegments; /**< (--)       trick_chkpnt_io(**) Number of heat exchanger segments. */
        double mHtcCoeff0;   /**< (W/K)      trick_chkpnt_io(**) Heat transfer 0th order coefficient. */
        double mHtcCoeff1;   /**< (W*s/K/kg) trick_chkpnt_io(**) Heat transfer 1st order coefficient. */
        double mHtcExponent; /**< (--)       trick_chkpnt_io(**) Heat transfer coefficient exponent. */
        double mHtcLimit;    /**< (W/K)      trick_chkpnt_io(**) Heat transfer coefficient limit. */
        /// @brief  Default constructs this Condensing Heat Exchanger configuration data.
        GunnsFluidCondensingHxConfigData(const std::string& name            = "",
                                         GunnsNodeList*     nodes           = 0,
                                         const double       maxConductivity = 0.0,
                                         const int          numSegments     = 0,
                                         const double       htcCoeff0       = 0.0,
                                         const double       htcCoeff1       = 0.0,
                                         const double       htcExponent     = 0.0,
                                         const double       htcLimit        = 0.0);
        /// @brief  Copy constructs this Condensing Heat Exchanger configuration data.
        GunnsFluidCondensingHxConfigData(const GunnsFluidCondensingHxConfigData& that);
        /// @brief  Default destructs this Condensing Heat Exchanger configuration data.
        virtual ~GunnsFluidCondensingHxConfigData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidCondensingHxConfigData& operator=(const GunnsFluidCondensingHxConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Condensing Heat Exchanger Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Condensing Heat Exchanger link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCondensingHxInputData : public GunnsFluidConductorInputData
{
    public:
        double mWallTemperature;          /**< (K) Initial default wall temperature all segments. */
        GunnsFluidCondensingHxInputData(const bool   malfBlockageFlag  = false,
                                        const double malfBlockageValue = 0.0,
                                        const double wallTemperature   = 0.0);
        /// @brief  Copy constructs this Condensing Heat Exchanger input data.
        GunnsFluidCondensingHxInputData(const GunnsFluidCondensingHxInputData& that);
        /// @brief  Default destructs this Condensing Heat Exchanger input data.
        virtual ~GunnsFluidCondensingHxInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidCondensingHxInputData& operator=(const GunnsFluidCondensingHxInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Condensing Heat Exchanger Model
///
/// @details  This is the fluid aspect of a condensing heat exchanger.  It acts like a normal
///           GunnsFluidHeatExchanger, but also condenses water vapor out of the bulk flow.
///
///           This interfaces to the thermal aspect of the HX similarly to GunnsFluidHeatExchanger,
///           and multiple segments can be used for better accuracy. Unlike GunnsFluidHeatExchanger,
///           this link does not allow different heat transfer coefficients from segment to segment.
///
///           This link does nothing with the condensate.  It only calculates the instantaneous
///           condensation rate, which other models may use to model handling of the condensate.
///           The condensate is assumed to flow downstream and doesn't pool on the HX fins, as if
///           the fins have a hydrophobic coating.
///
/// @note     This link only operates on water as the condensate and GUNNS_H2O gas must be present
///           in the network configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidCondensingHx : public  GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidCondensingHx);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool       mMalfHxDegradeFlag;  /**< (--) HX heat transfer degradation malf flag. */
        double     mMalfHxDegradeValue; /**< (--) HX heat transfer degradation malf HTC multiplier. */
        /// @}

        /// @brief    Default constructs this Condensing Heat Exchanger.
        GunnsFluidCondensingHx();
        /// @brief    Default destructs this Condensing Heat Exchanger.
        virtual ~GunnsFluidCondensingHx();
        /// @brief    Initializes this Condensing Heat Exchanger with configuration and
        ///           input data.
        void initialize(const GunnsFluidCondensingHxConfigData& configData,
                        const GunnsFluidCondensingHxInputData&  inputData,
                        std::vector<GunnsBasicLink*>&           networkLinks,
                        const int                               port0,
                        const int                               port1);
        /// @brief    Performs post-solution fluid calculations & transport.
        virtual void updateFluid(const double dt, const double flowRate);
        /// @brief    Moves the fluid from one port to the other.
        virtual void transportFluid(const bool forcedOutflow,
                                    const int  fromPort = 0,
                                    const int  toPort   = 1);
        /// @brief    Returns the total heat removed from the air by the heat exchanger.
        double       getTotalHeat() const;
        /// @brief    Returns condensation rate of water in the heat exchanger.
        double       getCondensationRate() const;
        /// @brief Sets and resets the HX heat transfer degradation malfunction
        void         setMalfHxDegrade(const bool flag = false, const double value = 0.0);

    protected:
        // Config data
        int        mNumSegments;        /**< *o (--)       trick_chkpnt_io(**) Number of heat exchanger segments. */
        double     mHtcCoeff0;          /**<    (W/K)      trick_chkpnt_io(**) Heat transfer 0th order coefficient. */
        double     mHtcCoeff1;          /**<    (W*s/K/kg) trick_chkpnt_io(**) Heat transfer 1st order coefficient. */
        double     mHtcExponent;        /**<    (--)       trick_chkpnt_io(**) Heat transfer coefficient exponent. */
        double     mHtcLimit;           /**<    (W/K)      trick_chkpnt_io(**) Heat transfer coefficient limit. */

        // Input data
        double*    mSegmentTemperature; /**<    (K)        trick_chkpnt_io(**) Segment wall temperatures (input from simbus). */

        // Internal state
        double     mSegmentHtc;         /**<    (W/K)      trick_chkpnt_io(**) Effective segment HTC including wet surface. */
        double     mSensibleHeat;       /**<    (W)        trick_chkpnt_io(**) Total sensible heat removed from fluid. */
        double     mLatentHeat;         /**<    (W)        trick_chkpnt_io(**) Total latent heat removed from fluid. */
        double     mTotalHeat;          /**<    (W)        trick_chkpnt_io(**) Total heat removed from fluid. */
        double     mCondensationRate;   /**<    (kg/s)     trick_chkpnt_io(**) Total mass rate of condensation. */
        PolyFluid* mCondensateFluid;    /**<    (--)       trick_chkpnt_io(**) Condensed liquid fluid object. */

        // Outputs
        double*    mSegmentHeat;        /**<    (W)        trick_chkpnt_io(**) Heat transferred to segment wall (output to simbus). */
        /// @brief  Validates the initialization of this Condensing Heat Exchanger.
        void validate(const GunnsFluidCondensingHxConfigData& configData,
                      const GunnsFluidCondensingHxInputData&  inputData) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief  Computes the nominal heat transfer coefficient for this Condensing Heat Exchanger.
        virtual void computeHeatTransferCoefficient();
        /// @brief  Applies degradations to the heat transfer coefficient for this Condensing Heat Exchanger.
        virtual void degradeHeatTransferCoefficient();
        /// @brief  Updates the heat exchanger properties over the time step.
        virtual void updateHeatExchanger(const double dt);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidCondensingHx(const GunnsFluidCondensingHx& );
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidCondensingHx& operator =(const GunnsFluidCondensingHx&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) Total heat removed from the air stream by the heat exchanger.
///
/// @details  Includes both sensible heat and latent heat of condensation.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidCondensingHx::getTotalHeat() const
{
    return mTotalHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (kg/s) Condensation rate of water in the heat exchanger.
///
/// @details  Rate of water removed from the air stream and pooled in the heat exchanger.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidCondensingHx::getCondensationRate() const
{
    return mCondensationRate;
}

#endif
