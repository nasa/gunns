#!/usr/bin/python
# @copyright Copyright 2024 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
import os
import sys
import collections
import re
import unicodedata

# Import Tkinter for Python 2.7 vs. 3 imports by feature detection.
# Since Tkinter isn't installed on many platforms, and it's only needed
# if the drawing filename isn't supplied from the command line, then
# if the user doesn't have Tkinter, we'll quietly not import it, then
# abort if it turns out we needed it later.  This allows use of this
# script without Tkinter installed as long as the filename is provided.
try:
    import Tkinter as TK
except ImportError:
    try:
        import tkinter as TK
    except ImportError:
        pass

try:
    import tkFileDialog as TKFILE
except ImportError:
    try:
        import tkinter.filedialog as TKFILE
    except ImportError:
        pass

try:
    import tkMessageBox as TKMBOX
except ImportError:
    try:
        import tkinter.messagebox as TKMBOX
    except ImportError:
        pass

# More imports:
from argparse import ArgumentParser
from datetime import datetime
from shutil import copyfile
import xml.etree.ElementTree as ET
import modules.compression as compression
import modules.shapeLibs as shapeLibs
import modules.consoleMsg as console
import modules.xmlUtils as xmlUtils
import json
from templates.BasicNetworkHeaderTemplate import BasicNetworkHeaderTemplate
from templates.FluidNetworkHeaderTemplate import FluidNetworkHeaderTemplate
from templates.BasicNetworkBodyTemplate import BasicNetworkBodyTemplate
from templates.FluidNetworkBodyTemplate import FluidNetworkBodyTemplate

START_TIME = datetime.now()

# TODO items:
# - support namespace in exported code
# - re-factor to modules & packages
#   - modules normally only imported into the main
#   - modules written so that if they're run as main, do built-in test
# - export a Trick input file with the network default data.
#   This file will be a python class that can be extended
#   to easily tweak initial conditions of the network.
#   We don't export this yet, needs to be implemented.
#   We have addded the <gunnsShapeData> tags to the link shapes,
#   that help define the network sim variables for non-standard
#   config & input data names & types:
#   - By default, targets in input file will be the attribute name with m added to
#     front, the usual naming convention: i.e. potential -> mPotential
#   - for pointer to vectors of primitives:
#     - <gunnsShapeData source="c01" target="vector*double cFluxDistributionFractions"/>
#   - for non-default targets in input file, for instance mArray[5] comes from shape
#     data c00.array0, c01.array1, ... c04.array5:
#     - <gunnsShapeData source="c04" target="mArray[4]"/>
#     For instance iSize comes from shape data i02.size:
#     - <gunnsShapeData source="i02" target="iSize"/>

# JGraph tools for compressing/decompressing:
# https://jgraph.github.io/drawio-tools/tools/convert.html

# Returns the id of the parent of the given object.
# obj must be either a <mxCell> or an <object> containing an <mxCell>
def getParentId(obj):
    result = ''
    if obj.tag.startswith('mxCell'):
        result = obj.attrib['parent']
    elif obj.tag.startswith('object'):
        cell = obj.findall('./mxCell')[0]
        result = cell.attrib['parent']
    return result

# returns True if 'obj' is a descendant of 'of', by their id attributes.
# This can be used to find out if an object belongs within a network, either as
# an immediate child or multiple levels down.
# Note that all items in objects must ultimately have "0" as their parent or
# this will infinite loop TODO improve
def isDescendant(obj, of, objects):
    obj_id    = obj.attrib['id']
    of_id     = of.attrib['id']
    parent_id = getParentId(obj)
    while '0' != parent_id:
        for an_object in objects:
            if parent_id == an_object.attrib['id']:
                obj_id    = an_object.attrib['id']
                parent_id = getParentId(an_object)
                break;
        if of_id == obj_id:
            return True
    return False

# If obj's (an <object>) parent is in cells, then return that cell, else None
def getParentCell(obj, cells):
    obj_parent_id = getParentId(obj)
    for cell in cells:
        if obj_parent_id == cell.attrib['id']:
            return cell
    return None

# Returns the config data from the given <object> attributes as a comma-delimited string
# Values enclosed with curly braces are replaced with zero, as vectors are handled later.
def getConfigData(attr):
    result = ''
    # attr is an unsorted dictionary, so first sort it
    od = collections.OrderedDict(sorted(attr.items()))
    # Config attributes start with 'c#'
    for key, value in od.items():
        if None != re.search('^c([0-9])', key):
            if value.startswith('{') and value.endswith('}'):
                result = result + ', 0'
            else:
                result = result + ', ' + value
    return result

# same as getConfigData except input attributes start with 'i#'
def getInputData(attr):
    result = ''
    od = collections.OrderedDict(sorted(attr.items()))
    for key, value in od.items():
        if None != re.search('^i([0-9])', key):
            value_to_use = value
            if value.startswith('{') and value.endswith('}'):
                value_to_use = '0'
            if '' == result:
                result = result + value_to_use
            else:
                result = result + ', ' + value_to_use
    return result

# Returns the link's name to be used in the exported network code.
def getLinkName(link):
    return link.attrib['label']

# Strips out any xml tags from the given object label attribute.  If there were any,
# stores the cleaned version back into the attribute and flags the content change for
# the drawing maintenance updates.
def cleanLabel(object):
    label = object.attrib['label']
    clean_label = re.sub('<.*?>', '', label)
    if (label != clean_label):
        print('    ' + console.note('cleaned shape name from ' + label + ' to ' + clean_label + '.'))
        object.attrib['label'] = clean_label
        return True
    return False

# Returns as a string the given link's or spotter's config or input data constructor body
# for loading vectors.
# Note this works for links and spotters.
# The dataType argument is 'c' or 'i' which determines config vs. input data.
def getLinkConstructorBody(link, dataType):
    result  = ''
    vectors = []
    shapeDatas = link.findall('./gunnsShapeData')
    for shapeData in shapeDatas:
        source = shapeData.attrib['source']
        if re.search('^' + dataType + '([0-9])', source):
            if shapeData.attrib['target'].startswith('vector*'):
                vectors.append(shapeData)
    link_attr = link.attrib
    linkName  = getLinkName(link)
    for vector in vectors:
        target_split = vector.attrib['target'].split(' ')
        target_type  = target_split[0].split('*')[-1]
        target       = target_split[-1]
        od           = collections.OrderedDict(sorted(link_attr.items()))
        for key, value in od.items():
            if key.startswith(vector.attrib['source']):
                config_value = value
                break;
        result = result + ('    {\n'
                           '        const ' + target_type + ' array[] = ' + config_value + ';\n'
                           '        ' + linkName + '.' + target + '.assign(array, array + sizeof(array) / sizeof(' + target_type + '));\n'
                           '    }\n')
    return result

# Formats the link port map as a comma-separated string.
def getPortMap(port_map):
    result = str(port_map[0])
    for index in range(1, len(port_map)):
        result = result + ', ' + str(port_map[index])
    return result

# Formats the link port map for its initialize call.
def getPortMapInitialize(port_map):
    result = ''
    for port in port_map:
        if int(port) < 0:
            result = result + ', GROUND + groundOffset'
        else:
            result = result + ', Node' + port + ' + netSuperNodesOffset'
    return result

