#ifndef GunnsFluidHotReactor_EXISTS
#define GunnsFluidHotReactor_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_HOT_REACTOR  Hot Chemical Reactor Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling a chemical reactor with heat.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- (
    (Reactions are performed serially. If a compound is involved in multiple reactions, the order
     in which the reactions are performed may affect the aggregate result.)
  )

 LIBRARY DEPENDENCY:
- ((GunnsFluidHotReactor.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-06))

@{
*/

#include "aspects/fluid/source/GunnsFluidReactor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hot Chemical Reactor Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the
///           GUNNS Hot Fluid Reactor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHotReactorConfigData : public GunnsFluidReactorConfigData
{
    public:
        double        mEfficiencyBias;         /**< (1/K)  trick_chkpnt_io(**) Bias for the dependency of efficiency on temperature. */
        double        mEfficiencyScaleFactor;  /**< (--)   trick_chkpnt_io(**) Scale factor for the dependency of efficiency on temperature. */
        /// @brief    Default constructs this Hot Chemical Reactor configuration data.
        GunnsFluidHotReactorConfigData(const std::string&              name                  = "",
                                       GunnsNodeList*                  nodes                 = 0,
                                       const double                    maxConductivity       = 0.0,
                                       const double                    expansionScaleFactor  = 0.0,
                                       const DefinedChemicalReactions* reactions             = 0,
                                       ChemicalReaction::Type*         reactionTypes         = 0,
                                       const int                       nReactions            = 0,
                                       const DefinedChemicalCompounds* compounds             = 0,
                                       ChemicalCompound::Type*         compoundTypes         = 0,
                                       const int                       nCompounds            = 0,
                                       const double                    thermalLength         = 0.0,
                                       const double                    thermalDiameter       = 0.0,
                                       const double                    surfaceRoughness      = 0.0,
                                       const double                    efficiencyBias        = 0.0,
                                       const double                    efficiencyScaleFactor = 0.0);
        /// @brief    Default destructs this Hot Chemical Reactor configuration data.
        virtual ~GunnsFluidHotReactorConfigData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHotReactorConfigData& operator =(const GunnsFluidHotReactorConfigData&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHotReactorConfigData(const GunnsFluidHotReactorConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hot Chemical Reactor Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the
///           GUNNS Hot Fluid Reactor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHotReactorInputData : public GunnsFluidReactorInputData
{
    public:
        /// @brief    Default constructs this Hot Chemical Reactor input data with arguments.
        GunnsFluidHotReactorInputData(const bool   malfBlockageFlag   = false,
                                      const double malfBlockageValue  = 0.0,
                                      double*      totalMasses        = 0,
                                      const double wallTemperature    = 0.0);
        /// @brief    Copy constructs this Hot Chemical Reactor input data.
        GunnsFluidHotReactorInputData(const GunnsFluidHotReactorInputData& that);
        /// @brief    Default destructs this Hot Chemical Reactor input data.
        ~GunnsFluidHotReactorInputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHotReactorInputData& operator=(const GunnsFluidHotReactorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hot Chemical Reactor
///
/// @details  Class for modeling a GUNNS Hot Fluid Reactor link model. A chemical reactor is a
///           conductive link masquerading as a source link that modifies the input fluid
///           constituents by consuming reactants and reagents and producing products. This reactor
///           may be heated or produce heat and the reaction efficiency is temprature dependent.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHotReactor : public GunnsFluidReactor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidHotReactor);
    public:
        /// @brief    Default constructs this Hot Chemical Reactor.
        GunnsFluidHotReactor();
        /// @brief    Default destructs this Hot Chemical Reactor.
        virtual ~GunnsFluidHotReactor();
        /// @brief    Initializes this Hot Chemical Reactor with configuration and input data.
        void initialize(      GunnsFluidHotReactorConfigData& configData,
                        const GunnsFluidHotReactorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&         networkLinks,
                        const int                             port0,
                        const int                             port1);
    protected:
        double        mEfficiencyBias;         /**< (1/K)  trick_chkpnt_io(**) Bias for the dependency of efficiency on temperature. */
        double        mEfficiencyScaleFactor;  /**< (--)   trick_chkpnt_io(**) Scale factor for the dependency of efficiency on temperature. */
        /// @brief    Validates the initialization inputs of this Hot Chemical Reactor.
        void validate(const GunnsFluidHotReactorConfigData& configData,
                      const GunnsFluidHotReactorInputData&  inputData);
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Computes the reaction efficiency.
        virtual double computeEfficiency(const int i);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHotReactor(const GunnsFluidHotReactor&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHotReactor& operator=(const GunnsFluidHotReactor&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data (unused).
/// @param[in]      inputData   (--)  Input data (unused).
///
/// @details  Validates the initialization of this GUNNS Fluid Hot Reactor link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidHotReactor::validate(const GunnsFluidHotReactorConfigData& configData __attribute__((unused)),
                                           const GunnsFluidHotReactorInputData&  inputData  __attribute__((unused)))
{
    // nothing to do
}

#endif
