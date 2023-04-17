#ifndef GunnsElectBatteryCell_EXISTS
#define GunnsElectBatteryCell_EXISTS

/**
@file     GunnsElectBatteryCell.hh
@brief    GUNNS Electrical Battery Cell declarations

@defgroup GUNNS_ELECTRICAL_BATTERY_CELL    GUNNS Electrical Battery Cell Model
@ingroup  GUNNS_ELECTRICAL_BATTERY

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical Battery cell model.)

 REFERENCE:
- (TBD)

 ASSUMPTIONS AND LIMITATIONS:
- (TBD)

 LIBRARY DEPENDENCY:
- ((GunnsElectBatteryCell.o))

 PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2016-09))

@{
*/

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Battery Cell Model Configuration Data
///
/// @details  This class provides a data structure for the GunnsElectBatteryCell configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectBatteryCellConfigData
{
    public:
        double mResistance;            /**< (ohm)    trick_chkpnt_io(**) Internal resistance. */
        double mMaxCapacity;           /**< (amp*hr) trick_chkpnt_io(**) Maximum charge capacity. */
        /// @brief Electrical Battery Cell Model configuration data default constructor.
        GunnsElectBatteryCellConfigData(const double resistance  = 0.0,
                                        const double maxCapacity = 0.0);
        /// @brief Electrical Battery Cell Model configuration data default destructor.
        virtual ~GunnsElectBatteryCellConfigData();
        /// @brief Electrical Battery Cell Model configuration data copy constructor.
        GunnsElectBatteryCellConfigData(const GunnsElectBatteryCellConfigData& that);

    private:
        /// @brief Assignment operator is not available since declared private and not implemented.
        GunnsElectBatteryCellConfigData& operator =(const GunnsElectBatteryCellConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Battery Cell Model Input Data
///
/// @details  This class provides a data structure for the GunnsElectBatteryCell input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectBatteryCellInputData
{
    public:
        bool   mMalfOpenCircuit;            /**< (1)      trick_chkpnt_io(**) Initial failed open-circuit malfunction. */
        bool   mMalfShortCircuit;           /**< (1)      trick_chkpnt_io(**) Initial failed short-circuit malfunction. */
        bool   mMalfCapacityFlag;           /**< (1)      trick_chkpnt_io(**) Initial capacity override malfunction activation flag. */
        double mMalfCapacityValue;          /**< (amp*hr) trick_chkpnt_io(**) Initial capacity override malfunction value. */
        bool   mMalfThermalRunawayFlag;     /**< (1)      trick_chkpnt_io(**) Initial thermal runaway malfunction activation flag. */
        double mMalfThermalRunawayDuration; /**< (s)      trick_chkpnt_io(**) Initial thermal runaway malfunction duration value. */
        double mSoc;                        /**< (1)      trick_chkpnt_io(**) Initial State of Charge (0-1). */
        /// @brief Electrical Battery Cell Model input data default constructor.
        GunnsElectBatteryCellInputData(const bool   malfOpenCircuit            = false,
                                       const bool   malfShortCircuit           = false,
                                       const bool   malfCapacityFlag           = false,
                                       const double malfCapacityValue          = 0.0,
                                       const bool   malfThermalRunawayFlag     = false,
                                       const double malfThermalRunawayDuration = 0.0,
                                       const double soc                        = 0.0);
        /// @brief Electrical Battery Cell Model input data default destructor.
        virtual ~GunnsElectBatteryCellInputData();
        /// @brief Electrical Battery Cell Model input data copy constructor.
        GunnsElectBatteryCellInputData(const GunnsElectBatteryCellInputData& that);

    private:
        /// @brief Assignment operator is not available since declared private and not implemented.
        GunnsElectBatteryCellInputData& operator =(const GunnsElectBatteryCellInputData&);
};

// Forward declarations
class TsLinearInterpolator;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Battery Cell Model Class
///
/// @details  Models an individual cell in a battery.  State of Charge is updated based on a given
///           current through the cell, and open-circuit voltage of the cell is compute based on the
///           State of Charge and a given Voc vs. Soc lookup table (given by caller).
///
///           The cell has short-circuit, open-circuit and thermal runaway malfunctions.  The short-
///           circuit signatures override the open-circuit signatures if both malfs are active.  The
///           open-circuit and thermal runaway malfs both open the ciruit, and the thermal runaway
///           also converts the stored energy into waste heat over a given time interval.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectBatteryCell
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectBatteryCell);

    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool        mMalfOpenCircuit;            /**<   (1)      Failed open-circuit malfunction. */
        bool        mMalfShortCircuit;           /**<   (1)      Failed short-circuit malfunction. */
        bool        mMalfCapacityFlag;           /**<   (1)      Capacity override malfunction activation flag. */
        double      mMalfCapacityValue;          /**<   (amp*hr) Capacity override malfunction value. */
        bool        mMalfThermalRunawayFlag;     /**<   (1)      Thermal runaway malfunction activation flag. */
        double      mMalfThermalRunawayDuration; /**<   (s)      Thermal runaway malfunction duration value. */
        /// @}
        /// @brief   Electrical Battery Cell Model default constructor.
        GunnsElectBatteryCell();
        /// @brief   Electrical Battery Cell Model default destructor.
        virtual ~GunnsElectBatteryCell();
        /// @brief   Electrical Battery Cell Model initialize method.
        void   initialize(const GunnsElectBatteryCellConfigData& configData,
        		          const GunnsElectBatteryCellInputData&  inputData,
                          const std::string&                     name);
        /// @brief   Updates the cell state in response to inputs.
        void   updateSoc(const double current, const double timeStep, TsLinearInterpolator* socVocTable);
        /// @brief   Gets the cell effective State of Charge.
        double getEffectiveSoc() const;
        /// @brief   Gets the cell effective capacity.
        double getEffectiveCapacity() const;
        /// @brief   Gets the cell effective resistance.
        double getEffectiveResistance() const;
        /// @brief   Gets the cell effective open-circuit voltage.
        double getEffectiveVoltage(TsLinearInterpolator* socVocTable) const;
        /// @brief   Gets the thermal runaway power.
        double getRunawayPower() const;
        /// @brief   Sets and resets the cell failed open-circuit malfunction.
        void   setMalfOpenCircuit(const bool flag = false);
        /// @brief   Sets and resets the cell failed short-circuit malfunction.
        void   setMalfShortCircuit(const bool flag = false);
        /// @brief   Sets and resets the cell capacity override malfunction.
        void   setMalfCapacity(const bool flag = false, const double value = 0.0);
        /// @brief   Sets and resets the cell thermal runaway malfunction.
        void   setMalfThermalRunaway(const bool flag = false, const double duration = 0.0);

    protected:
        std::string mName;             /**< *o (1)      trick_chkpnt_io(**) Instance name for error messages. */
        double      mResistance;       /**<    (ohm)    trick_chkpnt_io(**) Internal resistance. */
        double      mMaxCapacity;      /**<    (amp*hr) trick_chkpnt_io(**) Maximum charge capacity. */
        double      mSoc;              /**<    (1)                          Actual State of Charge (0-1). */
        double      mRunawayPower;     /**<    (W)                          Current discharge rate of thermal runaway. */
        double      mRunawayPowerRate; /**<    (W/s)                        Discharge delta-rate of thermal runaway. */
        /// @brief   Validates the cell initialization.
        void   validate();
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) The current thermal runaway power (heat) output.
///
/// @details  Returns the value of mRunawayPower.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectBatteryCell::getRunawayPower() const
{
    return mRunawayPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) Malfunction activation flag: true activates, false deactivates.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectBatteryCell::setMalfOpenCircuit(const bool flag)
{
    mMalfOpenCircuit = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) Malfunction activation flag: true activates, false deactivates.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectBatteryCell::setMalfShortCircuit(const bool flag)
{
    mMalfShortCircuit = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--)     Malfunction activation flag: true activates, false deactivates.
/// @param[in] value (amp*hr) Malfunction value.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectBatteryCell::setMalfCapacity(const bool flag, const double value)
{
    mMalfCapacityFlag  = flag;
    mMalfCapacityValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag     (--) Malfunction activation flag: true activates, false deactivates.
/// @param[in] duration (s)  Malfunction time to discharge all cell energy as heat.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectBatteryCell::setMalfThermalRunaway(const bool flag, const double duration)
{
    mMalfThermalRunawayFlag     = flag;
    mMalfThermalRunawayDuration = duration;
}

#endif
