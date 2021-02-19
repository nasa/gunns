/**
@file
@brief    GUNNS Fluid Shadow Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Fluid Shadow)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (GunnsFluidPotential.o)
   )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2012-01) (Initial Prototype))
    )
*/
#include "GunnsFluidShadow.hh"
#include "aspects/fluid/fluid/PolyFluid.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                  (--)  Link name
/// @param[in] nodes                 (--)  Network nodes array
/// @param[in] maxConductivity       (m2)  Maximum possible effective conductivity of the link
/// @param[in] expansionScaleFactor  (--)  Isentropic gas expansion scale factor
/// @param[in] controlTarget         (--)  The value the control must match to activate shadowing
/// @param[in] inputMixtureType      (--)  Type of the input fluid constituent partials
/// @param[in] outputMixtureType     (--)  Type of the output fluid constituent partials
/// @param[in] externalConfig        (--)  Pointer to the external network fluid config
/// @param[in] convertToType         (--)  Fluid type to convert extra external constituents into
///
/// @details  Default GUNNS Fluid Shadow Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidShadowConfigData::GunnsFluidShadowConfigData(
        const std::string&            name,
        GunnsNodeList*                nodes,
        const double                  maxConductivity,
        const double                  expansionScaleFactor,
        const int                     controlTarget,
        GunnsFluidUtils::MixtureTypes inputMixtureType,
        GunnsFluidUtils::MixtureTypes outputMixtureType,
        PolyFluidConfigData*          externalConfig,
        FluidProperties::FluidType    convertToType)
    :
    GunnsFluidPotentialConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mControlTarget               (controlTarget),
    mInputMixtureType            (inputMixtureType),
    mOutputMixtureType           (outputMixtureType),
    mExternalConfig              (externalConfig),
    mConvertToType               (convertToType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  GUNNS Fluid Shadow Config Data Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidShadowConfigData::GunnsFluidShadowConfigData(
        const GunnsFluidShadowConfigData& that)
    :
    GunnsFluidPotentialConfigData(that),
    mControlTarget               (that.mControlTarget),
    mInputMixtureType            (that.mInputMixtureType),
    mOutputMixtureType           (that.mOutputMixtureType),
    mExternalConfig              (that.mExternalConfig),
    mConvertToType               (that.mConvertToType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Shadow Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidShadowConfigData::~GunnsFluidShadowConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)  Blockage malfunction flag
/// @param[in] malfBlockageValue (--)  Blockage malfunction fractional value (0-1)
/// @param[in] sourcePressure    (kPa) Initial pressure rise of the link
///
/// @details  Default GUNNS Fluid Shadow Input Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidShadowInputData::GunnsFluidShadowInputData(const bool   malfBlockageFlag,
                                                     const double malfBlockageValue,
                                                     const double sourcePressure)
    :
    GunnsFluidPotentialInputData(malfBlockageFlag, malfBlockageValue, sourcePressure)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy
