#ifndef GunnsElectBattery_EXISTS
#define GunnsElectBattery_EXISTS

/**
@file     GunnsElectBattery.hh
@brief    GUNNS Electrical Battery declarations

@defgroup GUNNS_ELECTRICAL_BATTERY_LINK    GUNNS Electrical Battery Model
@ingroup  GUNNS_ELECTRICAL_BATTERY

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical Battery link model.)

 REFERENCE:
- (TBD)

 ASSUMPTIONS AND LIMITATIONS:
- (TBD)

 LIBRARY DEPENDENCY:
- ((GunnsElectBattery.o))

 PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2016-09))

@{
*/

#include "core/GunnsBasicPotential.hh"
#include "math/approximation/TsLinearInterpolator.hh"
#include "GunnsElectBatteryCell.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Battery Model Configuration Data
///
/// @details  This class provides a data structure for the GunnsElectBattery configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectBatteryConfigData : public GunnsBasicPotentialConfigData
{
    public:
        int                   mNumCells;               /**< (1)      trick_chkpnt_io(**) Number of battery cells. */
        bool                  mCellsInParallel;        /**< (1)      trick_chkpnt_io(**) Whether the cells are in parallel (True) or series (False). */
        double                mCellResistance;         /**< (ohm)    trick_chkpnt_io(**) Internal resistance of each cell. */
        double                mInterconnectResistance; /**< (ohm)    trick_chkpnt_io(**) Total interconnect resistance between all cells. */
        double                mMaxCapacity;            /**< (amp*hr) trick_chkpnt_io(**) Maximum charge capacity of the battery. */
        TsLinearInterpolator* mSocVocTable;            /**< (1)      trick_chkpnt_io(**) Pointer to open-circuit voltage vs. State of Charge table. */
        /// @brief Electrical Battery Model configuration data default constructor.
        GunnsElectBatteryConfigData(const std::string     name                   = "",
                                    GunnsNodeList*        nodes                  = 0,
                                    const int             numCells               = 0,
                                    const bool            cellsInParallel        = false,
                                    const double          cellResistance         = 0.0,
                                    const double          interconnectResistance = 0.0,
                                    const double          maxCapacity            = 0.0,
                                    TsLinearInterpolator* socVocTable            = 0);
        /// @brief Electrical Battery Model configuration data default destructor.
        virtual ~GunnsElectBatteryConfigData();

    private:
        /// @brief Copy constructor is not available since declared private and not implemented.
        GunnsElectBatteryConfigData(const GunnsElectBatteryConfigData&);
        /// @brief Assignment operator is not available since declared private and not implemented.
        GunnsElectBatteryConfigData& operator =(const GunnsElectBatteryConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Battery Model Input Data
///
/// @details  This class provides a data structure for the GunnsElectBattery input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectBatteryInputData : public GunnsBasicPotentialInputData
{
    public:
        double mSoc;                        /**< (1) trick_chkpnt_io(**) Initial battery State of Charge (0-1). */
        bool   mMalfThermalRunawayFlag;     /**< (1) trick_chkpnt_io(**) Initial thermal runaway malfunction activation flag. */
        double mMalfThermalRunawayDuration; /**< (s) trick_chkpnt_io(**) Initial duration of each cell thermal runaway malfunction. */
        double mMalfThermalRunawayInterval; /**< (s) trick_chkpnt_io(**) Initial time interval between malfunction trigger in each cell. */
        /// @brief Electrical Battery Model input data default constructor.
        GunnsElectBatteryInputData(const bool   malfBlockageFlag           = false,
                                   const double malfBlockageValue          = 0.0,
                                   const double soc                        = 0.0,
                                   const bool   malfThermalRunawayFlag     = false,
                                   const double malfThermalRunawayDuration = 0.0,
                                   const double malfThermalRunawayInterval = 0.0);
        /// @brief Electrical Battery Model input data default destructor.
        virtual ~GunnsElectBatteryInputData();

    private:
        /// @brief Copy constructor is not available since declared private and not implemented.
        GunnsElectBatteryInputData(const GunnsElectBatteryInputData&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectBatteryInputData& operator =(const GunnsElectBatteryInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Battery Model Class
///
/// @details  This models an electrical battery containing one or more voltage cells in series or
///           parallel.  The battery acts like a voltage (potential) source in the network, with
///           internal resistance based on the cells' internal resistance and resistance of the
///           interconnects between the cells.  The actual closed-circuit output voltage of this
///           battery in a circuit is its open-circuit voltage minus the drop under load (current)
///           due to the total internal resistance, same as any basic potential source.
///
///           With the cells in series, the battery's open-circuit voltage (base class
///           mSourcePotential term) is the sum of the voltages of the cells, which are a function
///           of their individual States of Charge.  With the cells in parallel, the battery's open-
///           circuit voltage is that of the cell with the highest voltage.
///
///           Port 0 of the link is the input port, and Port 1 is the output port.  The closed-
///           circuit output voltage is equal to the Port 1 node potential.
///
///           This is a consolidation & improvement of the old BattElect and BattElectEmu links
///           originally written for TS21.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectBattery: public GunnsBasicPotential
{
    TS_MAKE_SIM_COMPATIBLE (GunnsElectBattery);

    public:
        /// @name    Malfunction terms and embedded objects.
        /// @{
        /// @details Malfunctions and embedded objects are public to allow access from the Trick
        ///          events processor.
        GunnsElectBatteryCell* mCells;                      /**< (1) trick_chkpnt_io(**) Battery cells. */
        bool                   mMalfThermalRunawayFlag;     /**< (1)                     Thermal runaway malfunction activation flag. */
        double                 mMalfThermalRunawayDuration; /**< (s)                     Each cell thermal runaway malfunction duration. */
        double                 mMalfThermalRunawayInterval; /**< (s)                     Time interval between malfunction trigger in each cell. */
        /// @}
        /// @brief   Electrical Battery Model default constructor.
        GunnsElectBattery();
        /// @brief   Electrical Battery Model default destructor.
        virtual ~GunnsElectBattery();
        /// @brief   Electrical Battery Model initialize method.
        void           initialize(GunnsElectBatteryConfigData&  configData,
                                  GunnsElectBatteryInputData&   inputData,
                                  std::vector<GunnsBasicLink*>& networkLinks,
                                  const int                     port0,
                                  const int                     port1);
        /// @brief   Updates the link's conductance and potential source.
        virtual void   updateState(const double timeStep);
        /// @brief   Updates the flux through the link and its effects.
        virtual void   updateFlux(const double timeStep, const double flux);
        /// @brief   Returns the output closed-circuit voltage under load.
        virtual double getVoltage() const;
        /// @brief   Returns the state of charge.
        virtual double getSoc() const;
        /// @brief   Returns the capacity.
        virtual double getCapacity() const;
        /// @brief   Returns the heat output.
        double         getHeat() const;
        /// @brief   Returns the effective voltage of the given cell number.
        double         getCellEffectiveVoltage(const unsigned int cell) const;
        /// @brief   Sets and resets the cell thermal runaway malfunction.
        void           setMalfThermalRunaway(const bool flag = false, const double duration = 0.0,
                                             const double interval = 0.0);

    protected:
        unsigned int          mNumCells;               /**< (1)      trick_chkpnt_io(**) Number of battery cells. */
        bool                  mCellsInParallel;        /**< (1)      trick_chkpnt_io(**) Whether the cells are in parallel (True) or series (False). */
        double                mInterconnectResistance; /**< (ohm)    trick_chkpnt_io(**) Total interconnect resistance between all cells. */
        TsLinearInterpolator* mSocVocTable;            /**< (1)      trick_chkpnt_io(**) Pointer to open-circuit voltage vs. State of Charge table. */
        double                mSoc;                    /**< (1)      trick_chkpnt_io(**) Battery average State Of Charge (0-1) of active cells. */
        double                mCapacity;               /**< (amp*hr) trick_chkpnt_io(**) Battery effective capacity of active cells. */
        double                mCurrent;                /**< (amp)    trick_chkpnt_io(**) Battery current. */
        double                mVoltage;                /**< (V)      trick_chkpnt_io(**) Output closed-circuit voltage under load. */
        double                mHeat;                   /**< (W)      trick_chkpnt_io(**) Heat created by the battery. */
        unsigned int          mThermalRunawayCell;     /**< (1)                          Current cell index for the thermal runaway cascade. */
        double                mThermalRunawayTimer;    /**< (s)                          Elapsed time of the thermal runaway malfunction. */
        /// @brief   Validates the link's configuration and input data.
        void         validate(GunnsElectBatteryConfigData& configData,
                              GunnsElectBatteryInputData&  inputData);
        /// @brief   Allocates dynamic arrays based on the number of battery cells.
        void         allocateArrays();
        /// @brief   Deletes dynamic memory objects.
        void         cleanup();
        /// @brief   Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief   Finds resistance of all cells in parallel.
        double       computeParallelResistance() const;
        /// @brief   Finds resistance of all cells in series.
        double       computeSeriesResistance() const;
        /// @brief   Finds total Voc of all cells in parallel.
        double       computeParallelVoc() const;
        /// @brief   Finds total Voc of all cells in series.
        double       computeSeriesVoc() const;
        /// @brief   Updates the cells State of Charge.
        void         updateCells(const double timeStep);
        /// @brief   Updates the battery model output terms.
        void         updateOutputs();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectBattery(const GunnsElectBattery& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectBattery& operator =(const GunnsElectBattery& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (V) Output closed-circuit voltage under load.
///
/// @details Returns the battery's output closed-circuit voltage under load.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectBattery::getVoltage() const
{
    return mVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (--) Output average State Of Charge (0-1) of active cells.
///
/// @details Returns the battery's output average State Of Charge (0-1) of active cells.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectBattery::getSoc() const
{
    return mSoc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (amp*hr) Output sum of capacities of active cells.
///
/// @details Returns the battery's total effective capacity of active cells.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectBattery::getCapacity() const
{
    return mCapacity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (W) Heat created by the battery.
///
/// @details Returns the heat created by the battery due to charging, discharging and thermal
///          runaway.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectBattery::getHeat() const
{
    return mHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] cell (--) The cell number to return the voltage for.
///
/// @details Returns the effective cell voltage for the given cell number.  If the given cell
///          number if out of bound of the number of cells in this battery, we return zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectBattery::getCellEffectiveVoltage(const unsigned int cell) const
{
    if (cell < mNumCells) {
        return mCells[cell].getEffectiveVoltage(mSocVocTable);
    }
    return 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag     (--) Malfunction activation flag: true activates, false deactivates.
/// @param[in] duration (s)  Malfunction time to discharge all cell energy as heat.
/// @param[in] interval (s)  Malfunction time between start of runaway in each subsequent cell.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectBattery::setMalfThermalRunaway(const bool flag, const double duration,
                                                     const double interval)
{
    mMalfThermalRunawayFlag     = flag;
    mMalfThermalRunawayDuration = duration;
    mMalfThermalRunawayInterval = interval;
}

#endif
