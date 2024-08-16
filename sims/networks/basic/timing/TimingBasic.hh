/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (This is a GUNNS basic network class with a configurable number of nodes, for benchmarking timing.)

 REQUIREMENTS:
 ()

 REFERENCE:
 ()

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
 ( (TimingBasic.o)
 )

 PROGRAMMERS:
 ((Jason Harvey) (L-3) (Oct. 2015) (Initial))
*/

#ifndef TimingBasic_EXISTS
#define TimingBasic_EXISTS

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/Gunns.hh"
#include "core/GunnsBasicConductor.hh"
#include "core/GunnsBasicPotential.hh"
#include "core/GunnsBasicCapacitor.hh"
#include <string>

/*      Example of what a 4x4 network looks like
 *              ENV             ENV2
 *                \             /
 *                 X---X- -X---X        (Row 0)
 *                 |   |   |   |
 *                 |   |   |   |
 *                 |   |   |   |
 *                 X---X- -X---X        (Row 1)
 *                 |   |   |   |
 *                 |   |   |   |
 *                 |   |   |   |
 *                 X---X- -X---X        (Row 2)
 *                 |   |   |   |
 *                 |   |   |   |
 *                 |   |   |   |
 *                 X---X- -X---X        (Row 3)
 *                /             \
 *              GND             GND
 *
 *      Where X is a capacitive node, and O is an "environment temperature" node.
 *      Capacitive nodes are connected together by conductor links in an NxN grid.
 *      The environment temperature node is controlled by a potential link to ground.
 *      There is another "leak" conductor to Ground on the other side of the grid opposite the
 *      environment node.
 *
 *      Vertically down the middle, the conductors connecting the left & right halves of the grid
 *      are by default given zero conductance, splitting the left & right halves into islands.
 *
 *      Nodes are numbered 0 through (N*N). The zero index node is the upper-left X in the grid,
 *      attached to the 0 node.  The (N*N)-1 node is the bottom right of the grid attached to GND.
 *      The N*N node is the 0 node.
 */

//TODO support options to split the node grid in half for analyzing islands speeds.  Will need
//     multiple sources & sinks...

class TimingBasic
{
    TS_MAKE_SIM_COMPATIBLE( TimingBasic);

    public:
        int                             N;    // (--) MUST BE >0 AND EVEN NUMBER, set via input file
        double                          mSolveTimeAvg;
        double                          mStepTimeAvg;
        Gunns                           netSolver;
        int                             N_NODE; /**< (--) Number of network nodes, including ground. */
        int                             N_COND; /**< (--) Number of conductor links. */
        int                             N_CAP;  /**< (--) Number of capacitor links. */
        int                             GND;    /**< (--) Node # of the Ground node. */
        int                             ENV;    /**< (--) Node # of the top-left environment node. */
        int                             ENV2;   /**< (--) Node # of the top-right environment node. */
        int                             mNumSteps;
        double                          mSolveTimeTotal;
        double                          mStepTimeTotal;
        GunnsConfigData                 netSolverConfig;
        GunnsBasicNode*                 netNodes;
        GunnsNodeList                   netNodeList;
        std::vector<GunnsBasicLink*>    netLinks;
        GunnsBasicConductor*            mCond;
        GunnsBasicConductorConfigData   mCondConfig;
        GunnsBasicConductorInputData    mCondInput;
        double                          mEnvTemperature;
        GunnsBasicPotential             mEnv;
        GunnsBasicPotential             mEnv2;
        GunnsBasicPotentialConfigData   mEnvConfig;
        GunnsBasicPotentialConfigData   mEnv2Config;
        GunnsBasicPotentialInputData    mEnvInput;
        GunnsBasicCapacitor*            mCap;
        GunnsBasicCapacitorConfigData   mCapConfig;
        GunnsBasicCapacitorInputData    mCapInput;
        TimingBasic(std::string& name);
        virtual ~TimingBasic();
        void initialize(std::string& name);
        void update(const double timeStep);
        void shutdown();

    private:
        TimingBasic(const TimingBasic& rhs);
        TimingBasic& operator =(const TimingBasic& rhs);
};

#endif /* TimingBasic_EXISTS */
