[![Unit Tests (Valgrind & Coverage)](https://github.com/nasa/gunns/actions/workflows/unit_test.yml/badge.svg)](https://github.com/nasa/gunns/actions/workflows/unit_test.yml) [![Unit Tests (ASan)](https://github.com/nasa/gunns/actions/workflows/unit_test_asan.yml/badge.svg)](https://github.com/nasa/gunns/actions/workflows/unit_test_asan.yml) [![Test Trick Sim](https://github.com/nasa/gunns/actions/workflows/sim_test.yml/badge.svg)](https://github.com/nasa/gunns/actions/workflows/sim_test.yml) [![Trick Library Dependencies](https://github.com/nasa/gunns/actions/workflows/lib_deps.yml/badge.svg)](https://github.com/nasa/gunns/actions/workflows/lib_deps.yml)

# General-Use Nodal Network Solver (GUNNS)

GUNNS is a set of generic C++ math models that combines nodal circuit analysis algorithms with the hydraulic-thermal-electric analogy to model flow circuits (fluid, thermal, electrical).  The models include generic physical components like pumps, valves, resistors & capacitors.  These components, called 'links', are connected to 'nodes' and configured with their characteristic values, creating a 'network' (circuit) of links and nodes.  The state of the network and its contained models are then propagated in the time domain.

### GunnsDraw, the Network Design GUI

Networks can be drawn up with a [Draw.io](https://www.draw.io) drawing, from which run-time C++ code is auto-generated that can be built into simulations.  This is called 'GunnsDraw'.  GunnsDraw includes custom Draw.io shapes for the generic physical components and Python scripts to auto-generate the run-time C++ code.

### Extensible

GUNNS is designed to be extensible and flexible.  It is written with object-oriented polymorphic interfaces between the network solver, links, nodes, and other extensible object types.  This allows users to develop new classes to model things that the GUNNS baseline doesn't cover.  Users can create custom GunnsDraw shapes by taking advantage of Draw.io shapes' extensible XML schema.

GUNNS can also be extended to model other domains besides the above flow systems, as it can solve systems of the general form [A]{x} = {b} where [A] is a symmetric positive semi-definite matrix.  This is demonstrated in an included simple 6-DOF rigid body dynamics equations of motion model.

### Simulation Environment and Glue Code Not Included

GUNNS does not provide a complete simulation environment, as it is intended to be run in external environments.  GUNNS is optimized for the [NASA Trick](https://github.com/nasa/trick) simulation environment, but can be run in other environments with some additional work.

GUNNS is optimized for the 'thread safe sim bus', which is 'glue code' that integrates separate GUNNS networks together in a Trick sim.  However, this sim bus software not included in GUNNS and isn't released.  To get the most out of GUNNS, you either need to use the sim bus (as an internal NASA user) or replicate its functions on your own.  See information about what the [sim bus does in the Wiki.](https://github.com/nasa/gunns/wiki/Sim_Bus_Users_Guide_for_GUNNS)

# The Gunnsmiths

GUNNS is managed by the [Simulation & Robotics Division, Simulation & Graphics Branch](https://www.nasa.gov/software-robotics-and-simulation-division/simulation-and-graphics-branch/) at NASA Johnson Space Center.

GUNNS is developed by [CACI International Inc.](https://www.caci.com) under contract to NASA.  Contact info for our team can be found in the wiki.

<p align=left>
<img src="https://raw.github.com/nasa/gunns/master/ER7_logo.png" alt="ER7 Logo" height=75px>
<img src="https://raw.github.com/nasa/gunns/master/CACI_International_logo.png" alt="CACI Logo" height=75px>
</p>

# Wiki

See the [wiki](https://github.com/nasa/gunns/wiki) for more information including tutorials, user & developer guides.

# License

This software is released under the NASA Open Source Agreement Version 1.3 [license](https://github.com/nasa/gunns/blob/master/LICENSE).

# Notices

Copyright © 2024 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. No copyright is claimed in the United States under Title 17, U.S. Code. All Other Rights Reserved.

# Disclaimers

No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT SOFTWARE. FURTHER, GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE, IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."

Waiver and Indemnity: RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT. IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW. RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS AGREEMENT.
