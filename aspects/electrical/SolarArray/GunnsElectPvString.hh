#ifndef GunnsElectPvString_EXISTS
#define GunnsElectPvString_EXISTS

/**
@file
@brief    GUNNS Electrical Photovoltaic String Model declarations

@defgroup  GUNNS_ELECTRICAL_PHOTOVOLTAIC_STRING    Photovoltaic String Model
@ingroup   GUNNS_ELECTRICAL_PHOTOVOLTAIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Photovoltaic String Model.  Refer to the GunnsElectPvString class details
   below.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectPvString.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-10) (Initial))

@{
*/

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Cell Configuration Data.
///
/// @details  This provides a data structure for the Photovoltaic Cell config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvCellConfigData
{
    public:
        double mSurfaceArea;             /**< (m2)  trick_chkpnt_io(**) Surface area of one side. */
        double mEfficiency;              /**< (--)  trick_chkpnt_io(**) Photovoltaic efficiency (0-1). */
        double mSeriesResistance;        /**< (ohm) trick_chkpnt_io(**) Series resistance. */
        double mShuntResistance;         /**< (ohm) trick_chkpnt_io(**) Shunt resistance. */
        double mOpenCircuitVoltage;      /**< (V)   trick_chkpnt_io(**) Open-circuit voltage. */
        double mRefTemperature;          /**< (K)   trick_chkpnt_io(**) Reference temperature for temperature effects. */
        double mTemperatureVoltageCoeff; /**< (1/K) trick_chkpnt_io(**) Coefficient for temperature effect on open-circuit voltage. */
        double mTemperatureCurrentCoeff; /**< (1/K) trick_chkpnt_io(**) Coefficient for temperature effect on source current. */
        /// @brief Default constructs this Photovoltaic Cell config data.
        GunnsElectPvCellConfigData(const double cellSurfaceArea             = 0.0,
                                   const double cellEfficiency              = 0.0,
                                   const double cellSeriesResistance        = 0.0,
                                   const double cellShuntResistance         = 0.0,
                                   const double cellOpenCircuitVoltage      = 0.0,
                                   const double cellRefTemperature          = 0.0,
                                   const double cellTemperatureVoltageCoeff = 0.0,
                                   const double cellTemperatureCurrentCoeff = 0.0);
        /// @brief Default destructs this Photovoltaic Cell config data.
        virtual ~GunnsElectPvCellConfigData();
        /// @brief Assignment operator for this Photovoltaic Cell config data.
        GunnsElectPvCellConfigData& operator =(const GunnsElectPvCellConfigData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvCellConfigData(const GunnsElectPvCellConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Cell Equivalent Circuit Properties.
///
/// @details  This provides a data structure for the properties of an equivalent circuit of a PV
///           cell consisting of a current source, shunt diode and resistor, and series diode and
///           resistor.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvCellEquivProps
{
    public:
        double mSourceCurrent;     /**< (amp) Photovoltaic source current before shunt & series losses. */
        double mShuntVoltageDrop;  /**< (V)   Shunt voltage drop in reverse bias. */
        double mShuntResistance;   /**< (ohm) Shunt resistance in reverse bias. */
        double mSeriesVoltageDrop; /**< (V)   Series voltage drop in forward bias. */
        double mSeriesResistance;  /**< (ohm) Series resistance in forward bias. */
        /// @brief Default constructs this Photovoltaic Cell Equivalent Circuit Properties.
        GunnsElectPvCellEquivProps();
        /// @brief Default destructs this Photovoltaic Cell Equivalent Circuit Properties.
        virtual ~GunnsElectPvCellEquivProps();
        /// @brief Zeroes the attributes of this Photovoltaic Cell Equivalent Circuit Properties.
        void clear();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvCellEquivProps(const GunnsElectPvCellEquivProps& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvCellEquivProps& operator =(const GunnsElectPvCellEquivProps&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic String Model Load State.
///
/// @details  This provides a data structure for the properties of an electrical load.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvLoadState
{
    public:
        double mConductance; /**< (1/ohm) Electrical conductance. */
        double mCurrent;     /**< (amp)   Electrical current. */
        double mPower;       /**< (W)     Electrical power. */
        double mVoltage;     /**< (V)     Electrical voltage. */
        /// @brief Default constructs this Photovoltaic String Model Load State.
        GunnsElectPvLoadState();
        /// @brief Default destructs this Photovoltaic String Model Load State.
        virtual ~GunnsElectPvLoadState();
        /// @brief Assignment operator for this Photovoltaic String Model Load State.
        GunnsElectPvLoadState& operator =(const GunnsElectPvLoadState& that);
        /// @brief Zeroes the attributes of this Photovoltaic String Model Load State.
        void clear();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvLoadState(const GunnsElectPvLoadState& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic String Model Configuration Data.
///
/// @details  This provides a data structure for the Photovoltaic String Model config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvStringConfigData
{
    public:
        double                     mBlockingDiodeVoltageDrop; /**< (V)  trick_chkpnt_io(**) Voltage drop across the diode at end of string. */
        double                     mBypassDiodeVoltageDrop;   /**< (V)  trick_chkpnt_io(**) Voltage drop across each bypass diode. */
        unsigned int               mBypassDiodeInterval;      /**< (--) trick_chkpnt_io(**) Number of cells per bypass diode. */
        unsigned int               mNumCells;                 /**< (--) trick_chkpnt_io(**) Number of cells in this string. */
        GunnsElectPvCellConfigData mCellConfig;               /**< (--) trick_chkpnt_io(**) Config data for the cells. */
        /// @brief Default constructs this Photovoltaic String Model config data.
        GunnsElectPvStringConfigData(const double       stringBlockingDiodeVoltageDrop = 0.0,
                                     const double       stringBypassDiodeVoltageDrop   = 0.0,
                                     const unsigned int stringBypassDiodeInterval      = 0,
                                     const unsigned int stringNumCells                 = 0,
                                     const double       cellSurfaceArea                = 0.0,
                                     const double       cellEfficiency                 = 0.0,
                                     const double       cellSeriesResistance           = 0.0,
                                     const double       cellShuntResistance            = 0.0,
                                     const double       cellOpenCircuitVoltage         = 0.0,
                                     const double       cellRefTemperature             = 0.0,
                                     const double       cellTemperatureVoltageCoeff    = 0.0,
                                     const double       cellTemperatureCurrentCoeff    = 0.0);
        /// @brief Default destructs this Photovoltaic String Model config data.
        virtual ~GunnsElectPvStringConfigData();
        /// @brief Assignment operator for this Photovoltaic String Model config data.
        GunnsElectPvStringConfigData& operator =(const GunnsElectPvStringConfigData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvStringConfigData(const GunnsElectPvStringConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic String Model Input Data.
///
/// @details  This provides a data structure for the Photovoltaic String Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvStringInputData
{
    public:
        double mPhotoFlux;                /**< (W/m2) Photo power flux incident on the string. */
        double mSourceExposedFraction;    /**< (--)   Surface area fraction exposed to light source (0-1). */
        double mTemperature;              /**< (K)    Temperature of the string. */
        bool   mMalfPhotoFluxFlag;        /**< (--)   Photo power flux malfunction activation flag. */
        double mMalfPhotoFluxMagnitude;   /**< (W/m2) Photo power flux malfunction magnitude. */
        double mMalfPhotoFluxDuration;    /**< (s)    Photo power flux malfunction total duration. */
        double mMalfPhotoFluxRampTime;    /**< (s)    Photo power flux malfunction ramp up/down duration. */
        bool   mMalfExposedFractionFlag;  /**< (--)   Source exposed fraction malfunction activation flag. */
        double mMalfExposedFractionValue; /**< (--)   Source exposed fraction malfunction value. */
        bool   mMalfTemperatureFlag;      /**< (--)   Temperature malfunction activation flag. */
        double mMalfTemperatureValue;     /**< (K)    Temperature malfunction value. */
        /// @brief Default constructs this Photovoltaic String Model input data.
        GunnsElectPvStringInputData(const double stringPhotoFlux             = 0.0,
                                    const double stringSourceExposedFraction = 0.0,
                                    const double stringTemperature           = 0.0);
        /// @brief Default destructs this Photovoltaic String Model input data.
        virtual ~GunnsElectPvStringInputData();
        /// @brief Applies malfunctions to override this Photovoltaic String Model input data.
        void applyOverrides(const double dt);
        /// @brief Sets and clears the light level malfunction controls.
        void setMalfPhotoFlux(const bool flag = false, const double magnitude = 0.0,
                              const double duration = 0.0, const double ramptime = 0.0);
        /// @brief Sets and clears the source exposed fraction malfunction controls.
        void setMalfExposedFraction(const bool flag = false, const double value = 0.0);
        /// @brief Sets and clears the temperature malfunction controls.
        void setMalfTemperature(const bool flag = false, const double value = 0.0);

    protected:
        double mPhotoFluxElapsedTime;     /**< (s)    Elapsed time of the photo power flux malfunction. */
        double mPhotoFluxStartMagnitude;  /**< (W/m2) Starting magnitude of the photo power flux malfunction. */
        /// @brief Computes and returns the sinusoid ramped photo power flux.
        double rampPhotoFlux(const double time, const double outValue);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvStringInputData(const GunnsElectPvStringInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvStringInputData& operator =(const GunnsElectPvStringInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic String Model.
///
/// @details  This models a string of photovoltaic cells in series as an equivalent circuit.  This
///           models the effects of light source power, facing & shading, temperature, loading, and
///           cell blockage and overall degradation malfunctions.  The string's bypass and blocking
///           diodes are also modeled.
///
///           The equivalent circuit models the states of:
///           - A source current produced by the photovoltaic conversion.
///           - Cell parallel shunt resistances and diodes.
///           - Cell series resistances, bypass and blocking diodes.
///           - A terminal, which is the output to the load.
///
///           This is a linear model, where the diodes are assumed to be ideal & linear, instantly
///           switch between full forward & reverse bias, and their intermediate bias zone is not
///           modeled.  This simplification allows for easy solution of the string in circuit with
///           the load, and stable operation when embedded in a GUNNS link and network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvString
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectPvString);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool                       mMalfCellGroupFlag;   /**<    (--)                       Cell group failure malfunction activation flag. */
        int                        mMalfCellGroupValue;  /**<    (--)                       Cell group failure malfunction number of failed groups. */
        bool                       mMalfDegradeFlag;     /**<    (--)                       Cell group power degrade malfunction activation flag. */
        double                     mMalfDegradeValue;    /**<    (--)                       Cell group power degrade malfunction value (0-1). */
        /// @}
        static const double        mIdealDiodeFactor;    /**< ** (--)   trick_chkpnt_io(**) Ideal diode conductance multiplier. */
        /// @brief Default constructor for this Photovoltaic String Utility.
        GunnsElectPvString();
        /// @brief Normal constructor for this Photovoltaic String Utility.
        GunnsElectPvString(const GunnsElectPvStringConfigData* configData,
                           const GunnsElectPvStringInputData*  inputData);
        /// @brief Destructs this Photovoltaic String Utility.
        virtual ~GunnsElectPvString();
        /// @brief Iniitalizes this Photovoltaic String Utility.
        void   initialize(const std::string& name);
        /// @brief Updates this Photovoltaic String Utility state.
        void   update();
        /// @brief Loads the string at the given power output.
        void   loadAtPower(const double power, const bool shortSide);
        /// @brief Loads the string at the given terminal voltage.
        void   loadAtVoltage(const double v1);
        /// @brief Loads the string at its Maximum Power Point.
        void   loadAtMpp();
        /// @brief Loads the string with the given conductive load.
        void   loadAtConductance(const double g);
        /// @brief Sets the string shunted flag;
        void   setShunted(const bool flag);
        /// @brief Gets the string shunted flag;
        bool   isShunted() const;
        /// @brief Gets the open-circuit terminal voltage of this string.
        double getOpenCircuitVoltage() const;
        /// @brief Gets the short-circuit terminal current of this string.
        double getShortCircuitCurrent() const;
        /// @brief Returns a reference to this string's equivalent circuit properties.
        const GunnsElectPvCellEquivProps& getEqProps() const;
        /// @brief Gets the Maximum Power Point state of this string.
        const GunnsElectPvLoadState& getMpp() const;
        /// @brief Gets the Terminal load state of this string.
        const GunnsElectPvLoadState& getTerminal() const;
        /// @brief Returns current output of the string at the given terminal voltage.
        double predictCurrentAtVoltage(const double voltage) const;

    protected:
        std::string                mName;                /**<    (--)   trick_chkpnt_io(**) Instance name for H&S messages. */
        const GunnsElectPvStringConfigData* mConfig;     /**< ** (--)   trick_chkpnt_io(**) Pointer to common string config data. */
        const GunnsElectPvStringInputData*  mInput;      /**< ** (W/m2) trick_chkpnt_io(**) Pointer to photo power flux incident on the section. */
        double                     mShortCircuitCurrent; /**<    (amp)  trick_chkpnt_io(**) Short-circuit current at terminal node (max load). */
        double                     mOpenCircuitVoltage;  /**<    (V)    trick_chkpnt_io(**) Open-circuit voltage at terminal node (no load). */
        GunnsElectPvCellEquivProps mEqProps;             /**<    (--)   trick_chkpnt_io(**) Properties of the string equivalent circuit. */
        GunnsElectPvLoadState      mMpp;                 /**<    (--)   trick_chkpnt_io(**) Maximum Power Point load state. */
        GunnsElectPvLoadState      mTerminal;            /**<    (--)   trick_chkpnt_io(**) Terminal output load state. */
        unsigned int               mNumBypassedGroups;   /**<    (--)   trick_chkpnt_io(**) Number of bypassed cell groups. */
        unsigned int               mNumActiveCells;      /**<    (--)   trick_chkpnt_io(**) Number of cells that are not bypassed. */
        bool                       mShunted;             /**<    (--)   trick_chkpnt_io(**) String is currently shunted. */
        /// @brief Validates the initialization of this Gunns Photovoltaic String Utility.
        void validate() const;
        /// @brief Bypasses cell groups based on shading and malfunction.
        void updateBypassedGroups();
        /// @brief Computes the Maximum Power Point parameters.
        void updateMpp();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvString(const GunnsElectPvString& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvString& operator =(const GunnsElectPvString& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag      (--)   Malfunction activation flag.
/// @param[in] magnitude (W/m2) Malfunction magnitude.
/// @param[in] duration  (s)    Malfunction total duration.
/// @param[in] ramptime  (s)    Malfunction ramp in & out duration.
///
/// @details  Sets the photo power flux malfunction controls to the given values.  Calling this with
///           default arguments deactivates the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvStringInputData::setMalfPhotoFlux(const bool flag, const double magnitude,
                                                          const double duration, const double ramptime)
{
    mMalfPhotoFluxFlag      = flag;
    mMalfPhotoFluxMagnitude = magnitude;
    mMalfPhotoFluxDuration  = duration;
    mMalfPhotoFluxRampTime  = ramptime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag.
/// @param[in] value (--) Malfunction value (0-1).
///
/// @details  Sets the source exposed fraction malfunction controls to the given values.  Calling
///           this with default arguments deactivates the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvStringInputData::setMalfExposedFraction(const bool flag, const double value)
{
    mMalfExposedFractionFlag  = flag;
    mMalfExposedFractionValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag.
/// @param[in] value (K)  Malfunction value.
///
/// @details  Sets the temperature malfunction controls to the given values.  Calling this with
///           default arguments deactivates the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvStringInputData::setMalfTemperature(const bool flag, const double value)
{
    mMalfTemperatureFlag  = flag;
    mMalfTemperatureValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This loads the string at its Maximum Power Point, and updates the corresponding
///           terminal output state.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvString::loadAtMpp()
{
    mTerminal = mMpp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flag  (--)  True if the string is shunted, false otherwise.
///
/// @details  Sets the mShunted flag to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvString::setShunted(const bool flag)
{
    mShunted = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if the string is shunted, false otherwise.
///
/// @details  Returns the mShunted attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectPvString::isShunted() const
{
    return mShunted;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (V)  Open-circuit voltage at terminal node (no load) of this string.
///
/// @details  Returns the mOpenCircuitVoltage attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvString::getOpenCircuitVoltage() const
{
    return mOpenCircuitVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (amp)  Short-circuit current at terminal node (max load) of this string.
///
/// @details  Returns the mShortCircuitCurrent attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvString::getShortCircuitCurrent() const
{
    return mShortCircuitCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsElectPvCellEquivProps&  (--)  A reference to the equivalent circuit properties.
///
/// @details  Returns a const reference to the mEqProps attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const GunnsElectPvCellEquivProps& GunnsElectPvString::getEqProps() const
{
    return mEqProps;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsElectPvLoadState& (--) A reference to the Maximum Power Point state object.
///
/// @details  Returns a const reference to the mMpp attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const GunnsElectPvLoadState& GunnsElectPvString::getMpp() const
{
    return mMpp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsElectPvLoadState& (--) A reference to the terminal load state object.
///
/// @details  Returns a const reference to the mTerminal attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const GunnsElectPvLoadState& GunnsElectPvString::getTerminal() const
{
    return mTerminal;
}

#endif
