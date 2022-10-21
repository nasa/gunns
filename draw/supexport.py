#!/usr/bin/python
# @copyright Copyright 2022 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
import os, sys, copy, re
import collections

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
from datetime import datetime
import xml.etree.ElementTree as ET
import modules.compression as compression
import modules.consoleMsg as console
import modules.xmlUtils as xmlUtils
import string
import random
from templates.SuperNetworkSetupTemplate import SuperNetworkSetupTemplate

netConfigs = []
normalNodeSubtypes = ['Fluid', 'Basic']
numberedNodeSubtypes = ['Fluid', 'Basic', 'Reference']

#TODO
# maybe a better name would be LinkConnectionData or such...
class LinkDataModel:
    def __init__(self, elem, name):
        self.element    = elem # this link's XML element
        self.superPorts = []   # this link's overriding super-port XML elements
        self.ports      = []   # this link's default port XML elements
        self.name       = name.split('.')[1]
        self.subNetName = name.split('.')[0]
        self.portMap    = []   # string defining the port map array assignments, i.e. '[42, 12]'
        return
    
# Class describing a sub-network interface container.
class SubNetIf:
    def __init__(self, elem, objects_and_cells):
        self.element     = elem # XML element of this subnet interface box
        self.connections = []   # list of tuples of (link, port, interface_node_index)
        self.children    = []   # list of elements that have this subnet interface box as their direct parent
        self.parseConnections()
        self.findChildren(objects_and_cells)
        return
    
    # Loop over <gunnsSubnetIfConnections> in elem and parse them into our connections list.
    def parseConnections(self):
        allConnections = self.element.findall('./gunnsSubnetIfConnection')
        for aConnection in allConnections:
            newConnection = [aConnection.attrib['Link'], aConnection.attrib['Map'], aConnection.attrib['Port'], aConnection.attrib['Key']]
            self.connections.append(newConnection)

    # Loop over all the given objects and store those that are direct children of this sub-network interface container.
    def findChildren(self, objects_and_cells):
        for obj in objects_and_cells:
            mxCell = obj.find('./mxCell')
            if mxCell is not None:
                if mxCell.attrib['parent'] == self.element.attrib['id']:
                    self.children.append(obj)
            
# Class for describing a sub-network and its contained sub-network interface containers.
class SubNet:
    def __init__(self, elem, objects_and_cells):
        self.element    = elem
        self.interfaces = []    # list of SubNetIf objects
        self.nodeCount  = -1
        self.buildInterfaces(objects_and_cells)
        return
        
    # Find all subnetwork interface box elements in the super-drawing that are children of this sub-network,
    # create SubNetIf objects for them and add them to our list of SubNetIf objects.
    def buildInterfaces(self, objects_and_cells):
        for obj in objects_and_cells:
            gunns = obj.find('./gunns')
            if gunns is not None:
                if 'Network' == gunns.attrib['type'] and 'Subnet Interface' == gunns.attrib['subtype']:
                    if isDescendant(obj, self.element, objects_and_cells):
                        nodeCountElem = obj.find('./gunnsSubnetIfNodeCount')
                        if nodeCountElem is None:
                            sys.exit(console.abort('in sub-network: ' + obj.attrib['label'] + ', one or more sub-network interface boxes is missing the sub-network node count - you must export the sub-network drawing before adding it to this super-network drawing.'))
                        if 0 == len(self.interfaces):
                            self.nodeCount = int(nodeCountElem.text)
                        elif self.nodeCount != int(nodeCountElem.text):
                            sys.exit(console.abort('in sub-network: ' + obj.attrib['label'] + ', one or more sub-network interface boxes disagree on the sub-network node count - you must export the sub-network drawing before adding it to this super-network drawing.'))
                        self.interfaces.append(SubNetIf(obj, objects_and_cells))
    
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
# This returns '' if the given link is None.
def getLinkName(link):
    if link != None:
        return getSubNetworkName(getParentSubNetwork(link)) + '.' + link.attrib['label']
    return ''

# Returns the id attribute of the given port's source connection.
def getPortSourceId(port):
    source = ''
    cell_attr = port.find('./mxCell').attrib
    if 'source' in cell_attr:
        source = cell_attr['source']
    return source
    
# Returns the id attribute of the given port's target connection.
def getPortTargetId(port):
    target = ''
    cell_attr = port.find('./mxCell').attrib
    if 'target' in cell_attr:
        target = cell_attr['target']
    return target
    
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
    if node is None:
        return ''
    if node.find('gunns').attrib['subtype'] == 'Ground':
        return 'Ground'
    else:
        return 'node ' + node.attrib['label']
    return ''

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

# Return the link element that has this sub-network.link name, or None.
def getLinkByName(name, links):
    for link in links:
        if name == link.attrib['label']:
            return link
    return None