# Builds the link's initialize call, depending on whether it is a fixed-
# (numPorts > 0) or variable-port (numPorts == 0) link
def getLinkInitialize(link, port_map):
    result = ''
    name     = getLinkName(link)
    numPorts = int(link.find('./gunns').attrib['numPorts'])
    formatted_port_map = getPortMapInitialize(port_map)
    if numPorts > 0:
        result = ('    ' + name + '.initialize(netConfig.' + name + ', netInput.' + name + ', netLinks' + formatted_port_map + ');\n')
    else:
        # strip off the begining ', ' from formatted_port_map
        formatted_port_map = formatted_port_map[2:]
        result = ('    {\n'
            '        const int array[] = {' + formatted_port_map + '};\n'
            '        std::vector<int> vector (array, array + sizeof(array) / sizeof(int));\n'
            '        ' + name + '.initialize(netConfig.' + name + ', netInput.' + name + ', netLinks, &vector);\n'
            '    }\n')
    return result

# Given an object and a list of cells, returns a list of the value attribute of
# the set of cells that have the object as their parent.
# The 1st row in the list holds the object's name.
# The 2nd row in the list holds the number of items in the list, not including
# the 1st and 2nd rows
def buildSwimlane1D(obj, cells):
    result = []
    result.append(obj.attrib['label'])
    result.append(' ')
    for cell in cells:
        if obj.attrib['id'] == cell.attrib['parent']:
            result.append(cell.attrib['value'])
    result[1] = str(len(result)-2)
    return result

# Similar to 1D, except the cells that have the object as their parent are the
# Y value of each row, and each cell that has a Y cell as its parent is the Y's
# corresponding X value.  Then we return the 2D table as a list of (X,Y) tuples.
# The 1st row in the table holds the object's name.
# The 2nd row in the list holds the number of items in the list, not including
# the 1st and 2nd rows
def buildSwimlane2D(obj, cells):
    result = []
    result.append(('Name', obj.attrib['label']))
    result.append(('Size', ' '))
    # Find the Y cells
    y_ids  = []
    y_vals = []
    for cell in cells:
        if obj.attrib['id'] == cell.attrib['parent']:
            y_ids.append(cell.attrib['id'])
            y_vals.append(cell.attrib['value'])
    # Find the X cells and append the (X,Y) tuple.  This will break if there
    # are more than one cell with the same Y parent.  This might happen if
    # the user inserted another object into the same row.
    for y in range(0, len(y_ids)):
        for cell in cells:
            if y_ids[y] == cell.attrib['parent']:
                result.append((cell.attrib['value'], y_vals[y]))
    result[1] = ('Size', str(len(result)-2))
    return result

# Similar to getPortTargetName but restricted to links
def getPortLinkName(port, links):
    result = ''
    source = ''
    target = ''
    cell_attr = port.find('./mxCell').attrib
    if 'source' in cell_attr:
        source = cell_attr['source']
    if 'target' in cell_attr:
        target = cell_attr['target']
    for link in links:
        if source == link.attrib['id'] or target == link.attrib['id']:
            return 'link ' + getLinkName(link)
    return result

# Similar to getPortTargetName but restricted to nodes
def getPortNodeName(port, nodes, gnds):
    result = ''
    source = ''
    target = ''
    cell_attr = port.find('./mxCell').attrib
    if 'source' in cell_attr:
        source = cell_attr['source']
    if 'target' in cell_attr:
        target = cell_attr['target']
    for gnd in gnds:
        if source == gnd.attrib['id'] or target == gnd.attrib['id']:
            return 'Ground'
    for node in nodes:
        if source == node.attrib['id'] or target == node.attrib['id']:
            return 'node ' + node.attrib['label']
    return result

# Similar to getPortNodeName but returns the node element
def getPortNode(port, nodes, gnds):
    source = ''
    target = ''
    cell_attr = port.find('./mxCell').attrib
    if 'source' in cell_attr:
        source = cell_attr['source']
    if 'target' in cell_attr:
        target = cell_attr['target']
    for gnd in gnds:
        if source == gnd.attrib['id'] or target == gnd.attrib['id']:
            return gnd
    for node in nodes:
        if source == node.attrib['id'] or target == node.attrib['id']:
            return node
    return None

# Returns the name string of the link that is connected to this port.
# If there is no link connected, then returns the connected node and
# number.  If there is no node connected then returns ''
# 'link TestConductor5' or 'node 5'
def getPortTargetName(port, links, nodes, gnds):
    result = getPortLinkName(port, links)
    if '' == result:
        result = getPortNodeName(port, links)
    return result

# Copies attributes from 'from_attr' to 'to_attr' and returns True if
# there were any resulting changes to 'to_attr'
def forceCopyAttrib(to_attr, from_attr, name):
    if name in from_attr:
        if name in to_attr:
            if to_attr[name] != from_attr[name]:
                to_attr[name] = from_attr[name]
                return True
        else:
            to_attr[name] = from_attr[name]
            return True
    return False

# Updates the config and input data in to_attr to match the keys in from_attr.
# Returns True if there were any changes.
# Logic:
#   For each c##.term in 'from' (master shape), see if there's a match in 'to'
#   (object being updated).  If not:
#     Look for just the .term in 'to' with a different c##. If so:
#       rename the c## part in 'to', keep it's old value
#     else:
#       add the c##.term="value" from 'from' to 'to'
#   Now for each c##.term in 'to', look for a match in 'from'.  If not:
#     delete the attr from 'to'
def updateConfigInputData(to_attr, from_attr, obj_type):
    updated = False
    from_od = collections.OrderedDict(sorted(from_attr.items()))
    for from_key, from_value in from_od.items():
        if None != re.search('^([c,i])([0-9])', from_key):
            if from_key not in to_attr.keys():
                ciNum = from_key.split('.')[0] + '.'
                term  = from_key[len(ciNum):]
                found = False
                for to_key, to_value in to_attr.items():
                    if to_key.endswith(term):
                        found = True
                        print('    ' + console.note('moved shape data: ' + from_key + ' in ' + obj_type + ': ' + to_attr['label'] + '.'))
                        to_attr[from_key] = to_value
                        del to_attr[to_key]
                        updated = True
                        break
                if not found:
                    print('    ' + console.note('added shape data: ' + from_key + ' to ' + obj_type + ': ' + to_attr['label'] + '.'))
                    to_attr[from_key] = from_value
                    updated = True
    to_od = collections.OrderedDict(sorted(to_attr.items()))
    for to_key, to_value in to_od.items():
        if None != re.search('^([c,i])([0-9])', to_key):
            if to_key not in from_attr.keys():
                print('    ' + console.note('deleted shape data: ' + to_key + ' from ' + obj_type + ': ' + to_attr['label'] + '.'))
                del to_attr[to_key]
                updated = True
    return updated

# This returns True, and replaces the <gunnsShapeData> elements in obj
# with the master if there are any differences between them.
def updateShapeDataTargets(obj, master):
    updated = False
    master_targets = master.findall('./gunnsShapeData')
    obj_targets    = obj.findall('./gunnsShapeData')
    if len(master_targets) != len(obj_targets):
        updated = True
    else:
        for i in range(0, len(master_targets)):
            if ET.tostring(master_targets[i]).rstrip() != ET.tostring(obj_targets[i]).rstrip():
                updated = True
                break
    if updated:
        # For any updates, we simply delete <gunnsShapeData> in obj and copy them over
        # from master.  We insert them starting at index 1 in the obj, assuming index
        # 0 has the <gunns> element.
        for obj_target in obj_targets:
            obj.remove(obj_target)
        for i in range(0, len(master_targets)):
            obj.insert(i+1, master_targets[i])
    return updated

