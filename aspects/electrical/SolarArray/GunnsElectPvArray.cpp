/**
@file     GunnsElectPvArray.cpp
@brief    GUNNS Electrical Photovoltaic Array Link Model implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  (
   (core/GunnsBasicLink.o)
   (core/GunnsBasicNode.o)
   (GunnsElectPvSection.o)
   (software/exceptions/TsInitializationException.o)
   (software/exceptions/TsOutOfBoundsException.o)
  )
*/

#include "math/MsMath.hh"
#include "GunnsElectPvArray.hh"
#include "core/GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name  (--) Instance name for memory allocation and H&S messages.
/// @param[in] nodes (--) Pointer to the network nodes list.
///
/// @details  Default constructs this Photovoltaic Array Link config data.  This only exists to
///           avoid ambiguity with the overloaded custom constructors below.  This shouldn't
///           actually be used, as an array configured with this constructor will not be able to
///           initialize.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvArrayConfigData::GunnsElectPvArrayConfigData(const std::string& name,
                                                         GunnsNodeList*     nodes)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mNumSections(0),
    mNumStrings(0),
    mNumStringsBySection(0),
    mSectionConfig()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                           (--)   Instance name for memory allocation and H&S messages.
/// @param[in] nodes                          (--)   Pointer to the network nodes list.
/// @param[in] arrayNumSections               (--)   Number of sections in the array.
/// @param[in] arrayNumStrings                (--)   Default total number of strings in the array.
/// @param[in] sectionSourceAngleExponent     (--)   Exponent on trig function of light source incident angle.
/// @param[in] sectionBacksideReduction       (--)   Reduction fraction (0-1) when lit from back side.
/// @param[in] sectionSourceAngleEdgeOn       (--)   Angle of light source to surface is edge-on instead of normal.
/// @param[in] sectionRefSourceFluxMagnitude  (W/m2) Reference ambient flux magnitude of light source at the surface.
/// @param[in] stringBlockingDiodeVoltageDrop (V)    Voltage drop across the diode at end of string.
/// @param[in] stringBypassDiodeVoltageDrop   (V)    Voltage drop across each bypass diode.
/// @param[in] stringBypassDiodeInterval      (--)   Number of cells per bypass diode.
/// @param[in] stringNumCells                 (--)   Number of cells in this string.
/// @param[in] cellSurfaceArea                (m2)   Cell surface area of one side.
/// @param[in] cellEfficiency                 (--)   Cell photovoltaic efficiency (0-1).
/// @param[in] cellSeriesResistance           (ohm)  Cell series resistance.
/// @param[in] cellShuntResistance            (ohm)  Cell shunt resistance.
/// @param[in] cellOpenCircuitVoltage         (V)    Cell open-circuit voltage.
/// @param[in] cellRefTemperature             (K)    Cell Reference temperature for temperature effects.
/// @param[in] cellTemperatureVoltageCoeff    (1/K)  Cell coefficient for temperature effect on open-circuit voltage.
/// @param[in] cellTemperatureVoltageCoeff    (1/K)  Cell coefficient for temperature effect on source current.
///
/// @details  This overloaded constructor constructs this Photovoltaic Array Link config data for
///           the original version strings.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvArrayConfigData::GunnsElectPvArrayConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const unsigned int arrayNumSections,
        const unsigned int arrayNumStrings,
        const double       sectionSourceAngleExponent,
        const double       sectionBacksideReduction,
        const bool         sectionSourceAngleEdgeOn,
        const double       sectionRefSourceFluxMagnitude,
        const double       stringBlockingDiodeVoltageDrop,
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
        const double       cellTemperatureCurrentCoeff)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mNumSections(arrayNumSections),
    mNumStrings(arrayNumStrings),
    mNumStringsBySection(0),
    mSectionConfig(sectionSourceAngleExponent,
                   sectionBacksideReduction,
                   sectionSourceAngleEdgeOn,
                   sectionRefSourceFluxMagnitude,
                   stringBlockingDiodeVoltageDrop,
                   stringBypassDiodeVoltageDrop,
                   stringBypassDiodeInterval,
                   stringNumCells,
                   cellSurfaceArea,
                   cellEfficiency,
                   cellSeriesResistance,
                   cellShuntResistance,
                   cellOpenCircuitVoltage,
                   cellRefTemperature,
                   cellTemperatureVoltageCoeff,
                   cellTemperatureCurrentCoeff)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                           (--)    Instance name for memory allocation and H&S messages.
