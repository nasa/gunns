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
import xml.etree.ElementTree as ET
import modules.compression as compression
import json

# This is the shape libraries list.
# This default definition should always contain
# all of the built-in GUNNS libs in libraries/.
# During the shape update function of this script,
# custom libs will be appended to this list.
shapeLibs = ['libraries/GUNNS_Generic.xml',
             'libraries/GUNNS_Electric.xml',
             'libraries/GUNNS_Thermal.xml',
             'libraries/GUNNS_Fluid.xml',
             'libraries/GUNNS_Super.xml',
             'libraries/GUNNS_Obsolete.xml',
             'libraries/GUNNS_Spotters.xml',
             'libraries/GUNNS_Doxygen.xml']
shapeTree = ET.ElementTree()
shapeTree._setroot(ET.fromstring('<shapeTree></shapeTree>'))

# Loads links and spotters master shape xml from the given shape library
# into the master shapeTree.
# The linksOnly flag indicates to only load links and spotters
#TODO
# handle duplicated masters in the same or different libs
# - do we ignore duplicates, or override previous?
#   overriding might be a nice feature...
def loadShapeLibs(libFile, linksOnly):
    # Draw.io custom shape libraries are a mix of xml & json format:
    # <mxlibrary>[{"xml":"blob"..."title":"Data Table"}
    #             {"xml":"blob"..."title":"Spotter (Empty)"}
    #             {"xml":"blob"..."title":"Link Port 0"}
    #             ...]</mxlibrary>
    # Let's not bother with reading it into xml first, just read
    # the file into a string and strip the mxlibrary tags.
    with open(libFile, 'r') as fin:
        finData = fin.read()
        finJson = finData[len('<mxlibrary>'):-len('</mxlibrary>')]
        finList = json.loads(finJson)
        for shape in finList:
            # Newer versions of draw.io no longer compress the shape's xml element
            # when saving the library.  We need to determine whether the shape's
            # xml element value is compressed and only decompress if it is.
            # We'll do this by searching for 'mxGraphModel' in the value, which is
            # unlikely to appear in compressed data.  The uncompressed format might
            # have '&gt;' and '&lt;' instead of '>', '<', so replace if needed.
            compressedXml = shape['xml']
            if 'mxGraphModel' in compressedXml:
                if '&gt;' in compressedXml and '&lt;' in compressedXml:
                    xmlStr = compressedXml.replace('&gt;', '>').replace('&lt;', '<').replace('&amp;#xa;','')
                else:
                    xmlStr = compressedXml # it's already not compressed
            else:
                xmlStr = compression.decompress(compressedXml) # decompress it
            # we only want the <object> inside <mxGraphModel><root>
            root = ET.fromstring(xmlStr)
            obj  = root.findall('./root/object')
            # Do not load shapes that are sets of objects, as these will
            # conflict with a real shape master somewhere else
            if 1 == len(obj):
                gunns_elem = obj[0].find('./gunns')
                if None != gunns_elem:
                    if linksOnly:
                        gunns_type = gunns_elem.attrib['type']
                        # now add it to the master shape tree
                        if 'Link' == gunns_type and '' != gunns_elem.attrib['subtype']:
                            shapeTree.getroot().append(obj[0])
                        if 'Spotter' == gunns_type and '' != obj[0].attrib['Class']:
                            shapeTree.getroot().append(obj[0])
                    else:
                        shapeTree.getroot().append(obj[0])

# Returns the Network Config shape master from the shapeTree, or None.
def getNetworkShapeMaster(allShapes):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if 'Network' == shape_gunns_attr['type'] and 'Sub' == shape_gunns_attr['subtype']:
            return shape
    return None

# Returns the Super-Network Config shape master from the shapeTree, or None.
def getSuperNetworkShapeMaster(allShapes):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if 'Network' == shape_gunns_attr['type'] and 'Super' == shape_gunns_attr['subtype']:
            return shape
    return None

# Returns the Sub-Network Config shape master from the shapeTree, or None.
def getSubNetworkShapeMaster(allShapes):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if 'Network' == shape_gunns_attr['type'] and 'Sub' == shape_gunns_attr['subtype']:
            return shape
    return None

# Returns the Ground Node shape master from the shapeTree, or None.
def getGroundShapeMaster(allShapes):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if ('Node' == shape_gunns_attr['type']) and ('Ground' == shape_gunns_attr['subtype']):
            return shape
    return None