# Performs shape updates for the given link, returns True if
# there were any changes.
def updateLinkShapeData(link, master):
    updated = False
    if None == master:
        return
    link_attr         = link.attrib
    link_gunns_attr   = link.find('./gunns').attrib
    master_attr       = master.attrib
    master_gunns_attr = master.find('./gunns').attrib
    # Do the forced-sync items: <object> About, Ports, <gunns> numPorts, reqPorts, <gunnsShapedata>
    if updateShapeDataTargets(link, master):
        print('    ' + console.note('updated shape data targets in link: ' + link_attr['label'] + '.'))
        updated = True
    if forceCopyAttrib(link_attr, master_attr, 'About'):
        print('    ' + console.note('updated shape data: About in link: ' + link_attr['label'] + '.'))
        updated = True
    if forceCopyAttrib(link_attr, master_attr, 'Ports'):
        print('    ' + console.note('updated shape data: Ports in link: ' + link_attr['label'] + '.'))
        updated = True
    if forceCopyAttrib(link_gunns_attr, master_gunns_attr, 'numPorts'):
        print('    ' + console.note('updated shape data: numPorts in link: ' + link_attr['label'] + '.'))
        updated = True
    if forceCopyAttrib(link_gunns_attr, master_gunns_attr, 'reqPorts'):
        print('    ' + console.note('updated shape data: reqPorts in link: ' + link_attr['label'] + '.'))
        updated = True
    # Do config & input data items
    if updateConfigInputData(link_attr, master_attr, 'link'):
        updated = True
    return updated

# Performs shape updates for the given spotter, returns True if
# there were any changes.
def updateSpotterShapeData(spotter, master):
    updated = False
    if None == master:
        return
    spotter_attr       = spotter.attrib
    spotter_gunns_attr = spotter.find('./gunns').attrib
    master_attr        = master.attrib
    master_gunns_attr  = master.find('./gunns').attrib
    # Do the forced-sync items: <object> About, <gunnsShapeData>
    if updateShapeDataTargets(spotter, master):
        print('    ' + console.note('updated shape data targets in spotter: ' + spotter_attr['label'] + '.'))
        updated = True
    if forceCopyAttrib(spotter_attr, master_attr, 'About'):
        print('    ' + console.note('updated shape data: About in spotter: ' + spotter_attr['label'] + '.'))
        updated = True
    # Do config & input data items
    if updateConfigInputData(spotter_attr, master_attr, 'spotter'):
        updated = True
    return updated

# Assumes obj has already been checked for a valid <gunns> type.
def checkName(obj):
    if "" == obj.attrib['label'] or obj.attrib['label'].isspace():
        obj_type    = obj.find('./gunns').attrib['type']
        obj_about   = ''
        if 'About' in obj.attrib:
            obj_about = obj.attrib['About']
        if "" == obj_about or obj_about.isspace():
            if 'subtype' in obj.find('./gunns').attrib:
                obj_about = obj.find('./gunns').attrib['subtype']
        if "" == obj_about or obj_about.isspace():
            obj_about = 'unknown'
        sys.exit(console.abort('a ' + obj_about + ' ' + obj_type.lower() + ' is missing a name.'))

# Normalizes a unicode string to an ascii string.
def normalizeString(s):
    if isinstance(s, str):
        return s
    elif isinstance(s, unicode):
        return unicodedata.normalize('NFKD', s).encode('ascii', 'ignore')
    return ''

# Returns whether the GUNNS object is a basic or fluid type.  This works for links and spotters.
# Return None if it isn't a GUNNS object.
def isFluidObject(obj):
    subtype = obj.find('./gunns').attrib['subtype']
    if subtype is None:
        return None
    obj_class = subtype.split("/")[-1]
    if obj_class.startswith('GunnsFluid') or obj_class.startswith('GunnsGas') or obj_class.startswith('GunnsLiquid'):
        return True
    return False

# Returns the number of Basic GUNNS objects in the given object list.
def countBasicObjects(objs):
    result = 0
    for obj in objs:
        if not isFluidObject(obj):
            result = result + 1
    return result

# Returns the number of Fluid GUNNS objects in the given object list.
def countFluidObjects(objs):
    result = 0
    for obj in objs:
        if isFluidObject(obj):
            result = result + 1
    return result

# Returns a list of all numbered and ground nodes shapes within the given container.  It also
# checks that they have an interface 'key' attribute and adds one if missing, and returns a flag
# indicating that a key was added.
def keyContainedNodes(container, numberedNodes, gndNodes, allObjects):
    childNodes = []
    updated    = False
    allNodes   = numberedNodes + gndNodes
    index      = 0
    for node in allNodes:
        geom = node.find('./mxCell/mxGeometry')
        if isDescendant(node, container, allObjects):
            index += 1
            if 'Key' not in node.attrib or not node.attrib['Key'].strip():
                node.attrib['Key'] = str(index)
                updated = True
            childNodes.append(node)
    return childNodes, updated

# Returns a bool whether the given subnet interface containers and their contained nodes
# are duplicates.  This assumes subNetIf and otherSubNetIf are not the same object, and
# will return True if they are.  The containers are duplicates if they have the same name,
# and their contained nodes all have the same label, key, and node subtype, but this
# ignores any connected links.
def isDuplicateSubNetIf(subNetIf, ifNodes, otherSubnetIf, otherIfNodes):
    if otherSubNetIf.attrib['label'] != subNetIf.attrib['label']:
        return False # difference container label
    if len(ifNodes) != len(otherIfNodes):
        return False # difference number of contained nodes
    # loop over nodes and find a match in the other nodes
    for node in ifNodes:
        matchFound         = False
        node_gunns_attribs = node.find('./gunns').attrib
        node_type          = node_gunns_attribs['subtype']
        for otherNode in otherIfNodes:
            otherNode_gunns_attribs = otherNode.find('./gunns').attrib
            otherNode_type          = otherNode_gunns_attribs['subtype']
            if node_type == otherNode_type and node.attrib['label'] == otherNode.attrib['label'] and node.attrib['Key'] == otherNode.attrib['Key']:
                matchFound = True
                break
        if not matchFound:
            return False # at least one node has no match
    return True

#####################
# BEGIN MAIN SCRIPT #
#####################
homepath = os.path.dirname(os.path.abspath(__file__))

# Python 2.7 vs. 3 by feature detection.  TODO Maybe avoid this by defining the version string in a module instead...
try:
    execfile(homepath + '/version.py')
except NameError:
    exec(compile(open(homepath + '/version.py', "rb").read(), homepath + '/version.py', 'exec'))

# TODO add command line options: -mg
#   -m only does maintenance changes to the input diagram .xml, skips generation
#   -g only does generation of the output network C++, skips diagram file maintenance
#   -d looks in the user's ~/Downloads first
#   -p overrides the default external paths environment variable
cmd_parser = ArgumentParser(description='Auto-generate the GUNNS network code from a GunnsDraw drawing.')
cmd_parser.add_argument('FILE', nargs='*', help="The drawing filename.  This is optional; if not supplied, the script will pop up a file selection window.")
cmd_parser.add_argument("-d", action="store_true", help="Look in ~/Downloads for a newer version", dest="downloads", default="false")
cmd_parser.add_argument("-m", action="store_true", help="Only do error checks and maintenance updates to the diagram file", dest="maintenance", default="false")
cmd_parser.add_argument("-g", action="store_true", help="Only do generation of the output network code", dest="generation", default="false")
cmd_parser.add_argument("-p", action="store",      help="Use the provided environment variable for external paths", dest="ext_paths", default="GUNNS_EXT_PATH")
options = cmd_parser.parse_args()

