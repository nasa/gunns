#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
# This implements a templated output of the network header (.hh) file for GUNNS Fluid networks.
# It extends the basic network template with overrides for fluid-specific output syntax.
from templates.BasicNetworkHeaderTemplate import BasicNetworkHeaderTemplate

class FluidNetworkHeaderTemplate(BasicNetworkHeaderTemplate):

  def __init__(self, data):
    BasicNetworkHeaderTemplate.__init__(self, data)
    return

  def blockIncludesPostLinks(self):
    r = ''
    if len(self.data['reactions']) > 0:
      r = r + ('#include "properties/ChemicalReaction.hh"\n')
    if len(self.data['intTcConfig']) > 0 or len(self.data['compounds']) > 0:
      r = r + ('#include "properties/ChemicalCompound.hh"\n')
    return r

  def blockPreConfig(self):
    r =('////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @brief  Data structure for ' + self.data['networkName'] + ' GUNNS network mass fractions.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'typedef struct ' + self.data['networkName'] + 'Fractions {\n')
    for constituent in self.data['intFluidConfig'][2:]:
      r = r + ('    double ' + constituent + ';\n')
    r = r + ('} ' + self.data['networkName'] + 'Fractions;\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @brief  Union for ' + self.data['networkName'] + ' GUNNS network mass fractions as scalars and as an array.\n'
        '///         The array is used as an argument in the PolyFluidInputData constructor. \n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'union ' + self.data['networkName'] + 'MassFractions {\n'
        '    ' + self.data['networkNamespace'] + self.data['networkName'] + 'Fractions scalar;\n'
        '    double array[sizeof(' + self.data['networkNamespace'] + self.data['networkName'] + 'Fractions)/sizeof(double)];\n'
        '    ' + self.data['networkName'] + 'MassFractions(\n')
    for constituent in self.data['intFluidConfig'][2:-1]:
      r = r + ('        const double i' + constituent + ',\n')
    for constituent in self.data['intFluidConfig'][-1:]:
      r = r + ('        const double i' + constituent + ');\n')
    r = r + ('};\n'
        '\n')
    if len(self.data['intTcConfig']) > 0:
      r = r + (
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @brief  Data structure for a ' + self.data['networkName'] + ' GUNNS network trace compounds state as scalars.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'typedef struct ' + self.data['networkName'] + '_' + self.data['intTcConfig'][0] + 'Scalars {\n')
      for compound in self.data['intTcConfig'][2:]:
        r = r + ('    double ' + compound + ';\n')
      r = r + (
        '} ' + self.data['networkName'] + '_' + self.data['intTcConfig'][0] + 'Scalars;\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @brief  Union for a ' + self.data['networkName'] + ' GUNNS network trace compounds state as scalars and as an\n'
        '///         array. The array is used as an argument in various objects input data constructors. \n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'union ' + self.data['networkName'] + '_' + self.data['intTcConfig'][0] + 'State {\n'
        '    ' + self.data['networkNamespace'] + self.data['networkName'] + '_' + self.data['intTcConfig'][0] + 'Scalars scalar;\n'
        '    double array[sizeof(' + self.data['networkNamespace'] + self.data['networkName'] + '_' + self.data['intTcConfig'][0] + 'Scalars)/sizeof(double)];\n'
        '    ' + self.data['networkName'] + '_' + self.data['intTcConfig'][0] + 'State(\n')
      for compound in self.data['intTcConfig'][2:-1]:
        r = r + ('        const double i' + compound + ',\n')
      for compound in self.data['intTcConfig'][-1:]:
        r = r + ('        const double i' + compound + ');\n')
      r = r + ('};\n'
        '\n')
    for rxnCompounds in self.data['compounds']:
      r = r + (
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @brief  Data structure for reactor compound total masses as scalars.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'typedef struct ' + self.data['networkName'] + '_' + rxnCompounds[0][1] + 'Masses {\n')
      for compound in rxnCompounds[3:-1]:
        r = r + ('    double ' + compound[0] + ';\n')
      r = r + (
        '} ' + self.data['networkName'] + '_' + rxnCompounds[0][1] + 'Masses;\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @brief  Union for reactor compound masses as scalars and as an array.\n'
        '///         The array is used as an argument in the chemical reactor input data constructor. \n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'union ' + self.data['networkName'] + '_' + rxnCompounds[0][1] + 'TotalMasses {\n'
        '    ' + self.data['networkNamespace'] + self.data['networkName'] + '_' + rxnCompounds[0][1] + 'Masses scalar;\n'
        '    double array[sizeof(' + self.data['networkNamespace'] + self.data['networkName'] + '_' + rxnCompounds[0][1] + 'Masses)/sizeof(double)];\n'
        '    ' + self.data['networkName'] + '_' + rxnCompounds[0][1] + 'TotalMasses(\n')
      for compound in rxnCompounds[3:-2]:
        r = r + ('        const double i' + compound[0] + ',\n')
      for compound in rxnCompounds[-2:-1]:
        r = r + ('        const double i' + compound[0] + ');\n')
      r = r + ('};\n'
        '\n')
    return r

  def blockConfigPreSolver(self):
    r =('        /// @brief  Enumeration of the number of internal constituent fluids in this network.\n'
        '        enum {N_INTERNAL_FLUIDS = ' + self.data['intFluidConfig'][1] + '};\n')
    if len(self.data['intTcConfig']) > 0:
      r = r + (
        '        /// @brief  Number of trace compounds in this trace compounds config.\n'
        '        enum {N_' + self.data['intTcConfig'][0].upper() + ' = ' + self.data['intTcConfig'][1] + '};\n')
    for rxnReactions in self.data['reactions']:
      r = r + (
        '        /// @brief  Number of chemical reactions taking place in this chemical reactor.\n'
        '        enum {N_' + rxnReactions[0].upper() + ' = ' + rxnReactions[1] + '};\n')
    for rxnCompounds in self.data['compounds']:
      r = r + (
        '        /// @brief  Number of chemical compounds in this chemical reactor.\n'
        '        enum {N_' + rxnCompounds[0][1].upper() + ' = ' + str(int(rxnCompounds[1][1]) - 1) + '};\n')
    for extConfig in self.data['extFluidConfigs']:
      r = r +('        /// @brief  Enumeration of the number of constituent fluids in the ' + extConfig[0] + ' external network.\n'
        '        enum {N_' + extConfig[0].upper() + '_FLUIDS = ' + extConfig[1] + '};\n')
    r = r + ('        // Network fluid configurations\n'
        '        static FluidProperties::FluidType netInternalFluidTypes[' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::N_INTERNAL_FLUIDS];    /**< (--) trick_chkpnt_io(**) Network internal fluid types array. */\n')
    for extConfig in self.data['extFluidConfigs']:
      r = r +('        PolyFluidConfigData               ' + extConfig[0] + ';    /**< (--) trick_chkpnt_io(**) External network fluid config */\n'
        '        static FluidProperties::FluidType ' + extConfig[0] + 'FluidTypes[' + self.data['networkName'] + 'ConfigData::N_' + extConfig[0].upper() + '_FLUIDS];    /**< (--) trick_chkpnt_io(**) External network fluid types array. */\n')
    if len(self.data['intTcConfig']) > 0:
      r = r + (
        '        // Trace compounds data\n'
        '        static ChemicalCompound::Type ' + self.data['intTcConfig'][0] + 'Types[' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::N_' + self.data['intTcConfig'][0].upper() + '];    /**< (--) trick_chkpnt_io(**) ' + self.data['intTcConfig'][0] + ' chemical compounds list. */\n'
        '        GunnsFluidTraceCompoundsConfigData ' + self.data['intTcConfig'][0] + ';    /**< (--) trick_chkpnt_io(**) ' + self.data['intTcConfig'][0] + ' config data. */\n')
    if len(self.data['reactions']) > 0:
      r = r + (
        '        // Chemical reactions properties\n'
        '        DefinedChemicalReactions netReactions;    /**< (--) trick_chkpnt_io(**) Network defined chemical reactions */\n')
    if len(self.data['compounds']) > 0:
      r = r + (
        '        // Chemical compounds properties\n'
        '        DefinedChemicalCompounds netCompounds;    /**< (--) trick_chkpnt_io(**) Network defined chemical compounds */\n')
    for rxnReactions in self.data['reactions']:
      r = r + (
        '        static ChemicalReaction::Type ' + rxnReactions[0] + '[' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::N_' + rxnReactions[0].upper() + '];    /**< (--) trick_chkpnt_io(**) ' + rxnReactions[0] + ' chemical reactions list. */\n')
    for rxnCompounds in self.data['compounds']:
      r = r + (
        '        static ChemicalCompound::Type ' + rxnCompounds[0][1] + '[' + self.data['networkNamespace'] + self.data['networkName'] + 'ConfigData::N_' + rxnCompounds[0][1].upper() + '];    /**< (--) trick_chkpnt_io(**) ' + rxnCompounds[0][1] + ' chemical compounds list. */\n')
    return r

  def blockInputPreSpotter(self):
    r = ''
    if len(self.data['tcStates']) > 0:
      r = r + (
        '        // Trace compounds states\n')
      for tcState in self.data['tcStates']:
        r = r + (
        '        ' + self.data['networkNamespace'] + self.data['networkName'] + '_' + self.data['intTcConfig'][0] + 'State ' + tcState[0][1] + 'Values; /**< (--) trick_chkpnt_io(**) ' + tcState[0][1] + ' state values. */\n'
        '        GunnsFluidTraceCompoundsInputData ' + tcState[0][1] + '; /**< (--) trick_chkpnt_io(**) ' + tcState[0][1] + ' input data. */\n')
    r = r + ('        // Internal network fluids\n')
    for fluidState in self.data['fluidStates']:
      r = r +('        ' + self.data['networkNamespace'] + self.data['networkName'] + 'MassFractions ' + fluidState[0][1] + 'Fractions;    /**< (--) trick_chkpnt_io(**) Fluid mass fractions. */\n'
        '        PolyFluidInputData ' + fluidState[0][1] + ';    /**< (--) trick_chkpnt_io(**) Fluid input data. */\n')
    if len(self.data['compounds']) > 0:
      r = r + (
        '        // Chemical compounds\n')
    for rxnCompounds in self.data['compounds']:
      r = r + (
        '        ' + self.data['networkName'] + '_' + rxnCompounds[0][1] + 'TotalMasses ' + rxnCompounds[0][1] + 'Masses;    /**< (--) trick_chkpnt_io(**) ' + rxnCompounds[0][1] + ' chemical compounds mass array. */\n')
    return r

  def blockDeclarationsPreSpotters(self):
    r = ('        const DefinedFluidProperties netFluidProperties;        /**< (--) trick_chkpnt_io(**) Network defined fluid properties. */\n'
        '        PolyFluidConfigData          netInternalFluidConfig;    /**< (--) trick_chkpnt_io(**) Network internal fluid config. */\n')
    return r

  def blockDeclarationsEndPublic(self):
    r = ('        /// @brief  Returns this network\'s fluid configuration.\n'
        '        virtual const PolyFluidConfigData* getFluidConfig() const;\n')
    return r

  def blockPostClass(self):
    r = ('////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        '/// @returns  PolyFluidConfigData* (--) Pointer to this network\'s fluid configuration.\n'
        '///\n'
        '/// @details  Returns this network\'s fluid configuration.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'inline const PolyFluidConfigData* ' + self.data['networkName'] + '::getFluidConfig() const\n'
        '{\n'
        '    return &netInternalFluidConfig;\n'
        '}\n'
        '\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n')
    for constituent in self.data['intFluidConfig'][2:]:
      r = r +('/// @param[in] i' + constituent + ' (--) Mass fraction of the ' + constituent + ' constituent in the composite fluid.\n')
    r = r +('///\n'
        '/// @details  Constructs the mass fraction union from the scalar mass fraction arguments.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'inline ' + self.data['networkName'] + 'MassFractions::' + self.data['networkName'] + 'MassFractions (\n')
    for constituent in self.data['intFluidConfig'][2:-1]:
      r = r +('    const double i' + constituent + ',\n')
    for constituent in self.data['intFluidConfig'][-1:]:
      r = r +('    const double i' + constituent + ')\n')
    r = r +('{\n')
    for constituent in self.data['intFluidConfig'][2:]:
      r = r +('    scalar.' + constituent + ' = i' + constituent + ';\n')
    r = r +('}\n'
        '\n')
    for rxnCompounds in self.data['compounds']:
      r = r + (
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n')
      for compound in rxnCompounds[3:-1]:
        r = r + (
        '/// @param[in] i' + compound[0] + ' (--) Mass of the ' + compound[0] + ' compound in the reactor.\n')
      r = r + (
        '///\n'
        '/// @details  Constructs the total masses union from the scalar reactor mass arguments.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'inline ' + self.data['networkName'] + '_' + rxnCompounds[0][1] + 'TotalMasses::' + self.data['networkName'] + '_' + rxnCompounds[0][1] + 'TotalMasses(\n')
      for compound in rxnCompounds[3:-2]:
        r = r + (
        '    const double i' + compound[0] + ',\n')
      for compound in rxnCompounds[-2:-1]:
        r = r + (
        '    const double i' + compound[0] + ')\n')
      r = r + ('{\n')
      for compound in rxnCompounds[3:-1]:
        r = r + (
        '    scalar.' + compound[0] + '= i' + compound[0] + ';\n')
      r = r + (
        '}\n'
        '\n')
    if len(self.data['intTcConfig']) > 0:
      r = r + (
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n')
      for compound in self.data['intTcConfig'][2:]:
        r = r + (
        '/// @param[in] i' + compound + ' (--) Value of the ' + compound + ' trace compound state.\n')
      r = r + (
        '///\n'
        '/// @details  Constructs the trace compounds state union from the scalar value arguments.\n'
        '////////////////////////////////////////////////////////////////////////////////////////////////////\n'
        'inline ' + self.data['networkName'] + '_' + self.data['intTcConfig'][0] + 'State::' + self.data['networkName'] + '_' + self.data['intTcConfig'][0] + 'State(\n')
      for compound in self.data['intTcConfig'][2:-1]:
        r = r + ('    const double i' + compound + ',\n')
      for compound in self.data['intTcConfig'][-1:]:
        r = r + ('    const double i' + compound + ')\n')
      r = r + (
        '{\n')
      for compound in self.data['intTcConfig'][2:]:
        r = r + ('    scalar.' + compound + ' = i' + compound + ';\n')
      r = r + (
        '}\n'
        '\n')
    return r

