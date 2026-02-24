#!/usr/bin/python
# @copyright Copyright 2024 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
#TODO
# This needs to also be able to add new drawings to an existing super-drawing, and to increment/decrement/remove
# instances of old drawings
import os, sys, copy, re

# Python 2.7 vs. 3 imports by feature detection:
try:
    from Tkinter import *
except ModuleNotFoundError:
    from tkinter import *
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
import modules.consoleMsg as console
import modules.shapeLibs as shapeLibs
import modules.compression as compression
import modules.xmlUtils as xmlUtils
import string
import random
from ctypes import c_int64

# Class to hold values specific to a sub-network drawing.
class DrawingData:
  def __init__(self, filename):
    self.filename = filename
    self.count    = 1
    return

# Class to hold global values for the super-network.
class ProjectData:
  name = ''
  path = ''
  def __init__(self):
    return

###################################################################
# GUI to select sub-network drawings and define the number of each:
###################################################################
# Setup frames
win = Tk()
win.title('Super-Network Create')
win.geometry('+%d+%d' % (win.winfo_screenwidth()/2, win.winfo_screenheight()/2))
ppframe = Frame(win)
ppframe.pack(side=TOP)
label = Label(win, text='Select sub-network drawings and the number of each:')
label.pack(side=TOP)
frame = Frame(win)
frame.pack(side=TOP)
dnframe = Frame(frame)
dnframe.pack(side=LEFT)
dcframe = Frame(frame)
dcframe.pack(side=LEFT)
ccframe = Frame(win)
ccframe.pack(side=BOTTOM)
offrame = Frame(win)
offrame.pack(side=BOTTOM)
arframe = Frame(win)
arframe.pack(side=BOTTOM)

# Entry widgets
#entryLabel = Label(enframe, text='Enter the project environment path:')
#entryLabel.pack(side=TOP)
#envEntry = Entry(enframe, bg='white', fg='black')
#envEntry.pack(side=TOP)
#nameLabel = Label(enframe, text='Name this super-network configuration:')
#nameLabel.pack(side=TOP)
#nameEntry = Entry(enframe, bg='white', fg='black')
#nameEntry.pack(side=TOP)

outputLabels = []
projectPathLabels = []
drawings = []
drawingNameLabels = []
drawingCountLabels = []
project = ProjectData()

# Asks the user to select the output file, saves the result.
def select_output():
    ftypes = [('XML files', '*.xml')]
    f = TKFILE.asksaveasfile(title = "Select a drawing file to create", filetypes = ftypes)
    project.name = os.path.abspath(f.name)
    f.close()
    # Add the resulting file name as a label
    if outputLabels:
        outputLabels.pop().destroy()
    newOutputLabel = Label(offrame, text=project.name)
    newOutputLabel.pack(side=BOTTOM)
    outputLabels.append(newOutputLabel)

# Asks the user to select the project path, saves the result.
def select_project():
    f = TKFILE.askdirectory(title = "Select the project path")
    project.path = os.path.abspath(f) + '/'
    # Add the resulting path as a label
    if projectPathLabels:
        projectPathLabels.pop().destroy()
    newPathLabel = Label(ppframe, text=project.path)
    newPathLabel.pack(side=BOTTOM)
    projectPathLabels.append(newPathLabel)

# Closes the GUI and passes execution to the remaining script.
def create():
    win.destroy()

# Closes the GUI and exits the script.
def cancel():
    win.destroy()
    sys.exit()

# Remove the last drawing from the list, and its associated labels from the GUI.
def remove():
    drawings.pop()
    drawingNameLabels.pop().destroy()
    drawingCountLabels.pop().destroy()

# Redraws the last drawing label in the GUI so it will show the latest instance count.
def refreshLastDrawingCount():
    drawingCountLabels.pop().destroy()
    newDrawingCountLabel = Label(dcframe, text=str(drawings[-1].count))
    newDrawingCountLabel.pack(side=TOP)
    drawingCountLabels.append(newDrawingCountLabel)

# Increments the last drawing's instance count and refreshes its GUI display.
def plus():
    if len(drawings) > 0:
        drawings[-1].count += 1
        refreshLastDrawingCount()

# Decrements the last drawing's instanct count and refreshes its GUI display.
# Removes the drawing from the project if its instance count reaches zero.
def minus():
    if len(drawings) > 0:
        drawings[-1].count -= 1
        refreshLastDrawingCount()
        if drawings[-1].count <= 0:
            remove()

