#ifndef EpsConstantPowerLoad_EXISTS
#define EpsConstantPowerLoad_EXISTS

/**
@file      EpsConstantPowerLoad.hh
@brief     Electrical Constant Power Load Link declarations

@defgroup  TSM_GUNNS_ELECTRICAL_CONSTANT_POWER_LOAD_LINK    Constant Power Load Link
@ingroup   TSM_GUNNS_ELECTRICAL_CONSTANT_POWER_LOAD

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Constant Power Load Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((EpsConstantPowerLoad.o))

PROGRAMMERS:
- (
   (Jason Harvey) (L3)   (2011-02) (Initial Prototype))
   (Kevin Supak)  (L3)   (2011-02) (Updated to Coding Standards))
   (Chetan Patel) (ESCG) (2012-02) (Renamed and added min power to be constanct power load))
  )

@{
*/

#include <string>
#include <vector>

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Constant Power Load Link Configuration Data.
///
/// @details  This provides a data structure for the constant power load link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class EpsConstantPowerLoadConfigData : public GunnsBasicConductorConfigData {
    public:
        double mDefaultPower;        /**< (W) trick_chkpnt_io(**) The default power load of the link. */
        double mMinimumVoltageLimit; /**< (V) trick_chkpnt_io(**) Minimum voltage to act like constant power load. */
        /// @brief Default constructs this constant power load configuration data.
        EpsConstantPowerLoadConfigData(const std::string& name                 =  "",
                                       GunnsNodeList*     nodes                =   0,
                                       const double       defaultConductivity  = 0.0,
                                       const double       defaultPower         = 0.0,
                                       const double       minimumVoltageLimit  = 0.0);
        /// @brief Default destructs this constant power load configuration data.
        virtual ~EpsConstantPowerLoadConfigData();
        /// @brief Copy constructs this constant power load configuration data.
        EpsConstantPowerLoadConfigData(const EpsConstantPowerLoadConfigData& that);

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        EpsConstantPowerLoadConfigData& operator =(const EpsConstantPowerLoadConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Constant Power Load Link Input Data.
///
/// @details  This provides a data structure for the constant power load link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class EpsConstantPowerLoadInputData : public GunnsBasicConductorInputData
{
    public:
        /// @brief    Default constructs this constant power load input data.
        EpsConstantPowerLoadInputData(const bool   malfBlockageFlag  = false,
                                      const double malfBlockageValue = 0.0);
        /// @brief    Default destructs this constant power load input data.
        virtual ~EpsConstantPowerLoadInputData();
        /// @brief    Copy constructs this constant power load input data.
        EpsConstantPowerLoadInputData(const EpsConstantPowerLoadInputData& that);

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        EpsConstantPowerLoadInputData& operator =(const EpsConstantPowerLoadInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Constant Power Load Link
///
/// @details  GUNNS electrical constant power load is a non linear model which adjusts conductivity
///           to meet a constant desired power load.
////////////////////////////////////////////////////////////////////////////////////////////////////
class EpsConstantPowerLoad : public GunnsBasicConductor
{
    TS_MAKE_SIM_COMPATIBLE(EpsConstantPowerLoad);

    public:
        /// @brief  GUNNS Electrical Constant Power Load constructor.
        EpsConstantPowerLoad();
        /// @brief  GUNNS Electrical Constant Power Load destructor.
        virtual ~EpsConstantPowerLoad();
        /// @brief Initialize method.
        void initialize(const EpsConstantPowerLoadConfigData& configData,
                        const EpsConstantPowerLoadInputData&  inputData,
                        std::vector<GunnsBasicLink*>&         networkLinks,
                        const int                             port0,
                        const int                             port1);
        /// @brief Minor step method for non-linear iterations.
        virtual void minorStep(const double dt, const int);
        /// @brief Updates the State of the link.
        virtual void updateState(const double);
        /// @brief Returns if the link is non-linear.
        virtual bool isNonLinear();
        /// @brief Method for setting the Power and Nominal Potential.
        void setPowerAndNominalPotential(double power, double potential);

    protected:
        double mDesiredPower;         /**< (W)                     Desired link power load. */
        double mPowerDraw;            /**< (W) trick_chkpnt_io(**) Power draw by the link (opposite sign convention from BasicLink Power. */
        double mMinimumVoltageLimit;  /**< (V) trick_chkpnt_io(**) Minimum voltage to act like constant power load. */
        double mBiasPowerLoadValue;   /**< (W)                     User-specified amount to bias the power load. */
        /// @brief Method for validating the link initialization.
        void validate();
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Updates the effective conductivity to draw the desired power.
        void updateEffectiveConductivity(const double potential);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        EpsConstantPowerLoad(const EpsConstantPowerLoad& that);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        EpsConstantPowerLoad& operator =(const EpsConstantPowerLoad& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) Always returns true.
///
/// @details  This link is always non-linear.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool EpsConstantPowerLoad::isNonLinear()
{
    return true;
}

#endif
