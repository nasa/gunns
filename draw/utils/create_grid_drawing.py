#!/usr/bin/python
# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Jason Harvey, CACI, GUNNS, February 2019, --, Initial implementation.}
# @revs_end
#
# This auto-generates a draw.io drawing with a NxN grid of capacitive nodes and links.
# We can use this to assess scalability and export times of GunnsDraw with respect to
# number of network nodes.
#
# Usage:
# $ python create_grid_drawing.py N
# where N is an even number grid size.
import os
import sys
import string
import random
from ctypes import c_int64

ID = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(20)) + '-'
idn = c_int64(0)

def id(n):
    n.value += 1
    return ID + str(n.value)

n         = int(sys.argv[1])
numNodes  = n*n
gridSize  = 200
pageWidth = 120 + (n-1)*gridSize
netName   = 'GdTestGrid' + str(n)
netId     = ID + '1'

# Add the header & network
o = (
'<mxGraphModel arrows="0" connect="1" dx="0" dy="0" fold="1" grid="1" gridSize="10" guides="1" math="0" page="1" pageHeight="' + str(pageWidth) + '" pageScale="1" pageWidth="' + str(pageWidth) + '" shadow="0" tooltips="1">\n'
'  <root>\n'
'    <mxCell id="0" />\n'
'    <mxCell id="1" parent="0" />\n'
'    <object About="Network" c00.convergenceTolerance="0.001" c01.minLinearizationPotential="0.001" c02.minorStepLimit="1" c03.decompositionLimit="1" id="' + id(idn) +'" label="' + netName + '">\n'
'      <gunns subtype="Sub" type="Network" />\n'
'      <mxCell parent="1" style="swimlane;startSize=20;" vertex="1">\n'
'        <mxGeometry as="geometry" height="' + str(pageWidth) + '" width="' + str(pageWidth) + '">\n'
'          <mxRectangle as="alternateBounds" height="20" width="160" />\n'
'        </mxGeometry>\n'
'      </mxCell>\n'
'    </object>\n')

# Add the grid
for row in range(0, n):
    node_y = 40 + row*gridSize
    for col in range(0, n):
        node_x  = 40 + col*gridSize
        nodeNum = col + row*n
        node_id = id(idn)
        cap_id  = id(idn)
        port_id = id(idn)
        o = o + (
# Nodes, Caps & Cap Port 0:
'    <object About="Basic Node" i00.potential="1.0" id="' + node_id + '" label="' + str(nodeNum) + '">\n'
'      <gunns subtype="Basic" type="Node" />\n'
'      <mxCell parent="' + netId + '" style="ellipse;whiteSpace=wrap;html=1;aspect=fixed;strokeColor=#000000;fontSize=12;fontColor=#000000;fillColor=#FFFFFF;" vertex="1">\n'
'        <mxGeometry as="geometry" height="20" width="20" x="' + str(node_x) + '" y="' + str(node_y) + '" />\n'
'      </mxCell>\n'
'    </object>\n'
'    <object About="Thermal Capacitor" Ports="0=node" c00.editCapacitanceGroup="-1" i00.malfBlockageFlag="false" i01.malfBlockageValue="0.0" i02.capacitance="1.0" i03.potential="1.0" i04.temperatureOverrideFlag="false" i05.temperatureOverrideValue="DEFAULT_TEMPERATURE" id="' + cap_id + '" label="cap' + str(nodeNum) + '">\n'
'      <gunns numPorts="2" reqPorts="0" subtype="aspects/thermal/GunnsThermalCapacitor" type="Link" />\n'
'      <mxCell parent="' + netId + '" style="verticalLabelPosition=bottom;align=center;fillColor=#FFFFFF;html=1;verticalAlign=top;strokeColor=#000000;strokeWidth=1;shape=stencil(pVPbDsIgDP0aHjUT9APMvLwtJsYPqLM6ogPC8Pb3VphR5qYxvtFzOO2BtkykVQEGGU+gMpg7JiaM8xNYCesDwZyYIoDDJIQKSgzIfJVlS0IGvYW2jg4pGMil0zbcrJzVezzLjatTSFWglS6w54Bxn1ZMWTJm93PCRJprpciM1KqKmBeecoNULvJTGzRUo0SHNkIvUXSto/7Ix7OGAUK6PIh0qy3urD6qTas5A/fnvhEPutQnfNoZ8NhPi5tYfpCqWz78ro/LN7/jx/IN+c/lY/m/r/+g9x3taAytgJ/T1il467VHw8544AY=);fontSize=12;fontColor=#000000;" vertex="1">\n'
'        <mxGeometry as="geometry" height="20" width="10" x="' + str(node_x + 30) + '" y="' + str(node_y + 20) + '" />\n'
'      </mxCell>\n'
'    </object>\n'
'    <object About="Link Port" id="' + port_id + '" label="0">\n'
'      <gunns type="Port" />\n'
'      <mxCell edge="1" parent="' + netId + '" source="' + node_id + '" style="endArrow=none;html=1;fontSize=9;fontColor=#99CCFF;jumpStyle=arc;edgeStyle=orthogonalEdgeStyle;strokeColor=#000000;entryX=0;entryY=0.5;entryDx=0;entryDy=0;entryPerimeter=0;exitX=0.5;exitY=1;exitDx=0;exitDy=0;" target="' + cap_id + '">\n'
'        <mxGeometry as="geometry" height="50" relative="1" width="50" x="0.3333">\n'
'          <mxPoint as="sourcePoint" x="' + str(node_x - 10) + '" y="' + str(node_y + 40) + '" />\n'
'          <mxPoint as="targetPoint" x="' + str(node_x + 10) + '" y="' + str(node_y + 40) + '" />\n'
'          <Array as="points">\n'
'            <mxPoint x="' + str(node_x + 10) + '" y="' + str(node_y + 30) + '" />\n'
'          </Array>\n'
'          <mxPoint as="offset" />\n'
'        </mxGeometry>\n'
'      </mxCell>\n'
'    </object>\n')

