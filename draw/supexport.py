#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
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

# Python 2.7 vs. 3 imports by feature detection:
try:
    import Tkinter as TK
except ImportError:
    import tkinter as TK
try:
    import tkFileDialog as TKFILE
except ImportError:
    import tkinter.filedialog as TKFILE
try:
    import tkMessageBox as TKMBOX
except ImportError:
    import tkinter.messagebox as TKMBOX

from optparse import OptionParser
from shutil import copyfile
import xml.etree.ElementTree as ET
import modules.compression as compression
import modules.consoleMsg as console
import modules.xmlUtils as xmlUtils
from templates.SuperNetworkSetupTemplate import SuperNetworkSetupTemplate

netConfigs = []

#TODO
# maybe a better name would be LinkConnectionData or such...
class LinkDataModel:
    def __init__(self, elem, name):
        self.element    = elem
        self.superPorts = []
        self.ports      = []
        self.name       = name.split('.')[1]
        self.subNetName = name.split('.')[0]
        self.portMap    = []
        return
    
#TODO
# Maintenance functions:  This will allow users to manually delete or copy in sub-networks
# in draw.io prior to running this, and then this will clean it up
# - renumber nodes and update sub-network node offsets if needed
# - update sub-network config data
# - destroy link info
# TODO Should this script update sub-network contents from their source drawings, or should supcreate?
# - note to user when any content changes are made to the drawing, similar to netexport
# Error checks:
# -

#TODO refactor with netexport.py...
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

# TODO
def getSubNetworkName(net):
    return net.attrib['label']

# TODO
def getParentSubNetwork(obj):
    for netConfig in netConfigs:
        if netConfig.attrib['id'] == obj.find('mxCell').attrib['parent']:
            return netConfig

# Returns the given link's name prepend with the sub-network instance name it belongs to,
# to avoid ambiguity with other instances.
# This only works if links are direct descendants of sub-networks, not in some other container.
def getLinkName(link):
    return getSubNetworkName(getParentSubNetwork(link)) + '.' + link.attrib['label']

# Returns the link this port is connected to, or None.
def getPortLink(port, links):
    source = ''
    target = ''
    cell_attr = port.find('./mxCell').attrib
    if 'source' in cell_attr:
        source = cell_attr['source']
    if 'target' in cell_attr:
        target = cell_attr['target']
    for link in links:
        if source == link.attrib['id'] or target == link.attrib['id']:
            return link
    return None

# TODO refactor with netexport.py
def getNodeName(node):
    if node.find('gunns').attrib['subtype'] == 'Ground':
        return 'Ground'
    else:
        return 'node ' + node.attrib['label']

# TODO
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

# TODO refactor with netexport.py
# Returns the name string of the link that is connected to this port.
# If there is no link connected, then returns the connected node and
# number.  If there is no node connected then returns ''
# 'link TestConductor5' or 'node 5'
def getPortTargetName(port, links, nodes, gnds):
    result = getLinkName(getPortLink(port, links))
    if '' == result:
        result = getNodeName(getPortNode(port, nodes, gnds))
    return result

# TODO 
# Return the link element that has this sub-network.link name, or None.
def getLinkByName(name, links):
    for link in links:
        if name == link.attrib['label']:
            return link
    return None

#####################
# BEGIN MAIN SCRIPT #
#####################
homepath = os.path.dirname(os.path.abspath(__file__))

# TODO add command line options: -mg
#   -m only does maintenance changes to the input diagram .xml, skips generation
#   -g only does generation of the output network C++, skips diagram file maintenance
#   -d looks in the user's ~/Downloads first
cmd_parser = OptionParser()
cmd_parser.add_option("-d", action="store_true", help="look in ~/Downloads for a newer version", dest="downloads", default="false")
cmd_parser.add_option("-m", action="store_true", help="only do error checks and maintenance updates to the diagram file", dest="maintenance", default="false")
cmd_parser.add_option("-g", action="store_true", help="only do generation of the output file", dest="generation", default="false")
(options, args) = cmd_parser.parse_args()

# TODO refactor with netexport.py
# Use the supplied path/file name, else use a file browser to select the drawing.
if len(args) > 0:
    inputPathFile = args[0]
else:
    root = TK.Tk()
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
            root = TK.Tk()
            root.withdraw()
            if TKMBOX.askokcancel("supexport.py", "Use the newer version in ~/Downloads?"):
                inputPathFile = download_pathfile

print('\nProcessing ' + inputFile + '...')

# Save backup copy of input file.
copyfile(inputPathFile, outputPathFile+'.bak')
print('  Backup copy saved to ' + inputFile + '.bak.')