# Returns the given subtype node shape master from the shapeTree, or None.
def getNetNodeShapeMaster(allShapes, subtype, frame):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if 'Node' == shape_gunns_attr['type']:
            if subtype == shape_gunns_attr['subtype']:
                mxcell_attr = shape.find('mxCell').attrib
                if frame:
                    if 'shape=mxgraph.basic.rounded_frame' in mxcell_attr['style']:
                        return shape
                else:
                    if 'shape=mxgraph.basic.rounded_frame' not in mxcell_attr['style']:
                        return shape
    return None

# Returns the given subtype reference node shape master from the shapeTree, or None.
def getRefNodeShapeMaster(allShapes, subtype, vent):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if 'Node' == shape_gunns_attr['type']:
            if subtype == shape_gunns_attr['subtype']:
                mxcell_attr = shape.find('mxCell').attrib
                if vent:
                    if 'ellipse' not in mxcell_attr['style']:
                        return shape
                else:
                    if 'ellipse' in mxcell_attr['style']:
                        return shape
    return None

# Returns the given link shape master from the shapeTree, or None.
def getLinkSubtypeShapeMaster(allShapes, subtype):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if 'Link' == shape_gunns_attr['type']:
            if subtype == shape_gunns_attr['subtype']:
                return shape
    return None

# Returns the given link shape master from the shapeTree, or None.
# This looks for the first match to the link class name, rather than the full
# subtype.
def getLinkClassShapeMaster(allShapes, linkClass):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if 'Link' == shape_gunns_attr['type']:
            if shape_gunns_attr['subtype'].endswith(linkClass):
                return shape
    return None

# Returns the given link's shape master from the shapeTree, or None.
def getLinkShapeMaster(link, allShapes):
    link_gunns_attr = link.find('./gunns').attrib
    link_subtype = link_gunns_attr['subtype']
    link_variant = ''
    if 'variant' in link_gunns_attr:
        link_variant = link_gunns_attr['variant']
    # Find the link's shape master in shapeTree, as the first match
    # to the link's <gunns> subtype and variant attributes.
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if 'Link' == shape_gunns_attr['type']:
            shape_subtype = shape_gunns_attr['subtype']
            shape_variant = ''
            if 'variant' in shape_gunns_attr:
                shape_variant = shape_gunns_attr['variant']
            if shape_subtype == link_subtype and shape_variant == link_variant:
                return shape
    return None

# Returns the given spotter's shape master from the shapeTree, or None.
def getSpotterShapeMaster(spotter, allShapes):
    spotter_attr       = spotter.attrib
    spotter_gunns_attr = spotter.find('./gunns').attrib
    spotter_class      = spotter_attr['Class']
    spotter_variant    = ''
    empty_spotter = None
    if 'variant' in spotter_gunns_attr:
        spotter_variant = spotter_gunns_attr['variant']
    # Find the object's shape master in shapeTree, as the first match
    # to the spotter's <object> Class and <gunns> variant attributes.
    for shape in allShapes:
        shape_attr       = shape.attrib
        shape_gunns_attr = shape.find('./gunns').attrib
        if 'Spotter' == shape_gunns_attr['type']:
            shape_class   = shape_attr['Class']
            shape_variant = ''
            if 'variant' in shape_gunns_attr:
                shape_variant = shape_gunns_attr['variant']
            if shape_class == spotter_class and shape_variant == spotter_variant:
                return shape
            if shape_class == "":
                empty_spotter = shape
    # If there are no matching spotters, return the empty spotter.
    return empty_spotter

# Returns the Port shape master from the shapeTree, or None.
# portNum is the port number label as a string.
def getPortShapeMaster(allShapes, portNum):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if ('Port' == shape_gunns_attr['type']) and (portNum == shape.attrib['label']):
            return shape
    return None

# Returns the Super Port shape master from the shapeTree, or None.
# portNum is the port number label as a string.
def getSuperPortShapeMaster(allShapes, portNum):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if ('Super Port' == shape_gunns_attr['type']) and (portNum == shape.attrib['label']):
            return shape
    return None

# Returns the Subnetwork Interface Connection shape master from the shapeTree, or None.
def getSubNetworkIFConnectionShapeMaster(allShapes):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if ('Subnet Interface Connection' == shape_gunns_attr['type']):
            return shape
    return None

# Returns the shape master from the shapeTree matching the
# given GUNNS type and subtype, or None.
def getShapeMaster(allShapes, intype, subtype):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if (intype == shape_gunns_attr['type']) and (subtype == shape_gunns_attr['subtype']):
            return shape
    return None

# Returns the blank Spotter shape master from the shapeTree, or None.
def getBlankSpotterShapeMaster(allShapes):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if ('Spotter' == shape_gunns_attr['type']) and ('' == shape.attrib['Class']):
            return shape
    return None