# Checks that the given sub-netowrk has the correct config data for a sub-network.
# If not, changes the config data members.
# We don't have to change the sub-network and its childrens' ID, becuase draw.io
# will have already done that when they copied it into the super-drawing.
def cleanSubNetwork(obj):
    # Look for needed attributes in the object's config data.  If any are missing, we clean it.
    if ('SimVariable'      not in obj.attrib or
        'SuperNodesOffset' not in obj.attrib or
        'sourceDrawing'    not in obj.attrib):
        print('    Assimilated new sub-network instance "' + obj.attrib['label'] + '".')
        print('      ' + console.warn('you should set its "sourceDrawing" value in the drawing!'))
        # Add sim variable, super nodes offset, and project-relative source drawing attributes to the visible shape data
        obj.attrib['SimVariable']      = 'None'
        obj.attrib['SuperNodesOffset'] = 'None'
        obj.attrib['sourceDrawing']    = 'None'
        # Remove the other c## config data attributes.
        removeAttrs = []
        for attr in obj.attrib:
            if re.search('^([c,i])([0-9])', attr) is not None:
                removeAttrs.append(attr)
        for removeAttr in removeAttrs:
            obj.attrib.pop(removeAttr)

# TODO refactor with netexport.py:
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

# TODO refactor with netexport.py:
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

#TODO refactor with supcreate.py:
id_conversions = {}
# Adds a copy of the given element to the super-network, replacing all ID fields
# with a unique set to avoid duplicated ID's in the final super-drawing.
# Returns the copied element with its new ID to the caller.
def addElemToSuper(super, elem, position):
    # If an id prefix is not yet in the keys of id_conversions, then
    # add it as a new key, with a new random value.
    # This assumes id always have a 20-character prefix.
    elem_prefix = elem.attrib['id'][:20]
    if elem_prefix not in id_conversions:
        id_conversions[elem_prefix] = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(20))

    # Copy the source element and convert the id of the copy.
    copiedElem = copy.deepcopy(elem)
    copiedElem.attrib['id'] = id_conversions[elem.attrib['id'][:20]] + elem.attrib['id'][20:]

    # Save source element's ID into the copied element
    gunns = copiedElem.find('gunns')
    if gunns is not None:
        gunns.attrib['drawingId'] = elem.attrib['id']
            
    # Copy remaining id attributes throughout the element.
    if gunns is not None and gunns.attrib['type'] == 'Network' and gunns.attrib['subtype'] != 'Subnet Interface':
        # Network container element's parent is always 2, the super-network container.
        copiedElem.find('mxCell').attrib['parent'] = '2'
    else:
        # TODO lot of repetition here, refactor...
        if elem.tag.startswith('mxCell'):
            if elem.attrib['parent'][:20] not in id_conversions:
                id_conversions[elem.attrib['parent'][:20]] = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(20))
            copiedElem.attrib['parent'] = id_conversions[elem.attrib['parent'][:20]] + elem.attrib['parent'][20:]
            if 'source' in copiedElem.attrib:
                if elem.attrib['source'][:20] not in id_conversions:
                    id_conversions[elem.attrib['source'][:20]] = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(20))
                copiedElem.attrib['source'] = id_conversions[elem.attrib['source'][:20]] + elem.attrib['source'][20:]
            if 'target' in copiedElem.attrib:
                if elem.attrib['target'][:20] not in id_conversions:
                    id_conversions[elem.attrib['target'][:20]] = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(20))
                copiedElem.attrib['target'] = id_conversions[elem.attrib['target'][:20]] + elem.attrib['target'][20:]
        elif elem.tag.startswith('object'):
            if elem.find('mxCell').attrib['parent'][:20] not in id_conversions:
                id_conversions[elem.find('mxCell').attrib['parent'][:20]] = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(20))
            copiedElem.find('mxCell').attrib['parent'] = id_conversions[elem.find('mxCell').attrib['parent'][:20]] + elem.find('mxCell').attrib['parent'][20:]
            if 'source' in copiedElem.find('mxCell').attrib:
                if elem.find('mxCell').attrib['source'][:20] not in id_conversions:
                    id_conversions[elem.find('mxCell').attrib['source'][:20]] = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(20))
                copiedElem.find('mxCell').attrib['source'] = id_conversions[elem.find('mxCell').attrib['source'][:20]] + elem.find('mxCell').attrib['source'][20:]
            if 'target' in copiedElem.find('mxCell').attrib:
                if elem.find('mxCell').attrib['target'][:20] not in id_conversions:
                    id_conversions[elem.find('mxCell').attrib['target'][:20]] = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(20))
                copiedElem.find('mxCell').attrib['target'] = id_conversions[elem.find('mxCell').attrib['target'][:20]] + elem.find('mxCell').attrib['target'][20:]
    # Add copied element to the drawing at the specified location.
    super.insert(position, copiedElem)
    return copiedElem

# Returns the type attribute of the gunns element inside the given element, if found.  Otherwise
# returns None.
def getElemGunnsType(elem):
    gunns = elem.find('gunns')
    if gunns is not None:
        if 'type' in gunns.attrib:
            return gunns.attrib['type']
    return None
    
# Returns the subtype attribute of the gunns element inside the given element, if found.  Otherwise
# returns None.
def getElemGunnsSubtype(elem):
    gunns = elem.find('gunns')
    if gunns is not None:
        if 'subtype' in gunns.attrib:
            return gunns.attrib['subtype']
    return None