tree = ET.parse(inputPathFile)
root = tree.getroot()
# Handle draw.io's compressed vs. uncompressed xml file formats.
# If root node = mxGraphModel then this is uncompressed.
# If root node = mxfile then this is compressed, and the mxGraphModel
# section is compressed in the <mxfile><diagram>text</diagram></mxfile>.
if root.tag.startswith('mxfile'):
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
ports = []
superPorts = []
links = []
normalNodeSubtypes = ['Fluid', 'Basic']
numberedNodeSubtypes = ['Fluid', 'Basic', 'Reference']
numberedNodes = []
gndNodes = []
allNodes = []
superConfig = None
linkNamesWithSuperPorts = []
linkDataModels = []

# Make a list of various GUNNS object types
for obj in objects:
    gunns = obj.find('gunns')
    if gunns is not None:
        if gunns.attrib['type'] == 'Port':
            ports.append(obj)
            # add attached link & port #
        elif gunns.attrib['type'] == 'Super Port':
            superPorts.append(obj)
            # add attached link & port #
        elif gunns.attrib['type'] == 'Link':
            links.append(obj)
        elif gunns.attrib['type'] == 'Node':
            allNodes.append(obj)
            if gunns.attrib['subtype'] in numberedNodeSubtypes:
                numberedNodes.append(obj)
            elif gunns.attrib['subtype'] == 'Ground':
                gndNodes.append(obj)
        elif gunns.attrib['type'] == 'Network':
            if gunns.attrib['subtype'] == 'Sub':
                # Abort if this sub-network has the same local name as another one.
                for netConfig in netConfigs:
                    if netConfig.attrib['label'] == obj.attrib['label']:
                        sys.exit(console.abort('sub-networks have the same instance name ' + obj.attrib['label'] + '.'))
                netConfigs.append(obj)
            elif gunns.attrib['subtype'] == 'Super':
                superConfig = obj

if superConfig is None:
    sys.exit(console.abort('there is no super-network config object.'))
    
# TODO refactor with netexport.py
# TODO there can be multiple links with the same name because they're in different sub-network isntances.
#   these names should be scoped by the sub-network instance name.
# Check the super-port connections, each must connect between a node and a link.
for port in superPorts:
    port_attr = port.attrib
    if not port_attr['label'].isdigit():
        targetName = getPortTargetName(port, links, numberedNodes, gndNodes)
        sys.exit(console.abort('a super-port on ' + targetName + ' has invalid port # label: \'' + port_attr['label'] + '\'.'))
    cell_attr = port.find('./mxCell').attrib
    if 'source' not in cell_attr and 'target' not in cell_attr:
        sys.exit(console.abort('a super-port ' + port_attr['label'] + ' (id = ' + port_attr['id'] + ') is missing both connections.'))
    link     = getPortLink(port, links)
    linkName = getLinkName(link)
    nodeName = getNodeName(getPortNode(port, numberedNodes, gndNodes))
    if '' == linkName and '' == nodeName:
        sys.exit(console.abort('a super-port ' + port_attr['label'] + ' (id = ' + port_attr['id'] + ') is missing both link and node connections.'))
    if '' == linkName:
        sys.exit(console.abort('a super-port ' + port_attr['label'] + ' on ' + nodeName + ' isn\'t connected to a link.'))
    if '' == nodeName:
        sys.exit(console.abort('a super-port ' + port_attr['label'] + ' on link ' + linkName + ' isn\'t connected to a node.'))
    # Add the link to the list of those that have super-ports
    if linkName not in linkNamesWithSuperPorts:
        linkNamesWithSuperPorts.append(linkName)
        linkDataModels.append(LinkDataModel(link, linkName))
    
for linkData in linkDataModels:
    linkName = linkData.element.attrib['label']
    linkId   = linkData.element.attrib['id']
    allPorts = []   
    for superPort in superPorts:
        if getPortLink(superPort, links) == linkData.element:
            linkData.superPorts.append(superPort)
            allPorts.append(superPort)

    for port in ports:
        # If a normal port is overriden by a super-port on this link, then don't add it to the link's
        # data model, and also delete it from the drawing tree.
        portLink = getPortLink(port, links)
        if portLink is not None and portLink.attrib['id'] == linkId:
            overridden = False
            for superPort in linkData.superPorts:
                if superPort.attrib['label'] == port.attrib['label']:
                    overridden = True
            if overridden:
                rootroot.remove(port)
            else:
                # Before adding the normal port to the link, make sure it is connected to a node.
                port_attr = port.attrib
                if not port_attr['label'].isdigit():
                    sys.exit(console.abort('a port on link ' + linkName + ' has invalid port # label: \'' + port_attr['label'] + '\'.'))
                nodeName = getNodeName(getPortNode(port, numberedNodes, gndNodes))
                if '' == nodeName:
                    sys.exit(console.abort('a port ' + port_attr['label'] + ' on link ' + linkName + ' isn\'t connected to a node.'))
                linkData.ports.append(port)
                allPorts.append(port)

    # Check that all the link's required ports are present.
    #TODO refactor with netexport.py...
    req_ports = linkData.element.find('gunns').attrib['reqPorts'].split(',')
    num_ports = int(linkData.element.find('gunns').attrib['numPorts'])
    for req_port in req_ports:
        port_found = False
        if not (-1 < int(req_port) < max(1, num_ports)):
            sys.exit(console.abort(linkName + ' link\'s shape master has invalid required port # ' + req_port + '.'))
        for port in allPorts:
            if req_port == port.attrib['label']:
                port_found = True
        if not port_found:
            sys.exit(console.abort(linkName + ' link is missing required Port ' + req_port + '.'))