# Use the supplied path/file name, else use a file browser to select the drawing.
if len(options.FILE) > 0:
    inputPathFile = options.FILE[0]
else:
    try:
        root = TK.Tk()
    except NameError:
        sys.exit(console.abort('the Tkinter package is missing from your Python installation, but we need it to select the drawing file.\n         Install Tkinter, or re-run this script and supply the drawing filename in the command.'))
    root.withdraw()
    ftypes = [('XML files', '*.xml')]
    inputPathFile = TKFILE.askopenfilename(title = "Select a drawing file to process", filetypes = ftypes)
if not inputPathFile:
    sys.exit(console.abort('no drawing file selected.'))

outputPathFile        = inputPathFile
outputPath, inputFile = os.path.split(os.path.abspath(inputPathFile))
baseFileName          = os.path.splitext(inputFile)[0]
if options.downloads:
    download_pathfile = os.path.expanduser("~") + '/Downloads/' + inputFile
    if os.path.isfile(download_pathfile):
        if os.path.getmtime(download_pathfile) > os.path.getmtime(inputPathFile):
            #TODO we go to a lot of trouble for this Tkinter message box just to ask a Y/N.  Maybe ask it in the console instead?
            try:
                root = TK.Tk()
            except NameError:
                sys.exit(console.abort('the Tkinter package is missing from your Python installation, but we need it for the -d option.\n         Install Tkinter, or re-run this script without the -d option.'))
            root.withdraw()
            if TKMBOX.askokcancel("netexport.py", "Use the newer version in ~/Downloads?"):
                inputPathFile = download_pathfile

print('\nProcessing ' + inputFile + '...')

# Save backup copy of input file.
copyfile(inputPathFile, outputPathFile+'.bak')
print('  Backup copy saved to ' + inputFile + '.bak.')

tree = ET.parse(inputPathFile)
root = tree.getroot()
# Handle draw.io's compressed vs. uncompressed xml file formats.
# The meat of the drawing is in the <mxGraphModel> element.
# If root node = mxGraphModel, or the first elements are:
# <mxfile><diagram><mxGraphModel>, then this is uncompressed.
# Otherwise, this is compressed, and the mxGraphModel
# section is compressed in the <mxfile><diagram>text</diagram></mxfile>.
if root.tag.startswith('mxfile'):
    uncompressed_graph = root.find('./diagram/mxGraphModel')
    if uncompressed_graph is not None:
        root = uncompressed_graph
        tree._setroot(root)
    else:
        print('  Decompressing diagram data...')
        compressed_diagram = root[0].text
        diagram = compression.decompress(compressed_diagram)
        # Reset the root and tree after decompressing.  This discards
        # the outer <mxfile><diagram> elements and makes a consistent
        # tree structure with the un-compressed source file.
        root = ET.fromstring(diagram)
        tree._setroot(root)

if not root.tag.startswith('mxGraphModel'):
    sys.exit(console.abort('this is not a recognized file.'))

print('  Doing maintenance updates...')
contentsUpdated = False
# They have an element named root which isn't the actual root.  This is confusing.
rootroot = root.find('./root')
objects = root.findall('./root/object')
# This skips <mxCell id="0"/> since it has no parent attribute
mxcells = root.findall('./root/mxCell')[1:]
objects_and_cells = []
for an_object in objects:
    objects_and_cells.append(an_object)
for cell in mxcells:
    objects_and_cells.append(cell)
numNetNodes = 0
numLinks    = 0
networkName = ''
links_id = []
links = []
link_source_paths = []
spotter_source_paths = []
netNodes = []
refNodes = []
gndNodes = []
ports = []
port_maps = []
netConfig = []
netConfigData = []
basic_network = False
fluid_network = False
intFluidConfigs = []
extFluidConfigs = []
fluidStates = []
dataTables = []
spotters = []
intTcConfigs = []
tcStates = []
rxnReactions = []
rxnCompounds = []
socketLists = []
jumperPlugs = []
jumperLinks = []
numberedNodes = []
doxNotices = []
doxCopyrights = []
doxLicenses = []
doxData = []
doxReferences = []
doxAssumptions = []
subNetIfs = []

# First find the network config object
for an_object in objects:
    gunns_tag = an_object.find('./gunns')
    if None != gunns_tag:
        obj_attribs   = an_object.attrib
        gunns_attribs = gunns_tag.attrib
        if 'Network' == gunns_attribs['type']:
            if 'Super' == gunns_attribs['subtype']:
                sys.exit(console.abort('there is a super-network in this sub-network diagram.'))
            elif 'Sub' == gunns_attribs['subtype']:
                if len(netConfig) > 0:
                    sys.exit(console.abort('there is more than one network config.'))
                netConfig.append(an_object)
                netConfigData = getConfigData(obj_attribs)
                networkName = obj_attribs['label']

        # Collect all Doxygen elements.
        elif 'Dox' == gunns_attribs['type']:
            if 'notice' == gunns_attribs['subtype']:
                doxNotices.append(an_object)
            elif 'copyright' == gunns_attribs['subtype']:
                doxCopyrights.append(an_object)
            elif 'license' == gunns_attribs['subtype']:
                doxLicenses.append(an_object)
            elif 'data' == gunns_attribs['subtype']:
                doxData.append(an_object)
            elif 'references' == gunns_attribs['subtype']:
                doxReferences.append(an_object)
            elif 'assumptions' == gunns_attribs['subtype']:
                doxAssumptions.append(an_object)

# Collect all the network elements we care about -- only <gunns> tagged and are inside the
# network container.
for an_object in objects:
    gunns_tag = an_object.find('./gunns')
    if isDescendant(an_object, netConfig[0], objects_and_cells) and None != gunns_tag:
        obj_attribs   = an_object.attrib
        gunns_attribs = gunns_tag.attrib
        if 'Node' == gunns_attribs['type']:
            if 'Basic' == gunns_attribs['subtype']:
                basic_network = True
                numNetNodes = numNetNodes + 1
                netNodes.append(an_object)
                numberedNodes.append(an_object)
            elif 'Fluid' == gunns_attribs['subtype']:
                fluid_network = True
                numNetNodes = numNetNodes + 1
                netNodes.append(an_object)
                numberedNodes.append(an_object)
            elif 'Reference' == gunns_attribs['subtype']:
                refNodes.append(an_object)
                numberedNodes.append(an_object)
            elif 'Ground' == gunns_attribs['subtype']:
                gndNodes.append(an_object)
        elif 'Link' == gunns_attribs['type']:
            checkName(an_object)
            numLinks = numLinks + 1
            links_id.append(obj_attribs['id'])
            links.append(an_object)
            link_source_paths.append(gunns_attribs['subtype'])
        elif 'Port' == gunns_attribs['type']:
            ports.append(an_object)
        elif 'Fluid' == gunns_attribs['type']:
            checkName(an_object)
            if 'Config' == gunns_attribs['subtype']:
                if "1" == obj_attribs['isExternal']:
                    extFluidConfigs.append(an_object)
                else:
                    if intFluidConfigs:
                        sys.exit(console.abort('there is more than one internal fluid config.'))
                    else:
                        intFluidConfigs.append(an_object)
            elif 'State' == gunns_attribs['subtype']:
                fluidStates.append(an_object)
            elif 'TC Config' == gunns_attribs['subtype']:
                if "0" == obj_attribs['isExternal']:
                    if intTcConfigs:
                        sys.exit(console.abort('there is more than one internal trace compounds config.'))
                    else:
                        intTcConfigs.append(an_object)
            elif 'TC State' == gunns_attribs['subtype']:
                tcStates.append(an_object)
            else:
                sys.exit(console.abort('unrecognized gunns Fluid object subtype: ' + gunns_attribs['subtype']))
        elif 'Data Table' == gunns_attribs['type']:
            checkName(an_object)
            dataTables.append(an_object)
        elif 'Spotter' == gunns_attribs['type']:
            checkName(an_object)
            spotters.append(an_object)
            spotter_source_paths.append(obj_attribs['Class'])
        elif 'Reactor' == gunns_attribs['type']:
            checkName(an_object)
            if 'Reactions' == gunns_attribs['subtype']:
                rxnReactions.append(an_object)
            elif 'Compounds' == gunns_attribs['subtype']:
                rxnCompounds.append(an_object)
            else:
                sys.exit(console.abort('unrecognized gunns Reactor object subtype: ' + gunns_attribs['subtype']))
        elif 'Socket List' == gunns_attribs['type']:
            checkName(an_object)
            socketLists.append(an_object)
        elif 'Network' == gunns_attribs['type']:
            if 'Subnet Interface' == gunns_attribs['subtype']:
                subNetIfs.append(an_object)
        elif 'Dox' == gunns_attribs['type']:
            pass # collected above
        else:
            sys.exit(console.abort('unrecognized gunns object type: ' + gunns_attribs['type']))

    # Warn about GUNNS objects not in the network container, except for Dox objects, which can be anywhere.
    elif None != gunns_tag and an_object != netConfig[0] and not isDescendant(an_object, netConfig[0], objects_and_cells):
        if 'Dox' != gunns_tag.attrib['type']:
            print('    ' + console.warn('GUNNS ' + an_object.attrib['About'] + ' ' + an_object.attrib['label'] + ' is not a child of the network container, will be ignored.'))