/// @param[in] nodes                          (--)    Pointer to the network nodes list.
/// @param[in] cellRefVoc                     (V)     Reference cell open-circuit voltage.
/// @param[in] cellRefIsc                     (amp)   Reference cell short-circuit current.
/// @param[in] cellRefVmp                     (V)     Reference cell voltage at the maximum power point.
/// @param[in] cellRefImp                     (amp)   Reference cell current at the maximum power point.
/// @param[in] cellRefTemperature             (K)     Reference cell temperature.
/// @param[in] cellCoeffDVocDT                (V/K)   Coefficient for temperature effect on open-circuit voltage.
/// @param[in] cellCoeffDIscDT                (amp/K) Coefficient for temperature effect on source current.
/// @param[in] cellIdeality                   (1)     Cell equivalent diode ideality constant.
/// @param[in] cellArea                       (m2)    Optional cell area, used for efficiency estimation.
/// @param[in] sectionSourceAngleExponent     (--)    Exponent on trig function of light source incident angle.
/// @param[in] sectionBacksideReduction       (--)    Reduction fraction (0-1) when lit from back side.
/// @param[in] sectionSourceAngleEdgeOn       (--)    Angle of light source to surface is edge-on instead of normal.
/// @param[in] sectionRefSourceFluxMagnitude  (W/m2)  Reference ambient flux magnitude of light source absorbed by the surface.
/// @param[in] stringBlockingDiodeVoltageDrop (V)     Voltage drop across the diode at end of string.
/// @param[in] stringBypassDiodeVoltageDrop   (V)     Voltage drop across each bypass diode.
/// @param[in] stringBypassDiodeInterval      (--)    Number of cells per bypass diode.
/// @param[in] stringNumCells                 (--)    Number of cells in this string.
/// @param[in] arrayNumSections               (--)    Number of sections in the array.
/// @param[in] arrayNumStrings                (--)    Default total number of strings in the array.
///
/// @details  This overloaded constructor constructs this Photovoltaic Array Link config data for
///           the version 2 strings.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvArrayConfigData::GunnsElectPvArrayConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       cellRefVoc,
        const double       cellRefIsc,
        const double       cellRefVmp,
        const double       cellRefImp,
        const double       cellRefTemperature,
        const double       cellCoeffDVocDT,
        const double       cellCoeffDIscDT,
        const double       cellIdeality,
        const double       cellArea,
        const double       sectionSourceAngleExponent,
        const double       sectionBacksideReduction,
        const bool         sectionSourceAngleEdgeOn,
        const double       sectionRefSourceFluxMagnitude,
        const double       stringBlockingDiodeVoltageDrop,
        const double       stringBypassDiodeVoltageDrop,
        const unsigned int stringBypassDiodeInterval,
        const unsigned int stringNumCells,
        const unsigned int arrayNumSections,
        const unsigned int arrayNumStrings)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mNumSections(arrayNumSections),
    mNumStrings(arrayNumStrings),
    mNumStringsBySection(0),
    mSectionConfig(cellRefVoc,
                   cellRefIsc,
                   cellRefVmp,
                   cellRefImp,
                   cellRefTemperature,
                   cellCoeffDVocDT,
                   cellCoeffDIscDT,
                   cellIdeality,
                   cellArea,
                   sectionSourceAngleExponent,
                   sectionBacksideReduction,
                   sectionSourceAngleEdgeOn,
                   sectionRefSourceFluxMagnitude,
                   stringBlockingDiodeVoltageDrop,
                   stringBypassDiodeVoltageDrop,
                   stringBypassDiodeInterval,
                   stringNumCells)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Array Link config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvArrayConfigData::~GunnsElectPvArrayConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Reference to the object to be assigned from.
