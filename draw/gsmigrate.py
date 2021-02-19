#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, March 2019, --, Initial implementation.}
# @revs_end
#
# Input a GunnShow Visio drawing saved from Visio as .vdx (Visio XML)
# Output an equivalent GunnsDraw drawing .xml
#
# Known limitations:
# - We don't copy formatting such as color, fill, line style, font, etc.
# - Some connector line paths will be different near the link connection point.
# - We don't copy the link label orientation or pin location relative to the link shape.
# - These links aren't supported yet: IPS, Power Bus, Switch Card.
# - TS21's Sensors shapes aren't supported yet.
# - We don't yet support custom links from these legacy projects: TS21, HESTIA, Boeing.
#
# TODO:
# - TC Config, TC State
# - Link init order

import os
import sys
import re
import copy
import collections
import math
from optparse import OptionParser

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

# More imports:
import xml.etree.ElementTree as ET
import modules.shapeLibs as shapeLibs
import modules.consoleMsg as console
import modules.xmlUtils as xmlUtils
import string
import random
from ctypes import c_int64

# Random ID string for id attributes
ID  = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(20)) + '-'
idn = c_int64(0)

# Returns a drawing-unique id attribute and increments the id counter for next time.
def id(n):
    n.value += 1
    return ID + str(n.value)

# This copies a GunnShow shape's config & input data to the GunnsDraw shape.
# gs_config is a list of the GunnShow shape's config & input data values.
# gd is the GunnsDraw shape.
def copyConfigInputData(gs_config, gd):
    # Make an ordered list of the GunnsDraw config & input data attribute names.
    od = collections.OrderedDict(sorted(gd.attrib.items()))
    gd_list = []
    for key, value in od.items():
        if None != re.search('^([c,i])([0-9])', key):
            gd_list.append(key)
    # Abort if the list length doesn't match between GunnShow & GunnsDraw.
    if len(gd_list) != len(gs_config):
        # TODO more helpful info: which objects
        sys.exit(console.abort('config/input data length mismatch for shape: ' + gd.attrib['label']))
    for i in range(0, len(gd_list)):
        gd.attrib[gd_list[i]] = gs_config[i]

# Returns the opposite cardinal direction from the given GunnsDraw shape direction (east, south, etc.)
def flipDirection(d):
    if 'east' == d:
        return 'west'
    if 'west' == d:
        return 'east'
    if 'south' == d:
        return 'north'
    if 'north' == d:
        return 'south'
    return d

# Returns the value of the given key in the shape's mxCell's style attribute.
def getStyle(shape, key, default):
    styles = shape.find('./mxCell').attrib['style'].split(';')
    for style in styles:
        if key == style.split('=')[0]:
            return style.split('=')[1]
    return default
    
# Returns a string with the shape's mxCell's style value, with the
# given key's value replaced with the given value.  If the key wasn't
# in the source style, it is added along with the value to the end.
# value must be a string.
def setStyle(shape, key, value):
    styles = shape.find('./mxCell').attrib['style'].split(';')
    keyMissing = True
    result = ''
    for style in styles:
        if key == style.split('=')[0]:
            result += (key + '=' + value + ';')
            keyMissing = False
        elif len(style) > 1:
            result += (style + ';')
    if keyMissing:
        result += (key + '=' + value + ';')
    return result

# Returns, as an angle in degrees, the sum of the given GunnsDraw shape's direction and rotation angles.
# This is measured clockwise from 'east' (right).
def getTotalRotation(shape):
    result = float(getStyle(shape, 'rotation', 0.0))
    direction = getStyle(shape, 'direction', 'east')
    if 'south' == direction:
        result += 90.0
    elif 'west' == direction:
        result += 180.0
    elif 'north' == direction:
        result -= 90.0
    return result

# Returns the absolute coordinates in the GunnsDraw page of the given shape's center.
# Returns an [X,Y] tuple.
def getShapeCenter(shape):
    geomattr = shape.find('mxCell/mxGeometry').attrib
    shapeX = float(geomattr['x'])
    shapeY = float(geomattr['y'])
    shapeW = float(geomattr['width'])
    shapeH = float(geomattr['height'])
    return [(shapeX + 0.5 * shapeW), (shapeY + 0.5 * shapeH)]

# Returns the absolute coordinates in the GunnsDraw page of the relX,relY coordinates relative to the shape,
# including the shape's location in the drawing, total rotation and flips.
# Returns an [X,Y] tuple.
def getShapeRelAbsCoords(shape, relX, relY):
    shape_center = getShapeCenter(shape)
    shape_angle  = math.radians(getTotalRotation(shape))
    # Correct the given shape relative coordinates for flips
    rX = math.fabs(relX - float('1' == getStyle(shape, 'flipH', '0')))
    rY = math.fabs(relY - float('1' == getStyle(shape, 'flipV', '0')))
    # Vector to the relative point from the shape center
    geomattr = shape.find('mxCell/mxGeometry').attrib
    dX = (rX - 0.5) * float(geomattr['width'])
    dY = (rY - 0.5) * float(geomattr['height'])
    d  = math.sqrt(dX*dX + dY*dY)
    rel_angle = math.atan2(dY, dX)
    total_angle = shape_angle + rel_angle
    absX = shape_center[0] + d*math.cos(total_angle)
    absY = shape_center[1] + d*math.sin(total_angle)
    return [absX, absY]

# Returns the range between two sets of coordinates as [X,Y] tuples.
def computeRange(a, b):
    dX = a[0] - b[0]
    dY = a[1] - b[1]
    return math.sqrt(dX*dX + dY*dY)

# Adds a list object and its data row cells to the network.
#   root: XML element to add the list to.
#   netId: ID of the network, the parent of this list.
#   shape: XML element of the given list shape type (Fluid Config vs. Reactions, etc.)
#   data:  List data:
#     [0]: name
#     [1]: x position, GD coordinates
#     [2]: y position, GD coordinates
#     [3]: list row values
def addList(root, netId, shape, data):
    shape.attrib['label']  = str(data[0])
    gd_id = id(idn)
    shape.attrib['id'] = gd_id
    cell_attr = shape.find('mxCell').attrib
    cell_attr['parent'] = netId
    geom_attr = shape.find('./mxCell/mxGeometry').attrib
    rect_attr = shape.find('./mxCell/mxGeometry/mxRectangle').attrib
    geom_attr['x'] = str(data[1])
    geom_attr['y'] = str(data[2])
    rect_attr['x'] = str(data[1])
    rect_attr['y'] = str(data[2])
    rect_attr['height'] = str(20 * (1 + len(data[3])))
    root.append(shape)
    # Add the rows
    for i in range(0, len(data[3])):
        cell = ET.Element('mxCell')
        gd_cell_id = id(idn)
        cell.attrib['id'] = gd_cell_id
        cell.attrib['value']  = data[3][i]
        cell.attrib['style']  = 'text;strokeColor=none;fillColor=none;align=left;verticalAlign=top;spacingLeft=0;spacingRight=0;overflow=hidden;rotatable=0;points=[[0,0.5],[1,0.5]];portConstraint=eastwest;fontSize=11;'
        cell.attrib['vertex'] = '1'
        cell.attrib['parent'] = gd_id
        geom = ET.Element('mxGeometry')
        geom.attrib['y']      = str(20 * (i+1))
        geom.attrib['width']  = '160'
        geom.attrib['height'] = '20'
        geom.attrib['as']     = 'geometry'
        cell.append(geom)
        root.append(cell)

