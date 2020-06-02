/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling chemical compounds.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     (Only those  Chemical Compounds in the Defined Chemical Compounds class are available.)

 LIBRARY DEPENDENCY:
     ((properties/FluidProperties.o))

 PROGRAMMERS:
     ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-05))
 **************************************************************************************************/

#include "ChemicalCompound.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type                (--)     Type of Chemical Compound
/// @param[in]  fluidType           (--)     Corresponding type of fluid
/// @param[in]  mWeight             (1/mol)  Molecular Weight of Chemical Compound
/// @param[in]  thermoCoeffHighTemp (--)     Array of Coefficients, Applicable when T>1000 K
/// @param[in]  thermoCoeffLowTemp  (--)     Array of Coefficients, Applicable when T<1000 K
///
/// @details  Default constructs this Chemical Compound with the specified arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
ChemicalCompound::ChemicalCompound(const ChemicalCompound::Type     type,
                                   const std::string                name,
                                   const FluidProperties::FluidType fluidType,
                                   const double                     mWeight,
                                   const double*                    thermoCoeffHighTemp,
                                   const double*                    thermoCoeffLowTemp)
    :
    mType(type),
    mName(name),
    mFluidType(fluidType),
    mMWeight(mWeight),
    mThermoCoeffHighTemp(thermoCoeffHighTemp),
    mThermoCoeffLowTemp(thermoCoeffLowTemp)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Chemical Compound.