# Conductors on the rows and their ports:
for row in range(0, n):
    node_y = 40 + row*gridSize
    for col in range(0, n-1):
        node_x       = 40 + col*gridSize
        nodeLeftNum  = col + row*n
        nodeRightNum = nodeLeftNum + 1
        nodeLeftId   = ID + str(3*(nodeLeftNum  + 1) - 1)
        nodeRightId  = ID + str(3*(nodeRightNum + 1) - 1)
        link_id      = id(idn)
        o = o + (
'    <object About="Basic Conductor" Ports="0=in, 1=out" c00.defaultConductivity="1.0e-6" i00.malfBlockageFlag="false" i01.malfBlockageValue="0.0" id="' + link_id + '" label="g' + str(nodeLeftNum) + '_' + str(nodeRightNum) + '">\n'
'      <gunns numPorts="2" reqPorts="0,1" subtype="core/GunnsBasicConductor" type="Link" />\n'
'      <mxCell parent="' + netId + '" style="verticalLabelPosition=bottom;shadow=0;dashed=0;align=center;fillColor=#ffffff;html=1;verticalAlign=top;strokeWidth=1;shape=mxgraph.electrical.resistors.resistor_2;fontColor=#000000;" vertex="1">\n'
'        <mxGeometry as="geometry" height="20" width="80" x="' + str(node_x + 70) + '" y="' + str(node_y) + '" />\n'
'      </mxCell>\n'
'    </object>\n'
'    <object About="Link Port" id="' + id(idn) + '" label="0">\n'
'      <gunns type="Port" />\n'
'      <mxCell edge="1" parent="' + netId + '" source="' + nodeLeftId + '" style="endArrow=none;html=1;fontSize=9;fontColor=#99CCFF;jumpStyle=arc;edgeStyle=orthogonalEdgeStyle;entryX=0;entryY=0.5;entryDx=0;entryDy=0;entryPerimeter=0;exitX=1;exitY=0.5;exitDx=0;exitDy=0;" target="' + link_id + '">\n'
'        <mxGeometry as="geometry" height="50" relative="1" width="50">\n'
'          <mxPoint as="sourcePoint" x="' + str(node_x + 30) + '" y="' + str(node_y + 10) + '" />\n'
'          <mxPoint as="targetPoint" x="' + str(node_x + 50) + '" y="' + str(node_y + 10) + '" />\n'
'        </mxGeometry>\n'
'      </mxCell>\n'
'    </object>\n'
'    <object About="Link Port" id="' + id(idn) + '" label="1">\n'
'      <gunns type="Port" />\n'
'      <mxCell edge="1" parent="' + netId + '" source="' + link_id + '" style="endArrow=none;html=1;fontSize=9;fontColor=#99CCFF;jumpStyle=arc;edgeStyle=orthogonalEdgeStyle;entryX=0;entryY=0.5;entryDx=0;entryDy=0;exitX=1;exitY=0.5;exitDx=0;exitDy=0;exitPerimeter=0;" target="' + nodeRightId + '">\n'
'        <mxGeometry as="geometry" height="50" relative="1" width="50">\n'
'          <mxPoint as="sourcePoint" x="' + str(node_x + 170) + '" y="' + str(node_y + 10) + '" />\n'
'          <mxPoint as="targetPoint" x="' + str(node_x + 190) + '" y="' + str(node_y + 10) + '" />\n'
'        </mxGeometry>\n'
'      </mxCell>\n'
'    </object>\n')