# Returns a list of all numbered and ground nodes shapes within the given container.
# This also aborts if any such node is missing its 'key' attribute.
def getKeyedNodes(container, allNodes, allObjects):
    childNodes = []
    for node in allNodes:
        geom = node.find('./mxCell/mxGeometry')
        if isDescendant(node, container, allObjects):
            if 'Key' not in node.attrib or not node.attrib['Key'].strip():
                sys.exit(console.abort('in sub-network interface: ' + container.attrib['label'] + ', a node is missing its \'key\' attribute.'))
            childNodes.append(node)
    return childNodes

# Checks for compatibility between the sub-network interfaces connected by the given connector, and
# returns a list of super-port connections represented by the interface.
def generateSubNetIfSuperPorts(connection, subNets, allNodes, allObjects, links):
    # Check compatibility between sub-network interface connections.  The connector object must connect two
    # sub-network interface boxes.  The connected boxes must have compatible nodes:
    # - the same number of node objects
    # - all nodes must have interface key attributes and all keys must make a matching set
    # - each keyed node set between the boxes must have no more than one numbered (normal or reference) node
    sourceId = getPortSourceId(connection)
    targetId = getPortTargetId(connection)
    if '' == sourceId or '' == targetId:
        sys.exit(console.abort('a sub-network interface connector is missing one or both connections.'))
    if sourceId == targetId:
        sys.exit(console.abort('a sub-network interface connector is connected to the same interface at both ends.'))

    sourceIf = None
    targetIf = None
    for subNet in subNets:
        for interface in subNet.interfaces:
            if interface.element.attrib['id'] == sourceId:
                sourceIf = interface.element
            elif interface.element.attrib['id'] == targetId:
                targetIf = interface.element
    if sourceIf is None or targetIf is None:
        sys.exit(console.abort('a sub-network interface connector is missing one or both connections to an interface container.'))
    
    # Sort the interface nodes by their Key attributes.
    sourceNodes = getKeyedNodes(sourceIf, allNodes, allObjects)
    targetNodes = getKeyedNodes(targetIf, allNodes, allObjects)
    sourceSort = sorted(sourceNodes, key=lambda e: e.attrib['Key'])
    targetSort = sorted(targetNodes, key=lambda e: e.attrib['Key'])
    if len(sourceSort) != len(targetSort):
        sys.exit(console.abort('connected sub-network interfaces: ' + sourceIf.attrib['label'] +  ' and ' + targetIf.attrib['label'] + ' have mismatching number of interface nodes.'))
        
    for index in range(0, len(sourceSort)):
        if sourceSort[index].attrib['Key'] != targetSort[index].attrib['Key']:
            sys.exit(console.abort('connected sub-network interfaces: ' + sourceIf.attrib['label'] +  ' and ' + targetIf.attrib['label'] + ' have mismatching interface node Keys.'))
        if sourceSort[index].find('./gunns').attrib['subtype'] != 'Ground' and targetSort[index].find('./gunns').attrib['subtype'] != 'Ground':
            sys.exit(console.abort('connected sub-network interfaces: ' + sourceIf.attrib['label'] +  ' and ' + targetIf.attrib['label'] + ' have more than one numbered node for Key: ' + sourceSort[index].attrib['Key'] + '.'))
    
    # Create a list of super-ports represented by the connected interfaces.
    # List of tuples: (subnet.link name, default node map, port #, other sub-network's node #, other sub-network's element)
    superPorts = []
    sourceNetwork = getParentSubNetwork(sourceIf)
    targetNetwork = getParentSubNetwork(targetIf)
    sourceConnections = sourceIf.findall('./gunnsSubnetIfConnection')
    targetConnections = targetIf.findall('./gunnsSubnetIfConnection')
    for connection in sourceConnections:
        # Find the node # in the target network interface with the same key as our link connection's key.
        # If it doesn't have a number (Ground), then skip this connection and there is no super-port.
        for node in targetNodes:
            if node.attrib['Key'] == connection.attrib['Key']:
                if 'Ground' != node.find('./gunns').attrib['subtype']:
                    linkName = sourceNetwork.attrib['label'] + '.' + connection.attrib['Link']
                    superPorts.append((linkName, connection.attrib['Map'], connection.attrib['Port'], int(node.attrib['label']), targetNetwork))
        
    for connection in targetConnections:
        for node in sourceNodes:
            if node.attrib['Key'] == connection.attrib['Key']:
                if 'Ground' != node.find('./gunns').attrib['subtype']:
                    linkName = targetNetwork.attrib['label'] + '.' + connection.attrib['Link']
                    superPorts.append((linkName, connection.attrib['Map'], connection.attrib['Port'], int(node.attrib['label']), sourceNetwork))
    
    return superPorts

