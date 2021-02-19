/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ( (core/Gunns.o)
   (core/GunnsBasicConductor.o)
   (core/GunnsBasicPotential.o)
   (core/GunnsBasicCapacitor.o)
 )
*/

#include "TimingBasic.hh"
#include <sstream>
#include <iostream>

TimingBasic::TimingBasic(std::string& name)
    :
    N(10),
    N_NODE(0),
    N_COND(0),
    N_CAP(0),
    GND(0),
    ENV(0),
    ENV2(0),
    mNumSteps(0),
    mSolveTimeTotal(0.0),
    mSolveTimeAvg(0.0),
    mStepTimeTotal(0.0),
    mStepTimeAvg(0.0),
    netSolver(),
    netSolverConfig(name + ".netSolver", 1.0, 1.0E-6, 1, 1),
    netNodes(0),
    netNodeList(N_NODE, netNodes),
    netLinks(),
    mCond(0),
    mCondConfig(name + ".mCond", &netNodeList, 1.0),
    mCondInput(false, 0.0),
    mEnvTemperature(300.0),
    mEnv(),
    mEnv2(),
    mEnvConfig(name + ".mEnv", &netNodeList, 1.0),
    mEnv2Config(name + ".mEnv2", &netNodeList, 1.0),
    mEnvInput(false, 0.0, mEnvTemperature),
    mCap(0),
    mCapConfig(name + ".mCap", &netNodeList),
    mCapInput(false, 0.0, 1.0, mEnvTemperature)
{
    // nothing to do
}

TimingBasic::~TimingBasic()
{
    TS_DELETE_ARRAY(mCap);
    TS_DELETE_ARRAY(mCond);
    TS_DELETE_ARRAY(netNodes);
}

void TimingBasic::initialize(std::string& name)
{
    N_NODE = N*N+3;
    N_COND = 2*N*(N-1)+4;
    N_CAP  = N*N;
    GND = N_NODE-1;
    ENV    = N_NODE-2;
    ENV2   = N_NODE-3;

    // allocate dynamic arrays of objects
    TS_NEW_CLASS_ARRAY_EXT(netNodes, N_NODE, GunnsBasicNode,        (), name + ".netNodes");
    TS_NEW_CLASS_ARRAY_EXT(mCond,    N_COND, GunnsBasicConductor,   (), name + ".mCond");
    TS_NEW_CLASS_ARRAY_EXT(mCap,     N_CAP,  GunnsBasicCapacitor,   (), name + ".mCap");

    // initialize the nodes
    netNodeList.mNumNodes = N_NODE;
    netNodeList.mNodes    = netNodes;
    for (int i=0; i<N_NODE; ++i) {
        std::ostringstream nodeName;
        nodeName << name << ".netNodes_" << i;
        netNodes[i].initialize(nodeName.str(), mEnvTemperature);
    }
    netSolver.initializeNodes(netNodeList);

    // initialize the links
    netLinks.clear();

    // environment
    mEnv .initialize(mEnvConfig,  mEnvInput, netLinks, GND, ENV);
    mEnv2.initialize(mEnv2Config, mEnvInput, netLinks, GND, ENV2);

    // "horizontal" conductors in each row: these are mCond[0] - [N*(N-1)-1]
    for (int row=0; row<N; ++row) {
        for (int col=0; col<N-1; ++col) {
            std::ostringstream linkName;
            linkName << name << ".mCond_" << row*(N-1)+col;
            mCondConfig.mName = linkName.str();
            mCond[row*(N-1)+col].initialize(mCondConfig, mCondInput, netLinks, (row*N+col), (row*N+col+1));
        }
        mCond[row*(N-1) + N/2 - 1].setDefaultConductivity(0.0);
    }

    // "vertical" conductors in each column: these are mCond[N*(N-1)] - [2*N(N-1)-1]
    const int start = N*(N-1);
    for (int col=0; col<N; ++col) {
        for (int row=0; row<N-1; ++row) {
            std::ostringstream linkName;
            linkName << name << ".mCond_" << start+col*(N-1)+row;
            mCondConfig.mName = linkName.str();
            mCond[start+col*(N-1)+row].initialize(mCondConfig, mCondInput, netLinks, row*N+col, (row+1)*N+col);
        }
    }

    // source & leak conductors
    {
        // conductor from bottom-left grid X to GND node
        std::ostringstream linkName;
        linkName << name << ".mCond_" << N_COND-4;
        mCondConfig.mName = linkName.str();
        mCond[N_COND-4].initialize(mCondConfig, mCondInput, netLinks, N*(N-1), GND);
    } {
        // conductor from top-right grid X to ENV2 node
        std::ostringstream linkName;
        linkName << name << ".mCond_" << N_COND-3;
        mCondConfig.mName = linkName.str();
        mCond[N_COND-3].initialize(mCondConfig, mCondInput, netLinks, N-1, ENV2);
    } {
        // conductor from top-left grid X to ENV node
        std::ostringstream linkName;
        linkName << name << ".mCond_" << N_COND-2;
        mCondConfig.mName = linkName.str();
        mCond[N_COND-2].initialize(mCondConfig, mCondInput, netLinks, 0, ENV);
    } {
        // conductor from bottom-right grid X to GND node
        std::ostringstream linkName;
        linkName << name << ".mCond_" << N_COND-1;
        mCondConfig.mName = linkName.str();
        mCond[N_COND-1].initialize(mCondConfig, mCondInput, netLinks, N*N-1, GND);
    }

    // capacitors
    for (int row=0; row<N; ++row) {
        for (int col=0; col<N; ++col) {
            {
                std::ostringstream linkName;
                linkName << name << ".mCap_" << row*N+col;
                mCapConfig.mName = linkName.str();
            }
            mCap[row*N+col].initialize(mCapConfig, mCapInput, netLinks, row*N+col, GND);
        }
    }

    // initialize the solver
    netSolver.initialize(netSolverConfig, netLinks);
    netSolver.setWorstCaseTiming(true);
}

void TimingBasic::update(const double timestep)
{
    // change boundary condition to force a matrix decomposition every major step.
    //TODO this doesn't work!  Because it's not changing any conductances.  If the link from ENV
    //  node were a thermal radiator this would work since the conductance varies with the delta-
    //  potential... or just dither the conductance itslef.
    // another option for this sim is to just set the sovler's mWorstCaseTiming flag, and not have
    //  to mess with this link.
    mEnvTemperature += timestep;
    mEnv.setSourcePotential(mEnvTemperature);
    netSolver.step(timestep);
    mNumSteps++;
    mSolveTimeTotal += netSolver.getSolveTime();
    mSolveTimeAvg    = mSolveTimeTotal / mNumSteps;
    mStepTimeTotal  += netSolver.getStepTime();
    mStepTimeAvg     = mStepTimeTotal / mNumSteps;
}

void TimingBasic::shutdown()
{
    std::cout << "\nTimingBasic: " << mSolveTimeAvg << std::endl;
}
