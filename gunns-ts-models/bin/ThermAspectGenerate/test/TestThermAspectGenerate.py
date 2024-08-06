#!/usr/bin/python
#
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# programmer: joseph.valerioti@nasa.gov     Jan 2013
# =================================================================================================
import unittest,os,sys
## Set the sys.path to check the one-up directory.
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

from ThermSupport import ThermError
from ThermAspectConfiguring import ThermAspectConfig
from ThermAspectBuilding import ThermAspectBuilder
from IndivNetworkConfiguring import IndivNetworkConfig
from IndivNetworkBuilding import IndivNetworkBuilder
from XmlParsing import XmlParser

## Name of this file.
thisScript = os.path.basename(__file__)

## List all networks that are eligible to be built.
tNetwork = "test"
tNetworks = ["dummy", tNetwork]

## Declare symbols files.
tIssHome = os.environ["TS_ISS_HOME"] + '/'

## Declare file name specifics
tRegisPath = "ThermRegistry_%s.xml"
tTdPath = "%s/TdNetworkConfig_%s.xml"
tNodePath = "network/ThermNodes_%s.xml"
tCondPath = "network/ThermLinksCond_%s.xml"
tRadPath = "network/ThermLinksRad_%s.xml"
tHtrPath = "network/HtrRegistry_%s.xml"
tPanPath = "network/ThermPanels_%s.xml"
tEtcPath = "network/ThermLinksEtc_%s.xml"

## Symbol_loading module and files to read.
tSymFiles = ["symbolsTest.xml"]

## Name and path of ptcs enumeration header to generate.
tEnumFile = "TestEnum.hh"

## Declare icd_printing files.
tIcdFile = "TEXT.txt"
tTvPath = "TV_icd_%s.tv"

## Declare network specifics for testing.
tNumNodes = 14
tNumPanels = 1
tNumRadLinks = 3
tRadCoeffTd = 5.55  #Radiation coefficient should default to TD if none provided in registry.
tRadCoeffRegis = 60.0  #Radiation coefficient from registry should override that from TD.
tCapExplicit = 12345.6
tTotalMass = 60000

# -------------------------------------------------------------------------------------------------
# Redirect Stdout class (to disable printing to terminal)
# -------------------------------------------------------------------------------------------------
class SuppressOutput():
    devnull = open(os.devnull, 'w')
    stdout=devnull #sys.stdout
    stderr=devnull #sys.stderr

    def __init__(self):
        self._stdout = self.stdout or sys.stdout
        self._stderr = self.stderr or sys.stderr

    def __enter__(self):
        self.old_stdout, self.old_stderr = sys.stdout, sys.stderr
        self.old_stdout.flush(); self.old_stderr.flush()
        sys.stdout, sys.stderr = self._stdout, self._stderr

    def __exit__(self, exc_type, exc_value, traceback):
        self._stdout.flush(); self._stderr.flush()
        sys.stdout = self.old_stdout
        sys.stderr = self.old_stderr