# Adds a table object and its data row cells to the network.
# Similar to addList above except there are 2 columns.
#   root:  XML element to add the table to.
#   netId: ID of the network, the parent of this table.
#   shape: XML element of the given table shape type (Fluid State vs. Data Table, etc.)
#   data:  List data:
#     [0]: name
#     [1]: x position, GD coordinates
#     [2]: y position, GD coordinates
#     [3]: list row values
#   headRow: the data row number to be given a bottom border
#   width:   the total table width
def addTable(root, netId, shape, data, headRow, width):
    shape.attrib['label'] = str(data[0])
    gd_id = id(idn)
    shape.attrib['id'] = gd_id
    cell_attr = shape.find('mxCell').attrib
    cell_attr['parent'] = netId
    geom_attr = shape.find('./mxCell/mxGeometry').attrib
    rect_attr = shape.find('./mxCell/mxGeometry/mxRectangle').attrib
    geom_attr['x'] = str(data[1])
    geom_attr['y'] = str(data[2])
    rect_attr['x'] = str(data[1])
    rect_attr['y'] = str(data[2])
    rect_attr['height'] = str(20 * (1 + len(data[3])))
    root.append(shape)
    # Add the data rows.
    for i in range(0, len(data[3])):
        # Right column:
        cell2 = ET.Element('mxCell')
        gd_cell2_id = id(idn)
        cell2.attrib['id'] = gd_cell2_id
        cell2.attrib['value'] = data[3][i][1]
        # Set the bottom border on the header row:
        bottom = '0'
        if i == headRow:
            bottom = '1'
        cell2.attrib['style'] = 'shape=partialRectangle;top=0;left=0;right=0;bottom=' + bottom + ';align=left;verticalAlign=top;fillColor=none;spacingLeft=' + str(width/2 + 4) + ';spacingRight=4;overflow=hidden;rotatable=0;points=[[0,0.5],[1,0.5]];portConstraint=eastwest;dropTarget=0;fontStyle=0;fontSize=11;'
        cell2.attrib['vertex'] = '1'
        cell2.attrib['parent'] = gd_id
        geom2 = ET.Element('mxGeometry')
        geom2.attrib['y'] = str(20 * (i+1))
        geom2.attrib['width'] = str(width)
        geom2.attrib['height'] = '20'
        geom2.attrib['as'] = 'geometry'
        cell2.append(geom2)
        root.append(cell2)
        # Left column:
        cell1 = ET.Element('mxCell')
        gd_cell1_id = id(idn)
        cell1.attrib['id'] = gd_cell1_id
        cell1.attrib['value'] = data[3][i][0]
        cell1.attrib['style'] = 'shape=partialRectangle;fontStyle=0;top=0;left=0;bottom=0;fillColor=none;align=left;verticalAlign=top;spacingLeft=0;spacingRight=0;overflow=hidden;rotatable=0;points=[];portConstraint=eastwest;part=1;fontSize=11;'
        cell1.attrib['vertex'] = '1'
        cell1.attrib['connectable'] = '0'
        cell1.attrib['parent'] = gd_cell2_id
        geom1 = ET.Element('mxGeometry')
        geom1.attrib['width'] = str(width / 2)
        geom1.attrib['height'] = '20'
        geom1.attrib['as'] = 'geometry'
        cell1.append(geom1)
        root.append(cell1)

# Returns the right and bottom geometry bounds of the given element.
def getGeomBounds(elem):
    if elem is None:
        return [0.0, 0.0]
    attr = elem.attrib
    x = y = w = h = 0.0
    if 'x' in attr:
        x = float(attr['x'])
    if 'y' in attr:
        y = float(attr['y'])
    if 'width' in attr:
        w = float(attr['width'])
    if 'height' in attr:
        h = float(attr['height'])
    return [x + w, y + h]

# Sets the container's width & height to contain the geometry of the given
# GunnsDraw mxCell, plus a 40 pt pad to the right and bottom.
# This assumes the container location is at the 0,0 drawing origin.
def updateContainerBounds(container, cell):
    geom_bounds = getGeomBounds(cell.find('mxGeometry'))
    rect_bounds = getGeomBounds(cell.find('mxGeometry/mxRectangle'))
    rights  = []
    bottoms = []
    rights.append(geom_bounds[0])
    rights.append(rect_bounds[0])
    bottoms.append(geom_bounds[1])
    bottoms.append(rect_bounds[1])
    points = cell.findall('mxGeometry/Array/mxPoint')
    for point in points:
        point_bounds = getGeomBounds(point)
        rights.append(point_bounds[0])
        bottoms.append(point_bounds[1])
    right_bound = max(rights) + 40.0
    bottom_bound = max(bottoms) + 40.0
    if right_bound > float(container.find('mxCell/mxGeometry').attrib['width']):
        container.find('mxCell/mxGeometry').attrib['width'] = str(right_bound)
    if bottom_bound > float(container.find('mxCell/mxGeometry').attrib['height']):
        container.find('mxCell/mxGeometry').attrib['height'] = str(bottom_bound)

# Rounds x, y, width & height attributes of all elements of the given path to
# the nearest 10 pt.
def snapElements(path):
    allElems = gd_rootroot.findall(path)
    for elem in allElems:
        attr = elem.attrib
        if 'x' in attr:
            # Skip snapping 'x' if it's -1 < x < 1.  This leaves the location of
            # the port # label on Port lines alone, at the cost of not snapping
            # stuff near the 0,0 drawing origin.
            if math.fabs(float(attr['x'])) >= 1.0:
                attr['x'] = str(int(round(float(attr['x']), -1)))
        if 'y' in attr:
            attr['y'] = str(int(round(float(attr['y']), -1)))
        if 'width' in attr:
            attr['width'] = str(int(round(float(attr['width']), -1)))
        if 'height' in attr:
            attr['height'] = str(int(round(float(attr['height']), -1)))

#####################
# BEGIN MAIN SCRIPT #
#####################
homepath = os.path.dirname(os.path.abspath(__file__))

# Command line options:
#   -l paths to custom shape libraries
#   -z overrides the zoom level, default is 85
#   -s disables snap to grid
cmd_parser = OptionParser()
cmd_parser.add_option("-l", action="store", help="use the provided custom shape library paths, delimited by colons, e.g. -l path1:path2", dest="custom_lib_paths", default="")
cmd_parser.add_option("-s", action="store_false", help="disables snap to grid", dest="snap", default=True)
cmd_parser.add_option("-z", action="store", help="zoom amount, default is 85", dest="zoom", default="85")

(options, args) = cmd_parser.parse_args()

# Use the supplied path/file name, else use a file browser to select the drawing.
if len(args) > 0:
    inputPathFile = args[0]
else:
    root = TK.Tk()
    root.withdraw()
    ftypes = [('Visio XML files', '*.vdx')]
    inputPathFile = TKFILE.askopenfilename(title = "Select a drawing file to process", filetypes = ftypes)
if not inputPathFile:
    sys.exit(console.abort('no drawing file selected.'))

outputPathFile = os.path.splitext(inputPathFile)[0] + '.xml'

# Process command line options
customlibs = []
if options.custom_lib_paths:
    customlibs = options.custom_lib_paths.split(':')
zoom = float(options.zoom)

print('')
print('Parsing XML from ' + inputPathFile + '.')

# Don't want to deal with the namespace in the .vdx file's <VisioDocument> element,
# so delete it before parsing XML.
with open(inputPathFile, 'r') as fin:
    fin_str = fin.read()

# Find the <VisioDocument ...> and delete the ...
fin_fixed = re.sub('<VisioDocument.+?>', '<VisioDocument>', fin_str)

# Now parse the XML
gs_root = ET.fromstring(fin_fixed)

#gs_DocumentProperties = None
#gs_Page              = None
#for child in gs_root:
#    if 'DocumentProperties' in child.tag:
#        gs_DocumentProperties = child
#    elif 'Pages' in child.tag:
#        gs_Page = child.find('Page')

gs_DocumentProperties = gs_root.find('DocumentProperties')
gs_Page               = gs_root.find('Pages/Page')
gs_PageHeight         = float(gs_Page.find('PageSheet/PageProps/PageHeight').text)
gs_shapes             = gs_Page.findall('Shapes/Shape')
gs_layers             = gs_Page.findall('PageSheet/Layer')
gs_connects           = gs_Page.findall('Connects/Connect')
gs_masters            = gs_root.findall('Masters/Master')

