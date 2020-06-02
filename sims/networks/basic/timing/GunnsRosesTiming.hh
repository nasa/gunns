/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (TODO)

 REQUIREMENTS:
 ()

 REFERENCE:
 ()

 ASSUMPTIONS AND LIMITATIONS:
 ()

 LIBRARY DEPENDENCY:
 ((GunnsRosesTiming.o))

 PROGRAMMERS:
 ((Jason Harvey) (CACI) (Jan. 2018) (Initial))
*/

#ifndef GunnsRosesTiming_EXISTS
#define GunnsRosesTiming_EXISTS

#include "TimingBasic.hh"

// This will sweep through even values of N and run GunnsRosesTiming network
// at each value of N, and report its average decomposition time at
// each N.  We will run GunnsRosesTiming with all the GPU mode options
// for comparison & analysis of where the user should configure their
// GPU matrix size cutoff.
//
// We will dynamically create & destroy GunnsRosesTiming each time, since
// it can only be initialized once for each N and GPU options.
//
// Trick scheduled job should have a time step of 1 sec.

class GunnsRosesTiming
{
    public:
        int          N_START;
        int          N_STOP;
        int          N_STEPS;
        bool         errCheck;
        bool         islands;
        GunnsRosesTiming();
        virtual ~GunnsRosesTiming();
        void initialize();
        void update(const double timeStep);

    protected:
        TimingBasic* network;
        int          N;
        int          step;
        Gunns::GpuMode gpuMode;
        double       cpuTime;
        double       gpuTime;
        double       gpuSparseTime;
        double       cpuTimePrev;
        double       gpuTimePrev;
        double       gpuSparseTimePrev;
        bool         gpuEnabled;
        double       gpuThreshold;
        double       gpuSparseThreshold;
        double       cpuPotentials[5];
        double       gpuPotentials[5];
        double       gpuSparsePotentials[5];
        void storePotentials();
        void predictGpuThreshold();
        void predictGpuSparseThreshold();
        void dumpA();
        void dumpIslands();

    private:
        GunnsRosesTiming(const GunnsRosesTiming&);
        GunnsRosesTiming& operator =(const GunnsRosesTiming&);
};

#endif /* GunnsRosesTiming_EXISTS */
