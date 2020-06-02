/**
@file
@brief    GUNNS Electrical Battery implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((GunnsElectBatteryCell.o)
  (core/GunnsBasicPotential.o)
  (math/UnitConversion.o)
  (math/approximation/TsLinearInterpolator.o))
*/

#include "GunnsElectBattery.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                   (--)     Link instance name.
/// @param[in] nodes                  (--)     Network nodes array.
/// @param[in] numCells               (--)     Number of battery cells.
/// @param[in] cellsInParallel        (--)     Whether the cells are in parallel (True) or series (False).
/// @param[in] cellResistance         (ohm)    Internal resistance of each cell.
/// @param[in] interconnectResistance (ohm)    Total interconnect resistance between all cells.
/// @param[in] maxCapacity            (amp*hr) Maximum charge capacity of the battery.
/// @param[in] socVocTable            (--)     Pointer to open-circuit voltage vs. State of Charge table.
///
/// @details  Default constructs this GunnsElectBattery config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectBatteryConfigData::GunnsElectBatteryConfigData(const std::string     name,
                                                         GunnsNodeList*        nodes,
                                                         const int             numCells,
                                                         const bool            cellsInParallel,
                                                         const double          cellResistance,
                                                         const double          interconnectResistance,
                                                         const double          maxCapacity,
                                                         TsLinearInterpolator* socVocTable)
    :
    GunnsBasicPotentialConfigData(name, nodes, 0.0),
    mNumCells(numCells),
    mCellsInParallel(cellsInParallel),
    mCellResistance(cellResistance),
    mInterconnectResistance(interconnectResistance),
    mMaxCapacity(maxCapacity),
    mSocVocTable(socVocTable)
{
    // Nothing to to.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GunnsElectBattery config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectBatteryConfigData::~GunnsElectBatteryConfigData()
{
    // Nothing to to.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Initial blockage malfunction flag.
/// @param[in] malfBlockageValue (--) Initial blockage malfunction fractional value (0-1).
/// @param[in] sourcePotential   (--) Not used by this link.
/// @param[in] soc               (--) Initial battery State of Charge (0-1).
///
/// @details  Default constructs this GunnsElectBattery input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectBatteryInputData::GunnsElectBatteryInputData(const bool   malfBlockageFlag,
                                                       const double malfBlockageValue,
                                                       const double soc)
    :
    GunnsBasicPotentialInputData(malfBlockageFlag, malfBlockageValue, 0.0),
    mSoc(soc)
{
    // Nothing to to.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GunnsElectBattery input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectBatteryInputData::~GunnsElectBatteryInputData()
{
    // Nothing to to.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GunnsElectBattery.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectBattery::GunnsElectBattery()
:
    GunnsBasicPotential(),
    mCells(0),
    mNumCells(0),
    mCellsInParallel(false),
    mInterconnectResistance(0.0),
    mSocVocTable(0),
    mSoc(0.0),
    mCurrent(0.0),
    mVoltage(0.0)
{
    // Nothing to to.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GunnsElectBattery.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectBattery::~GunnsElectBattery()
{
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData   (--) Reference to Link Config Data.
/// @param[in] inputData    (--) Reference to Link Input Data.
/// @param[in] networkLinks (--) Reference to the Network Link Vector.
/// @param[in] port0        (--) Port Mapping.
/// @param[in] port1        (--) Port Mapping.
///
/// @details  Initializes this GunnsElectBattery with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectBattery::initialize(GunnsElectBatteryConfigData&  configData,
                                   GunnsElectBatteryInputData&   inputData,
                                   std::vector<GunnsBasicLink*>& networkLinks,
                                   const int                     port0,
                                   const int                     port1)
{
    GunnsBasicPotential::initialize(configData, inputData, networkLinks, port0, port1);
    mInitFlag = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize from configuration and input data.
    mNumCells               = configData.mNumCells;
    mCellsInParallel        = configData.mCellsInParallel;
    mInterconnectResistance = configData.mInterconnectResistance;
    mSocVocTable            = configData.mSocVocTable;

    allocateArrays();

    GunnsElectBatteryCellConfigData cellConfig(configData.mCellResistance,
                                               configData.mMaxCapacity / mNumCells);
    GunnsElectBatteryCellInputData  cellInput(false, false, inputData.mSoc);
    for (int i = 0; i < mNumCells; i++) {
        std::ostringstream cell;
        cell << i;
        mCells[i].initialize(cellConfig, cellInput, mName + "mCells_" + cell.str());
    }

    /// - Initialize remaining model state.
    updateOutputs();
    mInitFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Allocate arrays based on the number of battery cells.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectBattery::allocateArrays()
{
    TS_NEW_CLASS_ARRAY_EXT(mCells, mNumCells, GunnsElectBatteryCell, (), std::string(mName) + ".mCell");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes allocated memory objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectBattery::cleanup()
{
    if (mCells) {
        TS_DELETE_ARRAY(mCells);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectBattery::restartModel()
{
    /// - Reset the base class.
    GunnsBasicPotential::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    updateOutputs();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Configuration data.
/// @param[in] inputData  (--) Input Data.
///
/// @throws   TsInitializationException
///
/// @details  Validates link initialization from configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectBattery::validate(GunnsElectBatteryConfigData& configData,
                                 GunnsElectBatteryInputData&  inputData)
{
    /// - Issue an error on cell interconnect resistance < 0.
    if (configData.mInterconnectResistance < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Cell interconnect resistance < 0.");
    }

    /// - Issue an error on # cells < 1.
    if (configData.mNumCells < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Number of cells < 1.");
    }

    /// - Issue an error on NULL SOC/VOC table.
    if (!configData.mSocVocTable) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Missing SOC/VOC table.");
    }

    /// - Issue an error on initial SOC not in (0-1).
    if (!Math::isInRange(0.0, inputData.mSoc, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Initial State of Charge not in (0-1).");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep (s) Not used.
///
/// @details  Updates the link conductance and potential source from the battery resistance and
///           open-circuit voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectBattery::updateState(const double timeStep __attribute__((unused)))
{
    /// - Total resistance = battery interconnect resistance + total cell resistance.
    double resistance = mInterconnectResistance;
    if (mCellsInParallel) {
        resistance += computeParallelResistance();
    } else {
        resistance += computeSeriesResistance();
    }
    // Divide-by-zero is protected in calculation of resistance.
    mEffectiveConductivity = 1.0 / resistance;

    /// - Link source potential comes from teh cells open-circuit voltage based on their States of
    ///   Charge.  All cells share the same Voc/Soc table.
    if (mCellsInParallel) {
        mSourcePotential = computeParallelVoc();
    } else {
        mSourcePotential = computeSeriesVoc();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (ohm) Total resistance of all cells in parallel, limited to > 0.
///
/// @details  Cells are treated as simple resistors in parallel.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectBattery::computeParallelResistance() const
{
    double conductance = 0.0;
    for (int i = 0; i < mNumCells; i++ )
    {
        conductance += 1.0 / std::max(mCells[i].getEffectiveResistance(), DBL_EPSILON);
    }
    return 1.0 / std::max(conductance, DBL_EPSILON);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (ohm) Total resistance of all cells in series, limited to > 0.
///
/// @details  Cells are treated as simple resistors in series.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectBattery::computeSeriesResistance() const
{
    double resistance = 0.0;
    for (int i = 0; i < mNumCells; i++ )
    {
        resistance += mCells[i].getEffectiveResistance();
    }
    return std::max(resistance, DBL_EPSILON);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (V) Open-circuit voltage of the cells in parallel.
///
/// @details  Since the cells are in parallel, the battery's Voc is the cell with the highest Voc.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectBattery::computeParallelVoc() const
{
    double Voc = 0.0;
    for (int i = 0; i < mNumCells; i++ )
    {
        const double cellVoc = mCells[i].getEffectiveVoltage(mSocVocTable);
        if (cellVoc > Voc) {
            Voc = cellVoc;
        }
    }
    return Voc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (V) Open-circuit voltage of the cells in series.
///
/// @details  Since the cells are in series, the battery's Voc is the sum of all the cell's Voc.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsElectBattery::computeSeriesVoc() const
{
    double Voc = 0.0;
    for (int i = 0; i < mNumCells; i++ )
    {
        Voc += mCells[i].getEffectiveVoltage(mSocVocTable);
    }
    return Voc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep (s)  Integration time step.
/// @param[in] flux     (--) Not used.
///
/// @details  Updates output voltage, current and State of Charge.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectBattery::updateFlux(const double timeStep, const double flux __attribute__((unused)))
{
    updateCells(timeStep);
    updateOutputs();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timeStep (s) time step
///
/// @details  Updates the cells' State of Charge as a result of current integrated over the step.
///           In a real battery, cells with different SOC and Voc would get different loads, but we
///           assume they all get the same load as a simplification.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectBattery::updateCells(const double timeStep)
{
    /// - Count the number of cells contributing to the load.
    int count = 0;
    for (int i = 0; i < mNumCells; i++) {
        if (mCells[i].getEffectiveSoc() > DBL_EPSILON) {
            count++;
        }
    }

    /// - The link flux (current) is divided by the number of contributing cells and then passed to
    ///   all the cells to integrate.  Cells that are not contributing will ignore the current in
    ///   the cell model.
    if (count > 0) {
        const double current = mFlux / count;
        for (int i = 0; i < mNumCells; i++) {
            mCells[i].updateSoc(current, timeStep);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the output current, voltage and average State of Charge.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectBattery::updateOutputs()
{
    mCurrent = mFlux;
    mVoltage = mPotentialVector[1];
    double soc = 0.0;
    if (mNumCells > 0) {
        for (int i = 0; i < mNumCells; i++) {
            soc += mCells[i].getEffectiveSoc();
        }
        soc /= mNumCells;
    }
    mSoc = soc;
}
