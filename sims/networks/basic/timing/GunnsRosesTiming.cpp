/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((TimingBasic.o))
*/

#include "GunnsRosesTiming.hh"
#include <cstdio>
#include <sstream>
#include <string>
#include <cmath>

// Trick includes
#include "sim_services/Executive/include/exec_proto.h" // for exec_terminate

GunnsRosesTiming::GunnsRosesTiming()
    :
    N_START(2),
    N_STOP(40),
    N_STEPS(5),
    errCheck(false),
    islands(false),
    network(0),
    N(0),
    step(0),
    gpuMode(Gunns::NO_GPU),
    cpuTime(0.0),
    gpuTime(0.0),
    gpuSparseTime(0.0),
    cpuTimePrev(0.0),
    gpuTimePrev(0.0),
    gpuSparseTimePrev(0.0),
    gpuThreshold(0.0),
    gpuSparseThreshold(0.0),
    gpuEnabled(false),
    cpuPotentials(),
    gpuPotentials(),
    gpuSparsePotentials()
{
    for (unsigned int i=0; i<5; ++i) {
        cpuPotentials[i]       = 0.0;
        gpuPotentials[i]       = 0.0;
        gpuSparsePotentials[i] = 0.0;
    }
}

GunnsRosesTiming::~GunnsRosesTiming()
{
    if (network) delete network;
}

void GunnsRosesTiming::initialize()
{
    N = N_START;
    Gunns dummySolver;
    gpuEnabled = dummySolver.isGpuEnabled();
    printf("\n\n");
    printf("GUNNS & ROSES Timing Benchmark Results:\n\n");

    printf("GPU mode:   NO_GPU  ");
    if (gpuEnabled) {
        printf("    GPU_DENSE   GPU_SPARSE\n");
    } else {
        printf("    (GPU disabled)\n");
    }

    printf("# nodes     time (s)");
    if (gpuEnabled) {
        printf("     time (s)     time (s)\n");
    } else {
        printf("\n");
    }
    printf("----------------------------------------------\n");
}

void GunnsRosesTiming::update(const double timestep)
{
    if (N <= N_STOP) {
        if (0 == step) {
            if (network) delete network;
            std::ostringstream name;
            name << "rosesBenchmark.network_" << N;
            if (Gunns::GPU_SPARSE == gpuMode) {
                name << "_gpu_sparse";
            } else if (Gunns::GPU_DENSE == gpuMode) {
                name << "_gpu";
            } else if (Gunns::NO_GPU == gpuMode) {
                name << "_cpu";
            }
            std::string nameString = name.str();
            network = new TimingBasic(nameString);
            network->N = N;
            network->initialize(nameString);
            network->netSolver.setGpuOptions(gpuMode, 1);
            if (islands) network->netSolver.setIslandMode(Gunns::SOLVE);
        }
        network->update(timestep);
        step++;
        if (N_STEPS <= step) {
            storePotentials();
//            dumpA();
//            dumpIslands();
            if (Gunns::NO_GPU == gpuMode) {
                cpuTimePrev = cpuTime;
                cpuTime = network->mSolveTimeAvg;
                printf("%7d    %9.2e", N*N+1, network->mSolveTimeAvg);
                step = 0;
                if (gpuEnabled) {
                    gpuMode = Gunns::GPU_DENSE;
                } else {
                    printf("\n");
                    step = 0;
                    N += 2;
                }
            } else if (Gunns::GPU_DENSE == gpuMode) {
                gpuTimePrev = gpuTime;
                gpuTime = network->mSolveTimeAvg;
                printf("    %9.2e", network->mSolveTimeAvg);
                if (errCheck) {
                    double error = 0.0;
                    for (unsigned int i=0; i<5; ++i) {
                        error += fabs(gpuPotentials[i] - cpuPotentials[i]);
                    }
                    printf("  %9.2e", error);
                }
                step = 0;
                predictGpuThreshold();
                gpuMode = Gunns::GPU_SPARSE;
            } else if (Gunns::GPU_SPARSE == gpuMode) {
                gpuSparseTimePrev = gpuSparseTime;
                gpuSparseTime = network->mSolveTimeAvg;
                printf("    %9.2e", network->mSolveTimeAvg);
                if (errCheck) {
                    double error = 0.0;
                    for (unsigned int i=0; i<5; ++i) {
                        error += fabs(gpuSparsePotentials[i] - cpuPotentials[i]);
                    }
                    printf("  %9.2e", error);
                }
                printf("\n");
                step = 0;
                predictGpuSparseThreshold();
                gpuMode = Gunns::NO_GPU;
                N += 2;
            }
        }
    } else {
        printf("----------------------------------------------\n");
        if (gpuEnabled) {
            printf("GPU # nodes\n");
            printf("threshold estimates:      %7.0f      %7.0f\n", gpuThreshold, gpuSparseThreshold);
            printf("\n");
            printf("Your network's times and\n");
            printf("best thresholds may vary.\n");
            printf("----------------------------------------------\n");
        }
        printf("\nTerminating sim:\n");
        exec_terminate("", "");  // from Trick's exec_proto.h, tell Trick to kill the sim
    }
}

