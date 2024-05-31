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
             'libraries/GUNNS_Spotters.xml']
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
                    xmlStr = compressedXml.replace('&gt;', '>').replace('&lt;', '<')
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
    return None

# Returns the Port shape master from the shapeTree, or None.
# portNum is the port number label as a string.
def getPortShapeMaster(allShapes, portNum):
    for shape in allShapes:
        shape_gunns_attr = shape.find('./gunns').attrib
        if ('Port' == shape_gunns_attr['type']) and (portNum == shape.attrib['label']):
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

# Returns the subtype from the given shape.
def getShapeSubtype(shape):
    gunns_attr = shape.find('./gunns').attrib
    return gunns_attr['subtype']
