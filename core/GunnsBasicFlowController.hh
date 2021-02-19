#ifndef GunnsBasicFlowController_EXISTS
#define GunnsBasicFlowController_EXISTS

/**
@file
@brief    GUNNS Basic Flow Controller Model declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC_FLOW_CONTROLLER    Basic Flow Controller Model
@ingroup   TSM_GUNNS_CORE_LINK_BASIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Basic Flow Controller link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsBasicFlowController.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2016-06))

@{
*/

#include "core/GunnsBasicConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Flow Controller Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Basic Flow
///           Controller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicFlowControllerConfigData : public GunnsBasicConductorConfigData
{
    public:
        bool mNonLinear;            /**< (--) trick_chkpnt_io(**) Flag to operate in non-linear network mode. */
        bool mEnableReverseControl; /**< (--) trick_chkpnt_io(**) Flag to enable control in reverse flux direction. */
        /// @brief    Default constructs this Flow Controller configuration data.
        GunnsBasicFlowControllerConfigData(const std::string& name                 = "",
                                           GunnsNodeList*     nodes                = 0,
                                           const double       defaultConductivity  = 0.0,
                                           const bool         nonLinear            = false,
                                           const bool         enableReverseControl = false);
        /// @brief    Copy constructs this Flow Controller configuration data.
        GunnsBasicFlowControllerConfigData(const GunnsBasicFlowControllerConfigData& that);
        /// @brief    Default destructs this Flow Controller configuration data.
        virtual ~GunnsBasicFlowControllerConfigData();

    private:
        /// @brief    Assignment operator unavailable since declared private and not implemented.
        GunnsBasicFlowControllerConfigData& operator =(const GunnsBasicFlowControllerConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Flow Controller Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Basic Flow
///           Controller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicFlowControllerInputData : public GunnsBasicConductorInputData
{
    public:
        bool   mMalfFluxFlag;                              /**< (--) Malfunction flux rate flag. */
        double mMalfFluxValue;                             /**< (--) Malfunction flux rate value. */
        double mFluxCommand;                               /**< (--) Initial commanded flux. */
        /// @brief    Default constructs this Flow Controller input data.
        GunnsBasicFlowControllerInputData(const bool   malfBlockageFlag  = false,
                                          const double malfBlockageValue = 0.0,
                                          const bool   malfFluxFlag      = false,
                                          const double malfFluxValue     = 0.0,
                                          const double fluxCommand       = 0.0);
        /// @brief    Copy constructs this Flow Controller input data.
        GunnsBasicFlowControllerInputData(const GunnsBasicFlowControllerInputData& that);
        /// @brief    Default destructs this Flow Controller input data.
        virtual ~GunnsBasicFlowControllerInputData();

    private:
        /// @brief    Assignment operator unavailable since declared private and not implemented.
        GunnsBasicFlowControllerInputData& operator =(const GunnsBasicFlowControllerInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Flow Controller Model
///
/// @details  This link models a flow control resistor that modulates its resistance to control the
///           flux through it to a commanded rate.  An upper limit is applied representing the max
///           possible conductance of the device.
///
///           The link always controls flow in the forward direction from the inlet Port 0 to the
///           exit Port 1.  In the reverse direction, it can optionally either control flow or not
///           allow any flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicFlowController : public  GunnsBasicConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBasicFlowController);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfFluxFlag;         /**< (--)                     Malfunction flux rate flag. */
        double mMalfFluxValue;        /**< (--)                     Malfunction flux rate value. */
        /// @}

        /// @brief    Default constructs this Flow Controller.
        GunnsBasicFlowController();
        /// @brief    Default destructs this Flow Controller.
        virtual ~GunnsBasicFlowController();
        /// @brief    Initializes this Flow Controller with configuration and input data.
        void initialize(const GunnsBasicFlowControllerConfigData& configData,
                        const GunnsBasicFlowControllerInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        const int                                 port0,
                        const int                                 port1);
        /// @brief    Minor step method for non-linear iterations.
        virtual void minorStep(const double dt, const int minorStep);
        /// @brief    Updates the state of this Flow Controller.
        virtual void updateState(const double dt);
        /// @brief    Sets this Flow Controller commanded flux.
        void         setFluxCommand(const double flowRate = 0.0);
        /// @brief    Sets this Flow Controller flux malfunction.
        void         setMalfFlux(const bool flag = false, const double value = 0.0);
        /// @brief    Returns whether the this Flow Controller is non-linear.
        virtual bool isNonLinear();

    protected:
        bool   mNonLinear;            /**< (--) trick_chkpnt_io(**) Flag to operate in non-linear network mode. */
        bool   mEnableReverseControl; /**< (--) trick_chkpnt_io(**) Flag to enable control in reverse flux direction. */
        double mFluxCommand;          /**< (--)                     Input commanded flux. */
        /// @brief    Validates the initialization of this Flow Controller.
        void         validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsBasicFlowController(const GunnsBasicFlowController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsBasicFlowController& operator =(const GunnsBasicFlowController&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flux  (--)  The desired flux for the link to flow.
///
/// @details  Sets the input commanded flux.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicFlowController::setFluxCommand(const double flux)
{
    mFluxCommand = flux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True when the instance is operating as a non-linear link.
///
/// @details  Returns whether the instance is non-linear.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicFlowController::isNonLinear()
{
    return mNonLinear;
}

#endif
