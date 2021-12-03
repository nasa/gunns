#ifndef GunnsFluidSimpleH2Redox_EXISTS
#define GunnsFluidSimpleH2Redox_EXISTS

/**
@file
@brief Simple H2 Redox link declarations.

@defgroup  TSM_GUNNS_FLUID_SOURCE_SIMPLE_H2_REDOX  Simple H2 Redox Reaction Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling an H2 redox reaction device.)

REQUIREMENTS:
- ()

REFERENCE:
- (("The HESTIA Project: Solid Oxide Fuel Cell Model Validation", Jason Harvey, July 31, 2016.))

ASSUMPTIONS AND LIMITATIONS:
- (
   (H2, O2 and H2O are in gaseous phase and are ideal gas.)
   (Effects of temperature are not modeled.)
   (Energy is not conserved, because of the lack of reaction heat.)
   (Output voltage is constant, not a function of temperature or load.)
   (Reaction efficiency is constant.)
   (Reaction rate is not dependent on temperature or fluid concentration.)
   (Absorption of contaminants and their effects on efficiency are not modeled.)
   (Reaction trips off if inlet reactants pressure < 1 Pa.)
  )

 LIBRARY DEPENDENCY:
- ((GunnsFluidSimpleH2Redox.o))

 PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (initial) (2015-02))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Simple H2 Redox Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Simple H2 Redox
///           link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSimpleH2RedoxConfigData : public GunnsFluidLinkConfigData
{
    public:
        int    mNumCells;          /**< (1)        trick_chkpnt_io(**) Number of reaction cells in the stack. */
        double mCellVoltageLoaded; /**< (V)        trick_chkpnt_io(**) Nominal cell voltage under load. */
        double mCellH2ReactRate;   /**< (kg/s/amp) trick_chkpnt_io(**) Cell H2 reaction mass rate per amp. */
        double mMaxEfficiency;     /**< (1)        trick_chkpnt_io(**) Maximum efficiency (0-1) of the reaction. */
        /// @brief    Default constructs this Simple H2 Redox configuration data.
        GunnsFluidSimpleH2RedoxConfigData(const std::string& name               = "",
                                          GunnsNodeList*     nodes              = 0,
                                          const int          numCells           = 0,
                                          const double       cellVoltageLoaded  = 0.0,
                                          const double       cellH2ReactRate    = 0.0,
                                          const double       maxEfficiency      = 0.0);
        /// @brief    Default destructs this Simple H2 Redox configuration data.
        virtual ~GunnsFluidSimpleH2RedoxConfigData();

    private:
        /// @brief    Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSimpleH2RedoxConfigData& operator =(const GunnsFluidSimpleH2RedoxConfigData&);
        /// @brief    Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSimpleH2RedoxConfigData(const GunnsFluidSimpleH2RedoxConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Simple H2 Redox Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Simple H2 Redox