# Replaces the given sub-network and all of its children with a new instance from the source drawing.
# Note, rootroot is the drawing's mxGraphModel.root element, not the actual root element.
def updateSubNet(subNet, subPathFile, rootroot):
    print('    Updating sub-network "' + subNet.attrib['label'] + '" from source drawing.')
    saveSimVar = subNet.attrib['SimVariable']
    saveOffset = subNet.attrib['SuperNodesOffset']
    saveSource = subNet.attrib['sourceDrawing']
    saveLabel  = subNet.attrib['label']
    saveX      = subNet.find('mxCell/mxGeometry').attrib['x']
    saveY      = subNet.find('mxCell/mxGeometry').attrib['y']
    saveIndex  = list(rootroot).index(subNet)
    
    # Collect all objects and mxCells in the super-network drawing.
    allObjects = rootroot.findall('./object')
    allCells = rootroot.findall('./mxCell')[1:]
    allObjectsAndCells = []
    for object in allObjects:
        allObjectsAndCells.append(object)
    for cell in allCells:
        allObjectsAndCells.append(cell)
        
    # Collect all super-ports, subnet interfaces and subnet interface connections in the super-network drawing.
    allSuperPorts = []
    subnetInterfaces = []
    subnetInterfacesPresent = False
    for obj in allObjects:
        if 'Super Port' == getElemGunnsType(obj):
            allSuperPorts.append(obj)
        elif 'Subnet Interface' == getElemGunnsType(obj):
            subnetInterfaces.append(obj)
            subnetInterfacesPresent = True
        elif 'Subnet Interface Connection' == getElemGunnsType(obj):
            allSuperPorts.append(obj)
            
    # Find all child objects of the sub-network, and all super-ports connected to them.
    subObjects = []
    subCells = []
    subPorts = {}
    for obj in allObjects:
        if isDescendant(obj, subNet, allObjectsAndCells):
            subObjects.append(obj)
            objId = obj.attrib['id']
            for port in allSuperPorts:
                if getPortTargetId(port) == objId or getPortSourceId(port) == objId:
                    # For the super-ports, store the link/node type, label, and source drawing ID of
                    # source and target that are in this sub-network.  These will be used to find new
                    # source and target in the new sub-network.  This could be entered twice for the
                    # same port from different source & target objects, so only create the new value
                    # dictionary once.
                    if 'drawingId' in obj.find('gunns').attrib:
                        drawingId = obj.find('gunns').attrib['drawingId']
                    else:
                        drawingId = None
                    portId = port.attrib['id']
                    if portId not in subPorts.keys():
                        subPorts[portId] = {}
                        subPorts[portId]['port'] = port
                    if getPortTargetId(port) == objId:
                        subPorts[portId]['targetType'] = obj.find('./gunns').attrib['type']
                        subPorts[portId]['targetLabel'] = obj.attrib['label']
                        subPorts[portId]['targetDrawId'] = drawingId
                        port.find('mxCell').attrib['target'] = 'orphaned'
                    if getPortSourceId(port) == objId:
                        subPorts[portId]['sourceType'] = obj.find('./gunns').attrib['type']
                        subPorts[portId]['sourceLabel'] = obj.attrib['label']
                        subPorts[portId]['sourceDrawId'] = drawingId
                        port.find('mxCell').attrib['source'] = 'orphaned'

    for cell in allCells:
        if isDescendant(cell, subNet, allObjectsAndCells):
            subCells.append(cell)
            
    # Clean duplicates from the lists.
    subObjects = list(set(subObjects))
    subCells = list(set(subCells))
    
    # Remove all of this sub-network's objects and cells, then the sub-network itself.
    for cell in subCells:
        rootroot.remove(cell)            
    for obj in subObjects:
        rootroot.remove(obj)            
    rootroot.remove(subNet)
    
    # Parse the sub-network source drawing.
    subTree = ET.parse(subPathFile)
    subRoot = subTree.getroot()
    # Handle draw.io's compressed vs. uncompressed xml file formats.
    # If root node = mxGraphModel then this is uncompressed.
    # If root node = mxfile then this is compressed, and the mxGraphModel
    # section is compressed in the <mxfile><diagram>text</diagram></mxfile>.
    if subRoot.tag.startswith('mxfile'):
        compressed_diagram = subRoot[0].text
        diagram = compression.decompress(compressed_diagram)
        # Reset the root and tree after decompressing.  This discards
        # the outer <mxfile><diagram> elements and makes a consistent
        # tree structure with the un-compressed source file.
        subRoot = ET.fromstring(diagram)
        subTree._setroot(subRoot)

    if not subRoot.tag.startswith('mxGraphModel'):
        sys.exit(console.abort('sub-network\'s source drawing is not a recognized file.'))
    subRootroot = copy.deepcopy(subRoot.find('./root'))

    # Find all objects and cells in the source drawing network.
    # TODO refactor with supcreate.py
    subObjects = subRootroot.findall('./object')
    # This skips <mxCell id="0"/> since it has no parent attribute
    subCells = subRootroot.findall('./mxCell')[1:]
    subObjectsAndCells = []
    for object in subObjects:
        subObjectsAndCells.append(object)
    for cell in subCells:
        subObjectsAndCells.append(cell)

    # Find the sub-network container object.
    subConfig = None
    newSubNet = None
    for object in subObjects:
        if 'Network' == getElemGunnsType(object):
            if 'Super' == getElemGunnsSubtype(object):
                sys.exit(console.abort('nested super-networks aren\'t supported yet.'))
            elif 'Sub' == getElemGunnsSubtype(object):
                subConfig = object
                newSubNet = SubNet(object, subObjectsAndCells)
                break
    if subConfig is None:
        sys.exit(console.abort('a network config wasn\'t found in sub-network\'s source drawing.'))

    # Update the sub-network config data and geometry to saved values from before.
    subConfig.attrib['label']            = saveLabel
    subConfig.attrib['SimVariable']      = saveSimVar
    subConfig.attrib['SuperNodesOffset'] = saveOffset
    subConfig.attrib['sourceDrawing']    = saveSource
    subConfig.find('mxCell/mxGeometry').attrib['x']             = saveX
    subConfig.find('mxCell/mxGeometry').attrib['y']             = saveY
    subConfig.find('mxCell/mxGeometry/mxRectangle').attrib['x'] = saveX
    subConfig.find('mxCell/mxGeometry/mxRectangle').attrib['y'] = saveY
    # Remove the other c## config data attributes.
    # TODO refactor with cleanSubNetwork
    removeAttrs = []
    for attr in subConfig.attrib:
        if re.search('^([c,i])([0-9])', attr) is not None:
            removeAttrs.append(attr)
    for removeAttr in removeAttrs:
        subConfig.attrib.pop(removeAttr)
        
    # Add the sub-network to the super-drawing tree at the old sub-network location.  The location
    # is preserved to keep all sub-networks in the original order.
    newSubNet.element = addElemToSuper(rootroot, subConfig, saveIndex)
    newObjectsAndCells = []

    # Add subnet interfaces and their children to the end of the super-drawing tree.
    if len(newSubNet.interfaces) > 0:
        for interface in newSubNet.interfaces:
            newSubnetInterface = addElemToSuper(rootroot, interface.element, -1)
            newObjectsAndCells.append(newSubnetInterface)
            for child in interface.children:
                newObjectsAndCells.append(addElemToSuper(rootroot, child, -1))

    # Add all child objects and mxCells of the sub-network container to the end of the super tree.
    else:
        for object in subObjectsAndCells:
            if isDescendant(object, subConfig, subObjectsAndCells):
                if 'Node' == getElemGunnsType(object):
                    if getElemGunnsSubtype(object) in numberedNodeSubtypes:
                        object.attrib['label'] = str(int(saveOffset) + int(object.attrib['label']))
                elif 'Link' == getElemGunnsType(object):
                    # Destroy link info to prevent this network copy from being used as source for netexport.
                    object.find('gunns').attrib['subtype'] = ''
                newObjectsAndCells.append(addElemToSuper(rootroot, object, -1))

    # Reconnect super-ports to the node or link.  First a match of both label and drawingId is sought.
    # Otherwise a match of the label is sought, otherwise a match of the drawingId is sought.
    # This allows the user to change either name or the ID (as a new shape) in the source drawing and
    # we'll make the connection.  If still no match is found, then delete the super-port and warn the
    # user that it's gone.
    orphanedPorts = []
    for value in subPorts.values():
        port = value['port']
        if 'sourceType' in value.keys():
            for object in newObjectsAndCells:
                if (value['sourceType'] == getElemGunnsType(object)) and \
                   (value['sourceLabel'] == object.attrib['label']) and \
                   (value['sourceDrawId'] == object.find('gunns').attrib['drawingId']):
                    port.find('mxCell').attrib['source'] = object.attrib['id']
                    break
                    
            if port.find('mxCell').attrib['source'] == 'orphaned':
                for object in newObjectsAndCells:
                    if (value['sourceType'] == getElemGunnsType(object)) and \
                       (value['sourceLabel'] == object.attrib['label']):
                        port.find('mxCell').attrib['source'] = object.attrib['id']
                        break
                    
            if port.find('mxCell').attrib['source'] == 'orphaned':
                for object in newObjectsAndCells:
                    if (value['sourceType'] == getElemGunnsType(object)) and \
                       (value['sourceDrawId'] == object.find('gunns').attrib['drawingId']):
                        port.find('mxCell').attrib['source'] = object.attrib['id']
                        break
                    
            if port.find('mxCell').attrib['source'] == 'orphaned':
                orphanedPorts.append(port)
                print('      ' + console.warn('an orphaned super-port ' + port.attrib['label'] + ' on ' + saveLabel + ' ' + value['sourceType'] + ' ' + value['sourceLabel'] + ' was deleted.'))
                
        if 'targetType' in value.keys():
            for object in newObjectsAndCells:
                if (value['targetType'] == getElemGunnsType(object)) and \
                   (value['targetLabel']  == object.attrib['label']) and \
                   (value['targetDrawId'] == object.find('gunns').attrib['drawingId']):
                    port.find('mxCell').attrib['target'] = object.attrib['id']
                    break
                    
            if port.find('mxCell').attrib['target'] == 'orphaned':
                for object in newObjectsAndCells:
                    if (value['targetType'] == getElemGunnsType(object)) and \
                       (value['targetLabel']  == object.attrib['label']):
                        port.find('mxCell').attrib['target'] = object.attrib['id']
                        break
                    
            if port.find('mxCell').attrib['target'] == 'orphaned':
                for object in newObjectsAndCells:
                    if (value['targetType']  == getElemGunnsType(object)) and \
                       (value['targetDrawId'] == object.find('gunns').attrib['drawingId']):
                        port.find('mxCell').attrib['target'] = object.attrib['id']
                        break
                    
            if port.find('mxCell').attrib['target'] == 'orphaned':
                orphanedPorts.append(port)
                print('      ' + console.warn('an orphaned super-port ' + port.attrib['label'] + ' on ' + saveLabel + ' ' + value['targetType'] + ' ' + value['targetLabel'] + ' was deleted.'))
           
    for port in orphanedPorts:
        rootroot.remove(port)