# Get the Fluid or Basic network type from <DocumentProperties><AlternateNames>
gs_fluid = ('Fluid' in gs_DocumentProperties.find('AlternateNames').text)

# Build a list of layers info.
layers = {}
for layer in gs_layers:
    layer_id   = int(layer.attrib['IX'])
    layer_name = layer.find('Name').text
    layers[layer_name] = layer_id

#print(layers)

netConfig = []
netNodes = []
netLinks = []
netConnectors = []
netGrounds = []
netRefNodes = []
netFluidConfigs = []
netFluidStates = []
netSpotters = []
netBatteryTables = []
netSocketLists = []
netJumpers = []
netReactions = []
netCompounds = []
portIdMap = {}
textBoxes = []
lines = []

# Find GUNNS objects and store their info
# X,Y coords are from the bottom left corner
for shape in gs_shapes:
    layerMember = shape.find('LayerMem/LayerMember')
    if layerMember is not None:
        # Some GunnShow objects belong to more than one layer.
        # We'll handle these special cases individually:
        # ConverterElect and GunnsSolarArrayRegulator links belong
        # to 23 (Converter) and 2 (Link), so we just use 2 (Link).
        if len(layerMember.text.split(';')) > 1:
            layer_id = 2
        else:
            layer_id = int(layerMember.text)

        # Net Config
        if layers['Title'] == layer_id:
            name   = ''
            config = []
            props  = shape.findall('Prop')
            for prop in props:
                if 'InstanceName' == prop.attrib['NameU']:
                    name = prop.find('Value').text
                elif prop.attrib['NameU'].startswith('CD_'):
                    config.append(prop.find('Value').text)
            netConfig.append((name, config))

        # Net Nodes
        elif layers['Node'] == layer_id:
            gs_id = shape.attrib['ID']
            x = float(shape.find('XForm/PinX').text)
            y = float(shape.find('XForm/PinY').text)
            w = float(shape.find('XForm/Width').text)
            h = float(shape.find('XForm/Height').text)
            num    = ''
            config = []
            props  = shape.findall('Prop')
            for prop in props:
                if 'InstanceName' == prop.attrib['NameU'] and num == '':
                    num = prop.find('Value').text
                if 'Init' in prop.attrib['NameU'] and len(config) == 0:
                    config.append(prop.find('Value').text)
            # For basic nodes, this shape won't have a Prop element for the
            # initial potential unless it was changed from the default value in
            # GunnShow.  So we have to get the value from the shape Master.
            if len(config) == 0:
                # Get config[0] as initial potential from the Master shape.
                master_id = shape.attrib['Master']
                for master in gs_masters:
                    if master_id == master.attrib['ID']:
                        allProps = master.find('Shapes/Shape').findall('Prop')
                        config.append(allProps[-1].find('Value').text)
                        break
            isFrame = False
            if 'NameU' in shape.attrib and 'Segment' in shape.attrib['NameU']:
                isFrame = True
            netNodes.append((num, x, y, w, h, config, isFrame, gs_id))

        # Links
        elif layers['Link'] == layer_id or ('Thermal Source' in layers and layers['Thermal Source'] == layer_id) or ('Jumper' in layers and layers['Jumper'] == layer_id) or ('Reactor' in layers and layers['Reactor'] == layer_id):
            gs_id = shape.attrib['ID']
            x = float(shape.find('XForm/PinX').text)
            y = float(shape.find('XForm/PinY').text)
            w = float(shape.find('XForm/Width').text)
            h = float(shape.find('XForm/Height').text)
            a = math.degrees(-float(shape.find('XForm/Angle').text))
            flipX = bool(int(shape.find('XForm/FlipX').text))
            flipY = bool(int(shape.find('XForm/FlipY').text))
            name        = ''
            subtype     = ''
            plug0Type   = ''
            plug1Type   = ''
            config      = []
            props       = shape.findall('Prop')
            propsNameU  = []
            masterProps = None
            if 'Master' in shape.attrib:
                master_id   = shape.attrib['Master']
                for master in gs_masters:
                    if master_id == master.attrib['ID']:
                        masterProps = master.find('Shapes/Shape').findall('Prop')
                        break
            for prop in props:
                # Ignore duplicated properties with names that we've already processed
                if prop.attrib['NameU'] not in propsNameU:
                    propsNameU.append(prop.attrib['NameU'])
                    # Some props won't have a Value element, in which case we must get it from the visio document's master shape.
                    value = prop.find('Value')
                    if value is None:
                        if masterProps is not None:
                            for masterProp in masterProps:
                                 if masterProp.attrib['NameU'] == prop.attrib['NameU'] and masterProp.attrib['ID'] == prop.attrib['ID']:
                                     value = masterProp.find('Value')
                                     break
                        else:
                            sys.exit(console.abort('link ID ' + str(gs_id) + ' is missing a property value and there is no shape master.'))
                    if 'InstanceName' == prop.attrib['NameU']:
                        name = value.text
                    elif 'ModelPath' == prop.attrib['NameU']:
                        # GunnShow model paths include the filetype, i.e. '.hh' which we must strip off.
                        subtype = os.path.splitext(value.text)[0]
                    elif 'Plug0Type' == prop.attrib['NameU']:
                        plug0type = value.text
                    elif 'Plug1Type' == prop.attrib['NameU']:
                        plug1type = value.text
                    elif prop.attrib['NameU'].startswith('CD_'):
                        config.append(value.text)
                    elif prop.attrib['NameU'].startswith('ID_'):
                        config.append(value.text)
            netLinks.append((name, subtype, x, y, w, h, a, flipX, flipY, config, gs_id))
            # Special case for jumper as they're on their own layer, and we need to save their 'plug type' values for later.
            if 'Jumper' in layers and layers['Jumper'] == layer_id:
                netJumpers.append([gs_id, plug0type, plug1type])

        # Connectors
        elif layers['Connector'] == layer_id:
            netConnectors.append(shape)

        # Ground Nodes
        # Place them all on the drawing initially, then remove the ones that are left with no
        # connections because of optional ports.
        elif layers['Boundary'] == layer_id:
            gs_id = shape.attrib['ID']
            x = float(shape.find('XForm/PinX').text)
            y = float(shape.find('XForm/PinY').text)
            a = math.degrees(-float(shape.find('XForm/Angle').text))
            flipX = bool(int(shape.find('XForm/FlipX').text))
            flipY = bool(int(shape.find('XForm/FlipY').text))
            netGrounds.append((x, y, a, flipX, flipY, gs_id))

        # Fluid Configs
        elif 'PolyFluid' in layers and layers['PolyFluid'] == layer_id:
            x = zoom * float(shape.find('XForm/PinX').text) - 80.0
            y = zoom * (gs_PageHeight - float(shape.find('XForm/PinY').text)) - 10.0
            name         = ''
            internal     = 'False'
            constituents = []
            props        = shape.findall('Prop')
            for prop in props:
                if 'InstanceName' == prop.attrib['NameU'] and prop.find('Value') is not None:
                    name = prop.find('Value').text
                elif 'IsInternalFluid' == prop.attrib['NameU']:
                    internal = prop.find('Value').text
                elif prop.find('Label') is not None and 'Fluid Constituent' in prop.find('Label').text:
                    constituents.append(prop.find('Value').text)
            netFluidConfigs.append((name, x, y, constituents, internal))

        # Fluid States
        elif 'FluidState' in layers and layers['FluidState'] == layer_id:
            x = zoom * float(shape.find('XForm/PinX').text) - 80.0
            y = zoom * (gs_PageHeight - float(shape.find('XForm/PinY').text)) - 10.0
            name         = ''
            temperature  = ''
            pressure     = ''
            mass         = ''
            tc           = '0' # since some old fluid state boxes in GS don't have this field
            props        = shape.findall('Prop')
            mixture = []
            for prop in props:
                if 'InstanceName' == prop.attrib['NameU'] and prop.find('Value') is not None:
                    name = prop.find('Value').text
                if 'InitialStateInitTemperature' == prop.attrib['NameU'] and prop.find('Value') is not None:
                    temperature = prop.find('Value').text
                if 'InitialStateInitPressure' == prop.attrib['NameU'] and prop.find('Value') is not None:
                    pressure = prop.find('Value').text
                if 'InitialStateInitMass' == prop.attrib['NameU'] and prop.find('Value') is not None:
                    mass = prop.find('Value').text
                if 'InitialStateInitTraceCompoundsState' == prop.attrib['NameU'] and prop.find('Value') is not None:
                    tc = prop.find('Value').text
                if 'ConstituentData' in prop.attrib['NameU'] and prop.find('Value') is not None:
                    mixture.append([prop.attrib['NameU'][len('ConstituentData'):], prop.find('Value').text])
            states = [['Pressure', pressure], ['Temperature', temperature], ['Mass', mass], ['TC State', tc]]
            for mix in mixture:
                states.append(mix)
            netFluidStates.append((name, x, y, states))

        # Port Maps
        elif 'Port Map' in layers and layers['Port Map'] == layer_id:
            gs_id = shape.attrib['ID']
            portNum = shape.find('Prop/Value').text
            connector_ids = []
            connected_shape_ids = []
            # Find both connects on this port map.
            for gs_connect in gs_connects:
                if gs_connect.attrib['FromSheet'] == gs_id:
                    connector_ids.append(gs_connect.attrib['ToSheet'])
                elif gs_connect.attrib['ToSheet'] == gs_id:
                    connector_ids.append(gs_connect.attrib['FromSheet'])
            if 2 != len(connector_ids):
                sys.exit(console.abort('Port Map ID ' + str(gs_id) + ' does not have 2 connects.'))
            # connector_ids are the id's of the connector shapes (netConnectors) that connect to
            # this port map.  Find the other connects for those connector shapes, not on this
            # port map.  For those, the other connected shape is the node or link id we want.
            for connector_id in connector_ids:
                for gs_connect in gs_connects:
                    if gs_connect.attrib['FromSheet'] != gs_id and gs_connect.attrib['ToSheet'] != gs_id:
                        if gs_connect.attrib['FromSheet'] == connector_id:
                            connected_shape_ids.append(gs_connect.attrib['ToSheet'])
                        elif gs_connect.attrib['ToSheet'] == connector_id:
                            connected_shape_ids.append(gs_connect.attrib['FromSheet'])
            portIdMap[gs_id] = [portNum, connected_shape_ids[0], connected_shape_ids[1]]

        # Spotters
        elif 'Spotter' in layers and layers['Spotter'] == layer_id:
            x = float(shape.find('XForm/PinX').text)
            y = float(shape.find('XForm/PinY').text)
            name       = ''
            subtype    = ''
            configData = []
            inputData  = []
            args       = []
            props      = shape.findall('Prop')
            for prop in props:
                if 'InstanceName' == prop.attrib['NameU']:
                    name = prop.find('Value').text
                elif 'ModelPath' == prop.attrib['NameU']:
                    # GunnShow model paths include the filetype, i.e. '.hh' which we must strip off.
                    subtype = os.path.splitext(prop.find('Value').text)[0]
                elif prop.attrib['NameU'].startswith('CD_'):
                    configData.append([prop.attrib['NameU'], prop.find('Value').text])
                elif prop.attrib['NameU'].startswith('ID_'):
                    inputData.append([prop.attrib['NameU'], prop.find('Value').text])
                elif prop.attrib['NameU'].startswith('RefObject_'):
                    args.append(prop.find('Value').text)
            netSpotters.append((name, subtype, x, y, configData, inputData, args))

        # Battery Tables
        elif 'Battery Table' in layers and layers['Battery Table'] == layer_id:
            x = zoom * float(shape.find('XForm/PinX').text) - 80.0
            y = zoom * (gs_PageHeight - float(shape.find('XForm/PinY').text)) - 10.0
            name   = ''
            socstr = ''
            vocstr = ''
            size   = ''
            data = []
            data.append(['SOC', 'VOC'])
            props  = shape.findall('Prop')
            for prop in props:
                if 'InstanceName' == prop.attrib['NameU']:
                    name = prop.find('Value').text
                elif 'CD_Soc' == prop.attrib['NameU']:
                    socstr = prop.find('Value').text
                elif 'CD_Voc' == prop.attrib['NameU']:
                    vocstr = prop.find('Value').text
                elif 'CD_ArraySize' == prop.attrib['NameU']:
                    size = prop.find('Value').text
            soc = socstr.lstrip('{').rstrip('}').split(',')
            voc = vocstr.lstrip('{').rstrip('}').split(',')
            for i in range(0, int(size)):
                data.append([soc[i], voc[i]])
            netBatteryTables.append([name, x, y, data])

        # Reference Nodes
        elif 'ReferenceNode' in layers and layers['ReferenceNode'] == layer_id:
            gs_id = shape.attrib['ID']
            x = float(shape.find('XForm/PinX').text)
            y = float(shape.find('XForm/PinY').text)
            num = ''
            props  = shape.findall('Prop')
            for prop in props:
                if 'InstanceName' == prop.attrib['NameU']:
                    num = prop.find('Value').text
            netRefNodes.append((num, x, y, gs_id))

        # Socket Lists
        elif 'Socket List' in layers and layers['Socket List'] == layer_id:
            x = zoom * float(shape.find('XForm/PinX').text) - 80.0
            y = zoom * (gs_PageHeight - float(shape.find('XForm/PinY').text)) - 10.0
            name    = ''
            sockets = []
            props   = shape.findall('Prop')
            for prop in props:
                if 'InstanceName' == prop.attrib['NameU']:
                    name = prop.find('Value').text
                elif prop.attrib['NameU'].startswith('Socket'):
                    sockets.append(prop.find('Value').text)
            netSocketLists.append((name, x, y, sockets))

        # Reactions
        elif 'Reactions' in layers and layers['Reactions'] == layer_id:
            x = zoom * float(shape.find('XForm/PinX').text) - 80.0
            y = zoom * (gs_PageHeight - float(shape.find('XForm/PinY').text)) - 10.0
            name      = ''
            reactions = []
            props     = shape.findall('Prop')
            for prop in props:
                if 'InstanceName' == prop.attrib['NameU']:
                    name = prop.find('Value').text
                elif prop.attrib['NameU'].startswith('Reaction_'):
                    reactions.append(prop.find('Value').text)
            netReactions.append((name, x, y, reactions))

        # Compounds
        elif 'Compounds' in layers and layers['Compounds'] == layer_id:
            x = zoom * float(shape.find('XForm/PinX').text) - 80.0
            y = zoom * (gs_PageHeight - float(shape.find('XForm/PinY').text)) - 10.0
            name      = ''
            compounds = []
            compounds.append(['Compounds', 'Mass'])
            props     = shape.findall('Prop')
            for prop in props:
                if 'InstanceName' == prop.attrib['NameU']:
                    name = prop.find('Value').text
                elif prop.attrib['NameU'].startswith('Compound_'):
                    compounds.append([prop.attrib['NameU'][len('Compound_'):], prop.find('Value').text])
            netCompounds.append((name, x, y, compounds))

        # For unsupported GunnShow layers, give a warning that we didn't migrate.
        elif 'IPS' in layers and layers['IPS'] == layer_id:
            print('    ' + console.warn('IPS links are not supported, please migrate them manually.'))
        elif 'PowerBus' in layers and layers['PowerBus'] == layer_id:
            print('    ' + console.warn('Power Bus links are not supported, please migrate them manually.'))
        elif 'Load Switch Card' in layers and layers['Load Switch Card'] == layer_id:
            print('    ' + console.warn('Switch Card links are not supported, please migrate them manually.'))

        # TODO all the other objects we care about...
        # TC Config
        # TC State
        # Sensors
        # Switch Cards

    else: # not on a GunnShow layer
        Text      = shape.find('Text')
        Line      = shape.find('Line')
        XForm     = shape.find('XForm')
        XForm1D   = shape.find('XForm1D')

        # Lines
        if Line is not None and XForm1D is not None:
            beginX = float(XForm1D.find('BeginX').text)
            beginY = float(XForm1D.find('BeginY').text)
            endX = float(XForm1D.find('EndX').text)
            endY = float(XForm1D.find('EndY').text)
            lines.append([beginX, beginY, endX, endY])
            pass

        # Text boxes
        elif XForm is not None:
            text = ''
            x = float(XForm.find('PinX').text)
            y = float(XForm.find('PinY').text)
            w = float(XForm.find('Width').text)
            h = float(XForm.find('Height').text)
            valignDict = {'0':'top', '1':'middle', '2':'bottom'}
            # draw.io has no 'justified' horizontal alignment, so we just use 'left' instead.
            halignDict = {'0':'left', '1':'center', '2':'right', '3':'left'}
            valign = valignDict['1']
            halign = halignDict['1']
            if Text is not None:
                TextElems = Text.findall('*')
                if len(TextElems) > 0:
                    text = Text.findall('*')[-1].tail
                else:
                    text = Text.tail
                VerticalAlign = shape.find('TextBlock/VerticalAlign')
                if VerticalAlign is not None:
                    valign = valignDict[VerticalAlign.text]
                HorzAlign = shape.find('Para/HorzAlign')
                if HorzAlign is not None:
                    halign = halignDict[HorzAlign.text]
            if text is not None:
                textBoxes.append([text, x, y, w, h, halign, valign])

