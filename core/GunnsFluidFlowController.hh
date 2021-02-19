#ifndef GunnsFluidFlowController_EXISTS
#define GunnsFluidFlowController_EXISTS

/**
@file
@brief    GUNNS Fluid Flow Controller Model declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_FLOW_CONTROLLER    Fluid Flow Controller Model
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Flow Controller link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidFlowController.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2016-06))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Flow Controller Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Flow Controller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidFlowControllerConfigData : public GunnsFluidConductorConfigData
{
    public:
        double mFilterProportionalGain; /**< (--) trick_chkpnt_io(**) (0-1) proportional gain for stability filter. */
        bool   mEnableReverseControl;   /**< (--) trick_chkpnt_io(**) Flag to enable control in reverse flow direction. */
        /// @brief    Default constructs this Flow Controller configuration data.
        GunnsFluidFlowControllerConfigData(const std::string& name                   = "",
                                           GunnsNodeList*     nodes                  = 0,
                                           const double       maxConductivity        = 0.0,
                                           const double       expansionScaleFactor   = 0.0,
                                           const double       filterProportionalGain = 0.0,
                                           const bool         enableReverseControl   = false);
        /// @brief    Copy constructs this Flow Controller configuration data.
        GunnsFluidFlowControllerConfigData(const GunnsFluidFlowControllerConfigData& that);
        /// @brief    Default destructs this Flow Controller configuration data.
        virtual ~GunnsFluidFlowControllerConfigData();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidFlowControllerConfigData& operator =(const GunnsFluidFlowControllerConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Flow Controller Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Flow Controller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidFlowControllerInputData : public GunnsFluidConductorInputData
{
    public:
        bool   mMalfFlowRateFlag;                        /**< (--)   Malfunction flow rate flag. */
        double mMalfFlowRateValue;                       /**< (kg/s) Malfunction flow rate value. */
        double mFlowRateCommand;                         /**< (kg/s) Initial commanded flow rate. */
        /// @brief    Default constructs this Flow Controller input data.
        GunnsFluidFlowControllerInputData(const bool   malfBlockageFlag  = false,
                                          const double malfBlockageValue = 0.0,
                                          const bool   malfFlowRateFlag  = false,
                                          const double malfFlowRateValue = 0.0,
                                          const double flowRateCommand   = 0.0);
        /// @brief    Copy constructs this Flow Controller input data.
        GunnsFluidFlowControllerInputData(const GunnsFluidFlowControllerInputData& that);
        /// @brief    Default destructs this Flow Controller input data.
        virtual ~GunnsFluidFlowControllerInputData();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidFlowControllerInputData& operator =(const GunnsFluidFlowControllerInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Flow Controller Model
///
/// @details  This link models a flow control valve that modulates its position to control the mass
///           flow through it to a commanded rate.  An upper limit is applied representing the fully
///           opened limit of valve area.
///
///           The link always controls flow in the forward direction from the inlet Port 0 to the
///           exit Port 1.  In the reverse direction, it can optionally either control flow or not
///           allow any flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidFlowController : public  GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidFlowController);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfFlowRateFlag;       /**< (--)                     Malfunction flow rate flag. */
        double mMalfFlowRateValue;      /**< (kg/s)                   Malfunction flow rate value. */
        /// @}

        /// @brief    Default constructs this Flow Controller.
        GunnsFluidFlowController();
        /// @brief    Default destructs this Flow Controller.
        virtual ~GunnsFluidFlowController();
        /// @brief    Initializes this Flow Controller with configuration and input data.
        void initialize(const GunnsFluidFlowControllerConfigData& configData,
                        const GunnsFluidFlowControllerInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        const int                                 port0,
                        const int                                 port1);
        /// @brief    Sets this Flow Controller commanded flow rate.
        void setFlowRateCommand(const double flowRate = 0.0);
        /// @brief    Sets this Flow Controller flow rate malfunction.
        void setMalfFlowRate(const bool flag = false, const double value = 0.0);

    protected:
        double mFilterProportionalGain; /**< (--) trick_chkpnt_io(**) (0-1) proportional gain for stability filter. */
        bool   mEnableReverseControl;   /**< (--) trick_chkpnt_io(**) Flag to enable control in reverse flow direction. */
        double mFlowRateCommand;        /**< (kg/s)                   Input commanded flow rate. */
        double mLastConductivity;       /**< (m2) trick_chkpnt_io(**) Last pass of stability filter output. */
        /// @brief    Validates the initialization of this Flow Controller.
        void         validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the state of this Flow Controller.
        virtual void updateState(const double dt);

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidFlowController(const GunnsFluidFlowController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidFlowController& operator =(const GunnsFluidFlowController&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flowRate  (kg/s)  The desired flow rate for the link to flow.
///
/// @details  Sets the input commanded flow rate.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidFlowController::setFlowRateCommand(const double flowRate)
{
    mFlowRateCommand = flowRate;
}

#endif