///           link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSimpleH2RedoxInputData : public GunnsFluidLinkInputData
{
    public:
        double mCurrent;         /**< (amp) trick_chkpnt_io(**) Initial electrical stack current. */
        bool   mTrippedOff;      /**< (1)   trick_chkpnt_io(**) Initial tripped off flag. */
        /// @brief    Default constructs this Simple H2 Redox input data with arguments.
        GunnsFluidSimpleH2RedoxInputData(const bool   malfBlockageFlag  = false,
                                         const double malfBlockageValue = 0.0,
                                         const double current           = 0.0,
                                         const bool   trippedOff        = false);
        /// @brief    Default destructs this Simple H2 Redox input data.
        ~GunnsFluidSimpleH2RedoxInputData();

    private:
        /// @brief    Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSimpleH2RedoxInputData& operator=(const GunnsFluidSimpleH2RedoxInputData&);
        /// @brief    Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSimpleH2RedoxInputData(const GunnsFluidSimpleH2RedoxInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Simple H2 Redox
///
/// @details  Class for a very simple model of an H2 redox device, such as a fuel cell or
///           electrolyzer.  The reaction is reversible: reaction rate is directly proportional to
///           input electrical current, with positive current giving a forward H2 reduction reaction
///           direction (fuel cell), and negative current giving a reverse direction (electrolysis).
///
///           This link's blockage malfunction degrades the efficiency.  The reaction rate can be
///           degraded by blocking the electrical aspect -- this lowers the input current.
///
///           Note this link has many significant simplifications that may limit its usefulness.
///           These are addressed in the reference document.  Suggestions for improving the model,
///           possibly in another, higher-fidelity H2 redox link:
///           - Model unloaded voltage as a function of temperature.
///           - Model loaded voltage sag as a non-linear function of current.
///           - Model reaction efficiency as function of power load.
///           - Model non-zero unloaded reaction rate.
///           - Allow mixed fluid phases, although this might depend on other GUNNS upgrades.
///           - Model absorption/desorption of contaminants and their effects on efficiency.  This
///             might be best achieved with a separate absorber link that communicates the types and
///             amounts of absorbed contaminants to the H2 redox link.
///           - Improve effects of insufficient reactants: reduce output voltage, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSimpleH2Redox : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSimpleH2Redox);
    public:
        /// @brief    Default constructs this Simple H2 Redox.
        GunnsFluidSimpleH2Redox();
        /// @brief    Default destructs this Simple H2 Redox.
        virtual ~GunnsFluidSimpleH2Redox();
        /// @brief    Initializes this Simple H2 Redox with configuration and input data.
        void         initialize(      GunnsFluidSimpleH2RedoxConfigData& configData,
                                const GunnsFluidSimpleH2RedoxInputData&  inputData,
                                std::vector<GunnsBasicLink*>&            networkLinks,
                                const int                                port0,
                                const int                                port1);
        /// @brief    Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Step method for updating the link.
        virtual void step(const double dt);
        /// @brief    Computes the flows through the link.
        virtual void computeFlows(const double dt);
        /// @brief    Transport the flows through the link.
        virtual void transportFlows(const double dt);
        /// @brief    Sets the electrical current driving the reaction rate of this Simple H2 Redox.
        void         setCurrent(const double current);
        /// @brief    Gets the output stack voltage of this Simple H2 Redox.
        double       getOutputStackVoltage() const;
        /// @brief    Gets the output reaction heat of this Simple H2 Redox.
        double       getOutputHeat() const;
        /// @brief    Gets the mass rate of H2 in the Simple H2 Redox.
        double       getH2MassRate() const;
        /// @brief    Gets the mass rate of O2 in the Simple H2 Redox.
        double       getO2MassRate() const;
        /// @brief    Gets the mass rate of H2O in the Simple H2 Redox.
        double       getH2OMassRate() const;

    protected:
        int        mNumCells;           /**< (1)        trick_chkpnt_io(**) Number of reaction cells in the stack. */
        double     mCellVoltageLoaded;  /**< (V)        trick_chkpnt_io(**) Nominal cell voltage under load. */
        double     mCellH2ReactRate;    /**< (kg/s/amp) trick_chkpnt_io(**) Cell H2 reaction mass rate per amp. */
        double     mMaxEfficiency;      /**< (1)        trick_chkpnt_io(**) Maximum efficiency (0-1) of the reaction. */
        double     mCurrent;            /**< (amp)                          Electrical stack current driving the reaction. */
        bool       mTrippedOff;         /**< (1)                            Reaction is disabled. */
        double     mOutputStackVoltage; /**< (V)                            Output voltage of the stack. */
        double     mOutputHeat;         /**< (W)                            Output heat of the reaction. */
        double     mEfficiency;         /**< (1)        trick_chkpnt_io(**) Actual efficiency. */
        double     mH2MassRate;         /**< (kg/s)     trick_chkpnt_io(**) Produced mass rate of H2 from the reaction. */
        double     mO2MassRate;         /**< (kg/s)     trick_chkpnt_io(**) Produced mass rate of O2 from the reaction. */
        double     mH2OMassRate;        /**< (kg/s)     trick_chkpnt_io(**) Produced mass rate of H2O from the reaction. */
        double     mH2MoleRate;         /**< (kg*mol/s) trick_chkpnt_io(**) Produced molar rate of H2 from the reaction. */
        double     mO2MoleRate;         /**< (kg*mol/s) trick_chkpnt_io(**) Produced molar rate of O2 from the reaction. */
        double     mH2OMoleRate;        /**< (kg*mol/s) trick_chkpnt_io(**) Produced molar rate of H2O from the reaction. */
        PolyFluid* mH2Fluid;            /**< (1)        trick_chkpnt_io(**) Pointer to an internal fluid for the fuel constituent (H2). */
        PolyFluid* mO2Fluid;            /**< (1)        trick_chkpnt_io(**) Pointer to an internal fluid for the oxidizer constituent (O2). */
        PolyFluid* mH2OFluid;           /**< (1)        trick_chkpnt_io(**) Pointer to an internal fluid for the water constituent (H2O). */
        static const double mReactionHeat; /**< (J/kg*mol) trick_chkpnt_io(**) Heat output to environment per mass of H2 reacted. */
        /// @brief    Validates the initialization inputs of this Simple H2 Redox.
        void         validate(const GunnsFluidSimpleH2RedoxConfigData& configData,
                              const GunnsFluidSimpleH2RedoxInputData&  inputData);
        /// @brief    Builds the source vector terms of the link contribution to the network.
        virtual void buildSource();
        /// @brief    Zeroes all the fluid constituent reaction flow rates.
        void         zeroReactionRates();
        /// @brief Checks for valid implementation-specific port node assignment
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};
        /// @brief    Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSimpleH2Redox(const GunnsFluidSimpleH2Redox&);
        /// @brief    Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSimpleH2Redox& operator=(const GunnsFluidSimpleH2Redox&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  current  (amp)  Electrical current to the drive the reaction rate.
///
/// @details  Sets the electrical current to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSimpleH2Redox::setCurrent(const double current)
{
    mCurrent = current;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (V)  Voltage across the redox reactor stack.
///
/// @details  Returns the voltage across the redox reactor stack.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSimpleH2Redox::getOutputStackVoltage() const
{
    return mOutputStackVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (W)  Heat output from the reaction.
///
/// @details  Returns the output heat from the reaction.  Positive values are exothermic fuel-cell
///           reactions, and negative values are endothermic electrolysis reactions.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSimpleH2Redox::getOutputHeat() const
{
    return mOutputHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (kg/s)  H2 mass rate across the H2 redox reaction
///
/// @details  Returns the H2 Mass Rate of the reaction
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSimpleH2Redox::getH2MassRate() const
{
    return mH2MassRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (kg/s)  O2 mass rate across the H2 redox reaction
///
/// @details  Returns the O2 Mass Rate of the reaction
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSimpleH2Redox::getO2MassRate() const
{
    return mO2MassRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (kg/s)  H2O mass rate across the H2 redox reaction
///
/// @details  Returns the H2O Mass Rate of the reaction
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSimpleH2Redox::getH2OMassRate() const
{
    return mH2OMassRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Zeroes all the reaction constituent mass and molar flow rate terms.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSimpleH2Redox::zeroReactionRates()
{
    mH2MassRate   = 0.0;
    mO2MassRate   = 0.0;
    mH2OMassRate  = 0.0;
    mH2MoleRate   = 0.0;
    mO2MoleRate   = 0.0;
    mH2OMoleRate  = 0.0;
}

#endif