# For debugging:
#print(netConfig)
#print(netNodes)
#print(netLinks)
#print(netSpotters)
#print(netBatteryTables)
#print(netRefNodes)
#print(netSocketLists)
#print(netJumpers)
#print(netReactions)
#print(netCompounds)
#print(portIdMap)

########################
# Build GunnsDraw File #
########################
# Initialize the XML tree
gd_tree = ET.ElementTree()
gd_tree._setroot(ET.fromstring('<mxGraphModel arrows="0" connect="1" dx="0" dy="0" fold="1" grid="1" gridSize="10" guides="1" math="0" page="1" pageHeight="1100" pageScale="1" pageWidth="850" shadow="0" tooltips="1"><root><mxCell id="0" /></root></mxGraphModel>'))
gd_root = gd_tree.getroot()
gd_rootroot = gd_root.find('root')

# Add the Notes layer.
notesLayerId = id(idn)
notesLayerCell = ET.Element('mxCell')
notesLayerCell.attrib['parent'] = '0'
notesLayerCell.attrib['value'] = 'Notes'
notesLayerCell.attrib['id'] = notesLayerId
gd_rootroot.append(notesLayerCell)

# Add the Network layer after/in front of the Notes layer.  This puts text boxes in the Notes layer
# behind objects in the network, so they don't block access to the network objects.
netLayerCell = ET.Element('mxCell')
netLayerCell.attrib['parent'] = '0'
netLayerCell.attrib['value'] = 'Network'
netLayerCell.attrib['id'] = '1'
gd_rootroot.append(netLayerCell)