# Check for required or conflicting objects.
if not (basic_network or fluid_network):
    sys.exit(console.abort('there are no regular nodes.'))
if basic_network and fluid_network:
    sys.exit(console.abort('there are both basic and fluid nodes.'))
if len(links) < 1:
    sys.exit(console.abort('there are no links.'))
if (countBasicObjects(links) > 0) and (countFluidObjects(links) > 0):
    sys.exit(console.abort('there are both basic and fluid links.'))
if len(netConfig) < 1:
    sys.exit(console.abort('there is no network config.'))
if '' == networkName:
    sys.exit(console.abort('the network name is empty.'))
if networkName != baseFileName:
    print('    ' + console.warn('the network name ' + networkName + ' differs from the file name ' + baseFileName + '.'))
for node in refNodes:
    label = node.attrib['label']
    if not label.isdigit():
        sys.exit(console.abort('reference node ' + label + ' doesn\'t have an integer name.'))
    match = False
    for netNode in netNodes:
        if label == netNode.attrib['label']:
            match = True
            break
    if not match:
        sys.exit(console.abort('reference node ' + label + ' doesn\'t match any network nodes.'))

# Check for required or conflicting objects in a fluid network.
# TODO for all fluid state, check they have matching fluid types
# as the fluid config
if fluid_network:
    if len(intFluidConfigs) < 1:
        sys.exit(console.abort('there is no internal fluid config.'))
    if len(fluidStates) < 1:
        sys.exit(console.abort('there are no fluid states.'))
    if len(tcStates) > 0 and len(intTcConfigs) < 1:
        sys.exit(console.abort('there are TC states but no internal TC config.'))
    if len(rxnCompounds) > 0 and len(rxnReactions) < 1:
        sys.exit(console.abort('there are reactor compounds configs but no reactor reaction lists.'))

# Maintenance content updates:

# Re-sequence the nodes.
# Abort if a network node doesn't have an integer label.
nodeList = []
for node in netNodes:
    label = node.attrib['label']
    if not label.isdigit():
        sys.exit(console.abort('node ' + label + ' doesn\'t have an integer name.'))
    nodeList.append((int(label), node))
nodeList.sort(key=lambda tup:tup[0])
nodeCount  = 0
renumbered = False
for node in nodeList:
    if node[0] != nodeCount:
        # Renumber reference nodes to follow the renumbered normal node.  Note that
        # this can be unreliable - if a reference node shares a number with 2 or more
        # normal nodes, which normal node should the rerence node match?
        for refNode in refNodes:
            if refNode.attrib['label'] == node[1].attrib['label']:
                refNode.attrib['label'] = str(nodeCount)
        node[1].attrib['label'] = str(nodeCount)
        if not renumbered:
            renumbered      = True
            contentsUpdated = True
            print('    ' + console.note('re-ordered nodes starting at node ' + str(nodeCount) + '.'))
    nodeCount = nodeCount + 1

# Shape data updates
for shapeLib in shapeLibs.shapeLibs:
    shapeLibs.loadShapeLibs(homepath + '/' + shapeLib, True)

# Collect a list of custom libraries needed by the links & spotters.
customLibs = []
for link in links:
    gunns_attr = link.find('./gunns').attrib
    if 'customLib' in gunns_attr:
        customLibs.append(gunns_attr['customLib'])
for spotter in spotters:
    gunns_attr = link.find('./gunns').attrib
    if 'customLib' in gunns_attr:
        customLibs.append(gunns_attr['customLib'])

# Build the list of custom root paths from the environment variable.
customPaths = []
if 0 < len(customLibs):
    if options.ext_paths in os.environ:
        extPaths = os.environ[options.ext_paths]
        #TODO also split on ';' for Windows
        customPaths = extPaths.split(':')
    for i,value in enumerate(customPaths):
        if value.startswith('-I'):
            customPaths[i] = value[len('-I'):]

# Load the custom libs into the master shapeTree.
    for customLib in list(sorted(set(customLibs))):
        # Build the library's absolute file name from customPaths
        missing = True
        for path in customPaths:
            libPathFile = path + '/' + customLib
            if os.path.isfile(libPathFile):
                shapeLibs.loadShapeLibs(libPathFile, True)
                shapeLibs.shapeLibs.append(customLib)
                missing = False
                break
        if missing:
            print('    ' + console.warn('can\'t find custom shape library ' + customLib + '.'))

allShapeMasters = shapeLibs.shapeTree.findall('./object')
for link in links:
    master = shapeLibs.getLinkShapeMaster(link, allShapeMasters)
    if updateLinkShapeData(link, master) or cleanLabel(link):
        contentsUpdated = True
for spotter in spotters:
    master = shapeLibs.getSpotterShapeMaster(spotter, allShapeMasters)
    if updateSpotterShapeData(spotter, master) or cleanLabel(spotter):
        contentsUpdated = True