void GunnsRosesTiming::storePotentials()
{
    // [0] is network node 0, the source potential node.
    // [1] is the top left corner of the grid.
    // [2] is the top right corner of the grid.
    // [3] is the bottom left corner of the grid.
    // [4] is the bottom right corner (sink node) of the grid.
    int index[5] = {N*N, 0, N-1, N*(N-1), N*N-1};
    for (unsigned int i=0; i<5; ++i) {
        if (Gunns::GPU_DENSE == gpuMode) {
            gpuPotentials[i] = network->netSolver.getPotentialVector()[index[i]];
        } else if (Gunns::GPU_SPARSE == gpuMode) {
            gpuSparsePotentials[i] = network->netSolver.getPotentialVector()[index[i]];
        } else if (Gunns::NO_GPU == gpuMode){
            cpuPotentials[i] = network->netSolver.getPotentialVector()[index[i]];
        }
    }
}

void GunnsRosesTiming::predictGpuThreshold()
{
    if ((gpuTime < cpuTime) and (gpuTimePrev > cpuTimePrev)) {
        const double frac = (1.0 - cpuTimePrev/gpuTimePrev) / (cpuTime/gpuTime - cpuTimePrev/gpuTimePrev);
        const int nnPrev = (N-2)*(N-2)+1;
        const int nn     = N*N+1;
        gpuThreshold = frac * (nn - nnPrev) + nnPrev;
    }
}

void GunnsRosesTiming::predictGpuSparseThreshold()
{
    if ((gpuSparseTime < cpuTime) and (gpuSparseTimePrev > cpuTimePrev)) {
        const double frac = (1.0 - cpuTimePrev/gpuSparseTimePrev) / (cpuTime/gpuSparseTime - cpuTimePrev/gpuSparseTimePrev);
        const int nnPrev = (N-2)*(N-2)+1;
        const int nn     = N*N+1;
        gpuSparseThreshold = frac * (nn - nnPrev) + nnPrev;
    }
}

void GunnsRosesTiming::dumpA()
{
    printf("\n[A] = \n");
    for (int i=0; i<N*N+1; ++i) {
        for (int j=0; j<N*N+1; ++j) {
            printf(" %9.2e", network->netSolver.getAdmittanceMatrix()[i*(N*N+1)+j]);
        }
        printf("\n");
    }
}

void GunnsRosesTiming::dumpIslands()
{
    // Print the island sizes of the 2 islands (if in islands mode).
    // nodes[0] and [N*N-1] should be in different islands, and each should have exactly
    // N*N/2 + 2 nodes.
    const std::vector<int>* isle1 = network->netNodes[0].getIslandVector();
    const std::vector<int>* isle2 = network->netNodes[N*N-1].getIslandVector();
    if (islands and isle1 and isle2) {
        printf(" %d %d %d %d ", isle1->size(), isle2->size(), (*isle1)[0], (*isle2)[0]);
    }
}