# Adds a drawing to the drawing list and GUI.
def add():
    # Pop-up a message if the project path isn't set yet.
    if '' == project.path:
        TKMBOX.showerror("Error", "Please select a project path first.")
        return

    # Select a new drawing file
    ftypes = [('XML files', '*.xml')]
    newPathFile = TKFILE.askopenfilename(title = "Select a drawing file to add", filetypes = ftypes, initialdir = project.path)
    #newPath, newFile = os.path.split(os.path.abspath(newPathFile))

    # Abort if the selected drawing isn't in the project path
    commonPath = os.path.commonprefix([project.path, os.path.abspath(newPathFile)])
    if project.path not in commonPath:
        TKMBOX.showerror("Error", "Drawing files must descend from the project path.")
        return
    relPathFile = newPathFile[len(commonPath):]

    #newDrawingNameLabel = Label(dnframe, text=os.path.splitext(newFile)[0])
    newDrawingNameLabel = Label(dnframe, text=relPathFile)
    newDrawingNameLabel.pack(side=TOP)

    newDrawingCountLabel = Label(dcframe, text='1')
    newDrawingCountLabel.pack(side=TOP)

    drawings.append(DrawingData(relPathFile))
    drawingNameLabels.append(newDrawingNameLabel)
    drawingCountLabels.append(newDrawingCountLabel)

# Button widgets
addbutton = Button(arframe, text='Add', width=15, command=add)
addbutton.pack(side=LEFT)

minusbutton = Button(arframe, text='-', width=2, command=minus)
minusbutton.pack(side=RIGHT)

plusbutton = Button(arframe, text='+', width=2, command=plus)
plusbutton.pack(side=RIGHT)

projectButton = Button(ppframe, text='Select project path', command=select_project)
projectButton.pack(side=TOP)

nameButton = Button(offrame, text='Select output file', command=select_output)
nameButton.pack(side=TOP)

createbutton = Button(ccframe, text='Create', width=15, command=create)
createbutton.pack(side=LEFT)

cancelbutton = Button(ccframe, text='Cancel', width=15, command=cancel)
cancelbutton.pack(side=RIGHT)

# Main window GUI loop
win.mainloop()

################################
# Generate Super-Network Drawing
################################
homepath = os.path.dirname(os.path.abspath(__file__))

# TODO refactor with netexport.py...
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

# TODO refactor with netexport.py...
# Returns True if 'obj' is a descendant of 'of', by their id attributes.
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

# TODO for each drawingFiles, copy its contained network container and all child objects into the new tree,
# drawingCounts number of copies.
# Prior to copying in, modify the contents:
#   add instance name attribute
#   add super nodes offset attribute
#   update node #'s by the offset
#   destroy some links info to enforce one-way copying
#TODO maybe move the save as file browser to a pop-up at the end...
print('')
print('Creating', project.name, '...')

#TODO refactor with gsmigrate.py...
# Initialize the XML tree
gd_tree = ET.ElementTree()
gd_tree._setroot(ET.fromstring('<mxGraphModel arrows="0" connect="1" dx="0" dy="0" fold="1" grid="1" gridSize="10" guides="1" math="0" page="1" pageHeight="1100" pageScale="1" pageWidth="850" shadow="0" tooltips="1"><root><mxCell id="0" /></root></mxGraphModel>'))
gd_root = gd_tree.getroot()
gd_rootroot = gd_root.find('root')

id_conversions = {}
# Adds a copy of the given element to the super-network, replacing all ID fields
# with a unique set to avoid duplicated ID's in the final super-drawing.
def addElemToSuper(elem):
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
    if gunns is not None and gunns.attrib['type'] == 'Network' and gunns.attrib['subtype'] == 'Sub':
        # Sub-network container element's parent is always 2, the super-network container.
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
    # Add copied element to the drawing.
    gd_rootroot.append(copiedElem)

# Add the Notes layer.
notesLayerCell = ET.Element('mxCell')
notesLayerCell.attrib['parent'] = '0'
notesLayerCell.attrib['value'] = 'Notes'
notesLayerCell.attrib['id'] = '3'
gd_rootroot.append(notesLayerCell)

# Add the Network layer after/in front of the Notes layer.  This puts text boxes in the Notes layer
# behind objects in the network, so they don't block access to the network objects.
netLayerCell = ET.Element('mxCell')
netLayerCell.attrib['parent'] = '0'
netLayerCell.attrib['value'] = 'Network'
netLayerCell.attrib['id'] = '1'
gd_rootroot.append(netLayerCell)

