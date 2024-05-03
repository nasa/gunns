#ifndef TsMemoryManager_EXISTS
#define TsMemoryManager_EXISTS
/**
@defgroup  TSM_UTILITIES_SOFTWARE_SIM_COMPATIBILITY_MEMORY_MANAGER  TS21 Memory Manager
@ingroup   TSM_UTILITIES_SOFTWARE_SIM_COMPATIBILITY

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
- (TS21 Memory Manger class)

ASSUMPTIONS AND LIMITATIONS:
- (This file should only be included by TsSimCompatibility.hh and TsMemoryManager.cpp)

LIBRARY DEPENDENCY:
- ((TsMemoryManager.o))


PROGRAMMERS:
- ( ((Danny Strauss) (L-3) (Jun 2011))
    ((Danny Strauss) (L-3) (Jul 2014) (Added Trick naming of allocations, and _EXT versions)) )

@{
*/
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    This class provides an interface to Trick memory management (to be called via macros).
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsMemoryManager {

    public:
        /// @brief    Allocates memory.
        static char* tsAlloc(const int nelem, const int size, void* addr_var = 0, const std::string type="", const std::string name="");
        /// @brief    Declares already allocated memory to Trick.
        static char* tsAllocExt(const int nelem, void* var = 0, void* addr_var = 0, const std::string type="", const std::string name="");
        /// @brief    Copies a string.
        static char* tsStrdup(char* str, void* addr_var = 0, const std::string name="");
        /// @brief    Declares already allocated memory to Trick and copies the specified string into that memory.
        static char* tsStrdupExt(char* str, void* var = 0, void* addr_var = 0, const std::string name="");
        /// @brief    Returns true if the specified pointer was allocated by TS Memory Manager.
        static bool  tsIsAlloced(char* ptr);
        /// @brief    Deallocates memory.
        static void  tsDelete(void* ptr, const char* name);
        /// @brief    Default destructor.
        ~TsMemoryManager() {};

    private:
        /// @brief    Create a valid type/name specification for Trick allocation
        static const char* tsAllocSpec(const std::string type, const std::string name, void* var);

        /// @brief    Default constructor unavailable since declared private and not implemented.
        TsMemoryManager();
        /// @brief    Copy constructor unavailable since declared private and not implemented.
        TsMemoryManager(const TsMemoryManager&);
        /// @brief    Assignment operator unavailable since declared private and not implemented.
        TsMemoryManager& operator=(const TsMemoryManager&);
};

/// @}

#endif