///
/// @details  Assignment operator for the Photovoltaic Array Link config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvArrayConfigData& GunnsElectPvArrayConfigData::operator =(const GunnsElectPvArrayConfigData& that)
{
    if (&that != this) {
        mName                = that.mName;
        mNodeList            = that.mNodeList;
        mNumSections         = that.mNumSections;
        mNumStrings          = that.mNumStrings;
        mNumStringsBySection = that.mNumStringsBySection;
        mSectionConfig       = that.mSectionConfig;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] sectionSourceFluxMagnitude   (W/m2) Ambient flux magnitude of light source at the surface.
/// @param[in] sectionSourceAngle           (r)    Angle of light source to surface.
/// @param[in] sectionSourceExposedFraction (--)   Surface area fraction exposed to light source (0-1).
/// @param[in] sectionTemperature           (K)    Temperature of the section.
///
/// @details  Default constructs this Photovoltaic Array Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvArrayInputData::GunnsElectPvArrayInputData(
        const double sectionSourceFluxMagnitude,
        const double sectionSourceAngle,
        const double sectionSourceExposedFraction,
        const double sectionTemperature)
    :
    GunnsElectPvSectionInputData(sectionSourceFluxMagnitude,
                                 sectionSourceAngle,
                                 sectionSourceExposedFraction,
                                 sectionTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Array Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvArrayInputData::~GunnsElectPvArrayInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Photovoltaic Array Link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvArray::GunnsElectPvArray()
    :
    GunnsBasicLink(NPORTS),
    mSections(0),
    mConfig(),
    mOpenCircuitSide(false),
    mCommonStringsOutput(false),
    mPercentInsolation(0.0),
    mShortCircuitCurrent(0.0),
    mOpenCircuitVoltage(0.0),
    mMpp(),
    mTerminal(),
    mIvCornerVoltage(0.0),
    mIvCornerCurrent(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Photovoltaic Array Link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectPvArray::~GunnsElectPvArray()
{
    TS_DELETE_ARRAY(mSections);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Photovoltaic Array Config Data.
/// @param[in]     inputData    (--) Photovoltaic Section Input Data.
/// @param[in,out] networkLinks (--) Network links.
/// @param[in]     port0        (--) Network port 0.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Photovoltaic Array Link with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvArray::initialize(const GunnsElectPvArrayConfigData&  configData,
                                   const GunnsElectPvSectionInputData& inputData,
                                   std::vector<GunnsBasicLink*>&       networkLinks,
                                   const int                           port0)
{
    /// - Initialize the base class.
    GunnsBasicLinkInputData baseInput(false, 0.0);
    int ports[1] = {port0};
    GunnsBasicLink::initialize(configData, baseInput, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize & validate config data.
    mConfig = configData;
    validate();

    /// - Create and initialize the array of sections.
    int numSections = mConfig.mNumSections;
    TS_NEW_CLASS_ARRAY_EXT(mSections, numSections, GunnsElectPvSection, (&mConfig.mSectionConfig),
                           std::string(mName) + ".mSections");

    /// - By default, each section has the same number of strings.  However if the
    ///   mNumStringsBySection list is provided, then each section will get its number of strings
    ///   from the list, and the array's total number of strings will be changed to the list sum.
    if (mConfig.mNumStringsBySection) {
        mConfig.mNumStrings = 0;
    }
    for(int i=0; i<numSections; ++i) {
        std::ostringstream stream;
        stream << ".mSections_" << i;
        unsigned int numStrings = 0;
        if (mConfig.mNumStringsBySection) {
            numStrings = mConfig.mNumStringsBySection[i];
            mConfig.mNumStrings += numStrings;
        } else {
            numStrings = mConfig.mNumStrings / mConfig.mNumSections;
        }
        mSections[i].initialize(mName + stream.str(), inputData, numStrings);
    }

    /// - Initialize class attributes.
    mOpenCircuitSide     = true;
    mCommonStringsOutput = true;
    mPercentInsolation   = 0.0;
    mShortCircuitCurrent = 0.0;
    mOpenCircuitVoltage  = 0.0;
    mMpp.clear();
    mTerminal.clear();
    mIvCornerVoltage     = 0.0;
    mIvCornerCurrent     = 0.0;
    mInitFlag            = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Photovoltaic Array configuration.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvArray::validate() const
{
    /// - Throw an exception on # sections < 1.
    if (mConfig.mNumSections < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "number of sections < 1.");
    }

    /// - Throw an exception on # strings < # sections.
    if (mConfig.mNumStrings < mConfig.mNumSections) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "number of strings < number of sections.");
    }

    /// - Throw an exception if # sections not evenly divisible in total # strings when the
    ///   # of strings in each section custom list is not provided.
    if (!mConfig.mNumStringsBySection and (0 != mConfig.mNumStrings % mConfig.mNumSections)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "number of sections not evenly divisible in number of strings.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvArray::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config and non-checkpointed data.
    mOpenCircuitSide     = true;
    mPercentInsolation   = 0.0;
    mShortCircuitCurrent = 0.0;
    mOpenCircuitVoltage  = 0.0;
    mMpp.clear();
    mTerminal.clear();
    mIvCornerVoltage     = 0.0;
    mIvCornerCurrent     = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Computes this link's contributions to the network system of equations prior to the
///           network major step solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvArray::step(const double dt)
{
    processUserPortCommand();
    updateArray(dt);
    minorStep(dt, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt         (s)   Not used.
/// @param[in]  minorStep  (--)  Not used.
///
/// @details  Updates this link's contributions to the network system of equations for minor steps.
///           If the strings are not tied to a common output, then they are being individually
///           loaded by a downstream regulator and the output node of this link isn't used, so zero
///           this link's effects on it.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvArray::minorStep(const double dt __attribute__((unused)),
                                  const int    minorStep __attribute__((unused)))
{
    if (mCommonStringsOutput) {
        buildAdmittanceMatrix();
        buildSourceVector();
    } else {
        mSourceVector[0] = 0.0;
        if (mAdmittanceMatrix[0] != 0.0) {
            mAdmittanceMatrix[0] = 0.0;
            mAdmittanceUpdate    = true;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Updates the photovoltaic sections in their environment, and from their outputs
///           computes the average array performance parameters for this time step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvArray::updateArray(const double dt)
{
    /// - Update the sections, loop over the strings and sum up their short-circuit currents, and
    ///   find the highest string open-circuit voltage and maximum power.  The voltage will also be
    ///   the same for the entire array.
    double percentInsolation = 0.0;
    double isc               = 0.0;
    double voc               = 0.0;
    double vmpp              = 0.0;
    double impp              = 0.0;
    double pmpp              = 0.0;
    for (unsigned int section=0; section<mConfig.mNumSections; ++section) {
        mSections[section].update(dt);
        percentInsolation += mSections[section].getPercentInsolation();
        for (unsigned int string=0; string<mSections[section].getNumStrings(); ++string) {
            isc += mSections[section].mStrings[string].getShortCircuitCurrent();
            voc  = std::max(voc, mSections[section].mStrings[string].getOpenCircuitVoltage());
            const double stringMpp = mSections[section].mStrings[string].getMpp().mPower;
            if (stringMpp > pmpp) {
                pmpp = stringMpp;
                vmpp = mSections[section].mStrings[string].getMpp().mVoltage;
            }
        }
    }

    /// - Loop over the strings again and sum their currents when loaded at the maximum power
    ///   point voltage.
    for (unsigned int section=0; section<mConfig.mNumSections; ++section) {
        for (unsigned int string=0; string<mSections[section].getNumStrings(); ++string) {
            impp += mSections[section].mStrings[string].predictCurrentAtVoltage(vmpp);
        }
    }

    /// - Update average array performance.
    mPercentInsolation   = percentInsolation / mConfig.mNumSections;
    mShortCircuitCurrent = isc;
    mOpenCircuitVoltage  = voc;
    mIvCornerVoltage     = vmpp;
    mIvCornerCurrent     = impp;

    /// - Update the maximum power point for the array.
    const double dI = isc - impp;
    if (dI > DBL_EPSILON and vmpp > DBL_EPSILON) {
        mMpp.mVoltage     = std::min(0.5 * isc * vmpp / dI, mIvCornerVoltage);
        const double fluxMpp = isc - dI * mMpp.mVoltage / vmpp;
        mMpp.mPower       = fluxMpp * mMpp.mVoltage;
        mMpp.mCurrent     = mMpp.mPower / std::max(DBL_EPSILON, mMpp.mVoltage);
        mMpp.mConductance = mMpp.mCurrent / std::max(DBL_EPSILON, mMpp.mVoltage);
    } else {
        mMpp.clear();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the Admittance Matrix for the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvArray::buildAdmittanceMatrix()
{
    double admittance = 0.0;
    if (mOpenCircuitSide) {
        const double dV = mOpenCircuitVoltage - mIvCornerVoltage;
        if (dV > DBL_EPSILON) {
            admittance = mIvCornerCurrent / dV;
        }
    } else if (mIvCornerVoltage > DBL_EPSILON) {
        admittance = (mShortCircuitCurrent - mIvCornerCurrent) / mIvCornerVoltage;
    }
    if (admittance != mAdmittanceMatrix[0]) {
        mAdmittanceMatrix[0] = admittance;
        mAdmittanceUpdate    = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) Not used.
/// @param[in] absoluteStep  (--) Not used.
///
/// @returns  SolutionResult  (--)  Whether this link confirms or rejects the network solution.
///
/// @details  This link only directly contributes to the network system of equations when the array
///           strings are tied to the common output node.  In that case, check if the output voltage
///           has moved between the open- or short-circuit sides.  If so, reject the solution and
///           start over.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult GunnsElectPvArray::confirmSolutionAcceptable(
        const int convergedStep __attribute__((unused)),
        const int absoluteStep __attribute__((unused)))
{
    if (mCommonStringsOutput) {
        if (mOpenCircuitSide and mPotentialVector[0] < mIvCornerVoltage) {
            mOpenCircuitSide = false;
            return REJECT;
        } else if (not mOpenCircuitSide and mPotentialVector[0] >= mIvCornerVoltage) {
            mOpenCircuitSide = true;
            return REJECT;
        }
    }
    return CONFIRM;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Not used.
///
/// @details  Computes the flows and final outputs resulting from the network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvArray::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = -mPotentialVector[0];

    /// - Actual current and power delivered to the terminal node.
    mFlux  = mSourceVector[0] - mAdmittanceMatrix[0] * mPotentialVector[0];
    mPower = mFlux * mPotentialVector[0];

    /// - Transport current to the terminal node.
    mNodes[0]->collectInflux(mFlux);

    /// - Load the strings at the output node voltage if they haven't already been loaded by a
    ///   regulator model, so that their output states will always have some values.
    if (mCommonStringsOutput) {
        if (mNodeMap[0] == getGroundNodeIndex()) {
            for (unsigned int section=0; section<mConfig.mNumSections; ++section) {
                for (unsigned int string=0; string<mSections[section].getNumStrings(); ++string) {
                    mSections[section].mStrings[string].loadAtVoltage(mTerminal.mVoltage);
                }
            }
        } else {
            for (unsigned int section=0; section<mConfig.mNumSections; ++section) {
                for (unsigned int string=0; string<mSections[section].getNumStrings(); ++string) {
                    mSections[section].mStrings[string].loadAtVoltage(mPotentialVector[0]);
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] power       (W)     Returned output load power at the given terminal voltage.
/// @param[out] conductance (1/ohm) Returned output load conductance at the given terminal voltage.
/// @param[in]  voltage     (V)     The terminal voltage to calculate outputs for.
///
/// @details  This computes the output power and load conductance that will place the array at the
///           given terminal voltage on its I-V curve.  If the given terminal voltage exceeds the
///           limits of the I-V curve, then zeroes are returned.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvArray::predictLoadAtVoltage(
        double&      power,
        double&      conductance,
        const double voltage) const
{
    if (not MsMath::isInRange(DBL_EPSILON, voltage, mOpenCircuitVoltage)) {
        power       = 0.0;
        conductance = 0.0;

    } else if (voltage < mIvCornerVoltage) {
        const double current = std::max(DBL_EPSILON, mShortCircuitCurrent
                - (mShortCircuitCurrent - mIvCornerCurrent) * voltage / mIvCornerVoltage);
        power       = current * voltage;
        conductance = current * current / power;

    } else {
        const double current = std::max(DBL_EPSILON, mIvCornerCurrent
                * (voltage - mOpenCircuitVoltage) / (mIvCornerVoltage - mOpenCircuitVoltage));
        power       = current * voltage;
        conductance = current * current / power;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  power      (W)   The output power load to apply.
/// @param[in]  shortSide  (--)  True uses the solution on the short-circuit side of maximum power.
///
/// @details  This loads the array at the given power output and on the given side of the array's
///           Maximum Power Point on its I-V performance curve.  If the given power exceeds the
///           array's maximum power output then the terminal outputs are zeroed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectPvArray::loadAtPower(const double power, const bool shortSide)
{
    if (power < DBL_EPSILON) {
        mTerminal.mVoltage     = mOpenCircuitVoltage;
        mTerminal.mConductance = 0.0;
        mTerminal.mPower       = 0.0;
        mTerminal.mCurrent     = 0.0;

    } else if (power < mMpp.mPower) {
        // Divisors can't be zero when mMppPower > 0.
        double admittance = 0.0;
        double source     = 0.0;
        double rootSign   = 0.0;
        if (shortSide) {
            admittance = (mShortCircuitCurrent - mIvCornerCurrent) / mIvCornerVoltage;
            source     = mShortCircuitCurrent;
            rootSign   = -1.0;
        } else {
            admittance = mIvCornerCurrent / (mOpenCircuitVoltage - mIvCornerVoltage);
            source     = admittance * mOpenCircuitVoltage;
            rootSign   = 1.0;
        }
        /// - Solve voltage with the quadratic equation: v = (-b +/- sqrt(bb-4ac))/2/a,
        ///   where a = admittance, b = -source, c = power.  Always use the + root for the
        ///   open-circuit side and - root for short-circuit side.
        const double bb4ac = source*source - 4.0*admittance*power;
        if (bb4ac > 0.0) {
            mTerminal.mVoltage     = std::max(DBL_EPSILON, 0.5 * (source + rootSign * sqrt(bb4ac))
                                                           / admittance);
            mTerminal.mPower       = power;
            mTerminal.mCurrent     = mTerminal.mPower / mTerminal.mVoltage;
            mTerminal.mConductance = mTerminal.mCurrent / mTerminal.mVoltage;
        } else {
            GUNNS_WARNING("cannot predict load.");
        }
    } else {
        mTerminal.clear();
    }
}
