####################################################################################################
## @copyright Copyright 2024 United States Government as represented by the Administrator of the
##            National Aeronautics and Space Administration.  All Rights Reserved.
##
## created: Feb 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################

#===================================================================================================
# ThermError
#===================================================================================================
## @brief:
## This class is a simple exception used for passing errors among the ThermAspectGenerate classes.
## It is used to alert the user of a thermal-aspect-specific error.
class ThermError(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value).replace('"','')

#===================================================================================================
# ThermPrinter
#===================================================================================================
## @brief:
## This is a simple utility class used to contain some ThermAspect printing methods.
class ThermPrinter():
    #===============================================================================================
    ## @brief:
    ## Public function. Opens a file for writing and checks for success.
    ## @param[in]: file     file path and name of document to open
    ## @return:    f        pointer to opened file, with write() command
    def openFileForWriting(self, file):
        try:
            f = open(file, 'w')
        except IOError, e:
            print e
            raise ThermError("Error opening file for writing: %s" % file)
        if not f:
            raise ThermError("Opening write-file not successful: %s" % file)
        return f
    #===============================================================================================
    ## @brief:
    ## Public function. Enumerates a list and prints it to the opened file in C++ syntax.
    ## @param[in]:  f_enum     pointer to opened enumeration file
    ## @param[in]:  someList   list of strings without spaces
    ## @param[in]:  title      title of enumeration
    ## @note: Enumeration will have the following format.
    ##             \verbatim
    ##             enum capacitanceLinks_test {
    ##                 TEST_AIR_1 = 0,
    ##                 TEST_CIRC_FAN_1 = 1,
    ##                 TEST_DPA_1 = 2,
    ##             };
    ##             \endverbatim
    def printEnum(self, f_enum, someList, title):
        if 0 == len(someList):
            return
        en = enumerate(someList)
        f_enum.write('enum %s {\n' % title)
        for i,string in en:
             f_enum.write('    %s = %i,\n' % (string,i) )
        f_enum.write('};\n\n')
    #===============================================================================================
    ## @brief:
    ## Public function. Prints the xml elements in root into a pretty-xml format.
    ## @param[in]:  root            entire tree of xml data
    ## @param[in]:  file            name of xml-file to generate
    ## @param[in]:  callingScript   name of top-level script that called this function
    ## @note: Xml file will print in the following format.
    ##             \verbatim
    ##             <?xml version="1.0" ?>
    ##             <!--Autogenerated by TestThermAspectGenerate.py. Do not edit directly.-->
    ##             <list>
    ##                 <node>
    ##                     <name>TEST_AIR_1</name>
    ##                     <temperature units="K">296.00</temperature>
    ##                     <capacitance units="J/K">0.00</capacitance>
    ##                 </node>
    ##                 <node>
    ##                     <name>TEST_CIRC_FAN_1</name>
    ##                     <temperature units="K">296.00</temperature>
    ##                     <capacitance units="J/K">1000.00</capacitance>
    ##                 </node>
    ##             </list>
    ##             \endverbatim
    def printThermXml(self, root, file, callingScript):
        if 0 == len(root):
            return
        ## Open file
        f = self.openFileForWriting(file)
        f.write(ThermHeaders().xml % callingScript)

        ## Loop through all links.
        for link in root:
            if len(link) > 0:
                f.write("    <%s>\n" % link.tag)
                for member in link:
                    self.printElement(f, member, 2)
                f.write("    </%s>\n" % link.tag)
            else:
                self.printElement(f, link, 1)

        ## Close up shop.
        f.write("</list>\n")
        f.close()

    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Print function. Prints one xml element, with an attribute if necessary.
    ## @param[in]:  f            opened file for writing
    ## @param[in]:  elem         xml element to print
    ## @param[in]:  tabs         # of tabs (4-space width) to print
    def printElement(self,f, elem, tabs):
        spacing = "    " * tabs

        if 0 == len(elem.attrib):
            f.write("%s<%s>%s</%s>\n" % (spacing, elem.tag, elem.text, elem.tag) )
        else:
            k,v = elem.attrib.items()[0]
            f.write('%s<%s %s="%s">%s</%s>\n' % (spacing, elem.tag, k, v, elem.text, elem.tag) )

#===================================================================================================
# ThermHeaders
#===================================================================================================
## @brief:
## This is a simple utility class used to contain some headers that need to be included in some
## ThermAspect generated files.
class ThermHeaders():
    xml = \
"""<?xml version="1.0" ?>
<!--Autogenerated by %s. Do not edit directly.-->
<list>
"""

    enum = \
"""/********************* TRICK HEADER **************************************************************
PURPOSE: (This c++ enumeration is #included in the SimBus.hh header. It is used so that a thermal
         link on the simbus can be referenced by its node name instead of an array index.
            Example:   ptcs.n1.therm.mCapacitanceLinks[N1_SHELL_1].mTemperature)

PROGRAMMERS: (autogenerated from "%s")
*************************************************************************************************/
"""

    icd = \
"""PUI\tSYS\tBUS\tSUBSYS\tVAR_NAME\tACTION\tRATE\t\tSIM_OBJECT\t\tSIM_VAR_NAME\t\t\t\tTYPE\tUNITS\tCOMMENTS
# This\ticd\tis auto-\tgenerated by\tthe Thermal Aspect Registry.
# Do\tnot\tedit\tdirectly!\tYou can make changes by\tediting\tthe corresponding registry files\tin %s\tand re-running %s.
"""

    tv = \
"""<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<trickView>
    <cyclePeriod>0.5</cyclePeriod>
"""
