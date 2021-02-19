#ifndef GunnsFluidHeatExchanger_EXISTS
#define GunnsFluidHeatExchanger_EXISTS

/*************************************************************************************************
 @defgroup  TSM_GUNNS_FLUID_CONDUCTOR_HEAT_EXCHANGER Heat Exchanger
 @ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 @details
 \image html conductor/images/HeatExchangerInit.pdf "Initialization sequence"
 \image html conductor/images/HeatExchangerSeq.pdf  "Update sequence"
 PURPOSE:
 - ( Provides convective heat transfer between a fluid and material based on the heat transfer
     coefficient of the device and the temperature differential. This class is intended to be
     subclassed to provide a device specific heat transfer coefficient equation.)

 REFERENCE:
 - ((A Transient Thermal Model of the Portable Life Support System - AIAA-94-4622))

 ASSUMPTIONS AND LIMITATIONS:
 - ( SI units only )

 LIBRARY DEPENDENCY:
 -((GunnsFluidHeatExchanger.o))

 PROGRAMMERS:
 -(
   ((Kevin Supak) (L-3 Titan) (Dec 2009) (Initial prototype))
   ((Chuck Sorensen) (LZT) (Jan 2011) (Convert to C++))
  )
 @{
 */

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heat Exchanger Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Heat Exchanger link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHeatExchangerConfigData: public GunnsFluidConductorConfigData
{
    public:
       int mNumSegs;                         /**< (--) trick_chkpnt_io(**) Number of segments for this Heat Exchanger */
       /// @brief    Default constructs this Heat Exchanger configuration data.
       GunnsFluidHeatExchangerConfigData(const std::string& name                 = "",
                                         GunnsNodeList*     nodes                = 0,
                                         const double       maxConductivity      = 0.0,
                                         const double       expansionScaleFactor = 0.0,
                                         const int          numSegs              = 0);
       /// @brief    Copy constructs this Heat Exchanger configuration data.
       GunnsFluidHeatExchangerConfigData(const GunnsFluidHeatExchangerConfigData& that);
       /// @brief    Default destructs this Heat Exchanger configuration data.
       virtual ~GunnsFluidHeatExchangerConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
       GunnsFluidHeatExchangerConfigData& operator=(const GunnsFluidHeatExchangerConfigData&);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heat Exchanger Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Heat Exchanger link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHeatExchangerInputData : public GunnsFluidConductorInputData
{
    public:
        double  mHeatTransferCoefficient;    /**< (W/K) Default overall Heat Transfer coefficient */
        double  mInitialSegmentTemperature;  /**< (K)   Default segment wall temperature          */
        double* mSegmentHtcOverrides;        /**< (W/K) Segment heat transfer coeffs (optional)   */
        double  mTemperatureOverride;        /**< (K)   Fluid temperature override (optional)     */
        /// @brief    Default constructs this Heat Exchanger input data.
        GunnsFluidHeatExchangerInputData(const bool   malfBlockageFlag          = false,
                                         const double malfBlockageValue         = 0.0,
                                         const double heatTransferCoefficient   = 0.0,
                                         const double initialSegmentTemperature = 0.0);
        /// @brief    Copy constructs this Heat Exchanger input data.
        GunnsFluidHeatExchangerInputData(const GunnsFluidHeatExchangerInputData& that);
        /// @brief    Default destructs this Heat Exchanger input data.
        virtual ~GunnsFluidHeatExchangerInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHeatExchangerInputData& operator=(const GunnsFluidHeatExchangerInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heat Exchanger Model
///
/// @details  The GUNNS Fluid Heat Exchanger link model simulates a flow path through a segmented
///           pipe in a heat exchanger.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHeatExchanger : public  GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidHeatExchanger);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool    mMalfHxDegradeFlag;    /**< (--)                     Overall heat transfer degradation malf flag           */
        double  mMalfHxDegradeValue;   /**< (--)                     Overall heat transfer degradation malf HTC multiplier */
        bool*   mMalfSegDegradeFlag;   /**< (--) trick_chkpnt_io(**) Segment heat transfer degradation malf flag           */
        double* mMalfSegDegradeValue;  /**< (--) trick_chkpnt_io(**) Segment heat transfer degradation malf HTC multiplier */
        /// @}

        /// @brief    Default constructs this Heat Exchanger.
        GunnsFluidHeatExchanger();
        /// @brief    Default destructs this Heat Exchanger.
        virtual ~GunnsFluidHeatExchanger();
        /// @brief    Initializes this Heat Exchanger with configuration and input data.
        void initialize(const GunnsFluidHeatExchangerConfigData& configData,
                        const GunnsFluidHeatExchangerInputData&  inputData,
                        std::vector<GunnsBasicLink*>&            networkLinks,
                        const int                                port0,
                        const int                                port1);
        /// @brief    Updates the internal fluid of this Heat Exchanger.
        virtual void updateFluid(const double dt, const double flowRate);
        /// @brief    Gets the energy gain from this Heat Exchanger.
        double getTotalEnergyGain() const;
        /// @brief    Gets the delta temperature from this Heat Exchanger.
        double getDeltaTemperature() const;
        /// @brief    Gets the number of segments from this Heat Exchanger.
        int getNumSegs() const;
        /// @brief Sets and resets the HX heat transfer degradation malfunction.
        void setMalfHxDegrade(const bool flag = false, const double value = 0.0);
        /// @brief Sets and resets a segment heat transfer degradation malfunction.
        void setMalfSegDegrade(const int segment, const bool flag = false, const double value = 0.0);
        /// @brief Sets Temperature Override.
        void setTemperatureOverride(const double value = 0.0);

    protected:
        int     mNumSegs;                 /**< *o (--)  trick_chkpnt_io(**) Number of segment for this Heat Exchanger */
        double* mSegHtcDefault;           /**<    (W/K) trick_chkpnt_io(**) Default segment heat transfer coefficients */
        double* mSegHtc;                  /**<    (W/K) trick_chkpnt_io(**) Segment heat transfer coefficients (computed) */
        double* mSegTemperature;          /**<    (K)   trick_chkpnt_io(**) Segment temperatures */
        double* mSegEnergyGain;           /**<    (W)   trick_chkpnt_io(**) Segment energy gains */
        double  mTotalEnergyGain;         /**<    (W)   trick_chkpnt_io(**) Total heat exchanger energy gain */
        double  mDeltaTemperature;        /**<    (K)   trick_chkpnt_io(**) Change in temperature across heat exchanger */
        double  mTemperatureOverride;     /**<    (K)                       Temperature override-to value (0 turns off) */
        /// @brief    Validates the initialization of this Heat Exchanger.
        void validate(const GunnsFluidHeatExchangerConfigData& configData,
                      const GunnsFluidHeatExchangerInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Computes the heat transfer coefficient for this Heat Exchanger.
        virtual void computeHeatTransferCoefficient();
        /// @brief    Calculate the heat transfer coefficient from vendor specified data.
        virtual void updateSegments(const double dt, const double flowRate);
        /// @brief    Applies the temperature override to the internal fluid.
        virtual void applyTemperatureOverride();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHeatExchanger(const GunnsFluidHeatExchanger& );
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHeatExchanger& operator =(const GunnsFluidHeatExchanger&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (W)  Total energy gain.
///
/// @details  Returns the total energy gain (sum of segment energy gains) of this GUNNS Fluid
///           Heat Exchanger link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidHeatExchanger::getTotalEnergyGain() const
{
    return mTotalEnergyGain;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (K)  Delta temperature.
///
/// @details  Returns the delta temperature of this GUNNS Fluid Heat Exchanger link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidHeatExchanger::getDeltaTemperature() const
{
    return mDeltaTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (--)  Number of Segments.
///
/// @details  Returns the number of segments of this GUNNS Fluid Heat Exchanger link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsFluidHeatExchanger::getNumSegs() const
{
    return mNumSegs;
}

#endif