# TODO refactor with similar code in netexport.py:
# Normalizes a unicode string to an ascii string.
def normalizeString(s):
    if isinstance(s, str):
        return s
    elif isinstance(s, unicode):
        return unicodedata.normalize('NFKD', s).encode('ascii', 'ignore')
    return ''

# TODO refactor with similar code in netexport.py:
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
cmd_parser = OptionParser()
cmd_parser.add_option("-d", action="store_true", help="look in ~/Downloads for a newer version", dest="downloads", default="false")
cmd_parser.add_option("-m", action="store_true", help="only do error checks and maintenance updates to the diagram file", dest="maintenance", default="false")
cmd_parser.add_option("-g", action="store_true", help="only do generation of the output file", dest="generation", default="false")
cmd_parser.add_option("-u", action="store", help="update sub-networks from their sourceDrawing filenames relative to the provided project absolute path", dest="project_path", default="")
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
ports = []
superPorts = []          # XML elements for super-port connectors
superInterfacePorts = [] # Data for super-ports derived from subNetIfConnections
links = []
numberedNodes = []
gndNodes = []
allNodes = []
superConfig = None
linkNamesWithSuperPorts = []
linkDataModels = []
subNets = []
subNetIfConnections = [] # XML elements for connections between sub-network interface containers
doxNotices = []
doxCopyrights = []
doxLicenses = []
doxData = []
doxReferences = []
doxAssumptions = []

