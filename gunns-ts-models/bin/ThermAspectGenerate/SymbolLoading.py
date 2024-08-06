# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
####################################################################################################
## created: Feb 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
from XmlParsing import XmlParser, TagNotFound
from ThermSupport import ThermError

#===================================================================================================
# SymbolLoader
#===================================================================================================
## @brief:  This class is used to parse an xml-file that defines the symbols used in the
##          Thermal Aspect Registry. The symbols can be defined with other symbols, as long as no
##          circular references exist.
## @note:   The symbols file must be in the following format.
##             \verbatim
##             <?xml version="1.0" ?>
##             <list>
##                 <symbol>
##                     <name>cap_generic</name>
##                     <exp>1000</exp>
##                 </symbol>
##                 <symbol>
##                     <name>cap_fan</name>
##                     <exp>cap_generic * 2</exp>
##                 </symbol>
##             </list>
##             \endverbatim
class SymbolLoader():
    ## @brief:
    ## Constructs the SymbolLoader and creates a XmlParser object.
    def __init__(self):
        ## An XmlParser() object for getting data from xml elements.
        self.mParser = XmlParser()

        ## Stores name of current file being parsed.
        self.mCurrentFile = "[mCurrentFile not set]"

    #===============================================================================================
    ## @brief:
    ## Main public function. Creates a dictionary based on symbols defined in the xml file.
    ## @param[in]: symFiles   name of xml-file containing symbols data
    ## @return:    a dictionary with key = symbol, value = resulting numerical value of symbol
    def execute(self, symFiles):

        masterNameList = []
        masterExpList = []

        for file in symFiles:
            ## Read each file.
            [nameList, expList, desList, groupList] = self.loadSymbolsFrom(file)

            ## Update master lists
            masterNameList = masterNameList + nameList
            masterExpList = masterExpList + expList

        return self.defineSymbols(masterNameList, masterExpList,locals() )

    # ----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method. Defines a symbol list given an expression list.
    ## @param[in]: symFile       name of xml-file containing symbols data
    ## @param[in]: areDesAndGroupIncluded    a boolean to specify if the group/des tags need to be
    ##                                       read as well (like in the generate_TD_symbols case)
    ## @return:    nameList      list of text under <name> tag in symbols xml
    ## @return:    expList       list of text under <exp> tag in symbols xml
    def loadSymbolsFrom(self, symFile):

        ## Set current file for error reporting purposes.
        self.mCurrentFile = symFile

        ## Try to open the symbol file. Open it for reading and save its xml data.
        symbols = self.mParser.loadFile(symFile)

        ## Initialize lists.
        nameList = []
        expList = []
        desList = []
        groupList = []

        ## Loop over the symbols.
        for symbol in symbols:
            ## Initialize name for error reporting.
            name = "[error reading name]"

            ## Get data.
            try:
                name = self.mParser.getChildText(symbol, "name")
                expElem = self.mParser.getElements(symbol, "exp", True, name)[0]
                expText = self.mParser.getText(expElem, name)

                if name in nameList:
                    ## If the symbol is a mass, it can overwrite a previous instance.
                    if 'mass_' == name[:5]:
                        expList[nameList.index(name)] = expText
                        continue
                    else:
                        raise ThermError("Symbol previously defined (%s)." % name)

                ## Special instructions for capacitance symbols, for mass accounting purposes.
                if 'cap_' == name[:4]:
                    try:
                        ## Read the given mass from the symbols element.
                        specHeatText = self.mParser.getChildText(expElem, "specHeat", name)
                        massText = self.mParser.getChildText(expElem, "mass", name).replace('%Cp',specHeatText)
                        expText = "(%s) * (%s)" % (massText, specHeatText)

                    except (TagNotFound), e:
                        ## Store a "-1" as an indication that no mass data was provided.
                        massText = "-1"

                    ## Append a symbol for the mass.
                    nameList.append(name.replace("cap_", "mass_"))
                    expList.append(massText)

                ## Append name and expression to list.
                nameList.append(name)
                expList.append(expText)

            except (TagNotFound, ThermError), e:
                print e,
                print "Symbol will be ignored (%s)." % name
                continue

        return [nameList, expList, desList, groupList]

    # ----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method. Defines a symbol list given an expression list.
    ## @param[in]: nameList      list of text under <name> tag in symbols xml
    ## @param[in]: expList       list of text under <exp> tag in symbols xml
    ## @param[in]: localVars     dictionary defining variables to use in evaluating <exp> data
    ## @param[in]: attemptsMax   no. of loops to resolve all <exp> expressions before quitting
    ## @return:    symMap      dictionary {symbol: evaluated expression}
    def defineSymbols(self, nameList, expList, localVars, attemptsMax=5):

        try:
            ## Initialize.
            symMap = {}
            all_defined = False
            attempts = 0

            ## Loop until all symbols have been resolved.
            while False == all_defined:

                ## If not all symbols are resolved after so many attempts, raise exception.
                if attempts > attemptsMax:
                    raise StopIteration(undefined)

                ## Reset define info.
                all_defined = True
                undefined = []

                ## Loop over symbol names in list.
                for name, exp in zip(nameList, expList):
                    ## To save time, skip those symbols who are already defined in the symMap.
                    if name in symMap:
                        continue

                    ## Try to evaluate the expression. A NameError is raised and caught if the
                    ## a symbol contained in the expression has not been defined yet.
                    try:
                        symMap[name] = self.evaluateExpression(name, exp, symMap)

                    ## If a symbol name is not recognized, then not all symbols are defined yet.
                    except NameError, detail:
                        all_defined = False
                        undefined.append(str(detail))

                ## Monitor attempts. This is to make sure you don't get stuck in infinite loop.
                attempts = attempts + 1

        except StopIteration, e:
            undefined = e.args[0]
            for u in undefined:
                print "    ",u
            raise ThermError("Cannot resolve symbols.")

        ## dictionary: {symbol: evaluated expression}
        return symMap

    #===============================================================================================
    ## @brief:
    ## Evaluate an expression value based on symbols defined symMap.
    ## @param[in]: name    name of symbol who's expression is being evaluated
    ## @param[in]: expr    a mathematical expression that may contain symbols defined in symMap
    ## @param[in]: symMap  a dictionary of currently defined symbols
    ## @return:    the evaluated expression in numerical form
    def evaluateExpression(self, name, expr, symMap):
        try:
            return eval(expr, globals(), symMap)
        except (SyntaxError), e:
            print e
            print "***File: %s." %  self.mCurrentFile
            print "***Symbol: %s." %  name
            print "***Expression: %s." %  expr
            raise ThermError("Cannot evaluate symbol expression.")