# -------------------------------------------------------------------------------------------------
# Unit Test class
# -------------------------------------------------------------------------------------------------
class TestThermBuilder(unittest.TestCase):
    notSet = "[%s must explicitly be set]"
    uninitialized = "[%s not initialized]"

    def setUp(self):

        ## Instantiate ThermAspectConfig object.
        self.mThermAspectConfig = ThermAspectConfig()

        ## Loop through all individual networks.
        for network in tNetworks:

            ## Instantiate IndivNetworkConfig object.
            indivNetworkConfig = IndivNetworkConfig()

            ## Uniform ICD variables
            ## Name of sim system
            indivNetworkConfig.cIcdSettings.mSys = "iss"
            ## Name of icd bus
            indivNetworkConfig.cIcdSettings.mBus = "sim_bus"
            ## Name of icd trick sequence, or P-value.
            indivNetworkConfig.cIcdSettings.mTrickSeq = "P_PTCS "

            ## Name (abbreviation) of network to be built
            indivNetworkConfig.cNetwork = network

            ## Boolean to determine if the mass of structural nodes should be adjusted to match a
            ## total-mass expected value.
            indivNetworkConfig.cIsMassAdjustable = True

            #Files to read ............................................
            ## Path and file name of Thermal Aspect Registry file.
            indivNetworkConfig.cRegisFile = tRegisPath % network
            ## Path and file name of Thermal Desktop file (TdNetworkConfig).
            indivNetworkConfig.cTdFile = tTdPath % (network,network)
            ## Path and file name of HtrRegistry file.
            indivNetworkConfig.cHtrFile = tHtrPath % network
            ## Path and file name of ThermalPanel registry/config file.
            indivNetworkConfig.cPanFile = tPanPath % network

            #Files to generate ........................................
            ## Path and file name of Node configuration file.
            indivNetworkConfig.cNodeFile = tNodePath % network
            ## Path and file name of Conduction configuration file.
            indivNetworkConfig.cCondFile = tCondPath % network
            ## Path and file name of Radiation configuration file.
            indivNetworkConfig.cRadFile = tRadPath % network
            ## Path and file name of Et.Cetera (potentials, sources) configuration file.
            indivNetworkConfig.cEtcFile = tEtcPath % network
            ## Path and file name of TrickView file. Each network produces its own TrickView file.
            indivNetworkConfig.cTvFile = tTvPath % network

            ## Append the newly-constructed mIndivNetworkConfig to the list of network configs.
            self.mThermAspectConfig.cListNetworkConfigs.append(indivNetworkConfig)

            if "dummy" == network:
                #Files to read ............................................
                ## Path and file name of Thermal Aspect Registry file.
                indivNetworkConfig.cRegisFile = None
                ## Path and file name of Thermal Desktop file (TdNetworkConfig).
                indivNetworkConfig.cTdFile = None
                ## Path and file name of HtrRegistry file.
                indivNetworkConfig.cHtrFile = None
                ## Path and file name of ThermalPanel registry/config file.
                indivNetworkConfig.cPanFile = None

        ## Set the mIndivNetworkConfig member as the first in the above loop.
        self.mIndivNetworkConfig = indivNetworkConfig

        ## Name of top-level script that creates the ThermAspectBuilder that creates this object.
        self.mThermAspectConfig.cCallingScript = thisScript
        ## Path to repository home.
        self.mThermAspectConfig.cHome = tIssHome
        ## Assumption to use for any specific heat values (Cp) not provided directly.
        self.mThermAspectConfig.cAssumedCp = 1.0
        ## Set ICD file
        self.mThermAspectConfig.cIcdFile = tIcdFile
        ## Set enumeration header file.
        self.mThermAspectConfig.cEnumFile = tEnumFile
        ## Set symbol_loading module and files to read.
        self.mThermAspectConfig.cSymFiles = tSymFiles

        ## Instantiate an individual network indivPiece.
        self.indivPiece = IndivNetworkBuilder()

        ## Instantiate a full-scale article with the correct paths.
        self.article = ThermAspectBuilder()
        self.article.initialize(self.mThermAspectConfig)

    def test_11_construction(self):
        print "\n(1.1) Test generic construction.\n  ",
        newPiece = IndivNetworkBuilder()
        self.assertEqual(newPiece.mRegisFile, self.uninitialized % "mRegisFile")
        self.assertEqual(newPiece.mTdFile, self.uninitialized % "mTdFile")
        self.assertEqual(newPiece.mNodeFile, self.uninitialized % "mNodeFile")
        self.assertEqual(newPiece.mCondFile, self.uninitialized % "mCondFile")
        self.assertEqual(newPiece.mRadFile, self.uninitialized % "mRadFile")
        self.assertEqual(newPiece.mHtrFile, self.uninitialized % "mHtrFile")
        self.assertEqual(newPiece.mPanFile, self.uninitialized % "mPanFile")
        self.assertEqual(newPiece.mEtcFile, self.uninitialized % "mEtcFile")
        self.assertEqual(newPiece.mTvFile, self.uninitialized % "mTvFile")

        ## @test  Cannot execute on uninitialized article.
        with SuppressOutput():
            self.assertRaises(ThermError, newPiece.execute)

    def test_12_initialization(self):
        print "\n(1.2) Test initialize().\n  ",
        self.assertEqual(self.article.mSymFiles, tSymFiles)
        self.assertEqual(self.article.mIcdFile,  tIcdFile)
        self.assertEqual(self.article.mEnumFile, tEnumFile)

        self.assertEqual(self.article.mIndivNetworkBuilders[-1].mRegisFile, tRegisPath % tNetwork)
        self.assertEqual(self.article.mIndivNetworkBuilders[-1].mTdFile, tTdPath % (tNetwork,tNetwork))
        self.assertEqual(self.article.mIndivNetworkBuilders[-1].mNodeFile, tNodePath % tNetwork)
        self.assertEqual(self.article.mIndivNetworkBuilders[-1].mCondFile, tCondPath % tNetwork)
        self.assertEqual(self.article.mIndivNetworkBuilders[-1].mRadFile, tRadPath % tNetwork)
        self.assertEqual(self.article.mIndivNetworkBuilders[-1].mHtrFile, tHtrPath % tNetwork)
        self.assertEqual(self.article.mIndivNetworkBuilders[-1].mPanFile, tPanPath % tNetwork)
        self.assertEqual(self.article.mIndivNetworkBuilders[-1].mEtcFile, tEtcPath % tNetwork)
        self.assertEqual(self.article.mIndivNetworkBuilders[-1].mTvFile, tTvPath % tNetwork)

    def test_21_no_tdNetworkConfig_file(self):
        print "\n(2.1) Test exception when tdNetworkConfig file cannot be loaded.\n  ",
        with SuppressOutput():
            self.mIndivNetworkConfig.cTdFile = "td_file_doesnt_exist"
            self.indivPiece.initialize(self.mIndivNetworkConfig)
            self.assertRaises(ThermError, self.indivPiece.execute)

    def test_22_no_ThermRegistry_file(self):
        print "\n(2.2) Test exception when ThermRegistry file cannot be loaded.\n  ",
        with SuppressOutput():
            self.mIndivNetworkConfig.cRegisFile = "registry_file_doesnt_exist"
            self.indivPiece.initialize(self.mIndivNetworkConfig)
            self.assertRaises(ThermError, self.indivPiece.execute)

    def test_23_execute_before_initialize(self):
        print "\n(2.3) Test exception when IndivNetworkBuilder executed before initialized.\n  ",
        with SuppressOutput():
            self.assertRaises(ThermError, self.indivPiece.execute)

    def test_24_registry_not_well_formed(self):
        print "\n(2.4) Test exception when ThermRegistry not well formed.\n  ",
        with SuppressOutput():
            self.mIndivNetworkConfig.cRegisFile = "ThermRegistry_notwellformed.xml"
            self.indivPiece.initialize(self.mIndivNetworkConfig)
            self.assertRaises(ThermError, self.indivPiece.execute)

    def test_31_circular_symbols(self):
        print "\n(3.1) Test exception when circular reference in symbols file.\n  ",
        with SuppressOutput():
            ## Include a symbols file with a circular reference.
            self.mThermAspectConfig.cSymFiles = ["symbolsTest.xml", "symbolsCircular.xml"]
            newArticle = ThermAspectBuilder()
            self.assertRaises(ThermError, newArticle.initialize, self.mThermAspectConfig)

    def test_41_bad_destination_for_icd_file(self):
        print "\n(4.1) Test exception with a non-writable destination for ICD file.\n  ",
        with SuppressOutput():
            self.mThermAspectConfig.cIcdFile = "/users/delete_me.txt"
            newArticle = ThermAspectBuilder()
            newArticle.initialize(self.mThermAspectConfig)
            self.assertRaises(ThermError, newArticle.execute)

    def test_51_execution(self):
        print "\n(5.1) Test execution.\n  ",

        capExplicit = "0"
        radEntries = None
        radCoeffRegis = ""
        radCoeffTd = ""
        try:
            self.article.execute()

            ## Check mass calculation
            parser = XmlParser()
            data = parser.loadFile(tNodePath % tNetwork)
            ## Open it for reading and save its xml data.
            entries = parser.getElements(data, "node")

            ## Loop through each source entry, totaling the mass.
            massSum = 0;
            for entry in entries:
                ## Create and initialize XmlEntryAnalyzer object.
                mass = parser.getChildText(entry, "mass")
                massSum = massSum + float(mass)

                ## Store capacitance of a specific node in question.
                if "GOOD_EXPLICIT_12" == parser.getChildText(entry, "name"):
                    capExplicit = parser.getChildText(entry, "capacitance")

            ## Read radiation data for testing.
            radData = parser.loadFile(tRadPath % tNetwork)
            radEntries = parser.getElements(radData, "radiation")
            ## Check radiation coefficients.
            radCoeffTd = parser.getChildText(radEntries[0], "coefficient")
            radCoeffRegis = parser.getChildText(radEntries[1], "coefficient")

        except (Exception), e:
            print e
            self.fail("Uncaught exception during execute().")

        ## Test correct number of nodes.
        self.assertEqual(len(self.article.mIndivNetworkBuilders[-1].masterNodeList), tNumNodes)

        ## Test capacitance of node that should have been overwritten by an explicity symbol.
        self.assertAlmostEqual(float(capExplicit), tCapExplicit)

        ## Test that mass was appropriately scaled to match the given total.
        self.assertAlmostEqual(massSum, tTotalMass, 1)

        ## Test correct number of panels.
        self.assertEqual(len(self.article.mIndivNetworkBuilders[-1].mPanList), tNumPanels)

        ## Test correct number of radiation links.
        self.assertEqual(len(radEntries), tNumRadLinks)

        ## Test correct radiation coefficients.
        self.assertAlmostEqual(float(radCoeffRegis), tRadCoeffRegis)
        self.assertAlmostEqual(float(radCoeffTd), tRadCoeffTd)

# =================================================================================================
# Primary function
# =================================================================================================
if __name__ == "__main__":

    print "----------------------------------------------------------------------"
    print "Testing: build_thermal_aspects.py"
    unittest.main()