///
/// @details  Copy constructs this Fluid Shadow input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidShadowInputData::GunnsFluidShadowInputData(const GunnsFluidShadowInputData& that)
    :
    GunnsFluidPotentialInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Shadow Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidShadowInputData::~GunnsFluidShadowInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Shadow Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidShadow::GunnsFluidShadow()
    :
    GunnsFluidPotential (),
    mTransformMap       (0),
    mShadowActiveControl(0),
    mControlTarget      (0),
    mShadowIsActive     (false),
    mFirstPassFlag      (false),
    mExternalConfig     (0),
    mInputMixtureType   (GunnsFluidUtils::MASS_FRACTION),
    mOutputMixtureType  (GunnsFluidUtils::MASS_FRACTION),
    mInputPressure      (0.0),
    mInputTemperature   (0.0),
    mInputMixture       (0),
    mOutputPressure     (0.0),
    mOutputTemperature  (0.0),
    mOutputMixture      (0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Shadow Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidShadow::~GunnsFluidShadow()
{
    TS_DELETE_ARRAY(mOutputMixture);
    TS_DELETE_ARRAY(mInputMixture);
    {
        delete [] mTransformMap;
        mTransformMap = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to link Config Data
/// @param[in]     inputData    (--) Reference to link Input Data
/// @param[in,out] networkLinks (--) Network links vector
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
///
/// @throws   TsInitializationException
///
/// @details  This method initializes this Fluid Shadow link with configuration and input data.
///
/// @note     Because mInputMassFractions and mOutputMassFractions are dynamically allocated here,
///           their RATE entries in the sim bus ICD must use phasing to run their sim bus init job
///           after the Gunns network manager's init job.  For example, use P61000 ECLSS_RATE
///           instead of just ECLSS_RATE.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidShadow::initialize(const GunnsFluidShadowConfigData& configData,
                                  const GunnsFluidShadowInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&     networkLinks,
                                  const int                         port0,
                                  const int                         port1)
{
    /// - Initialize & validate parent.
    GunnsFluidPotential::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag
    mInitFlag = false;

    /// - Initialize from config data.
    mControlTarget     = configData.mControlTarget;
    mExternalConfig    = configData.mExternalConfig;
    mInputMixtureType  = configData.mInputMixtureType;
    mOutputMixtureType = configData.mOutputMixtureType;

    /// - Allocate memory and build the transform map array.
    delete [] mTransformMap;
    mTransformMap = new int[configData.mExternalConfig->mNTypes + 1];

    GunnsFluidUtils::buildTransformMap(mTransformMap, configData.mExternalConfig,
            mNodes[1]->getFluidConfig(), configData.mConvertToType);

    /// - Allocate memory for the read & write data mass fraction arrays.
    TS_DELETE_ARRAY(mInputMixture);
    TS_DELETE_ARRAY(mOutputMixture);
    TS_NEW_PRIM_ARRAY_EXT(mInputMixture,  configData.mExternalConfig->mNTypes,  double, std::string(mName) + ".mInputMixture");
    TS_NEW_PRIM_ARRAY_EXT(mOutputMixture, mNodes[1]->getFluidConfig()->mNTypes, double, std::string(mName) + ".mOutputMixture");

    /// - Initialize the mass fraction arrays to (1.0, 0.0, ...)
    mInputMixture[0] = 1.0;
    for (int i=1; i<configData.mExternalConfig->mNTypes; ++i) {
        mInputMixture[i] = 0.0;
    }

    mOutputMixture[0] = 1.0;
    for (int i=1; i<mNodes[1]->getFluidConfig()->mNTypes; ++i) {
        mOutputMixture[i] = 0.0;
    }

    /// - Validate initialization.
    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Validates this GUNNS Fluid Shadow initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidShadow::validate() const
{
    /// - There is currently nothing to validate.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidShadow::restartModel()
{
    /// - Reset the base class.
    GunnsFluidPotential::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles the incoming external fluid properties read from the sim bus.  Data is moved
///          from the sim bus input members into the link internal members.  If the external fluid
///          properties update at a different rate than this network, we always want their latest
///          properties regardless of how often it runs, therefore we don't use a queue in the sim
///          bus.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidShadow::processInputs()
{
    /// - The input fluid properties are only copied into the node if shadowing is active.  This
    ///   occurs when the control flag equals the mControlTarget value, allowing control to be
    ///   applied on any value as desired.  This allows a pair of GunnsFluidShadow links to
    ///   alternately shadow each other using the same input term.
    //TODO replace first pass flag with a better method to get sim bus data before we execute
    if (mFirstPassFlag) {
        mShadowIsActive = (mShadowActiveControl == mControlTarget);

        /// - Convert the incoming mixture information into partial masses.
        switch (mInputMixtureType) {
            case GunnsFluidUtils::MOLE_FRACTION :
                GunnsFluidUtils::convertMoleFractionToMassFraction(mInputMixture, mInputMixture,
                                                                   mExternalConfig);
                break;

            case GunnsFluidUtils::PARTIAL_PRESSURE :
                GunnsFluidUtils::convertPartialPressureToMoleFraction(mInputMixture, mInputMixture,
                                                                      mExternalConfig);
                GunnsFluidUtils::convertMoleFractionToMassFraction(mInputMixture, mInputMixture,
                                                                   mExternalConfig);
                break;

            default :
                break; // do nothing
        }

        if (mShadowIsActive) {
            setSourcePressure(mInputPressure);

            try {
                GunnsFluidUtils::transformState(mNodes[1]->getContent(), mInputPressure,
                        mInputTemperature, mInputMixture, mTransformMap);
            } catch (TsOutOfBoundsException &e) {
                GUNNS_WARNING("caught exception from PolyFluid class, most likely due to bad " <<
                              "external fluid data.");
            }

            /// - The transformState method assumes there is no quantity in the output fluid, so we
            ///   need to re-establish the node's mass.
            mNodes[1]->updateMass();
        }
    } else {
        mShadowIsActive = false;
        mFirstPassFlag  = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles data written to the external network's supply link, via the sim bus.  Data is
///          moved from the demand node into the sim bus output members.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidShadow::processOutputs()
{
    mOutputPressure    = mNodes[1]->getPotential();
    mOutputTemperature = mNodes[1]->getContent()->getTemperature();

    for (int i = 0; i < mNodes[1]->getContent()->getNConstituents(); ++i) {
        mOutputMixture[i] = mNodes[1]->getContent()->getMassFraction(i);
    }

    /// - Convert the outgoing mixture information from mass fractions to the desired parameter.
    switch (mOutputMixtureType) {
        case GunnsFluidUtils::MOLE_FRACTION :
            GunnsFluidUtils::convertMassFractionToMoleFraction(mOutputMixture, mOutputMixture,
                                                               mNodes[1]->getFluidConfig());
            break;

        case GunnsFluidUtils::PARTIAL_PRESSURE :
            GunnsFluidUtils::convertMassFractionToMoleFraction(mOutputMixture, mOutputMixture,
                                                               mNodes[1]->getFluidConfig());
            GunnsFluidUtils::convertMoleFractionToPartialPressure(mOutputMixture, mOutputMixture,
                                                                  mNodes[1]->getFluidConfig(),
                                                                  mOutputPressure);
            break;

        default :
            break; // do nothing
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Updates the State of the Link during the step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidShadow::updateState(const double dt __attribute__((unused)))
{
    /// - When shadowing is active, conductivity should be a reasonably high value (1.0E10 or so) to
    ///   produce a perfect pressure source.
    if (mShadowIsActive) {
        mEffectiveConductivity = mMaxConductivity;
    }

    /// - When shadowing is inactive, conductivity is zeroed to disable the pressure source effect
    ///   on the node.
    else {
        mEffectiveConductivity = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @returns  bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidShadow must map port 0 to the network vacuum node.
///           - A GunnsFluidShadow must not map port 1 to the network vacuum node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidShadow::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 1 is the vacuum node.
    if ((1 == port) && (node == getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 1 to the boundary node.");
        result = false;
    }

    /// - Fail if port 0 is not the vacuum node.
    else if ((0 == port) && (node != getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: must assign port 0 to the boundary node.");
        result = false;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] pressure    (kPa) The pressure of the input fluid
/// @param[in] temperature (K)   The temperature of the input fluid
/// @param[in] mixture     (--)  The generic constituent partials of the input fluid
///
/// @throws   TsOutOfBoundsException
///
/// @details  Sets the input fluid properties equal to an external supply, as an alternative to
///           using the sim bus.  These fluid properties will be processed by the processInputs
///           method just as if they had been set via the sim bus prior to the network step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidShadow::setInputs(const double  pressure,
                                 const double  temperature,
                                 const double* mixture)
{
    mInputPressure       = pressure;
    mInputTemperature    = temperature;

    if (mixture) {
        // mTransformMap[0] defines the size of the externally-supplied mixtures array.
        for (int i=0; i<mTransformMap[0]; ++i) {
            mInputMixture[i] = mixture[i];
        }
    } else {
        /// - Throw an exception if the supplied mixture array is null.
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Input Argument",
                    "null mixture array pointer supplied.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] pressure    (kPa) The pressure of the input fluid
/// @param[in] temperature (K)   The temperature of the input fluid
/// @param[in] mixture     (--)  The generic constituent partials of the input fluid
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the output fluid properties by argument, as an alternative to using the sim
///           bus.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidShadow::getOutputs(double& pressure,
                                  double& temperature,
                                  double* mixture) const
{
    pressure    = mOutputPressure;
    temperature = mOutputTemperature;

    if (mixture) {
        for (int i = 0; i < mNodes[1]->getContent()->getNConstituents(); ++i) {
            mixture[i] = mOutputMixture[i];
        }
    } else {
        /// - Throw an exception if the supplied mixture array is null.
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Input Argument",
                    "null mixture array pointer supplied.");
    }
}