# Load & decompress all shape masters from the shape libs, just like netexport.
for shapeLib in shapeLibs.shapeLibs:
    shapeLibs.loadShapeLibs(homepath + '/' + shapeLib, False)
for customLib in customlibs:
    shapeLibs.loadShapeLibs(customLib, False)
allShapeMasters = shapeLibs.shapeTree.findall('./object')

# Add the network object to the tree with a default size.  Size will be updated to contain objects as they are added.
netId = id(idn)
networkShape = copy.deepcopy(shapeLibs.getNetworkShapeMaster(allShapeMasters))
networkShape.attrib['label'] = netConfig[0][0]
networkShape.attrib['id']    = netId
copyConfigInputData(netConfig[0][1], networkShape)
mxcell_attr = networkShape.find('mxCell').attrib
del mxcell_attr['collapsed']
mxcellgeom_attr = networkShape.find('mxCell/mxGeometry').attrib
mxcellgeom_attr['x']      = '0'
mxcellgeom_attr['y']      = '0'
mxcellgeom_attr['width']  = '200'
mxcellgeom_attr['height'] = '200'
mxcellgeomrect_attr = networkShape.find('mxCell/mxGeometry/mxRectangle').attrib
mxcellgeomrect_attr['x']      = '0'
mxcellgeomrect_attr['y']      = '0'
mxcellgeomrect_attr['width']  = '200'
mxcellgeomrect_attr['height'] = '20'
gd_rootroot.append(networkShape)

# Add the network nodes to the tree
if gs_fluid:
    nodeSubtype = 'Fluid'
else:
    nodeSubtype = 'Basic'
    
# These dictionaries map the GunnShow Shape ID with our GunnsDraw object XML elements
nodeIdMap = {}
linkIdMap = {}
groundIdMap = {}
refNodeIdMap = {}

# Add nodes to the tree
for node in netNodes:
    netNodeShape = copy.deepcopy(shapeLibs.getNetNodeShapeMaster(allShapeMasters, nodeSubtype, node[6]))
    netNodeShape.attrib['label']  = str(node[0])
    gd_id = id(idn)
    nodeIdMap[node[7]]        = netNodeShape
    netNodeShape.attrib['id'] = gd_id
    copyConfigInputData(node[5], netNodeShape)
    mxcell_attr = netNodeShape.find('mxCell').attrib
    mxcell_attr['parent'] = netId
    mxcellgeom_attr = netNodeShape.find('./mxCell/mxGeometry').attrib
    gd_x = zoom * node[1]
    gd_y = zoom * (gs_PageHeight - node[2])
    if node[6]: # frame
        mxcellgeom_attr['width']  = str(node[3] * zoom)
        mxcellgeom_attr['height'] = str(node[4] * zoom)
        mxcellgeom_attr['x']      = str(gd_x - node[3] * zoom / 2.0)
        mxcellgeom_attr['y']      = str(gd_y - node[4] * zoom / 2.0)
    else:
        mxcellgeom_attr['x'] = str(gd_x - 10.0)
        mxcellgeom_attr['y'] = str(gd_y - 10.0)
    gd_rootroot.append(netNodeShape)
    updateContainerBounds(networkShape, netNodeShape.find('mxCell'))

# Add grounds to the tree
for ground in netGrounds:
    groundShape = copy.deepcopy(shapeLibs.getGroundShapeMaster(allShapeMasters))
    gd_id = id(idn)
    groundIdMap[ground[5]]   = groundShape
    groundShape.attrib['id'] = gd_id
    mxcell_attr = groundShape.find('mxCell').attrib
    mxcell_attr['parent'] = netId
    mxcellgeom_attr = groundShape.find('./mxCell/mxGeometry').attrib
    # location
    gd_x = zoom * ground[0]
    gd_y = zoom * (gs_PageHeight - ground[1])
    mxcellgeom_attr['x'] = str(gd_x - 0.5 * float(mxcellgeom_attr['width']))
    mxcellgeom_attr['y'] = str(gd_y - 0.5 * float(mxcellgeom_attr['height']))
    # rotation
    rotation = float(getStyle(groundShape, 'rotation', '0.0')) + ground[2]
    mxcell_attr['style'] = setStyle(groundShape, 'rotation', str(rotation))
    gd_rootroot.append(groundShape)
    updateContainerBounds(networkShape, groundShape.find('mxCell'))