# Make a list of all sub-networks and check for duplicate names.
for obj in objects:
    gunns = obj.find('gunns')
    if gunns is not None:
        if gunns.attrib['type'] == 'Network':
            if gunns.attrib['subtype'] == 'Sub':
                # Abort if this sub-network has the same local name as another one.
                for netConfig in netConfigs:
                    if netConfig.attrib['label'] == obj.attrib['label']:
                        sys.exit(console.abort('sub-networks have the same instance name ' + obj.attrib['label'] + '.'))
                cleanSubNetwork(obj)
                netConfigs.append(obj)

# Update sub-networks from their source drawings.  Do the updates here so that we can collect the
# latest object type lists after the updates, below.
if options.project_path:
    # Skip source drawings if their timestamp is older than this super-drawing.
    timeStamp = os.path.getmtime(inputPathFile)
    for subNet in netConfigs:
        if 'sourceDrawing' not in subNet.attrib or 'None' == subNet.attrib['sourceDrawing']:
            print('    ' + console.warn('sub-network "' + subNet.attrib['label'] + '" is missing its source drawing filename.'))
        else:
            subPathFile = options.project_path + '/' + subNet.attrib['sourceDrawing']
            # Check for existence of this file and warn if missing.
            if os.path.isfile(subPathFile):
                if os.path.getmtime(subPathFile) > timeStamp:
                    # Update from source, and re-collect all objects after sub-network source updates.
                    updateSubNet(subNet, subPathFile, rootroot)
                    objects = root.findall('./root/object')
                    # Reset the id prefix conversions dictionary for the next instance.
                    id_conversions = {}
            else:
                print('    ' + console.warn('can\'t open file: ' + subPathFile + ' to update sub-network "' + subNet.attrib['label'] + '".'))

    # Rebuild the sub-networks list since some may have been replaced.
    netConfigs = []
    for obj in objects:
        if 'Network' == getElemGunnsType(obj) and 'Sub' == getElemGunnsSubtype(obj):
            netConfigs.append(obj)
    
# Rebuild all objects and cells to refresh with the new copies.
# TODO refactor with above, DRY
objects = root.findall('./root/object')
mxcells = root.findall('./root/mxCell')[1:]
objects_and_cells = []
for an_object in objects:
    objects_and_cells.append(an_object)
for cell in mxcells:
    objects_and_cells.append(cell)

