#ifndef GunnsElectIps_EXISTS
#define GunnsElectIps_EXISTS

/**
@file     GunnsElectIps.hh
@brief    GUNNS Electrical Internal Power Supply declarations

@defgroup  TSM_GUNNS_ELECTRICAL_IPS_LINK Internal Power Supply
@ingroup   TSM_GUNNS_ELECTRICAL_IPS

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for the GUNNS Internal Power Supply link.)

REFERENCE:
- ((ISS DCSU system brief.))

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectIps.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (February 2019) (Initial))

@{
*/

#include <string>
#include <vector>
#include <math.h>
#include "core/GunnsBasicLink.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    IPS Configuration Data
/// @details  The sole purpose of this class is to provide a data structure for the IPS
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectIpsConfigData : public GunnsBasicLinkConfigData {
    public:
        bool   mBackUpPowerImplemented;             /**< (1)     trick_chkpnt_io(**) Backup power used */
        double mDefaultPowerConsumedOn;             /**< (W)     trick_chkpnt_io(**) Power Supply Load */
        double mAuxOnePowerConsumedOn;              /**< (W)     trick_chkpnt_io(**) Auxillary Load for power supply */
        double mAuxTwoPowerConsumedOn;              /**< (W)     trick_chkpnt_io(**) Auxillary Load for power supply */
        double mUnderVoltageLimit;                  /**< (V)     trick_chkpnt_io(**) IPS Minimum Primary voltage needed to operate */
        double mBackUpVoltageThreshold;             /**< (V)     trick_chkpnt_io(**) voltage threshold to use backup source */
        double mPotentialOnTolerance;               /**< (V)     trick_chkpnt_io(**) Power Sources Tolerance */
        double mThermalFraction;                    /**< (1)     trick_chkpnt_io(**) Fraction of power load output as waste heat. */
        double mBackupVoltageMin;                   /**< (V)     trick_chkpnt_io(**) Backup voltage minimum */
        double mBackupVoltageMax;                   /**< (V)     trick_chkpnt_io(**) Backup voltage maximum */
        double mConductanceTolerance;               /**< (1)     trick_chkpnt_io(**) Amount tolerance can change without notifing gunns */
        int    mConvergedFrameToCheckVoltage;       /**< (1)     trick_chkpnt_io(**) minor frame after convergance to check voltage switch */
        int    mNumberOfVoltageSwitchesInASolution; /**< (1)     trick_chkpnt_io(**) number of times a switch allowd to change in solutionAcceptable*/
        bool   mCommandOnUsed;                      /**< (1)     trick_chkpnt_io(**) Command on used for turning on/off IPS */
        double mUnselectedInputConductance;         /**< (1/ohm) trick_chkpnt_io(**) Conductance on un-selected input channels. */
        /// @brief Default constructs this IPS configuration data.
        GunnsElectIpsConfigData(const std::string& name                               = "",
                                GunnsNodeList*     nodes                              = 0,
                                const bool         backUpPowerImplemented             = false,
                                const double       powerConsumedOn                    = 0.0,
                                const double       auxillaryOnePowerOn                = 0.0,
                                const double       auxillaryTwoPowerOn                = 0.0,
                                const double       underVoltageLimit                  = 0.0,
                                const double       backUpVoltageThreshold             = 0.0,
                                const double       potentialOnTolerance               = 0.0,
                                const double       thermalFraction                    = 0.0,
                                const double       backupVoltageMin                   = 0.0,
                                const double       backupVoltageMax                   = 0.0,
                                const double       conductanceTolerance               = 0.0,
                                const int          convergedFrameToCheckVoltage       = 0,
                                const int          numberOfVoltageSwitchesInASolution = 0,
                                const bool         commandOnUsed                      = false,
                                const double       unselectedInputConductance         = 0.0);

        /// @brief Default destructs this IPS configuration data.
        virtual ~GunnsElectIpsConfigData();
        /// @brief Copy constructs this IPS configuration data.
        GunnsElectIpsConfigData(const GunnsElectIpsConfigData& that);

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsElectIpsConfigData& operator =(const GunnsElectIpsConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    IPS Elect Input Data
/// @details  This provides a data structure for the IPS Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectIpsInputData : public GunnsBasicLinkInputData
{
    public:
        /// @brief Default constructs this IPS Elect input data.
        GunnsElectIpsInputData(const bool   malfBlockageFlag  = false,
                               const double malfBlockageValue = 0.0);
        /// @brief Copy constructs this IPS Elect input data.
        GunnsElectIpsInputData(const GunnsElectIpsInputData& that);
        /// @brief Default destructs this IPS Elect input data.
        virtual ~GunnsElectIpsInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectIpsInputData& operator =(const GunnsElectIpsInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Internal Power Supply Link
///
/// @details  Internal Power Supply Electrical aspect. This can have multiple power sources and two
///           auxiliary loads.  The multiple power sources are attached directly to the GUNNS
///           network.  The loads are modeled as constant power loads.  The power loads can be given
///           a tolerance for easier convergence.  If no backup power source is selected. It will
///           switch to the power source with the greatest voltage. The current voltage is compared
///           against another source voltage minus the switching voltage tolerance.  This can work
///           for any number of input feeds.  If a back up power source is selected it will use the
///           ISS logic to switch between power sources.
///
///           This is a copy of the original IpsElect, modified to do its variable port assignment
///           and initialization similarly to the new GunnsDraw standard, and cleaned to match GUNNS
///           code standards an style.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectIps : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectIps);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool*  mMalfPowerInput;             /**< (1) trick_chkpnt_io(**) Fail off individual power supply input. */
        bool   mMalfAllPowerInputs;         /**< (1)                     Fail off all power supply inputs. */
        bool   mMalfBiasPowerConsumedFlag;  /**< (1)                     Power load bias activation flag. */
        double mMalfBiasPowerConsumedValue; /**< (W)                     Power load bias value. */
        /// @}

        /// @brief Enumeration of some model constants.
        enum LocalConstants {
            INVALID_SOURCE = -1             ///< Invalid voltage source selection.
        };

        /// @brief Constructor for IPS class
        explicit GunnsElectIps();
        /// @brief Destructor for IPS class
        virtual ~GunnsElectIps();
        /// @brief Initialize the internal power supply with configuration and initialize data
        virtual void initialize(      GunnsElectIpsConfigData &configData,
                               const GunnsElectIpsInputData &inputData,
                               std::vector<GunnsBasicLink*>& networkLinks,
                               std::vector<int>* portsVector);
        /// @brief Step method for updating the link
        virtual void step(const double timeStep);
        /// @brief Minor Step method for non-linear Systems
        virtual void minorStep(const double timeStep, const int minorStep);
        /// @brief Computes the flows through the link
        virtual void computeFlows(const double timeStep);
        /// @brief  Returns whether the link confirms, rejects, or delays the solution.
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);
        /// @brief Sets and resets the malfunction to fail a single power supply input.
        void setMalfPowerInput(const int input, const bool value = false);
        /// @brief Sets and resets the malfunction to fail all power supply inputs.
        void setMalfAllPowerInputs(const bool flag = false);
        /// @brief Command On the IPS
        void commandOn();
        /// @brief Command Off the IPS
        void commandOff();
        /// @brief Get Commanded on state
        bool isPowerCommandedOn() const;
        /// @brief Returns true if the link is non-linear
        bool isNonLinear();
        /// @brief Change the Power Supply Load
        virtual void setOutputPower(const double power);
        /// @brief Set the voltage trip limit
        void setUnderVoltageLimit(const double underVoltage);
        /// @brief Set the output power for Auxiliary item one
        virtual void setOutputPowerAuxiliaryOne(const double power);
        /// @brief Set the output power for Auxiliary item two
        virtual void setOutputPowerAuxiliaryTwo(const double power);
        /// @brief Gets the generated waste heat.
        double getPsHeatGenerated() const;
        /// @brief Gets the power valid flag.
        bool isPowerSupplyOn() const;
        /// @brief Gets the output power.
        double getOutputPower() const;

  protected:
        bool    mBackUpPowerImplemented;             /**< (1)     trick_chkpnt_io(**) Back up power logic Implemented */
        double  mDefaultPowerConsumedOn;             /**< (W)     trick_chkpnt_io(**) Default Power Supply Load */
        double  mAuxOnePowerConsumedOn;              /**< (W)     trick_chkpnt_io(**) Auxiliary Load for power supply */
        double  mAuxTwoPowerConsumedOn;              /**< (W)     trick_chkpnt_io(**) Auxiliary Load for power supply */
        double  mUnderVoltageLimit;                  /**< (V)     trick_chkpnt_io(**) Minimum voltage needed to operate */
        double  mBackUpVoltageThreshold;             /**< (V)     trick_chkpnt_io(**) voltage threshold to use backup source */
        double  mPotentialOnTolerance;               /**< (V)     trick_chkpnt_io(**) Power Sources Tolerance */
        double  mThermalFraction;                    /**< (1)     trick_chkpnt_io(**) Fraction of power load output as waste heat */
        double  mBackupVoltageMin;                   /**< (V)     trick_chkpnt_io(**) Backup voltage minimum */
        double  mBackupVoltageMax;                   /**< (V)     trick_chkpnt_io(**) Backup voltage maximum */
        double  mConductanceTolerance;               /**< (1/ohm) trick_chkpnt_io(**) Amount tolerance can change without notifying gunns */
        int     mNumberOfVoltageSwitchesInASolution; /**< (1)     trick_chkpnt_io(**) Number of times an input switch is allowed in solutionAcceptable */
        bool    mCommandOnUsed;                      /**< (1)     trick_chkpnt_io(**) Command on used for turning on/off IPS */
        double  mUnselectedInputConductance;         /**< (1/ohm) trick_chkpnt_io(**) Conductance on un-selected input channels. */
        double  mHeatGeneratedOn;                    /**< (W)     trick_chkpnt_io(**) Power Source On Heat generated */
        double  mHeatGenerated;                      /**< (W)                         Power Source Heat generated */
        double* mConductance;                        /**< (1)     trick_chkpnt_io(**) Power Supply Conductance */
        double* mActiveConductance;                  /**< (1)     trick_chkpnt_io(**) Power Supply Active Conductance */
        double* mOutputPower;                        /**< (W)     trick_chkpnt_io(**) Output Power from each input channel */
        bool    mPowerValid;                         /**< (1)                         Power Supply Output Good for use */
        double* mPowerSupplyVoltage;                 /**< (V)     trick_chkpnt_io(**) Power supply input voltage */
        int     mActivePowerSource;                  /**< (1)                         Power supply input source used */
        int     mLastActivePowerSource;              /**< (1)                         Power supply input source used last time*/
        double  mPowerConsumedOn;                    /**< (W)     trick_chkpnt_io(**) Power Supply Load */
        int     mNumberOfPowerSources;               /**< (1)     trick_chkpnt_io(**) Number of Power feeds for this load */
        int     mBackUpPowerSource;                  /**< (1)                         Number of Back Up Power for this load */
        double  mTotalPowerLoad;                     /**< (W)                         Total Power needed for IPS */
        bool    mBackupVoltageInRange;               /**< (1)                         Back up voltage in range */
        double  mLoadAddedFromInstructor;            /**< (W)                         Power added to load from instructor */
        int     mVoltageSwitches;                    /**< (1)                         Number of times an input switch has occurred this major step */
        bool    mControlInputPowerInUse;             /**< (1)                         Control power input being used */
        bool    mCommandOn;                          /**< (1)                         Command on state for IPS */
        bool    mVerbose;                            /**< (1)                         Increase H&S warning outputs */
        double* mInputCurrent;                       /**< (1)     trick_chkpnt_io(**) IPS input current feed x, needed for MDT */
        double* mInputVoltage;                       /**< (1)     trick_chkpnt_io(**) IPS input voltage feed x, needed for MDT */
        /// @brief Checks to see if the conductance has changed outside of tolerance value.
        virtual bool isConductanceChanged(const double conductanceOne,
                                          const double conductanceTwo) const;
        /// @brief Allocate the dynamic memory
        virtual void allocateArrays();
        /// @brief Deallocate the dynamic memory
        virtual void cleanup();
        /// @brief Validate some parameters
        virtual void validate();
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Returns the voltage minus the tolerance value for comparison
        double getPotentialValue(const double voltage) const;
        /// @brief Finds the input source to use.
        int getVoltagePrimarySourceGreaterThanBackupThreshold();
        /// @brief Finds the input source to use.
        int getVoltagePrimarySourceGreaterUnderVoltageLimit();
        /// @brief Calculates if the backup power source is in range.
        int getVoltageBackupWithinRange();
        /// @brief Calculates which voltage source to use including the backup source considerations.
        int getVoltageSourceToUseWithBackup();
        /// @brief Calculates which voltage source to use excluding the backup source.
        int getVoltageSourceToUseWithoutBackup();
        /// @brief Calculates the flows once the voltage source determined.
        void calculateFlow(const int sourceToUse);
        /// @brief Calculates the admittance matrix for gunns.
        void updateAdmittance();
        /// @brief Calculates the conductance of the input channels.
        void updateInputConductance();
        /// @brief Calculates the power load on the selected input channel.
        void updatePowerLoad();
        /// @brief Calculates the heat transfer to TCS
        virtual void calculateHeatTransfer();
        // @brief private copy constructor, prevents invocation
        GunnsElectIps(const GunnsElectIps&);
        // @brief private assignment operator, prevents invocation
        GunnsElectIps & operator =(const GunnsElectIps& n);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Command On the IPS
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectIps::commandOn()
{
    if (mCommandOnUsed) {
        mCommandOn = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Command Off the IPS
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectIps::commandOff()
{
    if (mCommandOnUsed) {
        mCommandOn = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return bool commanded on state
///
/// @details Get Commanded on state
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectIps::isPowerCommandedOn() const
{
    return mCommandOn;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  Returns true if the link is non-linear
///
/// @details  Returns true if the link is non-linear
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectIps::isNonLinear()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   [in] power  (W) New power value for the IPS
///
/// @details Change the Power Supply Load
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectIps::setOutputPower(const double power)
{
    if (0.0 <= power) {
        mPowerConsumedOn = power;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param  [in]  underVoltage  (V) New under voltage limit for IPS
///
/// @details Set the voltage trip limit
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectIps::setUnderVoltageLimit(const double underVoltage)
{
    if (0.0 <= underVoltage) {
        mUnderVoltageLimit = underVoltage;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param  [in]  power  (W) New power load for Auxiliary item two
///
/// @details Set the output power for Auxiliary item one
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectIps::setOutputPowerAuxiliaryOne(const double power)
{
    if (0.0 <= power) {
        mAuxOnePowerConsumedOn = power;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param  [in]  power  (W) New power load for Auxiliary item two
///
/// @details Set the output power for Auxiliary item two
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectIps::setOutputPowerAuxiliaryTwo(const double power)
{
    if (0.0 <= power) {
        mAuxTwoPowerConsumedOn = power;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  bool power status of IPS
///
/// @details Gets the power valid flag.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectIps::isPowerSupplyOn() const
{
    return mPowerValid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  bool power status of IPS
///
/// @details Gets the generated waste heat.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectIps::getPsHeatGenerated() const
{
    return mHeatGenerated;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  double power consumed  of IPS
///
/// @details Gets the output power.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectIps::getOutputPower() const
{
    //TODO should this return mTotalPowerLoad instead?
    return mPowerConsumedOn;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @details Calculates the heat transfer to TCS
///////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectIps::calculateHeatTransfer()
{
    if (mPowerValid) {
        mHeatGenerated = mHeatGeneratedOn;
    } else {
        mHeatGenerated = 0.0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @return bool true if the conductance needs to be updated.
///
/// @details Checks to see if the conductance has changed outside of tolerance value.
///////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectIps::isConductanceChanged(const double conductanceOne,
                                                const double conductanceTwo) const
{
    return (fabs(conductanceOne - conductanceTwo) > mConductanceTolerance);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calculates the total input power to be applied to the selected input channel.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectIps::updatePowerLoad() {
    if (mLoadAddedFromInstructor > 0.0){
        mTotalPowerLoad = mLoadAddedFromInstructor;
    } else {
        mTotalPowerLoad = mPowerConsumedOn + mAuxOnePowerConsumedOn + mAuxTwoPowerConsumedOn;
    }
}

#endif
