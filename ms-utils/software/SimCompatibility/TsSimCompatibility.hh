#ifndef TsSimCompatibility_EXISTS
#define TsSimCompatibility_EXISTS
/**
@defgroup  TSM_UTILITIES_SOFTWARE_SIM_COMPATIBILITY_MACROS  Sim Compatibility Macros
@ingroup   TSM_UTILITIES_SOFTWARE_SIM_COMPATIBILITY

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@brief
PURPOSE:
 -(Trick sim compatibility and memory interface macros)

PROGRAMMERS:
 -(
   ((David Hammen) (Odyssey Space Research) (Feb 2009) (Initial version))
   ((Danny Strauss) (L-3) (Jun 2011))
   ((Danny Strauss) (L-3) (Jul 2014) (Added Trick naming of allocations, and _EXT versions))
  )

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gives the Trick input processor and Trick logging capabilities full visibility of the
///           class in a Trick environment.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
class SimBus;
class MalfHookSimObj;
class TestHookSimObj;

#ifndef no_TRICK_ENV
#define TS_MAKE_SIM_COMPATIBLE(className) \
    friend class ::SimBus; \
    friend class InputProcessor; \
    friend class ::MalfHookSimObj; \
    friend class ::TestHookSimObj; \
    friend void init_attr##className()

#elif (defined SIM_COMPATIBILITY_MACRO_FILE)
#include SIM_COMPATIBILITY_MACRO_FILE

#elif defined(SIM_FRIEND_CLASS)
#define TS_MAKE_SIM_COMPATIBLE(className) \
    friend class SIM_FRIEND_CLASS
#else // non-Trick
#define TS_MAKE_SIM_COMPATIBLE(className)
#endif

// includes and declarations needed for memory management macros
#ifndef no_TRICK_ENV
#include <typeinfo>
#include "TsMemoryManager.hh"
#else // non-Trick
#include <cstring>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Allocates @b one instance of the specified primitive @a type and initializes it to
///           @a initial.
///
/// @param    var     The pointer to the allocated primitive.
/// @param    type    The underlying type, which must be a C++ primitive type.
/// @param    initial The initial value, enclosed in parentheses.
/// @param    name    (Optional) The sim-unique name to be assigned to the allocation, a std::string
///
/// @par      Example:
/// @code
///           double* foo;
///           TS_NEW_PRIM_OBJECT(foo, double, (3.14159265358979323846));
/// @endcode
///           This allocates a double and initializes it to PI using TS_NEW_PRIM_OBJECT.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_NEW_PRIM_OBJECT(var, type, initial, name...) \
    var = new (TsMemoryManager::tsAlloc(1, sizeof(type), &var, #type, ##name)) type initial
#define TS_NEW_PRIM_OBJECT_EXT(var, type, initial, name...) \
{ \
    if (var==NULL) var = new type initial; \
    TsMemoryManager::tsAllocExt(1, var, &var, #type, ##name); \
}
#else  // non-Trick
#define TS_NEW_PRIM_OBJECT(var, type, initial, name...) \
    var = new type initial
#define TS_NEW_PRIM_OBJECT_EXT(var, type, initial, name...) \
    if (var==NULL) var = new type initial
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Allocates @b one instance of the specified @a type. The supplied constructor
///           arguments, @a initial, are used as arguments to new. The default constructor will be
///           invoked if the @a initial argument is the empty list, otherwise a non-default
///           constructor will be invoked.
///
/// @param    var     The pointer to the allocated object.
/// @param    type    The underlying type, which must be a structured type.
/// @param    initial The constructor arguments, enclosed in parentheses.
/// @param    name    (Optional) The sim-unique name to be assigned to the allocation, a std::string
///
/// @par      Example:
/// @code
///           Foo* foo;
///           TS_NEW_CLASS_OBJECT(foo, Foo, (bar,baz));
/// @endcode
///           This allocates a new object of type Foo, invoking the Foo::Foo(bar, baz) constructor
///           using TS_NEW_CLASS_OBJECT.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_NEW_CLASS_OBJECT TS_NEW_PRIM_OBJECT
#define TS_NEW_CLASS_OBJECT_EXT TS_NEW_PRIM_OBJECT_EXT

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Allocates an array of @a nelem elements of the specified primitive @a type and assigns
///           it to @a var.
///
/// @param    var     The pointer to the allocated array of primitives.
/// @param    nelem   The size of the array.
/// @param    type    The underlying type, which must be a a C++ primitive type.
/// @param    name    (Optional) The sim-unique name to be assigned to the allocation, a std::string
///
/// @par      Example:
/// @code
///           double* foo; // double[2]
///           TS_NEW_PRIM_ARRAY(foo, 2, double);
///           foo[0] = 3.0;
///           foo[1] = 3.0;
/// @endcode
///           This allocates an array of two doubles using TS_NEW_PRIM_ARRAY and initializes them
///           to 3.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_NEW_PRIM_ARRAY(var, nelem, type, name...) \
    var = new (TsMemoryManager::tsAlloc(nelem, sizeof(type), &var, #type, ##name)) type[nelem]
#define TS_NEW_PRIM_ARRAY_EXT(var, nelem, type, name...) \
{ \
    if (var==NULL) var = new type[nelem]; \
    TsMemoryManager::tsAllocExt(nelem, var, &var, #type, ##name); \
}
#else  // non-Trick
#define TS_NEW_PRIM_ARRAY(var, nelem, type, name...) \
    var = new type[nelem]
#define TS_NEW_PRIM_ARRAY_EXT(var, nelem, type, name...) \
    if (var==NULL) var = new type[nelem]
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Allocates an array of @a nelem instances of the specified @a type and assigns it to
///           @a var. Each object is constructed with @a initial arguments ("()" for default
///           construction).
///
/// @param    var     The pointer to the allocated array of objects.
/// @param    nelem   The size of the array.
/// @param    type    The underlying type, which must be a structured type.
/// @param    initial The constructor arguments, enclosed in parentheses.
/// @param    name    (Optional) The sim-unique name to be assigned to the allocation, a std::string
///
/// @par      Example:
/// @code
///           Foo* foo; // Foo[2]
///           TS_NEW_CLASS_ARRAY(foo, 2, Foo, (bar,baz));
/// @endcode
///           This allocates an array of two objects of type Foo using TS_NEW_CLASS_ARRAY and then
///           constructs each array object by invoking Foo::Foo(bar, baz).
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_NEW_CLASS_ARRAY(var,nelem,type,initial, name...) \
   { \
       int ii ; \
       var = new (TsMemoryManager::tsAlloc(nelem, sizeof(type), &var, #type, ##name)) type[nelem] ; \
       for (ii=0; ii<nelem; ii++) { \
           new (&var[ii]) type initial ; \
       } \
   }
#define TS_NEW_CLASS_ARRAY_EXT(var,nelem,type,initial, name...) \
{ \
    if (var==NULL) { \
        int ii ; \
        var = new type[nelem] ; \
        for (ii=0; ii<nelem; ii++) { \
            new (&var[ii]) type initial ; \
        } \
    } \
    TsMemoryManager::tsAllocExt(nelem, var, &var, #type, ##name); \
}
#else  // non-Trick
#define TS_NEW_CLASS_ARRAY(var,nelem,type,initial, name...) \
   { \
       int ii ; \
       var = new type[nelem] ; \
       for (ii=0; ii<nelem; ii++) { \
           new (&var[ii]) type initial ; \
       } \
   }
#define TS_NEW_CLASS_ARRAY_EXT(var,nelem,type,initial, name...) \
    if (var==NULL) TS_NEW_CLASS_ARRAY(var,nelem,type,initial, ##name)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Allocates an array of @a nelem pointers to the specified primitive @a type and
///           assigns it to @a var.
///
/// @param    var     The pointer to the allocated array of pointers.
/// @param    nelem   The size of the array.
/// @param    type    The underlying type, which must be a a C++ primitive type.
/// @param    name    (Optional) The sim-unique name to be assigned to the allocation, a std::string
///
/// @par      Example:
/// @code
///           double** foo; // double[2][3]
///           TS_NEW_PRIM_POINTER_ARRAY(foo, 2, double);
///           TS_NEW_PRIM_ARRAY(foo[0], 3, double);
///           TS_NEW_PRIM_ARRAY(foo[1], 3, double);
///           foo[0][0] = 1.0; foo[0][1] = 1.0; foo[0][2] = 2.0;
///           foo[1][0] = 3.0; foo[1][1] = 4.0; foo[1][2] = 5.0;
/// @endcode
///           This first allocates an array of two pointers to pointers to double using
///           TS_NEW_PRIM_POINTER_ARRAY, then allocates the two arrays of three doubles using
///           TS_NEW_PRIM_ARRAY, and finally initializes the six doubles.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_NEW_PRIM_POINTER_ARRAY(var, nelem, type, name...) \
    TS_NEW_PRIM_MULTI_POINTER_ARRAY(var, nelem, type, *, ##name)
#define TS_NEW_PRIM_POINTER_ARRAY_EXT(var, nelem, type, name...) \
    TS_NEW_PRIM_MULTI_POINTER_ARRAY_EXT(var, nelem, type, *, ##name)
#else  // non-Trick
#define TS_NEW_PRIM_POINTER_ARRAY(var, nelem, type, name...) \
    TS_NEW_PRIM_MULTI_POINTER_ARRAY(var, nelem, type, *, ##name)
#define TS_NEW_PRIM_POINTER_ARRAY_EXT(var, nelem, type, name...) \
    TS_NEW_PRIM_MULTI_POINTER_ARRAY_EXT(var, nelem, type, *, ##name)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Allocates an array of @a nelem pointers to the specified @a type and assigns it to
///           @a var.
///
/// @param    var     The pointer to the allocated array of pointers.
/// @param    nelem   The size of the array.
/// @param    type    The underlying type, which must be a structured type.
/// @param    name    (Optional) The sim-unique name to be assigned to the allocation, a std::string
///
/// @par      Example:
/// @code
///           Foo** foo; // Foo[2][3]
///           TS_NEW_CLASS_POINTER_ARRAY(foo, 2, Foo);
///           TS_NEW_CLASS_ARRAY(foo[0], 3, Foo, ());
///           TS_NEW_CLASS_ARRAY(foo[1], 3, Foo, ());
/// @endcode
///           This first allocates an array of two pointers to pointers to objects of type Foo
///           using TS_NEW_CLASS_POINTER_ARRAY, then allocates the two arrays of three objects of
///           type Foo using TS_NEW_CLASS_ARRAY with default construction.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_NEW_CLASS_POINTER_ARRAY TS_NEW_PRIM_POINTER_ARRAY

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Allocates an array of @a nelem multi-level pointers to the specified primitive @a type
///           and assigns it to @a var. The @a asters are asterisks that specify the pointer level.
///
/// @param    var     The pointer to the allocated array of pointers.
/// @param    nelem   The size of the array.
/// @param    type    The underlying type, which must be a a C++ primitive type.
/// @param    asters  One or more asterisks.
/// @param    name    (Optional) The sim-unique name to be assigned to the allocation, a std::string
///
/// @par      Example:
/// @code
///           double*** foo;  // double*[2][3]
///           TS_NEW_PRIM_MULTI_POINTER_ARRAY(foo, 2, double, **);
///           TS_NEW_PRIM_POINTER_ARRAY(foo[0], 3, double);
///           TS_NEW_PRIM_POINTER_ARRAY(foo[1], 3, double);
///           TS_NEW_PRIM_OBJECT(foo[0][0], double, (0.0));
///           TS_NEW_PRIM_OBJECT(foo[0][1], double, (1.0));
///           TS_NEW_PRIM_OBJECT(foo[0][2], double, (2.0));
///           TS_NEW_PRIM_OBJECT(foo[1][0], double, (3.0));
///           TS_NEW_PRIM_OBJECT(foo[1][1], double, (4.0));
///           TS_NEW_PRIM_OBJECT(foo[1][2], double, (5.0));
/// @endcode
///           This first allocates an array of two pointers to pointers to objects of type double
///           using TS_NEW_PRIM_MULTI_POINTER_ARRAY, then allocates an array of three pointers
///           to objects of type double for each of those using TS_NEW_PRIM_POINTER_ARRAY, and
///           finally allocates and initializes the six doubles using TS_NEW_PRIM_OBJECT.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_NEW_PRIM_MULTI_POINTER_ARRAY(var, nelem, type, asters, name...) \
{ \
    int ii ; \
    var = reinterpret_cast<type asters*>(TsMemoryManager:: \
          tsAlloc(nelem, sizeof(type asters), &var, #type #asters, ##name)); \
    for (ii=0; ii<nelem; ii++) { \
        var[ii] = NULL; \
    } \
}
#define TS_NEW_PRIM_MULTI_POINTER_ARRAY_EXT(var, nelem, type, asters, name...) \
{ \
    if (var==NULL) { \
        int ii ; \
        var = new type asters[nelem]; \
        for (ii=0; ii<nelem; ii++) { \
            var[ii] = NULL; \
        } \
    } \
    TsMemoryManager::tsAllocExt(nelem, var, &var, #type #asters, ##name); \
}
#else  // non-Trick
#define TS_NEW_PRIM_MULTI_POINTER_ARRAY(var, nelem, type, asters, name...) \
{ \
    int ii ; \
    var = new type asters[nelem]; \
    for (ii=0; ii<nelem; ii++) { \
        var[ii] = NULL; \
    } \
}
#define TS_NEW_PRIM_MULTI_POINTER_ARRAY_EXT(var, nelem, type, asters, name...) \
{ \
    if (var==NULL) { \
        int ii ; \
        var = new type asters[nelem]; \
        for (ii=0; ii<nelem; ii++) { \
            var[ii] = NULL; \
        } \
    } \
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Allocates an array of @a nelem multi-level pointers to the specified @a type and
///           assigns it to @a var. The @a asters are asterisks that specify the pointer level.
///
/// @param    var     The pointer to the allocated array of pointers.
/// @param    nelem   The size of the array.
/// @param    type    The underlying type, which must be a structured type.
/// @param    asters  One or more asterisks.
/// @param    name    (Optional) The sim-unique name to be assigned to the allocation, a std::string
///
/// @par      Example:
/// @code
///           Foo*** foo; // Foo[2][2][2]
///           TS_NEW_CLASS_MULTI_POINTER_ARRAY(foo, 2, Foo, **);
///           TS_NEW_CLASS_POINTER_ARRAY(foo[0], 2, Foo);
///           TS_NEW_CLASS_POINTER_ARRAY(foo[1], 2, Foo);
///           TS_NEW_CLASS_ARRAY(foo[0][0], 2, Foo, ());
///           TS_NEW_CLASS_ARRAY(foo[0][1], 2, Foo, ());
///           TS_NEW_CLASS_ARRAY(foo[1][0], 2, Foo, ());
///           TS_NEW_CLASS_ARRAY(foo[1][1], 2, Foo, ());
/// @endcode
///           This first allocates an array of two pointers to pointers to objects of type Foo using
///           TS_NEW_CLASS_MULTI_POINTER_ARRAY, next allocates two arrays of two pointers to objects
///           of type Foo using TS_NEW_CLASS_POINTER_ARRAY, and finally allocates four arrays of
///           two objects of type Foo for each of those with default construction using
///           TS_NEW_CLASS_ARRAY.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_NEW_CLASS_MULTI_POINTER_ARRAY TS_NEW_PRIM_MULTI_POINTER_ARRAY

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Creates a copy of the input string @a str. The non-Trick macro invokes std::strcpy,
///           which may need @#include @<cstring@>.
///
/// @param    var     The pointer to the duplicated string.
/// @param    str     The string to be duplicated.
/// @param    name    (Optional) The sim-unique name to be assigned to the allocation, a std::string
///
/// @par      Example:
/// @code
///           char* foo;
///           TS_STRDUP(foo, "foo");
/// @endcode
///           This allocates a char* array large enough to hold the string "foo" (plus the
///           terminating null character) and copies it into the allocated array using TS_STRDUP.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_STRDUP(var, str, name...) \
    var = TsMemoryManager::tsStrdup(const_cast<char*>(str), &var, ##name)
#define TS_STRDUP_EXT(var, str, name...) \
{ \
    if (var==NULL) var = new char[strlen(str)+1]; \
    TsMemoryManager::tsStrdupExt(const_cast<char*>(str), var, &var, ##name); \
}
#else  // non-Trick
#define TS_STRDUP(var, str, name...) \
{ \
    var = new char[strlen(str)+1]; \
    strcpy(var, str);\
}
#define TS_STRDUP_EXT(var, str, name...) \
{ \
    if (var==NULL) var = new char[strlen(str)+1]; \
    strcpy(var, str);\
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Allocates @b one instance of the specified STL class @a type. The supplied constructor
///           arguments, @a initial, are used as arguments to new. The default constructor will be
///           invoked if the @a initial argument is the empty list, otherwise a non-default
///           constructor will be invoked.
///
/// @param    var     The pointer to the allocated object.
/// @param    type    The underlying type, which must be an STL type.
/// @param    initial The constructor arguments, enclosed in parentheses.
/// @param    name    (Optional) The sim-unique name to be assigned to the allocation, a std::string
///
/// @par      Example:
/// @code
///           std::string* foo;
///           TS_NEW_STL_OBJECT(foo, string, ("foo"));
/// @endcode
///           This allocates a new std::string, invoking the string::string("bar") constructor
///           using TS_NEW_STL_OBJECT.
///
/// @warning  Trick 10 (as of version 10.3) does not support pointers to STL objects.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_NEW_STL_OBJECT(var, type, initial, name...) \
    var = new (TsMemoryManager::tsAlloc(1, sizeof(std::type), &var, "std::"#type, ##name)) std::type initial
#define TS_NEW_STL_OBJECT_EXT(var, type, initial, name...) \
{ \
    if (var==NULL) var = new std::type initial; \
    TsMemoryManager::tsAllocExt(1, var, &var, "std::"#type, ##name); \
}
#else  // non-Trick
#define TS_NEW_STL_OBJECT(var, type, initial, name...) \
    var = new std::type initial
#define TS_NEW_STL_OBJECT_EXT(var, type, initial, name...) \
    if (var==NULL) var = new std::type initial
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Determines if @a ptr was allocated by some TS_NEW macro.
///
/// @param    ptr     The pointer to be checked.
///
/// @return           True if this pointer was allocated by this module, otherwise false.
///
/// @par      Example:
/// @code
///           Foo* foo;
///           if (TS_IS_ALLOCATED(foo)) {
///               TS_DELETE_OBJECT(foo);
///           }
/// @endcode
///           This deletes the memory at foo, but only if that memory was allocated with one of the
///           TS_NEW macros.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_IS_ALLOCATED(ptr) \
    TsMemoryManager::tsIsAlloced(reinterpret_cast<char*>(ptr))
#else  // non-Trick
#define TS_IS_ALLOCATED(ptr) \
    (ptr != 0)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Frees memory for single item at @a ptr if it was allocated by some TS_NEW_*_OBJECT
///           macro.
///
/// @param    ptr     The pointer to be deleted.
///
/// @par      Example:
/// @code
///           Foo* foo;
///           TS_NEW_CLASS_OBJECT(foo ,Foo, ());
///           ...
///           TS_DELETE_OBJECT(foo);
/// @endcode
///           This allocates memory for a single object of type Foo with TS_NEW_CLASS_OBJECT and
///           then deletes it with TS_DELETE_OBJECT.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_DELETE_OBJECT(ptr) \
    TsMemoryManager::tsDelete(reinterpret_cast<void*>(ptr), typeid(ptr).name())
#else  // non-Trick
#define TS_DELETE_OBJECT(ptr) \
    { \
        delete ptr; \
        ptr = 0; \
    }
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Frees memory for an array at @a ptr if it was allocated by some TS_NEW_*_ARRAY
///           macro.
///
/// @param    ptr     The pointer to be deleted.
///
/// @par      Example:
/// @code
///           Foo** foo; // Foo[2][3]
///           TS_NEW_CLASS_POINTER_ARRAY(foo, 2, Foo);
///           TS_NEW_CLASS_ARRAY(foo[0], 3, Foo, ());
///           TS_NEW_CLASS_ARRAY(foo[1], 3, Foo, ());
///           ...
///           TS_DELETE_ARRAY(foo[0]);
///           TS_DELETE_ARRAY(foo[1]);
///           TS_DELETE_ARRAY(foo);
/// @endcode
///           This allocates memory for a two dimensional array of doubles using
///           TS_NEW_CLASS_POINTER_ARRAY and TS_NEW_CLASS_ARRAY, then deletes it using multiple
///           invocations of TS_DELETE_ARRAY. Note that the arrays are deleted in the reverse order
///           of their allocation.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_DELETE_ARRAY(ptr) \
    { \
       TsMemoryManager::tsDelete(reinterpret_cast<void*>(ptr), typeid(ptr).name()); \
       ptr = 0; \
    }
#else  // non-Trick
#define TS_DELETE_ARRAY(ptr) \
    { \
        delete[] ptr; \
        ptr = 0; \
    }
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Still in use by cxtf sim to allocate an array of primitives.
///
/// @deprecated  For cxtf sim only. Use TS_NEW_PRIM_ARRAY instead.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_NEW_PRIMITIVE TS_NEW_PRIM_ARRAY

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Still in use by cxtf sim to allocate an array of objects.
///
/// @deprecated  For cxtf sim only. Use TS_NEW_CLASS_ARRAY instead.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_NEW(var, nelem, type) TS_NEW_CLASS_ARRAY(var, nelem, type, ())

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Still in use by cxtf sim to allocate an array of objects with constructor args.
///
/// @deprecated  For cxtf sim support only. Use TS_NEW_CLASS_ARRAY instead.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_NEW_ARGS(var, nelem, type, ...) TS_NEW_CLASS_ARRAY(var, nelem, type, (__VA_ARGS__))

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Still in use by cxtf sim to delete objects created with a TS_NEW.
///
/// @deprecated  For cxtf sim support only. Use TS_DELETE_OBJECT or TS_DELETE_ARRAY instead.
///
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef no_TRICK_ENV
#define TS_DELETE(var)
#else
#define TS_DELETE(var) delete [] var
#endif
/// @}

#endif
