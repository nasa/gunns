# IMPORTANT: This project is still pending release

# General-Use Nodal Network Solver (GUNNS)

The GUNNS code and wiki will be hosted here once it receives approval for open-source release from NASA.  Stay tuned!

GUNNS is a set of generic C++ math models that combines nodal circuit analysis algorithms with the hydraulic-thermal-electric analogy to model flow circuits (fluid, thermal, electrical).  The models include generic physical components like pumps, valves, resistors & capacitors.  These components, called 'links', are connected to 'nodes' and configured with their characteristic values, creating a 'network' (circuit) of links and nodes.  The state of the network and its contained models are then propagated in the time domain.

### GunnsDraw, the Network Design GUI

Networks can be drawn up with a [Draw.io](https://www.draw.io) drawing, from which run-time C++ code is auto-generated that can be built into simulations.  This is called 'GunnsDraw'.  GunnsDraw includes custom Draw.io shapes for the generic physical components and Python scripts to auto-generate the run-time C++ code.
  
### Extensible

GUNNS is designed to be extensible and flexible.  It is written with object-oriented polymorphic interfaces between the network solver, links, nodes, and other extensible object types.  This allows users to develop new classes to model things that the GUNNS baseline doesn't cover.  Users can create custom GunnsDraw shapes by taking advantage of Draw.io shapes' extensible XML schema.  
  
GUNNS can also be extended to model other domains besides the above flow systems, as it can solve systems of the general form [A]{x} = {b} where [A] is a symmetric positive semi-definite matrix.  This is demonstrated in an included simple 6-DOF rigid body dynamics equations of motion model.

### Simulation Environment Not Included

GUNNS does not provide a complete simulation environment, as it is intended to be run in external environments.  GUNNS is optimized for the [NASA Trick](https://github.com/nasa/trick) simulation environment, but can be run in other environments with some additional work.

# The Gunnsmiths

GUNNS is managed by the [Simulation & Robotics Division, Simulation & Graphics Branch](https://www.nasa.gov/centers/johnson/engineering/robotics_simulation/index.html) at NASA Johnson Space Center.

GUNNS is developed by [CACI International Inc.](https://www.caci.com) under contract to NASA.  Contact info for our team can be found in the wiki.

<p align=left>
<img src="https://raw.github.com/nasa/gunns/master/ER7_logo.png" alt="ER7 Logo" height=75px>
<img src="https://raw.github.com/nasa/gunns/master/CACI_International_logo.png" alt="CACI Logo" height=75px>
</p>

# Wiki

(TBD)

# License

(TBD)

# Notices

(TBD)

# Disclaimers

(TBD)

