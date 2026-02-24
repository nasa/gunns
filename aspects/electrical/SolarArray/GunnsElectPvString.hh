#ifndef GunnsElectPvString_EXISTS
#define GunnsElectPvString_EXISTS

/**
@file     GunnsElectPvString.hh
@brief    GUNNS Electrical Photovoltaic String Model declarations

@defgroup  GUNNS_ELECTRICAL_PHOTOVOLTAIC_STRING    Photovoltaic String Model
@ingroup   GUNNS_ELECTRICAL_PHOTOVOLTAIC

@copyright Copyright 2024 United States Government as represented by the Administrator of the
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
        double mSurfaceArea;             /**< (m2)   trick_chkpnt_io(**) Surface area of one side. */
        double mEfficiency;              /**< (1)    trick_chkpnt_io(**) Photovoltaic efficiency (0-1). */
        double mSeriesResistance;        /**< (ohm)  trick_chkpnt_io(**) Series resistance. */
        double mShuntResistance;         /**< (ohm)  trick_chkpnt_io(**) Shunt resistance. */
        double mOpenCircuitVoltage;      /**< (V)    trick_chkpnt_io(**) Open-circuit voltage. */
        double mRefTemperature;          /**< (K)    trick_chkpnt_io(**) Reference temperature for temperature effects. */
        double mTemperatureVoltageCoeff; /**< (1/K)  trick_chkpnt_io(**) Coefficient for temperature effect on open-circuit voltage. */
        double mTemperatureCurrentCoeff; /**< (1/K)  trick_chkpnt_io(**) Coefficient for temperature effect on source current. */
        double mShortCircuitCurrent;     /**< (amp)  trick_chkpnt_io(**) Short-circuit current. */
        double mMppVoltage;              /**< (V)    trick_chkpnt_io(**) Voltage at the maximum power point. */
        double mMppCurrent;              /**< (amp)  trick_chkpnt_io(**) Current at the maximum power point. */
        double mPhotoFlux;               /**< (W/m2) trick_chkpnt_io(**) Absorbed photo power flux incident on the string. */
        double mIdeality;                /**< (1)    trick_chkpnt_io(**) Diode ideality constant. */
        /// @brief Default constructs this Photovoltaic Cell config data.
        GunnsElectPvCellConfigData();
        /// @brief Constructs this Photovoltaic Cell config data for an original version cell model.
        GunnsElectPvCellConfigData(const double cellSurfaceArea,
                                   const double cellEfficiency,
                                   const double cellSeriesResistance,
                                   const double cellShuntResistance,
                                   const double cellOpenCircuitVoltage,
                                   const double cellRefTemperature,
                                   const double cellTemperatureVoltageCoeff,
                                   const double cellTemperatureCurrentCoeff);
        /// @brief Constructs this Photovoltaic Cell config data for a version 2 cell model.
        GunnsElectPvCellConfigData(const double voc,
                                   const double isc,
                                   const double vmp,
                                   const double imp,
                                   const double photoFlux,
                                   const double temperature,
                                   const double coeffDVocDT,
                                   const double coeffDIscDT,
                                   const double ideality,
                                   const double cellArea);
        /// @brief Default destructs this Photovoltaic Cell config data.
        virtual ~GunnsElectPvCellConfigData();
        /// @brief Assignment operator for this Photovoltaic Cell config data.
        GunnsElectPvCellConfigData& operator =(const GunnsElectPvCellConfigData& that);
        /// @brief Returns whether this is for a version 2 Photovoltaic Cell model.
        bool isVersion2() const;

    private:
        bool mIsVersion2; /**< (1) True if this is for a version 2 cell model. */
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvCellConfigData(const GunnsElectPvCellConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Cell Equivalent Circuit Model.
///
/// @details  This is a single-diode equivalent circuit model of a PV cell for use by the
///           Photovoltaic Cell model.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvCellEquivCircuit
{
    public:
        double mIL;          /**< (amp)  trick_chkpnt_io(**) Photovoltaic source current before shunt & series losses. */
        double mRsh;         /**< (ohm)  trick_chkpnt_io(**) Shunt resistance in reverse bias. */
        double mRs;          /**< (ohm)  trick_chkpnt_io(**) Series resistance in forward bias. */
        double mNVt;         /**< (V)    trick_chkpnt_io(**) Product of diode ideality and thermal voltage. */
        double mI0;          /**< (amp)  trick_chkpnt_io(**) Characteristic diode reverse saturation current. */
        double mFillFactor;  /**< (1)    trick_chkpnt_io(**) Fill factor. */
        double mEfficiency;  /**< (1)    trick_chkpnt_io(**) Maximum efficiency. */
        double mVoc;         /**< (V)    trick_chkpnt_io(**) Open-circuit voltage. */
        double mIsc;         /**< (amp)  trick_chkpnt_io(**) Short-circuit current. */
        double mVmp;         /**< (V)    trick_chkpnt_io(**) Voltage at the maximum power point. */
        double mImp;         /**< (amp)  trick_chkpnt_io(**) Current at the maximum power point. */
        double mPhotoFlux;   /**< (W/m2) trick_chkpnt_io(**) Absorbed photo power flux incident on the string. */
        double mIdeality;    /**< (1)    trick_chkpnt_io(**) Diode ideality constant. */
        double mTemperature; /**< (K)    trick_chkpnt_io(**) Temperature. */
        double mCoeffDVocDT; /**< (1/K)  trick_chkpnt_io(**) Coefficient for temperature effect on open-circuit voltage. */
        double mCoeffDIscDT; /**< (1/K)  trick_chkpnt_io(**) Coefficient for temperature effect on source current. */
        double mSurfaceArea; /**< (m2)   trick_chkpnt_io(**) Surface area of one side. */
        static const double mBoltzmannOverCharge; /**< (V/K) trick_chkpnt_io(**) Ratio of Boltzmann constant over electron charge constant. */
        /// @brief Default constructs this Photovoltaic Cell Equivalent Circuit Properties.
        GunnsElectPvCellEquivCircuit();
        /// @brief Default destructs this Photovoltaic Cell Equivalent Circuit Properties.
        virtual ~GunnsElectPvCellEquivCircuit();
        /// @brief Assignment operator for this Photovoltaic Cell Equivalent Circuit Properties.
        GunnsElectPvCellEquivCircuit& operator =(const GunnsElectPvCellEquivCircuit& that);
        /// @brief Zeroes the attributes of this Photovoltaic Cell Equivalent Circuit Properties.
        void clear();
        /// @brief Initializes this Photovoltaic Cell Equivalent Circuit Properties.
        virtual void initialize(const GunnsElectPvCellConfigData* configData, const std::string& name);
        /// @brief Updates this Photovoltaic Cell Equivalent Circuit Properties.
        virtual void update(const GunnsElectPvCellEquivCircuit* refCell, const double temperature,
                            const double photoFlux, const double degradation = 0.0);
        /// @brief Computes remaining equivalent circuit properties after initialization or update.
        virtual void derive();
        /// @brief Returns the cell current at the given voltage.
        virtual double computeCurrent(const double voltage) const;
        /// @brief Returns the cell voltage at the given current.
        virtual double computeVoltage(const double current) const;

    protected:
        std::string  mName; /**< (1) trick_chkpnt_io(**) Instance name for H&S messages. */

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvCellEquivCircuit(const GunnsElectPvCellEquivCircuit& that);
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
        double                     mBlockingDiodeVoltageDrop; /**< (V) trick_chkpnt_io(**) Voltage drop across the diode at end of string. */
        double                     mBypassDiodeVoltageDrop;   /**< (V) trick_chkpnt_io(**) Voltage drop across each bypass diode. */
        unsigned int               mBypassDiodeInterval;      /**< (1) trick_chkpnt_io(**) Number of cells per bypass diode. */
        unsigned int               mNumCells;                 /**< (1) trick_chkpnt_io(**) Number of cells in this string. */
        GunnsElectPvCellConfigData mCellConfig;               /**< (1) trick_chkpnt_io(**) Config data for the cells. */
        /// @brief Default constructs this Photovoltaic String Model config data.
        GunnsElectPvStringConfigData();
        /// @brief Constructs this Photovoltaic String Model config data for an original string model version.
        GunnsElectPvStringConfigData(const double       stringBlockingDiodeVoltageDrop,
                                     const double       stringBypassDiodeVoltageDrop,
                                     const unsigned int stringBypassDiodeInterval,
                                     const unsigned int stringNumCells,
                                     const double       cellSurfaceArea,
                                     const double       cellEfficiency,
                                     const double       cellSeriesResistance,
                                     const double       cellShuntResistance,
                                     const double       cellOpenCircuitVoltage,
                                     const double       cellRefTemperature,
                                     const double       cellTemperatureVoltageCoeff,
                                     const double       cellTemperatureCurrentCoeff);
        /// @brief Constructs this Photovoltaic String Model config data for the version 2 string model.
        GunnsElectPvStringConfigData(const double       stringBlockingDiodeVoltageDrop,
                                     const double       stringBypassDiodeVoltageDrop,
                                     const unsigned int stringBypassDiodeInterval,
                                     const unsigned int stringNumCells,
                                     const double       cellRefVoc,
                                     const double       cellRefIsc,
                                     const double       cellRefVmp,
                                     const double       cellRefImp,
                                     const double       cellRefPhotoFlux,
                                     const double       cellRefTemperature,
                                     const double       cellCoeffDVocDT,
                                     const double       cellCoeffDIscDT,
                                     const double       cellIdeality,
                                     const double       cellArea);
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
        double mSourceExposedFraction;    /**< (1)    Surface area fraction exposed to light source (0-1). */
        double mTemperature;              /**< (K)    Temperature of the string. */
        bool   mMalfPhotoFluxFlag;        /**< (1)    Photo power flux malfunction activation flag. */
        double mMalfPhotoFluxMagnitude;   /**< (W/m2) Photo power flux malfunction magnitude. */
        double mMalfPhotoFluxDuration;    /**< (s)    Photo power flux malfunction total duration. */
        double mMalfPhotoFluxRampTime;    /**< (s)    Photo power flux malfunction ramp up/down duration. */
        bool   mMalfExposedFractionFlag;  /**< (1)    Source exposed fraction malfunction activation flag. */
        double mMalfExposedFractionValue; /**< (1)    Source exposed fraction malfunction value. */
        bool   mMalfTemperatureFlag;      /**< (1)    Temperature malfunction activation flag. */
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
        bool                       mMalfCellGroupFlag;   /**<    (1)                     Cell group failure malfunction activation flag. */
        int                        mMalfCellGroupValue;  /**<    (1)                     Cell group failure malfunction number of failed groups. */
        bool                       mMalfDegradeFlag;     /**<    (1)                     Cell group power degrade malfunction activation flag. */
        double                     mMalfDegradeValue;    /**<    (1)                     Cell group power degrade malfunction value (0-1). */
        /// @}
        static const double        mIdealDiodeFactor;    /**< ** (1) trick_chkpnt_io(**) Ideal diode conductance multiplier. */
        /// @brief Default constructor for this Photovoltaic String Utility.
        GunnsElectPvString();
        /// @brief Normal constructor for this Photovoltaic String Utility.
        GunnsElectPvString(const GunnsElectPvStringConfigData* configData,
                           const GunnsElectPvStringInputData*  inputData);
        /// @brief Destructs this Photovoltaic String Utility.
        virtual ~GunnsElectPvString();
        /// @brief Initializes this Photovoltaic String Utility.
        virtual void initialize(const std::string& name);
        /// @brief Updates this Photovoltaic String Utility state.
        virtual void update();
        /// @brief Loads the string at the given power output.
        virtual void loadAtPower(const double power, const bool shortSide);
        /// @brief Loads the string at the given terminal voltage.
        virtual void loadAtVoltage(const double v1);
        /// @brief Loads the string at its Maximum Power Point.
        void   loadAtMpp();
        /// @brief Loads the string with the given conductive load.
        virtual void loadAtConductance(const double g);
        /// @brief Sets the string shunted flag;
        void   setShunted(const bool flag);
        /// @brief Gets the string shunted flag;
        bool   isShunted() const;
        /// @brief Gets the open-circuit terminal voltage of this string.
        double getOpenCircuitVoltage() const;
        /// @brief Gets the short-circuit terminal current of this string.
        double getShortCircuitCurrent() const;
        /// @brief Returns a reference to this string's equivalent circuit properties.
        const GunnsElectPvCellEquivCircuit& getEqProps() const;
        /// @brief Gets the Maximum Power Point state of this string.
        const GunnsElectPvLoadState& getMpp() const;
        /// @brief Gets the Terminal load state of this string.
        const GunnsElectPvLoadState& getTerminal() const;
        /// @brief Returns current output of the string at the given terminal voltage.
        virtual double predictCurrentAtVoltage(const double voltage) const;

    protected:
        std::string                         mName;                /**<    (1)    trick_chkpnt_io(**) Instance name for H&S messages. */
        const GunnsElectPvStringConfigData* mConfig;              /**< ** (1)    trick_chkpnt_io(**) Pointer to common string config data. */
        const GunnsElectPvStringInputData*  mInput;               /**< ** (W/m2) trick_chkpnt_io(**) Pointer to photo power flux incident on the section. */
        double                              mShuntVoltageDrop;    /**<    (V)    trick_chkpnt_io(**) Shunt voltage drop in reverse bias. */
        double                              mSeriesVoltageDrop;   /**<    (V)    trick_chkpnt_io(**) Series voltage drop in forward bias. */
        double                              mShortCircuitCurrent; /**<    (amp)  trick_chkpnt_io(**) Short-circuit current at terminal node (max load). */
        double                              mOpenCircuitVoltage;  /**<    (V)    trick_chkpnt_io(**) Open-circuit voltage at terminal node (no load). */
        GunnsElectPvCellEquivCircuit*       mEqProps;             /**<    (1)    trick_chkpnt_io(**) Properties of the string equivalent circuit. */
        GunnsElectPvLoadState               mMpp;                 /**<    (1)    trick_chkpnt_io(**) Maximum Power Point load state. */
        GunnsElectPvLoadState               mTerminal;            /**<    (1)    trick_chkpnt_io(**) Terminal output load state. */
        unsigned int                        mNumBypassedGroups;   /**<    (1)    trick_chkpnt_io(**) Number of bypassed cell groups. */
        unsigned int                        mNumActiveCells;      /**<    (1)    trick_chkpnt_io(**) Number of cells that are not bypassed. */
        bool                                mShunted;             /**<    (1)    trick_chkpnt_io(**) String is currently shunted. */
        GunnsElectPvCellEquivCircuit*       mRefCell;             /**<    (1)    trick_chkpnt_io(**) Reference cell properties. */
        /// @brief Validates the initialization of this Photovoltaic String Utility.
        virtual void validate() const;
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
/// @returns  bool  (--)  True if this is for a version 2 cell model.
///
/// @details  Returns the mIsVersion2 attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectPvCellConfigData::isVersion2() const
{
    return mIsVersion2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Pointer to the config data for this cell instance (not used).
/// @param[in] name       (--) Instance name for memory allocation and H&S messages.
///
/// @details  This initializes the instance name.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvCellEquivCircuit::initialize(const GunnsElectPvCellConfigData* configData __attribute__((unused)),
                                                     const std::string&                name)
{
    mName = name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] refCell     (--) Not used.
/// @param[in] temperature (--) Not used.
/// @param[in] photoFlux   (--) Not used.
/// @param[in] degradation (--) Not used.
///
/// @details  This is an empty virtual method intended for override by derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvCellEquivCircuit::update(const GunnsElectPvCellEquivCircuit* refCell     __attribute__((unused)),
                                                 const double                        temperature __attribute__((unused)),
                                                 const double                        photoFlux   __attribute__((unused)),
                                                 const double                        degradation __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is an empty virtual method intended for override by derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvCellEquivCircuit::derive()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage (V) Not used.
///
/// @returns  double (amp) Zero value.
///
/// @details  This is an empty virtual method intended for override by derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvCellEquivCircuit::computeCurrent(const double voltage __attribute__((unused))) const
{
    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] current (amp) Not used.
///
/// @returns  double (V) Zero value.
///
/// @details  This is an empty virtual method intended for override by derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvCellEquivCircuit::computeVoltage(const double current __attribute__((unused))) const
{
    return 0.0;
}

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
/// @returns  GunnsElectPvCellEquivCircuit&  (--)  A reference to the equivalent circuit properties.
///
/// @details  Returns a const reference to the mEqProps attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const GunnsElectPvCellEquivCircuit& GunnsElectPvString::getEqProps() const
{
    return *mEqProps;
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