# Add reference nodes to the tree
for node in netRefNodes:
    refNodeShape = copy.deepcopy(shapeLibs.getShapeMaster(allShapeMasters, 'Node', 'Reference'))
    refNodeShape.attrib['label'] = str(node[0])
    gd_id = id(idn)
    refNodeIdMap[node[3]] = refNodeShape
    refNodeShape.attrib['id'] = gd_id
    mxcell_attr = refNodeShape.find('mxCell').attrib
    mxcell_attr['parent'] = netId
    mxcellgeom_attr = refNodeShape.find('./mxCell/mxGeometry').attrib
    gd_x = zoom * node[1]
    gd_y = zoom * (gs_PageHeight - node[2])
    mxcellgeom_attr['x'] = str(gd_x - 10.0)
    mxcellgeom_attr['y'] = str(gd_y - 10.0)
    gd_rootroot.append(refNodeShape)
    updateContainerBounds(networkShape, refNodeShape.find('mxCell'))

# Load the geometry biases and connection point coordinates:
# Python 2.7 vs. 3 by feature detection.  TODO Maybe avoid this by defining the version string in a module instead...
try:
    execfile(homepath + '/modules/migrate_link_map.py')
except NameError:
    exec(compile(open(homepath + '/modules/migrate_link_map.py', "rb").read(), homepath + '/modules/migrate_link_map.py', 'exec'))

# Add links to the tree
for link in netLinks:
    linkShape = copy.deepcopy(shapeLibs.getLinkSubtypeShapeMaster(allShapeMasters, link[1]))
    if None == linkShape:
        sys.exit(console.abort('couldn\'t find shape master for link ' + link[0] + '.'))
    try:
        migrate_map = migrate_link_map[link[1]]
    except:
        migrate_map = None
        print('   ' + console.warn('link: ' + link[0] + ' has no migration map.'))
    linkShape.attrib['label']  = str(link[0])
    gd_id = id(idn)
    linkIdMap[link[10]]    = linkShape
    linkShape.attrib['id'] = gd_id
    copyConfigInputData(link[9], linkShape)
    mxcell_attr = linkShape.find('mxCell').attrib
    mxcell_attr['parent'] = netId
    mxcellgeom_attr = linkShape.find('./mxCell/mxGeometry').attrib
    # scaling
    if migrate_map:
        mxcellgeom_attr['width']  = str(migrate_map['scaleX'] * float(mxcellgeom_attr['width']))
        mxcellgeom_attr['height'] = str(migrate_map['scaleY'] * float(mxcellgeom_attr['height']))
    # flipping & rotation
    rotation  = link[6]
    flipX     = link[7]
    flipY     = link[8]
    direction = getStyle(linkShape, 'direction', 'east')
    if migrate_map:
        direction = migrate_map['direction']
        if not migrate_map['rotation']:
            rotation = 0.0
        if 'ignore' == migrate_map['flipX']:
            flipX = False
        elif 'flip' == migrate_map['flipX']:
            flipX = not flipX
        if 'ignore' == migrate_map['flipY']:
            flipY = False
        elif 'flip' == migrate_map['flipY']:
            flipY = not flipX
    mxcell_attr['style'] = setStyle(linkShape, 'direction', direction)
    mxcell_attr['style'] = setStyle(linkShape, 'rotation', str(rotation))
    mxcell_attr['style'] = setStyle(linkShape, 'flipH', str(int(flipX)))
    mxcell_attr['style'] = setStyle(linkShape, 'flipV', str(int(flipY)))
    # location
    gd_x = -0.5 * float(mxcellgeom_attr['width']) + link[2] * zoom 
    gd_y = -0.5 * float(mxcellgeom_attr['height']) + (gs_PageHeight - link[3]) * zoom
    if migrate_map:
        # Rotate the bias by the total rotation of the link.
        angle = math.radians(getTotalRotation(linkShape))
        gd_x += (migrate_map['biasX'] * math.cos(angle) - migrate_map['biasY'] * math.sin(angle))
        gd_y += (migrate_map['biasX'] * math.sin(angle) + migrate_map['biasY'] * math.cos(angle))
    mxcellgeom_attr['x'] = str(gd_x)
    mxcellgeom_attr['y'] = str(gd_y)

    # Special case for jumper links: change plug and connection values to the GunnsDraw way:
    #   c02.plug0 and co3.plug1: change to the netJumper's plug type 0 & 1
    #   connection0 and connection1: add the 'NetworkName_' in front
    for jumper in netJumpers:
        if link[10] == jumper[0]:
            if jumper[1] != '0':
                linkShape.attrib['c02.plug0'] = jumper[1]
                linkShape.attrib['i04.connection0'] = netConfig[0][0] + '_' + linkShape.attrib['i04.connection0']
            if jumper[2] != '0':
                linkShape.attrib['c03.plug1'] = jumper[2]
                linkShape.attrib['i05.connection1'] = netConfig[0][0] + '_' + linkShape.attrib['i05.connection1']

    gd_rootroot.append(linkShape)
    updateContainerBounds(networkShape, linkShape.find('mxCell'))