# Returns the type from the given shape.
def getShapeType(shape):
    gunns_attr = shape.find('./gunns').attrib
    return gunns_attr['type']

# Returns the subtype from the given shape.
def getShapeSubtype(shape):
    gunns_attr = shape.find('./gunns').attrib
    return gunns_attr['subtype']

# Convert style properties into a dictionary
# start with form:
# shape=mxgraph.basic.rounded_frame;fillColor=#000000;labelPosition=left;verticalLabelPosition=top;verticalAlign=bottom;align=right;etc...
# end with form:
# {'shape': 'mxgraph.basic.rounded_frame', 'fillColor': '#000000', 'labelPosition': 'left', etc...}
def stylePropsToDict(style_properties):
    style_properties = style_properties.split(';')[0:-1]
    style_properties = dict(zip(list(map(lambda x : x.split('=',1)[0]                                 ,   style_properties)),
                                list(map(lambda x : x.split('=',1)[1] if len(x.split('=',1))>1 else '',   style_properties))))
    return style_properties

# Convert dictionary back to style properties
def dictToStyleProps(style_dict):
    return ';'.join(list(map(lambda k: k if style_dict[k] == '' else k + '=' + style_dict[k], style_dict.keys()))) + ';'

# convert normalized RGB to normalized HSV
def rgb2hsv(r,g,b):
    c_max = max(r,g,b)
    c_min = min(r,g,b)
    delta = c_max - c_min
    # value
    v = c_max
    # saturation
    if c_max == 0:
        s = 0
    else:
        s = delta / c_max
    # hue
    if delta == 0:
        h = 0
    elif c_max == r:
        h = ((g - b) / delta) % 6
    elif c_max == g:
        h = ((b - r) / delta) + 2
    elif c_max == b:
        h = ((r - g) / delta) + 4
    h = h%6 / 6
    return (h,s,v)

# convert normalized HSV to normalized RGB
def hsv2rgb(h,s,v):
    chroma = v * s
    h *= 6
    x = chroma * (1 - abs(h%2 - 1))
    if h < 1:
        r,g,b = (chroma,x,0)
    elif h < 2:
        r,g,b = (x,chroma,0)
    elif h < 3:
        r,g,b = (0,chroma,x)
    elif h < 4:
        r,g,b = (0,x,chroma)
    elif h < 5:
        r,g,b = (x,0,chroma)
    else:
        r,g,b = (chroma,0,x)
    m = v - chroma
    return (r+m,g+m,b+m)

# input is a color of from '#rrggbb' or '#rrggbbaa'
def getDarkColor(light):
    alpha = ''
    if len(light) == len('#rrggbbaa'):
        alpha = light[-2:]

    dark = light
    # convert RGB hex to RGB dec
    r = int(dark[1:3],16)/255.0
    g = int(dark[3:5],16)/255.0
    b = int(dark[5:7],16)/255.0

    h,s,v = rgb2hsv(r,g,b)

    if s >= 0.95 and v >= 0.95:
        # for high saturation/value colors, just half the value
        # TODO this will be tweaked.
        v *=0.65
    else:
        # invert colors
        r = 1.0-r
        g = 1.0-g
        b = 1.0-b

        # convert to HSV
        h,s,v = rgb2hsv(r,g,b)

        # rotate hue by 180 degrees
        h = (h+0.5)%1

    # convert to RGB
    r,g,b = hsv2rgb(h,s,v)
    r = int(255*r)
    g = int(255*g)
    b = int(255*b)

    # convert RGB dec to RGB hex
    dark = '#' + f"{r:02X}{g:02X}{b:02X}" + alpha

    if dark != light:
        return 'light-dark(' + light + ',' + dark + ')'
    else:
        return light

# loops through style dictionary (see stylePropsToDict()) and sets dark mode for color properties
def setLightDarkColors(style_dict):
    for prop in style_dict:
        # filter for color properties of the form '#rrggbb(aa)'. this filters out colors
        # that are set to 'default', 'none', or 'light-dark(#rrggbb(aa),#rrggbb(aa))'
        if 'Color' in prop and style_dict[prop].startswith('#'):
            light = style_dict[prop].upper()

            if (light == '#FFFFFF' and (prop == 'fillColor' or prop == 'swimlaneFillColor' or prop == 'labelBackgroundColor')
             or light == '#000000' and (prop == 'fontColor' or prop == 'strokeColor')):
                    style_dict[prop] = 'default'

            elif prop != 'fillColor' or prop == 'fillColor' and ('fillOpacity' not in style_dict or 'fillOpacity' in style_dict and int(style_dict['fillOpacity']) > 25):
                style_dict[prop] = getDarkColor(light)
            else:
                style_dict[prop] = light
    return