# Make a list of various GUNNS object types
for obj in objects:
    gunnsType = getElemGunnsType(obj)
    gunnsSubtype = getElemGunnsSubtype(obj)
    if 'Port' == gunnsType:
        ports.append(obj)
    elif 'Super Port' == gunnsType:
        superPorts.append(obj)
    elif 'Link' == gunnsType:
        # re-wipe the link gunns subtype to prevent the sub-network from being used as
        # source for netexport.  This covers manual copies of networks into this drawing
        # after creation.
        obj.find('gunns').attrib['subtype'] = ''
        links.append(obj)
    elif 'Node' == gunnsType:
        allNodes.append(obj)
        if gunnsSubtype in numberedNodeSubtypes:
            numberedNodes.append(obj)
        elif 'Ground' == gunnsSubtype:
            gndNodes.append(obj)
    elif 'Network' == gunnsType:
        if 'Super' == gunnsSubtype:
            superConfig = obj
        elif 'Sub' == gunnsSubtype:
            subNets.append(SubNet(obj, objects_and_cells))
    elif 'Subnet Interface Connection' == gunnsType:
        subNetIfConnections.append(obj)
        
# Check for missing super-network config.
if superConfig is None:
    sys.exit(console.abort('there is no super-network config object.'))
        
# Once the super-network config is known, make a list fo all Doxygen objects that belong
# to the super-network drawing, outside of the super-network container itself.  Note that
# we ignore all doxygen objects within the super-network container or its contained sub-
# network containers.
#TODO problem, getting in the inf dependency loop....
for obj in objects:
    gunnsType = getElemGunnsType(obj)
    gunnsSubtype = getElemGunnsSubtype(obj)
    if 'Dox' == gunnsType and not isDescendant(obj, superConfig, objects_and_cells):
        if 'notice' == gunnsSubtype:
            doxNotices.append(obj)
        elif 'copyright' == gunnsSubtype:
            doxCopyrights.append(obj)
        elif 'license' == gunnsSubtype:
            doxLicenses.append(obj)
        elif 'data' == gunnsSubtype:
            doxData.append(obj)
        elif 'references' == gunnsSubtype:
            doxReferences.append(obj)
        elif 'assumptions' == gunnsSubtype:
            doxAssumptions.append(obj)

# Check for combination of super-ports and subnet interface connections: currently we don't
# support both at the same time.
if (len(superPorts) > 0 and len(subNetIfConnections) > 0):
    sys.exit(console.abort('a mix of super-ports and sub-net interface connections is not allowed - use one or the other.'))
        
# Get super-ports derived from sub-network interface container connections.
for subNetIfConnection in subNetIfConnections:
    superInterfacePorts += generateSubNetIfSuperPorts(subNetIfConnection, subNets, allNodes, objects_and_cells, links)
    
# Re-number nodes and sub-network super node offsets to account for new or deleted sub-networks
# that the user has manually added or removed in draw.io.  Note this won't fix gaps in node numbers
# within a sub-network.  This only fixes gaps between sub-networks.  Sub-networks with internal gaps
# are broken and should be fixed by netexport.py before using in a super-network.
superNodesCount = 0
for subNet in netConfigs:
    superNodeOffset = superNodesCount
    try:
        oldOffset = int(subNet.attrib['SuperNodesOffset'])
    except ValueError:
        oldOffset = -1
    if superNodeOffset != oldOffset:
        print('    Changed super nodes offset for sub-network "' + subNet.attrib['label'] + '".')
    subNet.attrib['SuperNodesOffset'] = str(superNodeOffset)
    
    theSubNetObject = None
    for subNetObj in subNets:
        if subNet == subNetObj.element:
            theSubNetObject = subNetObj
            break
        
    if theSubNetObject is not None and theSubNetObject.nodeCount > 0:
        superNodesCount += theSubNetObject.nodeCount
    else:
        subNodes = []
        lowestNodeNumber = 999999999
        for node in numberedNodes:
            # Collect all numbered nodes in this sub-network.  This includes References nodes as well
            # as normal (Basic, Fluid, etc.) nodes, but not Ground nodes.  Find the lowest node number
            # in this set.
            if node.find('mxCell').attrib['parent'] == subNet.attrib['id']:
                subNodes.append(node)
                nodeNumber = int(node.attrib['label'])
                if nodeNumber < lowestNodeNumber:
                    lowestNodeNumber = nodeNumber
        
        for node in subNodes:
            # Re-number the numbered nodes as their sub-network's super node offset, plus their relative
            # number in their sub-network.
            oldNodeNumber = int(node.attrib['label'])
            node.attrib['label'] = str(superNodeOffset + oldNodeNumber - lowestNodeNumber)
            if (node.find('./gunns').attrib['subtype'] in normalNodeSubtypes):
                # Increment the total super-network normal node count, not counting Reference nodes.
                superNodesCount += 1
    
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
    # Add the link to the list of those that have super-ports.
    if linkName not in linkNamesWithSuperPorts:
        linkNamesWithSuperPorts.append(linkName)
        linkDataModels.append(LinkDataModel(link, linkName))

# For super-ports defined by the sub-network interface connections, add their links
# to the list of links that have super-ports.
for port in superInterfacePorts:
    if port[0] not in linkNamesWithSuperPorts:
        linkNamesWithSuperPorts.append(port[0])
        linkDataModels.append(LinkDataModel(None, port[0]))
    
if len(subNetIfConnections) == 0:
    # Add port and super-port elements to the link data model, and remove normal port elements
    # from the drawing that are overridden by super-ports.
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

