#ifndef GunnsFluidShadow_EXISTS
#define GunnsFluidShadow_EXISTS

/**
@file
@brief    GUNNS Fluid Shadow Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_SHADOW    GUNNS Fluid Shadow Link
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Shadow link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidShadow.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-01) (Initial Prototype)

@{
*/

#include "GunnsFluidPotential.hh"
#include "GunnsFluidUtils.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Shadow Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Shadow
///           configuration data.
///
/// @note     We recommend for the maxConductivity you use 1.0E10 to provide a near-perfect pressure
///           source - but go no higher, as too high a value can cause instability in the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidShadowConfigData : public GunnsFluidPotentialConfigData
{
    public:
        /// @details Shadowing is active when the control input term, mShadowActiveControl, is equal
        ///          to this term.  So by default, if this term is 1, shadowing will be active when
        ///          mShadowActiveControl is 1, and so on.  This allows a pair of these links to
        ///          alternately shadow each other using the same value of the input control - one
        ///          link configures mControlTarget 0, the other uses 1.  Any values can be used.
        int                           mControlTarget;     /**< (--) trick_chkpnt_io(**) The value mShadowActiveControl must match to activate shadowing */
        GunnsFluidUtils::MixtureTypes mInputMixtureType;  /**< (--) trick_chkpnt_io(**) Type of the input fluid constituent partials */
        GunnsFluidUtils::MixtureTypes mOutputMixtureType; /**< (--) trick_chkpnt_io(**) Type of the output fluid constituent partials */
        PolyFluidConfigData*          mExternalConfig;    /**< (--) trick_chkpnt_io(**) Pointer to external network fluid config */
        FluidProperties::FluidType    mConvertToType;     /**< (--) trick_chkpnt_io(**) Fluid to convert extra constituents to */

        /// @brief Default constructs this Fluid Shadow configuration data.
        GunnsFluidShadowConfigData(
                const std::string&            name                 = "",
                GunnsNodeList*                nodes                = 0,
                const double                  maxConductivity      = 0.0,
                const double                  expansionScaleFactor = 0.0,
                const int                     controlTarget        = 0,
                GunnsFluidUtils::MixtureTypes inputMixtureType     = GunnsFluidUtils::MASS_FRACTION,
                GunnsFluidUtils::MixtureTypes outputMixtureType    = GunnsFluidUtils::MASS_FRACTION,
                PolyFluidConfigData*          externalConfig       = 0,
                FluidProperties::FluidType    convertToType        = FluidProperties::NO_FLUID);

        /// @brief Default destructs this Fluid Shadow configuration data.
        virtual ~GunnsFluidShadowConfigData();

        /// @brief Copy constructs this Fluid Shadow configuration data.
        GunnsFluidShadowConfigData(const GunnsFluidShadowConfigData& that);

    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidShadowConfigData& operator =(const GunnsFluidShadowConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Shadow Input Data
///
/// @details  This provides a data structure for the Fluid Shadow input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidShadowInputData : public GunnsFluidPotentialInputData
{
    public:
        /// @brief Default constructs this Fluid Shadow input data.
        GunnsFluidShadowInputData(const bool   malfBlockageFlag     = false,
                                  const double malfBlockageValue    = 0.0,
                                  const double sourcePressure       = 0.0);

        /// @brief Copy constructs this Fluid Shadow input data.
        GunnsFluidShadowInputData(const GunnsFluidShadowInputData& that);

        /// @brief Default destructs this Fluid Shadow input data.
        virtual ~GunnsFluidShadowInputData();

    protected:

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidShadowInputData& operator =(const GunnsFluidShadowInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Shadow class
///
/// @details  This class inherits GunnsFluidPotential, and is placed on a node to force its fluid
///           contents to match an externally supplied set of fluid properties.  This causes the
///           node to effectively shadow a fluid in an external network.  It also outputs the node's
///           fluid properties, so that it can be paired with another GunnsFluidShadow link,
///           allowing two nodes to switch between shadowing each other.  This is useful in a
///           master/slave relationship between two networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidShadow : public GunnsFluidPotential
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidShadow);

    public:
        /// @brief Default Constructor
        GunnsFluidShadow();

        /// @brief Default Destructor
        virtual ~GunnsFluidShadow();

        /// @brief Initializes the link, with input data
        void initialize(const GunnsFluidShadowConfigData& configData,
                        const GunnsFluidShadowInputData&  inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);

        /// @brief Special processing of data inputs to the model prior to the network update
        virtual void processInputs();

        /// @brief Special processing of data outputs from the model after the network update
        virtual void processOutputs();

        /// @brief Updates link for the frame
        virtual void updateState(const double dt);

        /// @brief Sets the input fluid properties.
        virtual void setInputs(const double  pressure,
                               const double  temperature,
                               const double* mixture);

        /// @brief Returns the output fluid properties.
        virtual void getOutputs(double& pressure,
                                double& temperature,
                                double* mixture) const;

    protected:
        int*                          mTransformMap;        /**< ** (--)  trick_chkpnt_io(**) Map to convert the external fluid to this config */
        int                           mShadowActiveControl; /**<    (--)                      Input control for making shadowing active */
        int                           mControlTarget;       /**<    (--)  trick_chkpnt_io(**) The value mShadowActiveControl must match to activate shadowing */
        bool                          mShadowIsActive;      /**<    (--)                      True when shadowing is currently active */
        bool                          mFirstPassFlag;       /**<    (--)                      Flag indicating first pass has completed */
        PolyFluidConfigData*          mExternalConfig;      /**<    (--)  trick_chkpnt_io(**) Pointer to external network fluid config */
        GunnsFluidUtils::MixtureTypes mInputMixtureType;    /**<    (--)  trick_chkpnt_io(**) Type of the input fluid constituent partials */
        GunnsFluidUtils::MixtureTypes mOutputMixtureType;   /**<    (--)  trick_chkpnt_io(**) Type of the output fluid constituent partials */
        double                        mInputPressure;       /**<    (kPa)                     Fluid input from sim bus */
        double                        mInputTemperature;    /**<    (K)                       Fluid temperature input from sim bus */
        double*                       mInputMixture;        /**<    (--)  trick_chkpnt_io(**) Generic constituent partials input from sim bus */
        double                        mOutputPressure;      /**<    (kPa)                     Fluid pressure output to sim bus */
        double                        mOutputTemperature;   /**<    (K)                       Fluid temperature output to sim bus */
        double*                       mOutputMixture;       /**<    (--)  trick_chkpnt_io(**) Generic constituent partials output to sim bus */

        /// @brief Validates the initialization of this Gunns Fluid Shadow
        void validate() const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Checks for valid implementation-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidShadow(const GunnsFluidShadow& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidShadow& operator =(const GunnsFluidShadow& that);
};

/// @}

#endif