# Rebuild the list of objects remaining in the tree, after some ports have been deleted.
theObjects = rootroot.findall('object')

# Find nodes that are left with no port connections on them.
for node in allNodes:
    node_id = node.attrib['id']
    noConnections = True
    for port in theObjects:
        port_gunns = port.find('gunns')
        if port_gunns is not None:
            if 'Port' == port_gunns.attrib['type']:
                port_cell_attr = port.find('mxCell').attrib
                if (('target' in port_cell_attr and node_id == port_cell_attr['target']) or ('source' in port_cell_attr and node_id == port_cell_attr['source'])):
                    noConnections = False
                    break
    if noConnections:
        if node.find('gunns').attrib['subtype'] in normalNodeSubtypes:
            # Warn the user about unconnected network nodes.
            print('    ' + console.warn('super-node ' + node.attrib['label'] + ' has no port connections.'))
        else:
            # Delete unconnected ground and reference nodes from the drawing.
            rootroot.remove(node)

# Update the input drawing with the readable formatted tree.
# Splitting the file into many lines like this makes merging easier.
xmlUtils.formatXml(root)
tree.write(outputPathFile, xml_declaration=False)
print('  ...saved updates to ' + inputFile + '.')

# Build data model for the trick input file renderer:
# - sub-network instance names, node offset, network class type
# - link connections data
# - super-net solver config
subNets = []
subNetLength = 0
for subNet in netConfigs:
    if len(subNet.attrib['label']) > subNetLength:
        subNetLength = len(subNet.attrib['label'])

for subNet in netConfigs:
    subNets.append([subNet.attrib['label'], subNet.attrib['SuperNodesOffset'], os.path.splitext(os.path.split(subNet.attrib['sourceDrawing'])[1])[0], ' '*(subNetLength - len(subNet.attrib['label']))])

for link in linkDataModels:
    # Sort the link ports by port number.
    sortedPorts = []
    for i in range(0, len(link.ports) + len(link.superPorts)):
        sortedPorts.append(None)
    for port in link.ports:
        portNum = int(port.attrib['label'])
        if portNum >= len(sortedPorts):
            sys.exit(console.abort('link ' + getLinkName(link) + ' has a gap in its port numbers.'))
        sortedPorts[portNum] = port
    for port in link.superPorts:
        portNum = int(port.attrib['label'])
        if portNum >= len(sortedPorts):
            sys.exit(console.abort('link ' + getLinkName(link) + ' has a gap in its port numbers.'))
        sortedPorts[portNum] = port

    # For each sorted port, find its original node's sub-network.
    link.portMap = '['
    for port in sortedPorts:
        node     = getPortNode(port, numberedNodes, gndNodes)
        nodeName = getNodeName(node)
        if 'Ground' == nodeName:
            portMap = 'super_ground'
        else:
            # Find the sub-network that this node belongs to.
            subNet = getParentSubNetwork(node)
            subNetName = subNet.attrib['label']
            subNodeNum = int(nodeName[5:]) - int(subNet.attrib['SuperNodesOffset'])
            # TODO abort if num < 0
            portMap = str(subNodeNum) + '+offset_' + subNetName
        link.portMap += (portMap + ', ')
    link.portMap = link.portMap[:-2] + ']'

data_model = dict([('functionName', baseFileName + 'Setup'),
                   ('subNets',      subNets),
                   ('links',        linkDataModels),
                   ('solverConfig', getConfigData(superConfig).split(','))
])
template = SuperNetworkSetupTemplate(data_model)

# For debug:
#print(data_model)

print ('  Rendering ' + baseFileName + '.py...')

trickFileName = outputPath + '/' + baseFileName + '.py'
with open(trickFileName, 'w') as fout:
    render = template.render()
    fout.write(render)

print ('...Complete!\n')