////////////////////////////////////////////////////////////////////////////////////////////////////
ChemicalCompound::~ChemicalCompound()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  True if the given ChemicalCompound is not equal to this one.
///
/// @details  Currently does not check the mThermoCoeffs.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ChemicalCompound::operator !=(const ChemicalCompound& that) const
{
    return (mType      != that.mType)
        or (mName      != that.mName)
        or (mFluidType != that.mFluidType)
        or (mMWeight   != that.mMWeight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Creates Thermodynamic Coefficient Arrays for each Compound.
////////////////////////////////////////////////////////////////////////////////////////////////////
const double DefinedChemicalCompounds::ThermoCoeffHighTempScaleCH4[] = {1.63552643E00,   1.00842795E-02, -3.36916254E-06,  5.34958667E-10, -3.15518833E-14, -1.00056455E+04,  9.99313326E+00};
const double DefinedChemicalCompounds::ThermoCoeffLowTempScaleCH4[]  = {5.14987613E00,  -1.36709788E-02,  4.91800599E-05, -4.84743026E-08,  1.66693956E-11, -1.02466476E+04, -4.64130376E+00};
const double DefinedChemicalCompounds::ThermoCoeffHighTempScaleCO[]  = {3.04848583E+00,  1.35172818E-03, -4.85794075E-07,  7.88536486E-11, -4.69807489E-15, -1.42661171E+04,  6.01709790E+00};
const double DefinedChemicalCompounds::ThermoCoeffLowTempScaleCO[]   = {3.57953347E+00, -6.10353680E-04,  1.01681433E-06,  9.07005884E-10, -9.04424499E-13, -1.43440860E+04,  3.50840928E+00};
const double DefinedChemicalCompounds::ThermoCoeffHighTempScaleCO2[] = {4.63659493E+00,  2.74131991E-03, -9.95828531E-07,  1.60373011E-10, -9.16103468E-15, -4.90249341E+04, -1.93534855E+00};
const double DefinedChemicalCompounds::ThermoCoeffLowTempScaleCO2[]  = {2.35677352E+00,  8.98459677E-03, -7.12356269E-06,  2.45919022E-09, -1.43699548E-13, -4.83719697E+04,  9.90105222E+00};
const double DefinedChemicalCompounds::ThermoCoeffHighTempScaleH2[]  = {2.93286579E+00,  8.26607967E-04, -1.46402335E-07,  1.54100359E-11, -6.88804432E-16, -8.13065597E+02, -1.02432887E+00};
const double DefinedChemicalCompounds::ThermoCoeffLowTempScaleH2[]   = {2.34433112E+00,  7.98052075E-03, -1.94781510E-05,  2.01572094E-08, -7.37611761E-12, -9.17935173E+02,  6.83010238E-01};
const double DefinedChemicalCompounds::ThermoCoeffHighTempScaleH2O[] = {2.67703787E+00,  2.97318329E-03, -7.73769690E-07,  9.44336689E-11, -4.26900959E-15, -2.98858938E+04,  6.88255571E+00};
const double DefinedChemicalCompounds::ThermoCoeffLowTempScaleH2O[]  = {4.19864056E+00, -2.03643410E-03,  6.52040211E-06, -5.48797062E-09,  1.77197817E-12, -3.02937267E+04, -8.49032208E-01};
const double DefinedChemicalCompounds::ThermoCoeffHighTempScaleH[]   = {2.50000286E+00, -5.65334214E-09,  3.63251723E-12, -9.19949720E-16,  7.95260746E-20,  2.54736589E+04, -4.46698494E-01};
const double DefinedChemicalCompounds::ThermoCoeffLowTempScaleH[]    = {2.50000000E+00,  0.00000000E+00,  0.00000000E+00,  0.00000000E+00,  0.00000000E+00,  2.54736599E+04, -4.46682853E-01};
const double DefinedChemicalCompounds::ThermoCoeffHighTempScaleO2[]  = {3.66096083E+00,  6.56365523E-04, -1.41149485E-07,  2.05797658E-11, -1.29913248E-15, -1.21597725E+03,  3.41536184E+00};
const double DefinedChemicalCompounds::ThermoCoeffLowTempScaleO2[]   = {3.78245636E+00, -2.99673415E-03,  9.84730200E-06, -9.68129508E-09,  3.24372836E-12, -1.06394356E+03,  3.65767573E+00};
const double DefinedChemicalCompounds::ThermoCoeffHighTempScaleOH[]  = {2.83864607E+00,  1.10725586E-03, -2.93914978E-07,  4.20524247E-11, -2.42169092E-15,  3.94395852E+03,  5.84452662E+00};
const double DefinedChemicalCompounds::ThermoCoeffLowTempScaleOH[]   = {3.99201543E+00, -2.40131752E-03,  4.61793841E-06, -3.88113333E-09,  1.36411470E-12,  3.61508056E+03, -1.03925458E-01};
const double DefinedChemicalCompounds::ThermoCoeffHighTempScaleO[]   = {2.54363697E+00, -2.73162486E-05, -4.19029520E-09,  4.95481845E-12, -4.79553694E-16,  2.92260120E+04,  4.92229457E+00};
const double DefinedChemicalCompounds::ThermoCoeffLowTempScaleO[]    = {3.16826710E+00, -3.27931884E-03,  6.64306396E-06, -6.12806624E-09,  2.11265971E-12,  2.91222592E+04,  2.05193346E+00};
const double DefinedChemicalCompounds::ThermoCoeffHighTempScaleHe[]  = {2.50000000E+00,  0.00000000E+00,  0.00000000E+00,  0.00000000E+00,  0.00000000E+00, -7.45375000E+02,  9.28724724E-01};
const double DefinedChemicalCompounds::ThermoCoeffLowTempScaleHe[]   = {2.50000000E+00,  0.00000000E+00,  0.00000000E+00,  0.00000000E+00,  0.00000000E+00, -7.45375000E+02,  9.28724724E-01};
const double DefinedChemicalCompounds::ThermoCoeffDefaultScale[]     = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0} ;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Defined Chemical Compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
DefinedChemicalCompounds::DefinedChemicalCompounds()
    :
    mCompoundCH2O      (ChemicalCompound::CH2O,       "CH2O",       FluidProperties::NO_FLUID,  30.0260,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundCH2CL2    (ChemicalCompound::CH2CL2,     "CH2CL2",     FluidProperties::NO_FLUID,  84.9330,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundCH4       (ChemicalCompound::CH4,        "CH4",        FluidProperties::GUNNS_CH4, 16.0425,  ThermoCoeffHighTempScaleCH4, ThermoCoeffLowTempScaleCH4),
    mCompoundCH4O      (ChemicalCompound::CH4O,       "CH4O",       FluidProperties::NO_FLUID,  32.0419,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundC2H4O     (ChemicalCompound::C2H4O,      "C2H4O",      FluidProperties::NO_FLUID,  44.0526,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundC2H6O     (ChemicalCompound::C2H6O,      "C2H6O",      FluidProperties::NO_FLUID,  46.0684,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundC3H6O     (ChemicalCompound::C3H6O,      "C3H6O",      FluidProperties::NO_FLUID,  58.0791,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundC3H10OSI  (ChemicalCompound::C3H10OSI,   "C3H10OSI",   FluidProperties::NO_FLUID,  90.1964,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundC4H4O     (ChemicalCompound::C4H4O,      "C4H4O",      FluidProperties::NO_FLUID,  68.0740,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundC4H10O    (ChemicalCompound::C4H10O,     "C4H10O",     FluidProperties::NO_FLUID,  74.1216,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundC6H6      (ChemicalCompound::C6H6,       "C6H6",       FluidProperties::NO_FLUID,  78.1118,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundC6H18O3SI3(ChemicalCompound::C6H18O3SI3, "C6H18O3SI3", FluidProperties::NO_FLUID, 222.4618,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundC7H8      (ChemicalCompound::C7H8,       "C7H8",       FluidProperties::NO_FLUID,  92.1384,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundC8H10     (ChemicalCompound::C8H10,      "C8H10",      FluidProperties::NO_FLUID, 106.1650,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundCO        (ChemicalCompound::CO,         "CO",         FluidProperties::GUNNS_CO,  28.0101,  ThermoCoeffHighTempScaleCO,  ThermoCoeffLowTempScaleCO),
    mCompoundCO2       (ChemicalCompound::CO2,        "CO2",        FluidProperties::GUNNS_CO2, 44.0095,  ThermoCoeffHighTempScaleCO2, ThermoCoeffLowTempScaleCO2),
    mCompoundH2        (ChemicalCompound::H2,         "H2",         FluidProperties::GUNNS_H2,   2.01588, ThermoCoeffHighTempScaleH2,  ThermoCoeffLowTempScaleH2),
    mCompoundH2O       (ChemicalCompound::H2O,        "H2O",        FluidProperties::GUNNS_H2O, 18.0153,  ThermoCoeffHighTempScaleH2O, ThermoCoeffLowTempScaleH2O),
    mCompoundH3PO4     (ChemicalCompound::H3PO4,      "H3PO4",      FluidProperties::NO_FLUID,  97.9952,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundH         (ChemicalCompound::H,          "H",          FluidProperties::NO_FLUID,   1.00794, ThermoCoeffHighTempScaleH,   ThermoCoeffLowTempScaleH),
    mCompoundLIOH      (ChemicalCompound::LIOH,       "LIOH",       FluidProperties::NO_FLUID,  23.94834, ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundLIOHH2O   (ChemicalCompound::LIOHH2O,    "LIOHH2O",    FluidProperties::NO_FLUID,  41.96364, ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundLI2CO3    (ChemicalCompound::LI2CO3,     "LI2CO3",     FluidProperties::NO_FLUID,  73.8909,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundNH3       (ChemicalCompound::NH3,        "NH3",        FluidProperties::GUNNS_NH3, 17.0305,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundNH42HPO4  (ChemicalCompound::NH42HPO4,   "NH42HPO4",   FluidProperties::NO_FLUID, 132.0562,  ThermoCoeffDefaultScale,     ThermoCoeffDefaultScale),
    mCompoundO2        (ChemicalCompound::O2,         "O2",         FluidProperties::GUNNS_O2,  31.9988,  ThermoCoeffHighTempScaleO2,  ThermoCoeffLowTempScaleO2),
    mCompoundOH        (ChemicalCompound::OH,         "OH",         FluidProperties::NO_FLUID,  17.0073,  ThermoCoeffHighTempScaleOH,  ThermoCoeffLowTempScaleOH),
    mCompoundO         (ChemicalCompound::O,          "O",          FluidProperties::NO_FLUID,  15.9994,  ThermoCoeffHighTempScaleO,   ThermoCoeffLowTempScaleO),
    mCompoundHe        (ChemicalCompound::He,         "He",         FluidProperties::NO_FLUID,  4.00260,  ThermoCoeffHighTempScaleHe,  ThermoCoeffLowTempScaleHe),
    mCompounds()
    {
        /// - Load the chemical compounds array with the pointers to the compounds
        mCompounds[ChemicalCompound::CH2O]       = &mCompoundCH2O;
        mCompounds[ChemicalCompound::CH2CL2]     = &mCompoundCH2CL2;
        mCompounds[ChemicalCompound::CH4]        = &mCompoundCH4;
        mCompounds[ChemicalCompound::CH4O]       = &mCompoundCH4O;
        mCompounds[ChemicalCompound::C2H4O]      = &mCompoundC2H4O;
        mCompounds[ChemicalCompound::C2H6O]      = &mCompoundC2H6O;
        mCompounds[ChemicalCompound::C3H6O]      = &mCompoundC3H6O;
        mCompounds[ChemicalCompound::C3H10OSI]   = &mCompoundC3H10OSI;
        mCompounds[ChemicalCompound::C4H4O]      = &mCompoundC4H4O;
        mCompounds[ChemicalCompound::C4H10O]     = &mCompoundC4H10O;
        mCompounds[ChemicalCompound::C6H6]       = &mCompoundC6H6;
        mCompounds[ChemicalCompound::C6H18O3SI3] = &mCompoundC6H18O3SI3;
        mCompounds[ChemicalCompound::C7H8]       = &mCompoundC7H8;
        mCompounds[ChemicalCompound::C8H10]      = &mCompoundC8H10;
        mCompounds[ChemicalCompound::CO]         = &mCompoundCO;
        mCompounds[ChemicalCompound::CO2]        = &mCompoundCO2;
        mCompounds[ChemicalCompound::H2]         = &mCompoundH2;
        mCompounds[ChemicalCompound::H2O]        = &mCompoundH2O;
        mCompounds[ChemicalCompound::H3PO4]      = &mCompoundH3PO4;
        mCompounds[ChemicalCompound::H]          = &mCompoundH;
        mCompounds[ChemicalCompound::LIOH]       = &mCompoundLIOH;
        mCompounds[ChemicalCompound::LIOHH2O]    = &mCompoundLIOHH2O;
        mCompounds[ChemicalCompound::LI2CO3]     = &mCompoundLI2CO3;
        mCompounds[ChemicalCompound::NH3]        = &mCompoundNH3;
        mCompounds[ChemicalCompound::NH42HPO4]   = &mCompoundNH42HPO4;
        mCompounds[ChemicalCompound::O2]         = &mCompoundO2;
        mCompounds[ChemicalCompound::OH]         = &mCompoundOH;
        mCompounds[ChemicalCompound::O]          = &mCompoundO;
        mCompounds[ChemicalCompound::He]         = &mCompoundHe;
    }

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Chemical Compound Properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
DefinedChemicalCompounds::~DefinedChemicalCompounds()
{
    // nothing to do
}
