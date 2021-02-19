#ifndef GunnsFluidHxDynHtc_EXISTS
#define GunnsFluidHxDynHtc_EXISTS

/**
@file
@brief    GUNNS Heat Exchanger With Dynamic HTC Model declarations

@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_HEAT_EXCHANGER_DYN_HTC Heat Exchanger With Dynamic HTC
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Extends GunnsFluidHeatExchanger with a dynamic heat transfer coefficient that varies with the
   fluid mass flow rate.)

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
-((GunnsFluidHxDynHtc.o))

PROGRAMMERS:
-(
  ((Jason Harvey) (CACI) (Nov 2018) (Initial))
 )

@{
*/

#include "aspects/fluid/conductor/GunnsFluidHeatExchanger.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heat Exchanger Segment Dynamic Heat Transfer Coefficient
///
/// @details  This class models a fluid convection heat transfer coefficient as a function of fluid
///           mass flow rate, a degradation scalar, and configuration constants as:
///           HTC = 0 < degradation * (Coeff0 + Coeff1 * mdot^Exponent) < Limit
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHxDynHtcSegment
{
    public:
        double mCoeff0;   /**< (W/K)      0th order coefficient. */
        double mCoeff1;   /**< (W*s/K/kg) 1st order coefficient. */
        double mExponent; /**< (--)       Mass flow rate exponent. */
        double mLimit;    /**< (W/K)      Upper limit. */
        /// @brief  Default constructs this Heat Exchanger Segment Dynamic Heat Transfer Coefficient.
        GunnsFluidHxDynHtcSegment(const double coeff0   = 0.0,
                                  const double coeff1   = 0.0,
                                  const double exponent = 0.0,
                                  const double limit    = 0.0);
        /// @brief  Default destructs this Heat Exchanger Segment Dynamic Heat Transfer Coefficient.
        ~GunnsFluidHxDynHtcSegment();
        /// @brief  Copy constructs this Heat Exchanger Segment Dynamic Heat Transfer Coefficient.
        GunnsFluidHxDynHtcSegment(const GunnsFluidHxDynHtcSegment& that);
        /// @brief  Assignment operator for this Heat Exchanger Segment Dynamic Heat Transfer Coefficient.
        GunnsFluidHxDynHtcSegment& operator=(const GunnsFluidHxDynHtcSegment& that);
        /// @brief  Calculates and returns the resulting Heat Transfer Coefficient value.
        double update(const double mdot, const double degradation);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heat Exchanger With Dynamic HTC Configuration Data
///
/// @details  This class provides a data structure for the GUNNS Fluid Heat Exchanger With Dynamic
///           HTC link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHxDynHtcConfigData : public GunnsFluidHeatExchangerConfigData
{
    public:
        double                                 mHtcCoeff0;   /**< (W/K)      trick_chkpnt_io(**) Overall HTC 0th order coefficient. */
        double                                 mHtcCoeff1;   /**< (W*s/K/kg) trick_chkpnt_io(**) Overall HTC 1st order coefficient. */
        double                                 mHtcExponent; /**< (--)       trick_chkpnt_io(**) HTC mass flow rate exponent. */
        double                                 mHtcLimit;    /**< (W/K)      trick_chkpnt_io(**) Overall HTC upper limit. */
        std::vector<GunnsFluidHxDynHtcSegment> mSegsHtc;     /**< (--)       trick_chkpnt_io(**) Optional segment overrides config data. */
        /// @brief  Default constructs this Heat Exchanger With Dynamic HTC configuration data.
        GunnsFluidHxDynHtcConfigData(const std::string& name                 = "",
                                     GunnsNodeList*     nodes                = 0,
                                     const double       maxConductivity      = 0.0,
                                     const double       expansionScaleFactor = 0.0,
                                     const int          numSegs              = 0,
                                     const double       htcCoeff0            = 0.0,
                                     const double       htcCoeff1            = 0.0,
                                     const double       htcExponent          = 0.0,
                                     const double       htcLimit             = 0.0);
        /// @brief  Copy constructs this Heat Exchanger With Dynamic HTC configuration data.
        GunnsFluidHxDynHtcConfigData(const GunnsFluidHxDynHtcConfigData& that);
        /// @brief  Default destructs this Heat Exchanger With Dynamic HTC configuration data.
        virtual ~GunnsFluidHxDynHtcConfigData();
        /// @brief  Adds a segment heat transfer coefficient override to this configuration data.
        void addSegment(const double coeff0   = 0.0,
                        const double coeff1   = 0.0,
                        const double exponent = 0.0,
                        const double limit    = 0.0);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHxDynHtcConfigData& operator=(const GunnsFluidHxDynHtcConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heat Exchanger With Dynamic HTC Input Data
///
/// @details  This class provides a data structure for the GUNNS Fluid Heat Exchanger With Dynamic
///           HTC link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHxDynHtcInputData : public GunnsFluidHeatExchangerInputData
{
    public:
        /// @brief  Default constructs this Heat Exchanger With Dynamic HTC input data.
        GunnsFluidHxDynHtcInputData(const bool   malfBlockageFlag          = false,
                                    const double malfBlockageValue         = 0.0,
                                    const double initialSegmentTemperature = 0.0);
        /// @brief  Copy constructs this Heat Exchanger With Dynamic HTC input data.
        GunnsFluidHxDynHtcInputData(const GunnsFluidHxDynHtcInputData& that);
        /// @brief  Default destructs this Heat Exchanger With Dynamic HTC input data.
        virtual ~GunnsFluidHxDynHtcInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHxDynHtcInputData& operator=(const GunnsFluidHxDynHtcInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heat Exchanger With Dynamic HTC Model
///
/// @details  The GUNNS Fluid Heat Exchanger With Dynamic HTC link model extends
///           GunnsFluidHeatExchanger with a dynamic heat transfer coefficient that varies with the
///           fluid mass flow rate.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHxDynHtc : public GunnsFluidHeatExchanger
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidHxDynHtc);
    public:
        /// @brief  Default constructs this Heat Exchanger With Dynamic HTC.
        GunnsFluidHxDynHtc();
        /// @brief  Default destructs this Heat Exchanger With Dynamic HTC.
        virtual ~GunnsFluidHxDynHtc();
        /// @brief  Initializes this Heat Exchanger With Dynamic HTC with config and input data.
        void initialize(const GunnsFluidHxDynHtcConfigData& configData,
                        const GunnsFluidHxDynHtcInputData&  inputData,
                        std::vector<GunnsBasicLink*>&       networkLinks,
                        const int                           port0,
                        const int                           port1);

    protected:
        GunnsFluidHxDynHtcSegment* mSegsDynHtc; /**< (--) trick_chkpnt_io(**) Segment dynamic heat transfer coefficients. */
        /// @brief  Validates the initialization of this Heat Exchanger With Dynamic HTC.
        void validate(const GunnsFluidHxDynHtcConfigData& configData) const;
        /// @brief  Computes the dynamic segment heat transfer coefficients.
        virtual void computeHeatTransferCoefficient();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidHxDynHtc(const GunnsFluidHxDynHtc& );
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHxDynHtc& operator =(const GunnsFluidHxDynHtc&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to be assigned to.
///
/// @details  Assignment operator for this Heat Exchanger Segment Dynamic Heat Transfer Coefficient.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsFluidHxDynHtcSegment& GunnsFluidHxDynHtcSegment::operator=(const GunnsFluidHxDynHtcSegment& that)
{
    if (this != &that) {
        mCoeff0   = that.mCoeff0;
        mCoeff1   = that.mCoeff1;
        mExponent = that.mExponent;
        mLimit    = that.mLimit;
    }
    return *this;
}

#endif