# Add a super-network container to hold its config data.  It's size and config data values
# will be set later.
shapeLibs.loadShapeLibs(homepath + '/' + 'libraries/GUNNS_Super.xml', False)
allShapeMasters = shapeLibs.shapeTree.findall('./object')
superNetConfig  = shapeLibs.getSuperNetworkShapeMaster(allShapeMasters)
superNetConfig.attrib['id'] = '2'
superNetConfig.attrib['SimVariable'] = 'None'
superNetConfig.find('mxCell').attrib['parent'] = '1'
gd_rootroot.append(superNetConfig)

super_nodes_offset = 0
sub_network_count  = 0

# Main drawing loop:
for drawing in drawings:
    print('  Including', drawing.count, 'instances of', project.path + drawing.filename)
    # TODO refactor with netexport.py...
    tree = ET.parse(project.path + drawing.filename)
    root = tree.getroot()
    # Handle draw.io's compressed vs. uncompressed xml file formats.
    # If root node = mxGraphModel then this is uncompressed.
    # If root node = mxfile then this is compressed, and the mxGraphModel
    # section is compressed in the <mxfile><diagram>text</diagram></mxfile>.
    if root.tag.startswith('mxfile'):
        print('    Decompressing diagram data...')
        compressed_diagram = root[0].text
        diagram = compression.decompress(compressed_diagram)
        # Reset the root and tree after decompressing.  This discards
        # the outer <mxfile><diagram> elements and makes a consistent
        # tree structure with the un-compressed source file.
        root = ET.fromstring(diagram)
        tree._setroot(root)
    if not root.tag.startswith('mxGraphModel'):
        sys.exit(console.abort('this is not a recognized file.'))
    rootroot = root.find('./root')

    # Drawing instance loop
    for instance in range(0, drawing.count):
        instance_root = copy.deepcopy(rootroot)

        sub_nodes_count = 0 # this will be set later by either lookng up the value from the sub-
                            # network interface containers, if present, otherwise by counting the
                            # normal nodes we find.  When the instance is complete, we add this
                            # to super_nodes_offset.

        objects = instance_root.findall('./object')
        # This skips <mxCell id="0"/> since it has no parent attribute
        mxcells = instance_root.findall('./mxCell')[1:]
        objects_and_cells = []
        for an_object in objects:
            objects_and_cells.append(an_object)
        for cell in mxcells:
            objects_and_cells.append(cell)

        netConfig = None
        # Find the network container object and add it to the super tree.
        for an_object in objects:
            gunns = an_object.find('./gunns')
            if gunns is not None:
                if 'Network' == gunns.attrib['type']:
                    if 'Super' == gunns.attrib['subtype']:
                        sys.exit(console.abort('nested super-networks aren\'t supported yet.'))
                    elif 'Sub' == gunns.attrib['subtype']:
                        netConfig = an_object
                        break

        if netConfig is None:
            sys.exit(console.abort('a network config wasn\'t found.'))

        # Error check any sub-network interface containers that are not duplicates.
        subnetIfsPresent = False
        for an_object in objects:
            gunns = an_object.find('./gunns')
            if gunns is not None:
                if 'Network' == gunns.attrib['type'] and 'Subnet Interface' == gunns.attrib['subtype'] and not an_object.findall('./gunnsSubnetIfDuplicate'):
                    subnetIfsPresent = True
                    # Check for missing information in the subnet interface.
                    subnetIfNodeCountElem = an_object.find('./gunnsSubnetIfNodeCount')
                    if subnetIfNodeCountElem is None:
                        sys.exit(console.abort('a sub-network interface container in network type: ' + netConfig.attrib['label'] + ' is missing the network node count.  Make sure to export the sub-network drawing first.'))
                    else:
                        try:
                            sub_nodes_count_str = subnetIfNodeCountElem.attrib['count']
                        except (KeyError):
                            sys.exit(console.abort('a sub-network interface container in network type: ' + netConfig.attrib['label'] + ' has obsolete syntax for the network node count.  Please re-export the sub-network drawing first.'))
                        sub_nodes_count = int(sub_nodes_count_str)
                    # This is a nuisance if there are no connections (all reference nodes) by design.  So commenting out for now.
                    #if 0 == len(an_object.findall('./gunnsSubnetIfConnection')):
                    #    print('    ' + console.warn('sub-network inteface container: ' + an_object.attrib['label'] + ' in network type: ' + netConfig.attrib['label'] + ' has no link connections.  Make sure to export the sub-network drawing first.'))

        # Make changes to the config before appending:
        # - Add sim variable, super nodes offset, and project-relative source drawing attributes to the visible shape data
        netConfig.attrib['SimVariable']      = 'None'
        netConfig.attrib['SuperNodesOffset'] = str(super_nodes_offset)
        netConfig.attrib['sourceDrawing']    = drawing.filename
        # - Copy its network configuration data to the super-network, then delete it here.
        for attr in netConfig.attrib:
            if re.search('^([c,i])([0-9])', attr) is not None:
                superNetConfig.attrib[attr] = netConfig.attrib[attr]
                netConfig.attrib[attr] = ''
        # - Set container to collapsed, we also have to swap its mxGeometry and mxRectangle values
        netConfig.find('mxCell').attrib['collapsed'] = '1'
        height           = netConfig.find('mxCell/mxGeometry').attrib['height']
        width            = netConfig.find('mxCell/mxGeometry').attrib['width']
        height_collapsed = netConfig.find('mxCell/mxGeometry/mxRectangle').attrib['height']
        width_collapsed  = netConfig.find('mxCell/mxGeometry/mxRectangle').attrib['width']
        netConfig.find('mxCell/mxGeometry').attrib['height']             = height_collapsed
        netConfig.find('mxCell/mxGeometry').attrib['width']              = width_collapsed
        netConfig.find('mxCell/mxGeometry/mxRectangle').attrib['height'] = height
        netConfig.find('mxCell/mxGeometry/mxRectangle').attrib['width']  = width
        # - Set x,y coordinates to stack all sub-network containers vertically inside the super-network container.
        netConfig.find('mxCell/mxGeometry').attrib['x']             = '20'
        netConfig.find('mxCell/mxGeometry').attrib['y']             = str(40 + 40*sub_network_count)
        netConfig.find('mxCell/mxGeometry/mxRectangle').attrib['x'] = '20'
        netConfig.find('mxCell/mxGeometry/mxRectangle').attrib['y'] = str(40 + 40*sub_network_count)
        addElemToSuper(netConfig)

        regular_node_types  = ['Basic', 'Fluid']
        numbered_node_types = ['Basic', 'Fluid', 'Reference']

        # Add all child objects and mxCells of the network container to the super tree.
        for an_object in objects_and_cells:
            if isDescendant(an_object, netConfig, objects_and_cells):
                gunns = an_object.find('gunns')
                if subnetIfsPresent:
                    # When sub-network interface containers are present, they and their children
                    # are the only thing we import.  But we don't import duplicate interface containers.
                    if gunns is not None:
                        if 'Network' == gunns.attrib['type'] and 'Subnet Interface' == gunns.attrib['subtype'] and not an_object.findall('./gunnsSubnetIfDuplicate'):
                            addElemToSuper(an_object)
                            for child_object in objects_and_cells:
                                if isDescendant(child_object, an_object, objects_and_cells):
                                    addElemToSuper(child_object)
                else:
                    if gunns is not None:
                        if 'Node' == gunns.attrib['type']:
                            # Regular nodes increment the instance node count.
                            if gunns.attrib['subtype'] in regular_node_types:
                                sub_nodes_count += 1
                            # For nodes with a number (regular or reference nodes), add the
                            # super nodes offset to their number.
                            if gunns.attrib['subtype'] in numbered_node_types:
                                an_object.attrib['label'] = str(super_nodes_offset + int(an_object.attrib['label']))
                        elif 'Link' == gunns.attrib['type']:
                            # Destroy link info to prevent this network copy from being used as source for netexport.
                            gunns.attrib['subtype'] = ''
                    addElemToSuper(an_object)

        # Increment the super-network node offset by this instance's final node count.
        super_nodes_offset += sub_nodes_count
        sub_network_count  += 1

        # Reset the id prefix conversions dictionary for the next instance.
        id_conversions = {}

# Set the super-network container geometry to expanded, and fitting all the collapsed sub-networks inside.
superNetConfig.find('mxCell').attrib['collapsed'] = '0'
superNetConfig.find('mxCell/mxGeometry').attrib['height']             = str(40 * (sub_network_count + 1))
superNetConfig.find('mxCell/mxGeometry').attrib['width']              = '250'
superNetConfig.find('mxCell/mxGeometry/mxRectangle').attrib['height'] = '20'
superNetConfig.find('mxCell/mxGeometry/mxRectangle').attrib['width']  = '160'

# Write the output file.
xmlUtils.formatXml(gd_root)
gd_tree.write(project.name, xml_declaration=False)

print('Complete!')
print('')
