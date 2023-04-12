#ifndef UtGunnsEpsConstantPowerLoad_EXISTS
#define UtGunnsEpsConstantPowerLoad_EXISTS

/**
@defgroup  UT_GUNNS_CPL_TEST_LINK    Constant Power Load Link for Solver Test
@ingroup   UT_GUNNS

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Constant Power Load Solver Test Link.  This is a variant of aspects/electrical/
   ConstantPowerLoad/EpsConstantPowerLoad that is modified to support testing the GUNNS solver in
   UtGunns.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((UtGunnsEpsConstantPowerLoad.o))

PROGRAMMERS:
- (
   (Jason Harvey) (CACI) (2023-04) (Initial)
  )

@{
*/

#include <string>
#include <vector>

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Constance Power Load Configuration Data
/// @details  The sole purpose of this class is to provide a data structure for the Basic Constant
///           Load configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class EpsConstantPowerLoadConfigData : public GunnsBasicConductorConfigData {
    public:
        double mDefaultPower;         /**< (--) trick_chkpnt_io(**) The default power load of the link */
        double mMinimumVoltageLimit;  /**< (V) trick_chkpnt_io(**) Minimum voltage to act like constant power load */

        /// @brief Default constructs this Basic Constant Load configuration data.
        EpsConstantPowerLoadConfigData(const std::string& name                 =  "",
                                         GunnsNodeList*   nodes                =   0,
                                         const double     defaultConductivity  = 0.0,
                                         const double     defaultPower         = 0.0,
                                         const double     minimumVoltageLimit  = 0.0);

        /// @brief Default destructs this Basic Constant Load configuration data.
        virtual ~EpsConstantPowerLoadConfigData();

        /// @brief Copy constructs this Basic Constant Load configuration data.
        EpsConstantPowerLoadConfigData(const EpsConstantPowerLoadConfigData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        EpsConstantPowerLoadConfigData& operator =(const EpsConstantPowerLoadConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Potential Input Data
/// @details  The sole purpose of this class is to provide a data structure for the Basic Constant
///           Load input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class EpsConstantPowerLoadInputData : public GunnsBasicConductorInputData {
    public:
        /// @brief    Default constructs this Basic Potential input data.
        EpsConstantPowerLoadInputData(const bool   malfBlockageFlag  = false,
                                      const double malfBlockageValue = 0.0);

        /// @brief    Default destructs this Basic Potential input data.
        virtual ~EpsConstantPowerLoadInputData();

        /// @brief    Copy constructs this Basic Potential input data.
        EpsConstantPowerLoadInputData(const EpsConstantPowerLoadInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        EpsConstantPowerLoadInputData& operator =(const EpsConstantPowerLoadInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basic Constant Load
/// @details  Gunns basic constant load is a non linear model which adjusts conductivity
///           to meet a constant power load.
////////////////////////////////////////////////////////////////////////////////////////////////////
class EpsConstantPowerLoad : public GunnsBasicConductor {
    TS_MAKE_SIM_COMPATIBLE(EpsConstantPowerLoad);

    public:
        /// @brief  Gunns Basic Constant Load Constructor
        EpsConstantPowerLoad();

        /// @brief  Gunns Basic Constant Load Destructor
        virtual ~EpsConstantPowerLoad();

        /// @brief Initialize method
        void initialize(const EpsConstantPowerLoadConfigData& configData,
                        const EpsConstantPowerLoadInputData&  inputData,
                        std::vector<GunnsBasicLink*>&         networkLinks,
                        const int                             port0,
                        const int                             port1);

        /// @brief Minor step method for non-linear iterations
        virtual void minorStep(const double dt, const int);

        /// @brief Updates the State of the link
        virtual void updateState(const double);

        /// @brief Returns if the link is non-linear
        virtual bool isNonLinear();

        /// @brief Method for setting the Power and Nominal Potential
        void setPowerAndNominalPotential(double power, double potential);

    protected:
        double mDesiredPower;         /**< (W) trick_chkpnt_io(**) Desired link power load */
        double mPowerDraw;            /**< (W) trick_chkpnt_io(**) Power draw by the link (opposite sign convention from BasicLink Power*/
        double mMinimumVoltageLimit;  /**< (V) trick_chkpnt_io(**) Minimum voltage to act like constant power load */
        double mBiasPowerLoadValue;   /**< (W)                     User-specified amount to bias the power load */

        /// @brief Method for validating the link
        void validate();

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        EpsConstantPowerLoad(const EpsConstantPowerLoad& that);

        /// @details  Assignment operator unavailable since declared private and not implemented.
        EpsConstantPowerLoad& operator =(const EpsConstantPowerLoad& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns if the link is Non-Linear
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool EpsConstantPowerLoad::isNonLinear() {
    return true;
}

#endif