# Check the port connections, each must connect between a node and a link.
for port in ports:
    port_attr = port.attrib
    if not port_attr['label'].isdigit():
        targetName = getPortTargetName(port, links, numberedNodes, gndNodes)
        sys.exit(console.abort('a port on ' + targetName + ' has invalid port # label: \'' + port_attr['label'] + '\'.'))
    cell_attr = port.find('./mxCell').attrib
    if 'source' not in cell_attr and 'target' not in cell_attr:
        sys.exit(console.abort('a port ' + port_attr['label'] + ' (id = ' + port_attr['id'] + ') is missing both connections.'))
    linkName = getPortLinkName(port, links)
    nodeName = getPortNodeName(port, numberedNodes, gndNodes)
    if '' == linkName and '' == nodeName:
        sys.exit(console.abort('a port ' + port_attr['label'] + ' (id = ' + port_attr['id'] + ') is missing both link and node connections.'))
    if '' == linkName:
        sys.exit(console.abort('a port ' + port_attr['label'] + ' on ' + nodeName + ' isn\'t connected to a link.'))
    if '' == nodeName:
        sys.exit(console.abort('a port ' + port_attr['label'] + ' on ' + linkName + ' isn\'t connected to a node.'))
    # A link cannot have more than one port with the same label unless they connect
    # to nodes in different containers, i.e. the main network and a sub-net interface.
    portNodeParentId = getParentId(getPortNode(port, numberedNodes, gndNodes))
    for otherPort in ports:
        if otherPort is not port:
            otherLinkName = getPortLinkName(otherPort, links)
            otherPortLabel = otherPort.attrib['label']
            otherPortNodeParentId = getParentId(getPortNode(otherPort, numberedNodes, gndNodes))
            if otherLinkName == linkName and otherPortLabel == port_attr['label'] and otherPortNodeParentId == portNodeParentId:
                sys.exit(console.abort('link ' + linkName + ' has more than one port ' + otherPortLabel + ' connected to nodes in the same container.'))

# Build jumper plugs
#   loop over links
#     if they have a plugs attrib in <gunns> then parse it for the plug port #'s
#     for each plug port #:
#       Verify there is a c##.plug# in the shape data for that port #.
#         Abort if not: it's a bad link
#       Get the plug# value.  This is the name of the socket list
#       append to the jumperPlugs list a tuple (plug type, plug name)
#       Record the link's eventual new c##.plug# value with its new plug name,
#         will be updated in the data model for export to the network code, but
#         without changing the config data value in the drawing itself.
jumperPlugConfigs = []
for link in links:
    gunns_attribs = link.find('./gunns').attrib
    if 'plugs' in gunns_attribs:
        jumperLinks.append(link)
        plugNums     = gunns_attribs['plugs'].split(',')
        link_attribs = link.attrib
        link_items   = link_attribs.items()
        for plugNum in plugNums:
            plugConfigName = '.plug' + str(plugNum)
            plugType       = ''
            for item in link_items:
                if item[0].startswith('c') and plugConfigName in item[0]:
                    plugConfigName = item[0]
                    plugType       = item[1]
                    break;
            if not plugConfigName.startswith('c'):
                sys.exit(console.abort('config data for plug' + str(plugNum) + ' not found in link ' + getLinkName(link) + '.'))
            if '0' == plugType:
                # Skip processing if the value is '0' as this is the valid option for specifying there is no plug on this link port.
                pass
            elif '' != plugType:
                plugMissing = True
                for socketList in socketLists:
                    if plugType == socketList.attrib['label']:
                        plugName = getLinkName(link) + 'Plug' + str(plugNum)
                        jumperPlugs.append((networkName + '_' + plugType, plugName))
                        jumperPlugConfigs.append([link, plugConfigName, '&network->' + plugName]) # link elem, config data name, config data value
                        plugMissing = False
                        break
                if plugMissing:
                    sys.exit(console.abort('invalid plug type for plug' + str(plugNum) + ' in link ' + getLinkName(link) + '.'))
            else:
                sys.exit(console.abort('empty plug type for plug' + str(plugNum) + ' in link ' + getLinkName(link) + '.'))

# Move all jumper links to the end of the link list, since they must init last.
# But keep their order relative to each other.
for jumperLink in jumperLinks:
    links.append(links.pop(links.index(jumperLink)))

# Build the link port map:
for link in links:
    gunns_attribs = link.find('./gunns').attrib
    num_ports     = int(gunns_attribs['numPorts'])
    link_attribs  = link.attrib
    link_id       = link_attribs['id']
    link_ports    = []
    if '' == gunns_attribs['reqPorts']:
        sys.exit(console.abort(getLinkName(link) + ' link\'s shape master has empty reqPorts attribute.'))
    req_ports     = gunns_attribs['reqPorts'].split(',')

    # collect the ports that connect to this link
    for port in ports:
        cell_attr = port.find('./mxCell').attrib
        if (link_id == cell_attr['source']) or (link_id == cell_attr['target']):
            link_ports.append(port)
    num_found_ports = len(link_ports)

    # Check that all required ports are present
    for req_port in req_ports:
        port_found = False
        if (num_ports > 0) and not (-1 < int(req_port) < max(1, num_ports)):
            sys.exit(console.abort(getLinkName(link) + ' link\'s shape master has invalid required port # ' + req_port + '.'))
        for link_port in link_ports:
            if req_port == link_port.attrib['label']:
                port_found = True
        if not port_found:
            sys.exit(console.abort(getLinkName(link) + ' link is missing required Port ' + req_port + '.'))

    # Sizing the port_map list by the larger of the number of fixed ports or found ports
    # supports variable port links.
    # Default all ports to -1 (Ground), then found ports will override this for their
    # slot.  This leaves optional ports with no connection on Ground.
    port_map = [-1] * max(num_ports, num_found_ports)
    for link_port in link_ports:
        port_number = int(link_port.attrib['label'])
        cell_attr   = link_port.find('./mxCell').attrib
        source_id   = cell_attr['source']
        target_id   = cell_attr['target']
        if link_id == source_id:
            node_target = target_id
        else:
            node_target = source_id
        for node in netNodes:
            if node_target == node.attrib['id']:
                 port_map[port_number] = node.attrib['label']
        for node in refNodes:
            if node_target == node.attrib['id']:
                 port_map[port_number] = node.attrib['label']
    port_maps.append(port_map)