# Add Ports to the tree
completedPortMaps = []
for shape in netConnectors:
    shape_id = shape.attrib['ID']
    # These appear to be absolute coordinates in Visio:
    gs_beginX = float(shape.find('XForm1D/BeginX').text)
    gs_beginY = float(shape.find('XForm1D/BeginY').text)
    gs_endX   = float(shape.find('XForm1D/EndX').text)
    gs_endY   = float(shape.find('XForm1D/EndY').text)
    if gs_beginX == gs_endX and gs_beginY == gs_endY:
        # Ignore a floating connector that has the same begin & end point, continue with next connector.
        print('    ' + console.warn('ignored connector ID ' + str(shape_id) + ', it has same begin & end points in GunnShow drawing near (' + str(int(gs_endX/0.03937)) + ', ' + str(int(gs_endY/0.03937)) + ').'))
        continue
         
    gd_beginXY = [zoom * gs_beginX, zoom * (gs_PageHeight - gs_beginY)]
    gd_endXY   = [zoom * gs_endX,   zoom * (gs_PageHeight - gs_endY)]
    # loop over the gs_connects, find them what have 'FromSheet' matching this shape's gs_id
    #   These are the connections for this connector.
    #   Their 'ToSheet' is the shape id of the connected object (node, link, etc.)
    # Make Source be the node's ID, Target be the link's ID
    node = link = portNum = ground = portMap = refNode = None
    nodeBegins = False
    for gs_connect in gs_connects:
        if shape_id == gs_connect.attrib['FromSheet']:
            other_id = gs_connect.attrib['ToSheet']
            try:
                node = nodeIdMap[other_id]
            except:
                try:
                    link   = linkIdMap[other_id]
                    portNum = int(gs_connect.attrib['ToPart']) - 100
                    nodeBegins = ('End' in gs_connect.attrib['FromCell'])
                except:
                    try:
                        ground = groundIdMap[other_id]
                    except:
                        try:
                            refNode = refNodeIdMap[other_id]
                        except:
                            try:
                                portMap = portIdMap[other_id]
                                for i in range(1,3):
                                    # TODO refactor this, it's repeat of above:
                                    try:
                                        node = nodeIdMap[portMap[i]]
                                    except:
                                        try:
                                            link   = linkIdMap[portMap[i]]
                                            portNum = int(portMap[0])
                                            nodeBegins = ('End' in gs_connect.attrib['FromCell'])
                                        except:
                                            try:
                                                ground = groundIdMap[portMap[i]]
                                            except:
                                                try:
                                                    refNode = refNodeIdMap[portMap[i]]
                                                except:
                                                    pass
                            except:
                                pass

    if nodeBegins:
        node_end = 'source'
        link_end = 'target'
        gd_node_connectionX = gd_beginXY[0]
        gd_node_connectionY = gd_beginXY[1]
    else:
        node_end = 'target'
        link_end = 'source'
        gd_node_connectionX = gd_endXY[0]
        gd_node_connectionY = gd_endXY[1]

    portShape = copy.deepcopy(shapeLibs.getPortShapeMaster(allShapeMasters, '0'))
    portShape.attrib['label']  = str(portNum)
    gd_id = id(idn)
    portShape.attrib['id'] = gd_id
    mxcell_attr = portShape.find('mxCell').attrib
    mxcell_attr['parent'] = netId
    if link is not None:
        mxcell_attr[link_end] = link.attrib['id']
    entryX = entryY = exitX = exitY = None

    # Node connection point
    # - node (target) connection locations are style: entryX, entryY
    nodeX = nodeY = 0.0  # connection location in drawing
    if node is not None:
        mxcell_attr[node_end] = node.attrib['id']
        if 'shape=mxgraph.basic.rounded_frame' in node.find('mxCell').attrib['style']:
            # Compute locations of port connections to Frame nodes.
            nodeX = gd_node_connectionX
            nodeY = gd_node_connectionY
            frameX = float(node.find('mxCell/mxGeometry').attrib['x'])
            frameY = float(node.find('mxCell/mxGeometry').attrib['y'])
            frameW = float(node.find('mxCell/mxGeometry').attrib['width'])
            frameH = float(node.find('mxCell/mxGeometry').attrib['height'])
            # Frame nodes are rotated 90 clockwise (direction=south) so we have to flip X/Y axis.
            snapX = round(frameH, -1) / 10.0
            snapY = round(frameW, -1) / 10.0
            if nodeBegins:
                exitY  = 1.0 - (nodeX - frameX) / frameW
                exitX  =       (nodeY - frameY) / frameH
                if options.snap:
                    exitX = round(exitX * snapX) / snapX
                    exitY = round(exitY * snapY) / snapY
            else:
                entryY = 1.0 - (nodeX - frameX) / frameW
                entryX =       (nodeY - frameY) / frameH
                if options.snap:
                    entryX = round(entryX * snapX) / snapX
                    entryY = round(entryY * snapY) / snapY
        else:
            # Center of the normal node in the drawing.
            nodeX = float(node.find('mxCell/mxGeometry').attrib['x']) + 10.0
            nodeY = float(node.find('mxCell/mxGeometry').attrib['y']) + 10.0

    elif refNode is not None:
        mxcell_attr[node_end] = refNode.attrib['id']
        # Center of the reference node in the drawing.
        nodeX = float(refNode.find('mxCell/mxGeometry').attrib['x']) + 10.0
        nodeY = float(refNode.find('mxCell/mxGeometry').attrib['y']) + 10.0

    elif ground is not None:
        mxcell_attr[node_end] = ground.attrib['id']
        if nodeBegins:
            exitX = 0.5
            exitY = 0.0
        else:
            entryX = 0.5
            entryY = 0.0
        # Center of the ground node in the drawing.
        nodeX = float(ground.find('mxCell/mxGeometry').attrib['x']) + 10.0
        nodeY = float(ground.find('mxCell/mxGeometry').attrib['y']) + 10.0

    # Connection point geometry
    # <mxGeometry ...>
    #   <Array as="points">
    #     <mxPoint x="#" y="#"/>
    #     <mxPoint x="#" y="#"/>
    #   </Array>
    # Visio routing points are in the Shape's <Geom> element.
    # there is a starting <MoveTo> sub, followed by <LineTo> subs
    # The LineTo's give X/Y of vertices.
    # The MoveTo is the bias in the LineTo coordinates, so we subtract the MoveTo values
    # from the LineTo values.
    # The mxPoint are in absolute page coords so we have to adjust the LineTo's
    # connection point relative coords by the connection point's location in the page.
    if link is not None:
        allLineTo = shape.findall('Geom/LineTo')
        if len(allLineTo) > 0:
            biasVertX = 0.0
            biasVertY = 0.0
            moveToX = shape.find('Geom/MoveTo/X')
            moveToY = shape.find('Geom/MoveTo/Y')
            if moveToX is not None:
                biasVertX = float(moveToX.text)
            if moveToY is not None:
                biasVertY = float(moveToY.text)
            array = ET.Element('Array')
            array.attrib['as'] = 'points'
            for i in range(0, len(allLineTo)):
                gs_x = allLineTo[i].find('./X')
                gs_y = allLineTo[i].find('./Y')
                if gs_x is not None and gs_y is not None:
                    x = zoom * (                gs_beginX + float(gs_x.text) - biasVertX)
                    y = zoom * (gs_PageHeight - gs_beginY - float(gs_y.text) + biasVertY)
                    mxpoint = ET.Element('mxPoint')
                    mxpoint.attrib['x'] = str(x)
                    mxpoint.attrib['y'] = str(y)
                    array.append(mxpoint)
            portShape.find('mxCell/mxGeometry').append(array)

    # Link connection point
    #
    # Find the coordinates of the closest connector line vertex to the center of the link, or
    # if there are no vertices, then the coordinates of the node attach point.
    # Start with the node center or node frame attach point, then consider the line vertices.
    closeVertX = nodeX
    closeVertY = nodeY
    migrate_map = None
    if link is not None:
        linkCenter = getShapeCenter(link)
        minRange   = computeRange(linkCenter, [closeVertX, closeVertY])
        for arrayPoint in portShape.findall('mxCell/mxGeometry/Array/mxPoint'):
            this_coords = [float(arrayPoint.attrib['x']), float(arrayPoint.attrib['y'])]
            this_range  = computeRange(linkCenter, this_coords)
            if this_range < minRange:
                minRange = this_range
                closeVertX = this_coords[0]
                closeVertY = this_coords[1]
        try:
            migrate_map = migrate_link_map[link.find('gunns').attrib['subtype']]
        except:
            pass
    else:
        sys.exit(console.abort('connection line ID ' + shape_id + ' between coordinates (' + str(int(gs_endX/0.03937)) + ', ' + str(int(gs_endY/0.03937)) + ') and (' + str(int(gs_beginX/0.03937)) + ', ' + str(int(gs_beginY/0.03937)) + ') has no link connected.'))

    if migrate_map is not None:
        # Make a list of link connection point coordinates that this port # has affinity for.
        # If there are no affinity connection points, use them all.
        connection_points = []
        for connect in migrate_map['connects']:
            if portNum in connect[2]:
                connection_points.append(connect)
        if 0 == len(connection_points):
            connection_points = migrate_map['connects']
        # For each of these candidate connection points, find its absolute coordinates in the
        # drawing, accounting for link rotation and direction.
        candidates = []
        for connection_point in connection_points:
            candidates.append(getShapeRelAbsCoords(link, connection_point[0], connection_point[1]))
        # Find the candiate connection point that is closest to the close line vertex.
        #   The endX/Y coordinates are that point's link relative values from the migration map.
        minRange = 1e10
        for i in range(0, len(candidates)):
            candidate_range = computeRange(candidates[i], [closeVertX, closeVertY])
            if candidate_range < minRange:
                minRange = candidate_range
                # Correct the shape relative connection points for flips.
                if nodeBegins:
                    entryX = math.fabs(connection_points[i][0] - float('1' == getStyle(link, 'flipH', '0')))
                    entryY = math.fabs(connection_points[i][1] - float('1' == getStyle(link, 'flipV', '0')))
                else:
                    exitX  = math.fabs(connection_points[i][0] - float('1' == getStyle(link, 'flipH', '0')))
                    exitY  = math.fabs(connection_points[i][1] - float('1' == getStyle(link, 'flipV', '0')))

    mxcellgeom_attr = portShape.find('./mxCell/mxGeometry').attrib
    # This pins the port label number close to the link.
    if nodeBegins:
        mxcellgeom_attr['x'] = '0.8'
    else:
        mxcellgeom_attr['x'] = '-0.8'
    if entryX is not None:
        mxcell_attr['style'] = setStyle(portShape, 'entryX', str(entryX))
    if entryY is not None:
        mxcell_attr['style'] = setStyle(portShape, 'entryY', str(entryY))
    if exitX is not None:
        mxcell_attr['style'] = setStyle(portShape, 'exitX', str(exitX))
    if exitY is not None:
        mxcell_attr['style'] = setStyle(portShape, 'exitY', str(exitY))
    
    # Omit the port from the drawing if it's connected to Ground and not a required port of the link.
    if ground is not None and str(portNum) not in link.find('gunns').attrib['reqPorts'].split(','):
        pass
    # Omit duplicated ports that come from a GS port map.
    elif portMap is not None and portMap in completedPortMaps:
        pass
    else:
        gd_rootroot.append(portShape)
        updateContainerBounds(networkShape, portShape.find('mxCell'))
    if portMap is not None:
        completedPortMaps.append(portMap)

