/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (TS21 Memory Manger class.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

 LIBRARY DEPENDENCY:
     ()

 PROGRAMMERS:
  ( ((Danny Strauss) (L-3) (Jun 2011))
    ((Danny Strauss) (L-3) (Jul 2014) (Added Trick naming of allocations, and _EXT versions)) )
***************************************************************************************************/

#include <stdexcept>
#include <algorithm> // for std::replace
#include <execinfo.h>  // for backtrace
#include <pthread.h> 
#include <cstring>

#include "sim_services/Executive/include/exec_proto.h" // for exec_get_current_version
#include "sim_services/MemoryManager/include/MemoryManager.hh" // for ref_name_from_address
#include "sim_services/MemoryManager/include/memorymanager_c_intf.h"
#include "sim_services/Message/include/message_proto.h" // for message_publish

#include "TsMemoryManager.hh"

/// @brief    Protect multiple threads from accessing tsAllocSpec naming code at same time
static pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    sName   --   Name for the allocation itself
/// @param    sFrom   --   Substring to replace
/// @param    sTo     --   New substring that replaces "sFrom"
///
/// @return           --   N/A
///
/// @details  Utility function to replace invalid characters "from" to "to" in the given variable name
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string tsNameFix(const std::string sName, const std::string sFrom, const std::string sTo)
{
    std::string sNew = sName;
    //std::string sFrom = from;
    //std::string sTo = to;
    if (! sNew.empty())
    {
        std::string::size_type toLen = sTo.length();
        std::string::size_type frLen = sFrom.length();
        std::string::size_type loc = 0;
        while (std::string::npos != (loc = sNew.find(sFrom, loc)))
        {
            sNew.replace(loc, frLen, sTo);
            loc += toLen;
            if (loc >= sNew.length())
                break;
            }
        }

    return sNew;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    type    --   Name of the type to be allocated
/// @param    name    --   Name for the allocation itself (optional)
/// @param    var     --   Pointer to the variable to be allocated.
///
/// @return           --   N/A
///
/// @details  Return a string containing type/name specification (e.g., "MyClass foo")
////////////////////////////////////////////////////////////////////////////////////////////////////
const char* TsMemoryManager::tsAllocSpec(const std::string type, const std::string name, void* var)
{
    /*********************** DEBUG ***************************/
//    void *backtrace_buffer[100];
//    char **backtrace_strings;
//    static std::map<std::string, int> backtrace_map;
    /******************* END DEBUG ***************************/

    static std::string tsAllocName;
    pthread_mutex_lock(&alloc_mutex);
    std::string tsName = name; // user may pass in a name to use
    tsAllocName = type;  // type is the default alloc name
    if (name.empty()) {
#if ((TRICK_VER > 13) || (TRICK_VER == 13 && TRICK_MINOR >= 4))
        // if user did not pass in a name, create name using Trick's ref_name_from_address()
        // but ref_name_from_address() is not valid if called from a constructor
        // so we need to at least be in Trick Initialization mode.
        if (exec_get_mode() >= Initialization) {
            extern Trick::MemoryManager * trick_MM;
            tsName = trick_MM->ref_name_from_address(var);
//fprintf(stderr, "tsAllocSpec ref_name_from_address= <%s>\n", tsName.c_str()); //DEBUG
            if ((tsName.compare(0, 7, "ERROR -")==0) || (tsName.length()==1)) {
            // if ref_name_from_address returned ERROR msg, it is invalid so clear it
            // TODO: bug? where ref_name_from_address only returns an ampersand in constructor
/*********************** DEBUG ***************************/
//#if 0
//// print the function name that called tsAlloc() via the builtin backtrace function
//int nptrs = backtrace(backtrace_buffer, 100);
//backtrace_strings = backtrace_symbols(backtrace_buffer, 100);
//if (backtrace_strings==NULL) {
//    perror("backtrace_symbols");
//    exit(1);
//}
//fprintf(stderr, "%s\n", backtrace_strings[2]);
//std::pair<std::map<std::string,int>::iterator,bool> ret;
//// just keep track of the function name that called tsAlloc(), it's the 3rd one in backtrace
//ret = backtrace_map.insert ( std::pair<std::string,int>(backtrace_strings[2],1) );
//if (ret.second==false) {
//    ret.first->second++; // it already exists, increment its count
//} else {
//    fprintf(stderr, "TS_NEW returned error: ");
//    fprintf(stderr, "%s\n", ret.first->first.c_str());
//}
//#endif
/******************* END DEBUG ***************************/
                tsName.clear();
            } else {
                // see if we have a linked list next pointer
                int findit = tsName.rfind("[0].");
                if (findit != -1) {
                    std::string baseName = tsName.substr(0,findit);
                    baseName = tsNameFix(baseName, "&", "");
                    REF2 *R2 = ref_attributes((char*)baseName.c_str());
                    if (R2!=0) {
                        //fprintf(stderr, "_______pointer ref= %s\n", R2->reference); //DEBUG
                        ALLOC_INFO* ai = get_alloc_info_of(R2->address);
                        if (ai!=0) {
                            //fprintf(stderr, "       alloc type= %s", ai->user_type_name); //DEBUG
                            if (type.compare(ai->user_type_name)==0) { // pointing to same type: it's a linked list
                                findit = baseName.rfind("_LIST");
                                if (findit == -1) {
                                // 1st item in linked list, use the full name + "_LIST1"
                                    tsName = tsName + "_LIST1";
                                    //fprintf(stderr, "       LINKED LIST 1\n"); //DEBUG
                                } else {
                                // other items in linked list, use the prev full name + "_LIST<prev number+1>"
                                    int prevnum = atoi(baseName.substr(findit+5).c_str());
                                    char numstr[21];
                                    sprintf(numstr, "%d", prevnum+1);
                                    tsName = baseName.substr(0,findit) + "_LIST" + numstr;
                                    //fprintf(stderr, "       LINKED LIST %s\n", numstr); //DEBUG
                                }
                            } // end its a linked list
                            else {
                                //fprintf(stderr, "\n"); //DEBUG
                            }
                        } // end found alloc info
                    } // end found ref_attributes
                } // end found "[0]."
            } // end else not ERROR
        } // end not in a constructor
          else {
              //message_publish(MSG_WARNING, "TsMemoryManager WARNING: TS_NEW macro automatic naming of allocation not possible "
                                           //"in a constructor. Allocation type = %s\n", type.c_str());
/*********************** DEBUG ***************************/
//#if 0
//// print the function name that called tsAlloc() via the builtin backtrace function
//int nptrs = backtrace(backtrace_buffer, 100);
//backtrace_strings = backtrace_symbols(backtrace_buffer, 100);
//if (backtrace_strings==NULL) {
//    perror("backtrace_symbols");
//    exit(1);
//}
//std::pair<std::map<std::string,int>::iterator,bool> ret;
//// just keep track of the function name that called tsAlloc()
//ret = backtrace_map.insert ( std::pair<std::string,int>(backtrace_strings[2],1) );
//if (ret.second==false) {
//    ret.first->second++; // it already exists, increment its count
//} else {
//    fprintf(stderr, "TS_NEW in constructor: ");
//    fprintf(stderr, "%s\n", ret.first->first.c_str());
//}
//#endif
/******************* END DEBUG ***************************/
        }

#endif
    }
    if (!tsName.empty()) {
        // create type/name spec but first replace invalid characters in the name
        tsName = tsNameFix(tsName, "&", "");
        tsName = tsNameFix(tsName, ".", "__");
        tsName = tsNameFix(tsName, " ", "");
        tsName = tsNameFix(tsName, "+", "__");
        tsName = tsNameFix(tsName, "[", "_");
        tsName = tsNameFix(tsName, "]", "_");
        tsAllocName = tsAllocName + " " + tsName;
//fprintf(stderr, "tsAllocSpec fixed name           = <%s>\n", tsAllocName.c_str()); //DEBUG
    }

    pthread_mutex_unlock(&alloc_mutex);
    return tsAllocName.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    nelem   --   Number of array elements (> 0) to be allocated.
/// @param    size    --   Size in bytes (> 0) of type to be allocated.
/// @param    addr_var--   Pointer to the variable to be allocated.
/// @param    type    --   Name of the type to be allocated.
/// @param    name    --   Name for the allocation itself (optional).
///
/// @return           --   Pointer to the allocated object.
///
/// @details  Allocates memory.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
char* TsMemoryManager::tsAlloc(const int nelem, const int size, void* addr_var, const std::string type, const std::string name)
{
    return (char*)alloc_type(nelem, tsAllocSpec(type, name, addr_var));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    nelem   --   Number of array elements (> 0) to be allocated.
/// @param    var     --   The pointer variable that was allocated.
/// @param    addr_var--   The address of pointer variable that was allocated.
/// @param    type    --   Name of the type to be allocated.
/// @param    name    --   Name for the allocation itself (optional).
///
/// @return           --   Pointer to the allocated object.
///
/// @details  Declares already allocated memory to Trick.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
char* TsMemoryManager::tsAllocExt(const int nelem, void* var, void* addr_var, const std::string type, const std::string name)
{
    return (char*)TMM_declare_ext_var_1d(var, tsAllocSpec(type, name, addr_var), nelem);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    str     --   Pointer to the string to be copied.
/// @param    addr_var--   Pointer to the variable to be allocated.
/// @param    name    --   Name for the allocation itself (optional).
///
/// @return           --   Pointer to the copied string.
///
/// @details  Makes a copy of the specified string.
////////////////////////////////////////////////////////////////////////////////////////////////////
char* TsMemoryManager::tsStrdup(char* str, void* addr_var, const std::string name)
{
   //return TMM_strdup(str) ; // Trick's call has no option to name the allocation
   char * allocation;
   int extent;

   extent = (int)strlen(str)+1;
   allocation = (char*)alloc_type(extent, tsAllocSpec("char", name, addr_var));
   return (strcpy(allocation, str));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    str     --   Pointer to the string to be copied.
/// @param    var     --   The pointer variable that was allocated.
/// @param    addr_var--   The address of pointer variable that was allocated.
/// @param    name    --   Name for the allocation itself (optional).
///
/// @return           --   Pointer to the copied string.
///
/// @details  Declares already allocated memory to Trick and copies the specified string into that memory.
////////////////////////////////////////////////////////////////////////////////////////////////////
char* TsMemoryManager::tsStrdupExt(char* str, void* var, void* addr_var, const std::string name)
{
   char * allocation;
   int extent;

   extent = (int)strlen(str)+1;
   allocation = (char*)TMM_declare_ext_var_1d(var, tsAllocSpec("char", name, addr_var), extent);
   return (strcpy(allocation, str));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    ptr     --   Pointer to the object to be checked.
///
/// @return           --   True if this object was allocated by Ts Memory Manager..
///
/// @details  Tests if this pointer was allocated by TS Memory Manager.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsMemoryManager::tsIsAlloced(char* ptr)
{
    return TMM_is_alloced(ptr) ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    ptr     --   Pointer to the object to be deallocated.
/// @param    name    --   Name of type to be deallocated.
///
/// @return   void
///
/// @details  If the specified pointer is to allocated memory, it is deallocated.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsMemoryManager::tsDelete(void* ptr, const char* name)
{
    if (TMM_is_alloced(reinterpret_cast<char*>(ptr))) {
        TMM_delete_var_a(ptr);
    }
}
