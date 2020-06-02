#ifndef GunnsElectShort_EXISTS
#define GunnsElectShort_EXISTS

/**
@file
@brief     GUNNS Electrical Short Link declarations

@defgroup  TSM_GUNNS_ELECTRICAL_RESISTIVE_SHORT    GUNNS Electrical Short Link
@ingroup   TSM_GUNNS_ELECTRICAL_RESISTIVE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical Short link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsElectShort.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2019-10) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicConductor.hh"
#include "GunnsShortUtil.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Short Link Configuration Data
///
/// @details  This class provides a data structure for the Electrical Short link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectShortConfigData : public GunnsBasicConductorConfigData
{
    public:
        /// @brief Default constructs this Electrical Short configuration data.
        GunnsElectShortConfigData(const std::string& name  = "",
                                  GunnsNodeList*     nodes = 0);
        /// @brief Default destructs this Electrical Short configuration data.
        virtual ~GunnsElectShortConfigData();
        /// @brief Copy constructs this Electrical Short configuration data.
        GunnsElectShortConfigData(const GunnsElectShortConfigData& that);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectShortConfigData& operator =(const GunnsElectShortConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Short Link Input Data
///
/// @details  This class provides a data structure for the Electrical Short link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectShortInputData : public GunnsBasicConductorInputData
{
    public:
        GunnsShortUtilInputData mShort; /**< (--) trick_chkpnt_io(**) The electrical short model input data. */
        /// @brief    Default constructs this Electrical Short input data.
        GunnsElectShortInputData(const bool   malfBlockageFlag         = false,
                                 const double malfBlockageValue        = 0.0,
                                 const double malfMinimumVoltage       = 0.0,
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
        /// @brief    Default destructs this Electrical Short input data.
        virtual ~GunnsElectShortInputData();
        /// @brief    Copy constructs this Electrical Short input data.
        GunnsElectShortInputData(const GunnsElectShortInputData& that);

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsElectShortInputData& operator =(const GunnsElectShortInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Short Link Class
///
/// @details  This extends GunnsBasicConductor with an electrical short model, GunnsShortUtil.  See
///           GunnsShortUtil for its features and limitations.  This link conductance is driven by
///           the short model.  If there is no short active, this link conductance is zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectShort : public GunnsBasicConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectShort);

    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsShortUtil mShort;                              /**< (--) The electrical short model. */
        /// @}
        /// @brief Default Electrical Short Constructor.
        GunnsElectShort();
        /// @brief Default Electrical Short Destructor.
        virtual ~GunnsElectShort();
        /// @brief Initialize method.
        void initialize(const GunnsElectShortConfigData& configData,
                        const GunnsElectShortInputData&  inputData,
                        std::vector<GunnsBasicLink*>&    networkLinks,
                        const int                        port0,
                        const int                        port1);
        /// @brief Minor Step method for non-linear networks.
        virtual void minorStep(const double dt, const int minorStep);
        /// @brief Updates the active link conductance based on current malfunctions and network conditions.
        virtual void updateState(const double dt);
        /// @brief Boolean for telling the solver if it is a non-linear link.
        virtual bool isNonLinear();
        /// @brief Returns the link's assessment of the solution.
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);

    protected:
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectShort(const GunnsElectShort& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectShort& operator =(const GunnsElectShort& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  Returns true when the link is non-linear.
///
/// @details  This returns true (non-linear) whenever the short load is operating in constant-
///           current or constant-power mode.  This will vary during run-time.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectShort::isNonLinear()
{
    return mShort.isConstantLoad();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  convergedStep  (--)  Not used.
/// @param[in]  absoluteStep   (--)  Not used.
///
/// @returns  GunnsBasicLink::SolutionResult  (--)  Always returns CONFIRM.
///
/// @details  This link never has a reason to reject or delay the solution so it always confirms.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsBasicLink::SolutionResult GunnsElectShort::confirmSolutionAcceptable(
        const int convergedStep __attribute__((unused)),
        const int absoluteStep  __attribute__((unused)))
{
    return GunnsBasicLink::CONFIRM;
}

#endif