# Add spotters to the tree
for spotter in netSpotters:
    spotterShape = copy.deepcopy(shapeLibs.getBlankSpotterShapeMaster(allShapeMasters))
    spotterShape.attrib['label']  = str(spotter[0])
    gd_id = id(idn)
    spotterShape.attrib['id'] = gd_id
    # Config & Input data
    for i in range(0, len(spotter[4])):
        spotterShape.attrib['c'+'{:02d}'.format(i)+'.'+spotter[4][i][0][3:]] = str(spotter[4][i][1])
    for i in range(0, len(spotter[5])):
        spotterShape.attrib['i'+'{:02d}'.format(i)+'.'+spotter[5][i][0][3:]] = str(spotter[5][i][1])
    # Class and constructor args
    spotterShape.attrib['Class'] = spotter[1]
    spotterShape.attrib['ConstructorArgs'] = ', '.join(spotter[6])
    # location
    spotterCell = spotterShape.find('mxCell')
    spotterCell.attrib['parent'] = netId
    spotterGeomAttr = spotterCell.find('mxGeometry').attrib
    spotterGeomAttr['x'] = str(zoom * spotter[2] - 10.0)
    spotterGeomAttr['y'] = str(zoom * (gs_PageHeight - spotter[3]) - 10.0)
    gd_rootroot.append(spotterShape)
    updateContainerBounds(networkShape, spotterShape.find('mxCell'))

# Add compounds lists to the tree
for table in netCompounds:
    addTable(gd_rootroot, netId, copy.deepcopy(shapeLibs.getShapeMaster(allShapeMasters, 'Reactor', 'Compounds')), table, 0, 240)

# Add reactions lists to the tree
for reactions in netReactions:
    addList(gd_rootroot, netId, copy.deepcopy(shapeLibs.getShapeMaster(allShapeMasters, 'Reactor', 'Reactions')), reactions)

# Add battery tables to the tree
for table in netBatteryTables:
    addTable(gd_rootroot, netId, copy.deepcopy(shapeLibs.getShapeMaster(allShapeMasters, 'Data Table', '2D')), table, 0, 160)

# Add socket lists to the tree
for socketList in netSocketLists:
    addList(gd_rootroot, netId, copy.deepcopy(shapeLibs.getShapeMaster(allShapeMasters, 'Socket List', '')), socketList)

# Add the fluid states to the tree
for state in netFluidStates:
    addTable(gd_rootroot, netId, copy.deepcopy(shapeLibs.getShapeMaster(allShapeMasters, 'Fluid', 'State')), state, 3, 240)

# Add the fluid configs to the tree
for config in netFluidConfigs:
    shape = copy.deepcopy(shapeLibs.getShapeMaster(allShapeMasters, 'Fluid', 'Config'))
    shape.attrib['isExternal'] = str(int('False' == config[4]))
    addList(gd_rootroot, netId, shape, config)

# Delete grounds from the tree that have no connection.
theObjects = gd_rootroot.findall('object')
for ground in theObjects:
    ground_gunns = ground.find('gunns')
    if ground_gunns is not None:
        if 'Node' == ground_gunns.attrib['type'] and 'Ground' == ground_gunns.attrib['subtype']:
            gnd_id = ground.attrib['id']
            noConnections = True
            for port in theObjects:
                port_gunns = port.find('gunns')
                if port_gunns is not None:
                    if 'Port' == port_gunns.attrib['type']:
                        port_cell_attr = port.find('mxCell').attrib
                        if (('target' in port_cell_attr and gnd_id == port_cell_attr['target']) or ('source' in port_cell_attr and gnd_id == port_cell_attr['source'])):
                            noConnections = False
                            break
            if noConnections:
                gd_rootroot.remove(ground)

# Add text boxes to the Notes layer.
for textBox in textBoxes:
    # Omit text boxes that appear to be a fluid sensor type label.
    # If a text box value is < 6 chars long, and the text box center is within 20 GS units
    # of a GunnsFluidSensor center, then skip adding this text box to the drawing.
    # Instead, we replace the sensor link's TypeLabel attribute with the text.
    sensorLabel = False
    if 0 < len(textBox[0]) < 6:
        for link in netLinks:
            if 'aspects/fluid/conductor/GunnsFluidSensor' == link[1]:
                if computeRange([textBox[1], textBox[2]], [link[2], link[3]]) < 0.2:
                    for gd_link in gd_rootroot.findall('object'):
                        if link[0] == gd_link.attrib['label']:
                            gd_link.attrib['TypeLabel'] = textBox[0].rstrip().replace(' ', '')
                            break
                    sensorLabel = True
                    break
    if not sensorLabel:
        gs_id = id(idn)
        boxCell = ET.Element('mxCell')
        boxCell.attrib['parent'] = notesLayerId
        boxCell.attrib['vertex'] = '1'
        boxCell.attrib['style'] = 'text;html=1;strokeColor=#999999;fillColor=none;align=' + textBox[5] + ';verticalAlign=' + textBox[6] + ';whiteSpace=wrap;rounded=0;dashed=1;dashPattern=1 2;fontColor=#999999;'
        boxCell.attrib['value'] = textBox[0]
        boxCell.attrib['id'] = gs_id
        geom = ET.Element('mxGeometry')
        geom.attrib['as'] = 'geometry'
        geom.attrib['height'] = str(zoom * textBox[4])
        geom.attrib['width'] = str(zoom * textBox[3])
        geom.attrib['y'] = str(zoom * (gs_PageHeight - textBox[2] - 0.5 * textBox[4]))
        geom.attrib['x'] = str(zoom * (textBox[1] - 0.5 * textBox[3]))
        boxCell.append(geom)
        gd_rootroot.append(boxCell)
        updateContainerBounds(networkShape, boxCell)

# Add lines to the Notes layer.
for line in lines:
    gs_id = id(idn)
    lineCell = ET.Element('mxCell')
    lineCell.attrib['parent'] = notesLayerId
    lineCell.attrib['edge'] = '1'
    lineCell.attrib['style'] = 'endArrow=none;html=1;strokeColor=#999999;dashed=1;dashPattern=1 2;'
    lineCell.attrib['value'] = ''
    lineCell.attrib['id'] = gs_id
    geom = ET.Element('mxGeometry')
    geom.attrib['as'] = 'geometry'
    geom.attrib['relative'] = '1'
    geom.attrib['height'] = str(zoom * math.fabs(line[2] - line[0]))
    geom.attrib['width']  = str(zoom * math.fabs(line[3] - line[1]))
    source = ET.Element('mxPoint')
    source.attrib['as'] = 'sourcePoint'
    source.attrib['y'] = str(zoom * (gs_PageHeight - line[1]))
    source.attrib['x'] = str(zoom * line[0])
    target = ET.Element('mxPoint')
    target.attrib['as'] = 'targetPoint'
    target.attrib['y'] = str(zoom * (gs_PageHeight - line[3]))
    target.attrib['x'] = str(zoom * line[2])
    geom.append(source)
    geom.append(target)
    lineCell.append(geom)
    gd_rootroot.append(lineCell)
    updateContainerBounds(networkShape, lineCell)

# Go through the whole tree and apply snap to all geometries
if options.snap:
    snapElements('*/mxCell/mxGeometry')
    snapElements('*/mxCell/mxGeometry/mxRectangle')
    snapElements('*/mxCell/mxGeometry/Array/mxPoint')
    snapElements('mxCell/mxGeometry')
    snapElements('mxCell/mxGeometry/mxPoint')

# Write the output file
xmlUtils.formatXml(gd_root)
gd_tree.write(outputPathFile, xml_declaration=False)

