#ifndef GunnsMutexLock_EXISTS
#define GunnsMutexLock_EXISTS

/**
@file
@brief     GUNNS Mutex Lock Utility Class

@defgroup  TSM_GUNNS_CORE_MUTEX_LOCK    GUNNS Mutex Lock Utility Class
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides a class for safely locking and unlocking a mutex avoiding double-locks, double-unlocks.
            Also provides an exception safe method for ensuring a mutex is unlocked if an exception is encountered
            because destructors are guaranteed to be called even in an exception case.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ()

PROGRAMMERS:
- ((Thomas Brain ) (METECS) (2026-02) (Initial))

@{
*/
#include <pthread.h>

class GunnsMutexLock
{
public:
    explicit GunnsMutexLock(pthread_mutex_t & mutexIn, bool initLocked = false) :
        myMutex(mutexIn), isLocked(false)
    {        
        if(initLocked)
        {
            lock();
        }
    }
    explicit GunnsMutexLock(const pthread_mutex_t & mutexIn, bool initLocked = false) :
        myMutex(const_cast<pthread_mutex_t &>(mutexIn)), isLocked(false)
    {        
        if(initLocked)
        {
            lock();
        }
    }
    virtual ~GunnsMutexLock() 
    {
        unlock();
    }

    void lock()
    {
        pthread_mutex_lock(&myMutex);
        isLocked = true;
    }
    
    void unlock() 
    {
        if(isLocked)
        {
            pthread_mutex_unlock(&myMutex);
            isLocked = false;
        }
    }

    pthread_mutex_t & myMutex; // ** no io_src process

protected:
    bool isLocked;
};

#endif