# Update the sub-network interface containers with link connections to Ground nodes and nuber of sub-network nodes.
# In the super-network, the only ports that will be moved are those that connect to Ground nodes in the sub-network interface.
updatedSubNetIfsNodeCount = False
updatedSubNetIfLabels     = []
updatedSubNetIfKeys       = []
for subNetIf in subNetIfs:
    subNetUpdated = False
    ifKeysUpdated = False
    isDuplicateOf = None
    ifNodes, ifKeysUpdated = keyContainedNodes(subNetIf, numberedNodes, gndNodes, objects_and_cells)

    # Determine if this interface box is an identical duplicate of another.
    for otherSubNetIf in subNetIfs:
        if otherSubNetIf is subNetIf:
            break
        # This shouldn't rekey the nodes because the other subnetIf has already been processed by the outer loop
        otherIfNodes, otherIfKeysUpdate = keyContainedNodes(otherSubNetIf, numberedNodes, gndNodes, objects_and_cells)
        if isDuplicateSubNetIf(subNetIf, ifNodes, otherSubNetIf, otherIfNodes):
            isDuplicateOf = otherSubNetIf
            break

    duplicateElems = subNetIf.findall('./gunnsSubnetIfDuplicate')
    if isDuplicateOf is None:
        # If this is not a duplicate, then clean out any old duplicates information.
        for duplicateElem in duplicateElems:
            subNetIf.remove(duplicateElem)
            subNetUpdated = True
    else:
        if not duplicateElems:
            # Add a new duplicate of element if there isn't already one.
            newElement = ET.SubElement(subNetIf, 'gunnsSubnetIfDuplicate')
            newElement.attrib['OfId'] = isDuplicateOf.attrib['id']
            subNetUpdated = True
        elif duplicateElems[0].attrib['OfId'] != isDuplicateOf.attrib['id']:
            # Modify the existing duplicate of element and delete any others.
            duplicateElems[0].attrib['OfId'] != isDuplicateOf.attrib['id']
            for duplicateElem in duplicateElems[1:]:
                subNetIf.remove(duplicateElem)
            subNetUpdated = True

        # In duplicates, we delete any old connection information.
        oldConnections = subNetIf.findall('./gunnsSubnetIfConnection')
        for oldConnection in oldConnections:
            subNetIf.remove(oldConnection)
            subNetUpdated = True

    if subNetUpdated:
        updatedSubNetIfLabels.append(subNetIf.attrib['label'])
        subNetUpdated = False
    if ifKeysUpdated:
        updatedSubNetIfKeys.append(subNetIf.attrib['label'])

    # List all ports in the network connecting to Ground nodes in this interface or any duplicates of this interface.
    ifPorts = []
    for port in ports:
        # Return value of None shouldn't be possible because we've aborted above if any ports aren't connected to a node.
        node = getPortNode(port, numberedNodes, gndNodes)
        for ifNode in ifNodes:
            if node == ifNode and node in gndNodes:
                linkName = getPortLinkName(port, links)[len('link '):]  # strip 'link ' off the front of the returned name
                # Find the index of the link in the links and port_map lists.
                linkIndex = None
                for index in range(0, len(links)):
                    if linkName == getLinkName(links[index]):
                        linkIndex = index
                        break
                # Build the link's default port map string and the interface port attributes.
                ifPorts.append((linkName, getPortMap(port_maps[index]), port.attrib['label'], ifNode.attrib['Key']))
                break

    # Add any new drawing connection missing from the sub-network's interface box, and flag update.
    # If this is a duplicate box, we add the connections to the box it duplicates, not this one.
    if isDuplicateOf is not None:
        usingSubNetIf = isDuplicateOf
    else:
        usingSubNetIf = subNetIf
    connections = usingSubNetIf.findall('./gunnsSubnetIfConnection')
    for ifPort in ifPorts:
        isFound = False
        for connection in connections:
            if ifPort[0] == connection.attrib['Link'] and \
               ifPort[1] == connection.attrib['Map'] and \
               ifPort[2] == connection.attrib['Port'] and \
               ifPort[3] == connection.attrib['Key']:
                isFound = True
                break
        if not isFound:
            newElement = ET.SubElement(usingSubNetIf, 'gunnsSubnetIfConnection')
            newElement.attrib['Link'] = ifPort[0]
            newElement.attrib['Map']  = ifPort[1]
            newElement.attrib['Port'] = ifPort[2]
            newElement.attrib['Key']  = ifPort[3]
            subNetUpdated = True

    # For a sub-network interface box that isn't a duplicate, prune old connections from its xml
    # that aren't in the drawing anymore, and flag update.  We don't need to to this for duplicate
    # interface boxes since all their connections have already been remove above.
    if isDuplicateOf is None:
        connections = subNetIf.findall('./gunnsSubnetIfConnection')
        for connection in connections:
            isFound = False
            for ifPort in ifPorts:
                if ifPort[0] == connection.attrib['Link'] and \
                   ifPort[1] == connection.attrib['Map'] and \
                   ifPort[2] == connection.attrib['Port'] and \
                   ifPort[3] == connection.attrib['Key']:
                    isFound = True
                    continue
            if not isFound:
                # TODO this falsely deletes a connection if the link connects to a duplicate interface box in the drawing,
                # and it gets added back by the duplicate later, creating an extra drawing update message to the user that
                # is a nuisance (technically the etree has been updated but ends up being identical).  But fixing this is
                # going to be really tricky without a lot more code refactoring.
                subNetIf.remove(connection)
                subNetUpdated = True

    if subNetUpdated:
        updatedSubNetIfLabels.append(usingSubNetIf.attrib['label'])

    # Update the node count element or add one if it is missing.
    oldNodeCount = subNetIf.find('./gunnsSubnetIfNodeCount')
    if oldNodeCount is None:
        newNodeCount = ET.SubElement(subNetIf, 'gunnsSubnetIfNodeCount')
        newNodeCount.set('count', str(nodeCount))
        updatedSubNetIfsNodeCount = True
    else:
        # convert old syntax (element text) to new syntax (element attribute)
        oldCountText = oldNodeCount.text
        if oldCountText is not None:
            oldNodeCount.set('count', str(nodeCount))
            oldNodeCount.text = None
            updatedSubNetIfsNodeCount = True
        # update the old count value if it has changed
        elif nodeCount != int(oldNodeCount.attrib['count']):
            oldNodeCount.set('count', str(nodeCount))
            updatedSubNetIfsNodeCount = True

# Output notifications about updated drawing contents in the subnet interfaces.
if updatedSubNetIfsNodeCount:
    print('    ' + console.note('updated network node count in the sub-network interfaces.'))
    contentsUpdated = True

# Loop over the list (ignoring duplicates) of interface labels.
for label in set(updatedSubNetIfLabels):
    print('    ' + console.note('updated connections to sub-network interface: ' + label + '.'))
    contentsUpdated = True

for label in set(updatedSubNetIfKeys):
    print('    ' + console.note('updated interface node keys in sub-network interface: ' + label + '.'))
    contentsUpdated = True

# Update the input file with the readable formatted tree.
# Splitting the file into many lines like this makes merging easier.
xmlUtils.formatXml(root)
tree.write(outputPathFile, xml_declaration=False)
print('  ...saved updates to ' + inputFile + '.')

# Skip generating the network class code in the maintenance option.
if 'false' != options.maintenance:
    quit()

# Assemble the data model to pass to the template engine:
print('  Building data model...')
revline = '  ((Auto-generated by the GunnsDraw netexport script version ' + GUNNSDRAW_VERSION + ') (' + str(datetime.now()) + '))'

# Replace jumper plug config data with their new jumper plug object names.
for jumperPlugConfig in jumperPlugConfigs:
    for link in links:
        if jumperPlugConfig[0] == link:
            link.attrib[jumperPlugConfig[1]] = jumperPlugConfig[2]
            break

# This is a list of data for each link: class, name, initialize block, configData, inputData
linksData = []
index = 0
for link in links:
    gunns_attr = link.find('./gunns').attrib
    linkClass  = gunns_attr['subtype'].split("/")[-1]
    linkName   = getLinkName(link)
    linkData   = (linkClass, linkName, getConfigData(link.attrib), getInputData(link.attrib), getLinkInitialize(link, port_maps[index]), getLinkConstructorBody(link, 'c'), getLinkConstructorBody(link, 'i'))
    linksData.append(linkData)
    index = index + 1

# This is a list of data for each node: number, initial state, sorted by node number
nodesData = []
for node in netNodes:
    numStr = node.attrib['label']
    if basic_network:
        nodeData = (numStr, node.attrib['i00.potential'])
    else:
        nodeData = (numStr, node.attrib['i00.initialFluidState'])
        if '0' == nodeData[1] or '' == nodeData[1]:
            sys.exit(console.abort('node ' + nodeData[0] + ' is missing initialFluidState.'))
    nodesData.append(nodeData)
nodesData.sort(key=lambda tup: int(tup[0]))

# This is a list of data for each spotter: class, name, config data, input data, constructor block
spottersData = []
for spotter in spotters:
    spotterClass = spotter.attrib['Class'].split("/")[-1]
    spotterName  = spotter.attrib['label']
    spotterData  = (spotterClass, spotterName, getConfigData(spotter.attrib), getInputData(spotter.attrib), spotter.attrib['ConstructorArgs'], getLinkConstructorBody(spotter, 'c'), getLinkConstructorBody(spotter, 'i'))
    spottersData.append(spotterData)