# Delete Ground nodes that are left with no port connections on them, and are not in a sub-network interface container.
# Those in sub-network interface containers have had a Key attribute added, so we check for that.
for node in allNodes:
    if 'Key' not in node.attrib:
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
            if node.find('gunns').attrib['subtype'] in numberedNodeSubtypes:
                # Warn the user about unconnected network nodes.
                # TODO this gives a nuisance warning for reference nodes that have super-port connections but no normal connections.
                #      Does noConnections include super-ports, and should it?
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
revline = '@author Auto-generated by the GunnsDraw supexport script version ' + GUNNSDRAW_VERSION + ' on ' + str(datetime.now())
subNets = []
subNetLength = 0
for subNet in netConfigs:
    if len(subNet.attrib['label']) > subNetLength:
        subNetLength = len(subNet.attrib['label'])

for subNet in netConfigs:
    subNets.append([subNet.attrib['label'], subNet.attrib['SuperNodesOffset'], os.path.splitext(os.path.split(subNet.attrib['sourceDrawing'])[1])[0], ' '*(subNetLength - len(subNet.attrib['label']))])

if len(subNetIfConnections) > 0:
    for link in linkDataModels:
        portMapList = None
        for ifPort in superInterfacePorts:
            # Find all superInterfacePorts that apply to this link
            # List of tuples: (subnet.link name, default node map, port #, other sub-network's node #, other sub-network's element)
            localSubnetName = ifPort[0].split('.')[0]
            linkName        = ifPort[0].split('.')[1]
            if linkName == link.name and localSubnetName == link.subNetName:
                if portMapList is None:
                    # First set the port map to the default, but only do this on the first superport
                    portMapList = ifPort[1].split(',')
                    for index in range(0, len(portMapList)):
                        if int(portMapList[index]) < 0:
                            # Now go through and replace negative numbers (Ground) with the super-network ground.
                            portMapList[index] = 'super_ground'
                        else:
                            # Add the local subnet's offset
                            portMapList[index] = portMapList[index].strip() + '+offset_' + localSubnetName
                # Override the port map index that the sub-network interface connection applies to.
                portMapList[int(ifPort[2])] = str(ifPort[3]) + '+offset_' + ifPort[4].attrib['label']
        # Format the link.portMap, into a string
        link.portMap = '[' + ', '.join(portMapList) + ']'
        link.ports = portMapList # template uses the length of this list to size the Trick array allocation.
        
else:
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

# Format Doxygen fields: TODO refactor with similar code in netexport.py:
# Format Doxygen Attention fields
noticeData = []
for notice in doxNotices:
    # Strip html tags out of the label value, but convert html breaks into new lines.
    newNotice = '@attention  ' + re.sub('<[^<]+?>', '', re.sub('<br>', '\n            ', notice.attrib['label']) + '\n\n')
    noticeData.append(normalizeString(newNotice))

# Format Doxygen Copyright fields
# TODO lot in common with notices above, refactor for DRY...
copyrightData = []
for copyright in doxCopyrights:
    # Strip html tags out of the label value, but convert html breaks into new lines.
    newCopyright = '@copyright  ' + re.sub('<[^<]+?>', '', re.sub('<br>', '\n            ', copyright.attrib['label']) + '\n\n')
    copyrightData.append(normalizeString(newCopyright))

# Format Doxygen License fields
# TODO lot in common with notices above, refactor for DRY...
licenseData = []
for license in doxLicenses:
    # Strip html tags out of the label value, but convert html breaks into new lines.
    newLicense = '@license  ' + re.sub('<[^<]+?>', '', re.sub('<br>', '\n          ', license.attrib['label']) + '\n\n')
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

# Format document Reference fields.  Note this is different than the Trick header style.
referencesData = []
for reference in doxReferences:
    theReferencesList = buildSwimlane1D(reference, mxcells)
    for referenceItem in theReferencesList[2:]:
        newReference = '@cite ' + re.sub('<[^<]+?>', '', re.sub('<br>', '\n   ', referenceItem) + '\n')
        referencesData.append(normalizeString(newReference))

# Format document Assumptions & Limitations fields.
# TODO DRY with References above...
assumptionsData = []
for assumption in doxAssumptions:
    theAssumptionsList = buildSwimlane1D(assumption, mxcells)
    for assumptionItem in theAssumptionsList[2:]:
        newAssumption = '(' + re.sub('<[^<]+?>', '', re.sub('<br>', '\n   ', assumptionItem) + ')')
        assumptionsData.append(normalizeString(newAssumption))

data_model = dict([('functionName',   baseFileName),
                   ('revline',        revline),
                   ('subNets',        subNets),
                   ('links',          linkDataModels),
                   ('solverConfig',   getConfigData(superConfig).split(',')),
                   ('doxNotices',     noticeData),
                   ('doxCopyrights',  copyrightData),
                   ('doxLicenses',    licenseData),
                   ('doxData',        doxygenData),
                   ('doxReferences',  referencesData),
                   ('doxAssumptions', assumptionsData),
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