# Conductors between the rows and their ports:
for row in range(0, n-1):
    node_y = 40 + row*gridSize
    col = n - 1 # even, right side
    label = 'left'
    if row & 1: # odd, left side
        col   = 0
        label = 'right'
    node_x      = 40 + col*gridSize
    nodeUpNum   = col + row*n
    nodeDownNum = nodeUpNum + n
    nodeUpId    = ID + str(3*(nodeUpNum   + 1) - 1)
    nodeDownId  = ID + str(3*(nodeDownNum + 1) - 1)
    link_id     = id(idn)
    o = o + (
'    <object About="Basic Conductor" Ports="0=in, 1=out" c00.defaultConductivity="1.0e-6" i00.malfBlockageFlag="false" i01.malfBlockageValue="0.0" id="' + link_id + '" label="g' + str(nodeUpNum) + '_' + str(nodeDownNum) + '">\n'
'      <gunns numPorts="2" reqPorts="0,1" subtype="core/GunnsBasicConductor" type="Link" />\n'
'      <mxCell parent="' + netId + '" style="verticalLabelPosition=middle;shadow=0;dashed=0;align=right;fillColor=#ffffff;html=1;verticalAlign=middle;strokeWidth=1;shape=mxgraph.electrical.resistors.resistor_2;fontColor=#000000;direction=south;labelPosition=' + label + ';" vertex="1">\n'
'        <mxGeometry as="geometry" height="80" width="20" x="' + str(node_x) + '" y="' + str(node_y + 70) + '" />\n'
'      </mxCell>\n'
'    </object>\n'
'    <object About="Link Port" id="' + id(idn) + '" label="0">\n'
'      <gunns type="Port" />\n'
'      <mxCell edge="1" parent="' + netId + '" source="' + nodeUpId + '" style="endArrow=none;html=1;fontSize=9;fontColor=#99CCFF;jumpStyle=arc;edgeStyle=orthogonalEdgeStyle;entryX=0;entryY=0.5;entryDx=0;entryDy=0;entryPerimeter=0;exitX=0.5;exitY=1;exitDx=0;exitDy=0;" target="' + link_id + '">\n'
'        <mxGeometry as="geometry" height="50" relative="1" width="50">\n'
'          <mxPoint as="sourcePoint" x="' + str(node_x - 30) + '" y="' + str(node_y + 50) + '" />\n'
'          <mxPoint as="targetPoint" x="' + str(node_x - 10) + '" y="' + str(node_y + 50) + '" />\n'
'        </mxGeometry>\n'
'      </mxCell>\n'
'    </object>\n'
'    <object About="Link Port" id="' + id(idn) + '" label="1">\n'
'      <gunns type="Port" />\n'
'      <mxCell edge="1" parent="' + netId + '" source="' + link_id + '" style="endArrow=none;html=1;fontSize=9;fontColor=#99CCFF;jumpStyle=arc;edgeStyle=orthogonalEdgeStyle;entryX=0.5;entryY=0;entryDx=0;entryDy=0;exitX=1;exitY=0.5;exitDx=0;exitDy=0;exitPerimeter=0;" target="' + nodeDownId + '">\n'
'        <mxGeometry as="geometry" height="50" relative="1" width="50">\n'
'          <mxPoint as="sourcePoint" x="' + str(node_x - 40) + '" y="' + str(node_y + 180) + '" />\n'
'          <mxPoint as="targetPoint" x="' + str(node_x - 20) + '" y="' + str(node_y + 180) + '" />\n'
'        </mxGeometry>\n'
'      </mxCell>\n'
'    </object>\n')

# Footer:
o = o + (
'  </root>\n'
'</mxGraphModel>\n')

print(o)

with open(netName + '.xml', 'w') as fout:
    fout.write(o)