# This is a list of data for each socket list
socketListsData = []
for socketList in socketLists:
    theSocketList = buildSwimlane1D(socketList, mxcells)
    socketListsData.append(theSocketList)

# Format Doxygen Attention fields
noticeData = []
for notice in doxNotices:
    # Strip html tags out of the label value, but convert html breaks into new lines.
    newNotice = '@attention  ' + re.sub('<[^<]+?>', '', re.sub('<br>', '\n            ', notice.attrib['label']) + '\n')
    noticeData.append(normalizeString(newNotice))

# Format Doxygen Copyright fields
# TODO lot in common with notices above, refactor for DRY...
copyrightData = []
for copyright in doxCopyrights:
    # Strip html tags out of the label value, but convert html breaks into new lines.
    newCopyright = '@copyright  ' + re.sub('<[^<]+?>', '', re.sub('<br>', '\n            ', copyright.attrib['label']) + '\n')
    copyrightData.append(normalizeString(newCopyright))

# Format Doxygen License fields
# TODO lot in common with notices above, refactor for DRY...
licenseData = []
for license in doxLicenses:
    # Strip html tags out of the label value, but convert html breaks into new lines.
    newLicense = '@license  ' + re.sub('<[^<]+?>', '', re.sub('<br>', '\n          ', license.attrib['label']) + '\n')
    licenseData.append(normalizeString(newLicense))

# Format Doxygen data fields
# Every attribute="value" field in <object> becomes a '@attribute value' line in the output.
# So ingroup="GUNNS' becomes @ingroup GUNNS
doxygenData = []
for data in doxData:
    result = ''
    # Skip attributes: label, About, id
    for key, value in data.items():
        if key != 'label' and key != 'About' and key != 'id':
            newData = '@' + key + '  ' + value
            doxygenData.append(normalizeString(newData))

# Format document Reference fields
referencesData = []
for reference in doxReferences:
    theReferencesList = buildSwimlane1D(reference, mxcells)
    for referenceItem in theReferencesList[2:]:
        newReference = '(' + re.sub('<[^<]+?>', '', re.sub('<br>', '\n   ', referenceItem) + ')')
        referencesData.append(normalizeString(newReference))

# Format document Assumptions & Limitations fields
# TODO DRY with References above...
assumptionsData = []
for assumption in doxAssumptions:
    theAssumptionsList = buildSwimlane1D(assumption, mxcells)
    for assumptionItem in theAssumptionsList[2:]:
        newAssumption = '(' + re.sub('<[^<]+?>', '', re.sub('<br>', '\n   ', assumptionItem) + ')')
        assumptionsData.append(normalizeString(newAssumption))

# TODO namespace
#  maybe do namespaces by having them put it in the name of the network
#  config, i.e. Ts21::DrawFluid
data_model = dict([('networkName', networkName),
                   ('networkNamespace', ''),
                   ('revline', revline),
                   ('linkSourcePaths', list(sorted(set(link_source_paths)))),
                   ('links', linksData),
                   ('nodes', nodesData),
                   ('numNodes', numNetNodes),
                   ('spotters', spottersData),
                   ('spotterSourcePaths', list(sorted(set(spotter_source_paths)))),
                   ('solverConfig', netConfigData),
                   ('socketLists', socketListsData),
                   ('jumperPlugs', jumperPlugs),
                   ('doxNotices', noticeData),
                   ('doxCopyrights', copyrightData),
                   ('doxLicenses', licenseData),
                   ('doxData', doxygenData),
                   ('doxReferences', referencesData),
                   ('doxAssumptions', assumptionsData),
])

# Add Data Tables to the data model.
# For now we only support 2D tables.
theDataTables = []
for dataTable in dataTables:
    gunns_attr = dataTable.find('./gunns').attrib
    if '2D' == gunns_attr['subtype']:
        thisDataTable = buildSwimlane2D(dataTable, mxcells)
        theDataTables.append(thisDataTable)
data_model['dataTables'] = theDataTables

# For fluid networks, add fluid objects to the data model.
if fluid_network:
    theIntFluidConfig = buildSwimlane1D(intFluidConfigs[0], mxcells)
    data_model['intFluidConfig'] = theIntFluidConfig

    theExtFluidConfigs = []
    for extConfig in extFluidConfigs:
        thisExtConfig = buildSwimlane1D(extConfig, mxcells)
        theExtFluidConfigs.append(thisExtConfig)
    data_model['extFluidConfigs'] = theExtFluidConfigs

    theFluidStates = []
    for fluidState in fluidStates:
        thisFluidState = buildSwimlane2D(fluidState, mxcells)
        # Add a tuple that contains all the comma-separated mass fractions.
        mixture = ''
        for i in thisFluidState[6:-1]:
            mixture = mixture + i[1] + ', '
        mixture = mixture + thisFluidState[-1][1]
        thisFluidState.append(('Mixture', mixture))
        theFluidStates.append(thisFluidState)
    data_model['fluidStates'] = theFluidStates

    theIntTcConfig = []
    if len(intTcConfigs) > 0:
        theIntTcConfig = buildSwimlane1D(intTcConfigs[0], mxcells)
    data_model['intTcConfig'] = theIntTcConfig

    theTcStates = []
    for tcState in tcStates:
        thisTcState = buildSwimlane2D(tcState, mxcells)
        # Add a tuple that contains all the comma-separated mass fractions.
        mixture = ''
        for i in thisTcState[3:-1]:
            mixture = mixture + i[1] + ', '
        mixture = mixture + thisTcState[-1][1]
        thisTcState.append(('Mixture', mixture))
        theTcStates.append(thisTcState)
    data_model['tcStates'] = theTcStates

    theReactions = []
    for reactions in rxnReactions:
        thisReaction = buildSwimlane1D(reactions, mxcells)
        theReactions.append(thisReaction)
    data_model['reactions'] = theReactions

    theCompounds = []
    for compounds in rxnCompounds:
        thisCompounds = buildSwimlane2D(compounds, mxcells)
        # Add a tuple that contains all the comma-separated masses.
        masses = ''
        for i in thisCompounds[3:-1]:
            masses = masses + i[1] + ', '
        masses = masses + thisCompounds[-1][1]
        thisCompounds.append(('Masses', masses))
        theCompounds.append(thisCompounds)
    data_model['compounds'] = theCompounds

# Instantiate the output templates
    data_model['networkType'] = 'Fluid'
    hhTemplate = FluidNetworkHeaderTemplate(data_model)
    ccTemplate = FluidNetworkBodyTemplate(data_model)
else:
    data_model['networkType'] = 'Basic'
    hhTemplate = BasicNetworkHeaderTemplate(data_model)
    ccTemplate = BasicNetworkBodyTemplate(data_model)

# For debugging:
#print(data_model)

hhFileName = outputPath + '/' + networkName + '.hh'
ccFileName = outputPath + '/' + networkName + '.cpp'
#ccFileName = os.path.splitext(outputPathFile)[0] + '.cpp'

# Render templates to output files.
print ('  Rendering ' + networkName + '.hh...')
with open(hhFileName, 'w') as fhh:
    hhRender = hhTemplate.render()
    fhh.write(hhRender)

print ('  Rendering ' + networkName + '.cpp...')
with open(ccFileName, 'w') as fcc:
    ccRender = ccTemplate.render()
    fcc.write(ccRender)

END_TIME = datetime.now()
dt = (END_TIME - START_TIME).total_seconds()
console.success(dt)
if contentsUpdated:
    print (console.note('Remember to synchronize or re-load ' + inputFile + ' in draw.io to see the content updates from maintenance.'))
print ('')
