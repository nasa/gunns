#ifndef GunnsShortUtil_EXISTS
#define GunnsShortUtil_EXISTS

/**
@file
@brief     GUNNS Electrical Short Utility declarations

@defgroup  TSM_GUNNS_ELECTRICAL_RESISTIVE_SHORT_UTIL    GUNNS Electrical Short Utility
@ingroup   TSM_GUNNS_ELECTRICAL_RESISTIVE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical Short Utility.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsShortUtil.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2019-10) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Short Utility Input Data
///
/// @details  This class provides a data structure for the Electrical Short utility input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsShortUtilInputData
{
    public:
        double mMalfMinimumVoltage;       /**< (V)   Initial minimum voltage for all malfunction modes to flow. */
        bool   mMalfConstantPowerFlag;    /**< (--)  Initial constant power short malfunction activation flag. */
        double mMalfConstantPowerValue;   /**< (W)   Initial constant power short malfunction value. */
        bool   mMalfConstantCurrentFlag;  /**< (--)  Initial constant current short malfunction activation flag. */
        double mMalfConstantCurrentValue; /**< (amp) Initial constant current short malfunction value. */
        bool   mMalfInitialPowerFlag;     /**< (--)  Initial initial power short malfunction activation flag. */
        double mMalfInitialPowerValue;    /**< (W)   Initial initial power short malfunction value. */
        bool   mMalfInitialCurrentFlag;   /**< (--)  Initial initial current short malfunction activation flag. */
        double mMalfInitialCurrentValue;  /**< (amp) Initial initial current short malfunction value. */
        bool   mMalfResistanceFlag;       /**< (--)  Initial constant resistance short malfunction activation flag. */
        double mMalfResistanceValue;      /**< (ohm) Initial constant resistance short malfunction value. */
        /// @brief  Default constructs this Electrical Short input data.
        GunnsShortUtilInputData(const double malfMinimumVoltage       = 0.0,
                                const bool   malfConstantPowerFlag    = false,
                                const double malfConstantPowerValue   = 0.0,
                                const bool   malfConstantCurrentFlag  = false,
                                const double malfConstantCurrentValue = 0.0,
                                const bool   malfInitialPowerFlag     = false,
                                const double malfInitialPowerValue    = 0.0,
                                const bool   malfInitialCurrentFlag   = false,
                                const double malfInitialCurrentValue  = 0.0,
                                const bool   malfResistanceFlag       = false,
                                const double malfResistanceValue      = 0.0);
        /// @brief  Default destructs this Electrical Short input data.
        virtual ~GunnsShortUtilInputData();
        /// @brief  Copy constructs this Electrical Short input data.
        GunnsShortUtilInputData(const GunnsShortUtilInputData& that);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsShortUtilInputData& operator =(const GunnsShortUtilInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Short Utility Class
///
/// @details  This models an electrical short-circuit as a conductance across a voltage.  There are
///           several malfunction modes, listed below.  Only one mode can operate at a time, so if
///           multiple malfs are active, they operate in order of preference, with the most
///           preferred listed last:
///
///           - Constant Resistance: the short is a normal constant resistance.
///           - Initial Current: the short is a constant resistance calculated to create the
///             desired initial current based on network conditions at the moment of activation.
///             After starting, the actual current will vary as the network changes.
///           - Initial Power: similar to Initial Current, except based on an initial power value.
///           - Constant Current: the short is a constant current load, similar to
///             GunnsBasicFlowController.
///           - Constant Power: the short is a constant power load, similar to EpsConstantPowerLoad.
///
/// @note     The minimum voltage term is a placeholder for future upgrades but currently has no
///           function in the model.  The intention is for this to eventually be the minimum voltage
///           below which a short cannot flow, such as the voltage required to arc across an air
///           gap.  But implementation of this will be tricky in the non-linear network, so it is
///           left for future effort.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsShortUtil
{
    TS_MAKE_SIM_COMPATIBLE(GunnsShortUtil);

    public:
        /// @name  Malfunction terms.
        /// @{
        /// @details  Malfunction targets are public to allow access from the Trick events processor.
        double mMalfMinimumVoltage;       /**< (V)     Placeholder for future functionality, currently has no function. */
        bool   mMalfConstantPowerFlag;    /**< (--)    Constant power short malfunction activation flag. */
        double mMalfConstantPowerValue;   /**< (W)     Constant power short malfunction value. */
        bool   mMalfConstantCurrentFlag;  /**< (--)    Constant current short malfunction activation flag. */
        double mMalfConstantCurrentValue; /**< (amp)   Constant current short malfunction value. */
        bool   mMalfInitialPowerFlag;     /**< (--)    Initial power short malfunction activation flag. */
        double mMalfInitialPowerValue;    /**< (W)     Initial power short malfunction value. */
        bool   mMalfInitialCurrentFlag;   /**< (--)    Initial current short malfunction activation flag. */
        double mMalfInitialCurrentValue;  /**< (amp)   Initial current short malfunction value. */
        bool   mMalfResistanceFlag;       /**< (--)    Constant resistance short malfunction activation flag. */
        double mMalfResistanceValue;      /**< (ohm)   Constant resistance short malfunction value. */
        /// @}
        /// @brief  Default Electrical Short Utility Constructor.
        GunnsShortUtil();
        /// @brief  Default Electrical Short Utility Destructor.
        virtual ~GunnsShortUtil();
        /// @brief  Initialize method.
        void initialize(const GunnsShortUtilInputData& inputData);
        /// @brief  Computes the short conductance based on current malfunctions and voltage.
        virtual double computeShort(const double voltage);
        /// @brief  Returns whether the short is active in a constant load mode.
        bool isConstantLoad() const;
        /// @brief  Sets and clears the constant power short malfunction.
        void setMalfConstantPower(const bool flag = false, const double value = 0.0, const double minV = 0.0);
        /// @brief  Sets and clears the constant current short malfunction.
        void setMalfConstantCurrent(const bool flag = false, const double value = 0.0, const double minV = 0.0);
        /// @brief  Sets and clears the initial power short malfunction.
        void setMalfInitialPower(const bool flag = false, const double value = 0.0, const double minV = 0.0);
        /// @brief  Sets and clears the initial current short malfunction.
        void setMalfInitialCurrent(const bool flag = false, const double value = 0.0, const double minV = 0.0);
        /// @brief  Sets and clears the constant resistance short malfunction.
        void setMalfResistance(const bool flag = false, const double value = 0.0, const double minV = 0.0);

    protected:
        double mShortConductance;         /**< (1/ohm) Conductance of the short. */
        double mPreviousCurrent;          /**< (amp)   Last-pass value of initial current malfunction value. */
        double mPreviousPower;            /**< (W)     Last-pass value of initial power malfunction value. */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsShortUtil(const GunnsShortUtil& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsShortUtil& operator =(const GunnsShortUtil& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True when active in a constant-load mode.
///
/// @details  This returns true whenever the short load is non-zero in constant-current or constant-
///           power mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsShortUtil::isConstantLoad() const
{
    return (mMalfConstantPowerFlag or mMalfConstantCurrentFlag) and (mShortConductance > 0.0);
}

/// @}

#endif
