/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ()

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-06))
***************************************************************************************************/

#include <iostream>
#include <string>

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "software/SimCompatibility/test/UtSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::setUp()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new primitive object macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewPrimObject()
{
    std::cout << "--------------------------------------------------------------------------------";
    std::cout << "\n SimCompatibility 01: New Primitive Object Test ";

    // new (int)
    int* article;
    TS_NEW_PRIM_OBJECT(article, int, (10));

    // verify
    CPPUNIT_ASSERT(article);
    CPPUNIT_ASSERT_EQUAL(10, *article);

    // delete
    TS_DELETE_OBJECT(article);

    std::cout << "                     ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new named primitive object macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewPrimObjectNamed()
{
    std::cout << "\n SimCompatibility 02: New Named Primitive Object Test ";

    // new (int)
    int* article;
    TS_NEW_PRIM_OBJECT(article, int, (10), "name");

    // verify
    CPPUNIT_ASSERT(article);
    CPPUNIT_ASSERT_EQUAL(10, *article);

    // delete
    TS_DELETE_OBJECT(article);

    std::cout << "                     ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new primitive array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewPrimArray()
{
    std::cout << "\n SimCompatibility 03: New Primitive Array Test ";

    // new (double[4])
    double* article;
    TS_NEW_PRIM_ARRAY(article, 4, double);

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 4; i++) {
        CPPUNIT_ASSERT(&article[i]);
    }

    // delete
    TS_DELETE_ARRAY(article);

    std::cout << "                      ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new named primitive array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewPrimArrayNamed()
{
    std::cout << "\n SimCompatibility 04: New Named Primitive Array Test ";

    // new (double[4])
    double* article;
    TS_NEW_PRIM_ARRAY(article, 4, double, "name");

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 4; i++) {
        CPPUNIT_ASSERT(&article[i]);
    }

    // delete
    TS_DELETE_ARRAY(article);

    std::cout << "                      ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new primitive pointer array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewPrimPointerArray()
{
    std::cout << "\n SimCompatibility 05: New Primitive Pointer Array Test ";

    // new (A[3][3])
    double** article;
    TS_NEW_PRIM_POINTER_ARRAY(article, 3, double);
    for (int i = 0; i < 3; i++) {
        TS_NEW_PRIM_ARRAY(article[i], 3, double);
    }

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 3; i++) {
        CPPUNIT_ASSERT(article[i]);
    }

    // delete
    for (int i = 0; i < 3; i++) {
        TS_DELETE_ARRAY(article[i]);
    }
    TS_DELETE_ARRAY(article);

    std::cout << "              ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new named primitive pointer array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewPrimPointerArrayNamed()
{
    std::cout << "\n SimCompatibility 06: New Named Primitive Pointer Array Test ";

    // new (A[3][3])
    double** article;
    TS_NEW_PRIM_POINTER_ARRAY(article, 3, double, "name");
    for (int i = 0; i < 3; i++) {
        TS_NEW_PRIM_ARRAY(article[i], 3, double, "subname");
    }

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 3; i++) {
        CPPUNIT_ASSERT(article[i]);
    }

    // delete
    for (int i = 0; i < 3; i++) {
        TS_DELETE_ARRAY(article[i]);
    }
    TS_DELETE_ARRAY(article);

    std::cout << "              ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new primitive multi-pointer array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewPrimMultiPointerArray()
{
    std::cout << "\n SimCompatibility 07: New Primitive Multi-Pointer Array Test ";

    // new A*[3][3]
    double*** article;
    TS_NEW_PRIM_MULTI_POINTER_ARRAY(article, 3, double, **);
    for (int i = 0; i < 3; i++) {
        TS_NEW_PRIM_POINTER_ARRAY(article[i], 3, double);
        for (int j = 0; j < 3; j++) {
            TS_NEW_PRIM_OBJECT(article[i][j], double, (1.0 * (i + j + 1)));
        }
    }

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 3; i++) {
        CPPUNIT_ASSERT(article[i]);
        for (int j = 0; j < 3; j++) {
            CPPUNIT_ASSERT(article[i][j]);
            CPPUNIT_ASSERT_EQUAL(1.0 * (i + j + 1), *article[i][j]);
        }
   }

    // delete
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            TS_DELETE_OBJECT(article[i][j]);
        }
        TS_DELETE_ARRAY(article[i]);
    }
    TS_DELETE_ARRAY(article);

    std::cout << "        ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new named primitive multi-pointer array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewPrimMultiPointerArrayNamed()
{
    std::cout << "\n SimCompatibility 08: New Named Primitive Multi-Pointer Array Test ";

    // new A*[3][3]
    double*** article;
    TS_NEW_PRIM_MULTI_POINTER_ARRAY(article, 3, double, **, "name");
    for (int i = 0; i < 3; i++) {
        TS_NEW_PRIM_POINTER_ARRAY(article[i], 3, double, "subname");
        for (int j = 0; j < 3; j++) {
            TS_NEW_PRIM_OBJECT(article[i][j], double, (1.0 * (i + j + 1)), "subsubname");
        }
    }

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 3; i++) {
        CPPUNIT_ASSERT(article[i]);
        for (int j = 0; j < 3; j++) {
            CPPUNIT_ASSERT(article[i][j]);
            CPPUNIT_ASSERT_EQUAL(1.0 * (i + j + 1), *article[i][j]);
        }
   }

    // delete
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            TS_DELETE_OBJECT(article[i][j]);
        }
        TS_DELETE_ARRAY(article[i]);
    }
    TS_DELETE_ARRAY(article);

    std::cout << "        ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for string duplication macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testStrdup()
{
    std::cout << "\n SimCompatibility 09: String Duplication Test ";

    // new (char[])
    char* article;
    TS_STRDUP(article, "Foo");

    // verify
    CPPUNIT_ASSERT(article);
    CPPUNIT_ASSERT(strcmp(article, "article"));

    // delete
    TS_DELETE_ARRAY(article);

    std::cout << "                       ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new STL object macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewStlObject()
{
    std::cout << "\n SimCompatibility 10: New STL Object Test ";

    // new (std::string*)
    std::string* article;
    TS_NEW_STL_OBJECT(article, string, ("article"));

    // verify
    CPPUNIT_ASSERT(article);
    CPPUNIT_ASSERT(!article->compare("article"));

    // delete
    TS_DELETE_OBJECT(article);

    std::cout << "                           ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new named STL object macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewStlObjectNamed()
{
    std::cout << "\n SimCompatibility 11: New Named STL Object Test ";

    // new (std::string*)
    std::string* article;
    TS_NEW_STL_OBJECT(article, string, ("article"), "name");

    // verify
    CPPUNIT_ASSERT(article);
    CPPUNIT_ASSERT(!article->compare("article"));

    // delete
    TS_DELETE_OBJECT(article);

    std::cout << "                           ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new class object macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewClassObject()
{
    std::cout << "\n SimCompatibility 12: New Class Object Test ";

    // new (A*)
    A* article;
    TS_NEW_CLASS_OBJECT(article, A, (0.7));

    // verify
    CPPUNIT_ASSERT(article);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7, article->getX(), 0.0);

    // delete
    TS_DELETE_OBJECT(article);

    std::cout << "                         ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new named class object macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewClassObjectNamed()
{
    std::cout << "\n SimCompatibility 13: New Named Class Object Test ";

    // new (A*)
    A* article;
    TS_NEW_CLASS_OBJECT(article, A, (0.7), "name");

    // verify
    CPPUNIT_ASSERT(article);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.7, article->getX(), 0.0);

    // delete
    TS_DELETE_OBJECT(article);

    std::cout << "                         ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new class array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewClassArray()
{
    std::cout << "\n SimCompatibility 14: New Class Array Test ";

    // new (A[4] default construction)
    A* article;
    TS_NEW_CLASS_ARRAY(article, 4, A, ());

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 4; i++) {
        CPPUNIT_ASSERT(&article[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, article[i].getX(), 0.0);
    }

    // delete
    TS_DELETE_ARRAY(article);

    std::cout << "                          ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new named class array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewClassArrayNamed()
{
    std::cout << "\n SimCompatibility 15: New Named Class Array Test ";

    // new (A[4] default construction)
    A* article;
    TS_NEW_CLASS_ARRAY(article, 4, A, (), "name");

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 4; i++) {
        CPPUNIT_ASSERT(&article[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, article[i].getX(), 0.0);
    }

    // delete
    TS_DELETE_ARRAY(article);

    std::cout << "                          ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new class array with argument constructor macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewClassArrayArg()
{
    std::cout << "\n SimCompatibility 16: New Class Array with Argument Constructor Test ";

    // new (A[4] construction with arguments)
    A* article;
    TS_NEW_CLASS_ARRAY(article, 4, A, (7.0));

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 4; i++) {
        CPPUNIT_ASSERT(&article[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, article[i].getX(), 0.0);
    }

    // delete
    TS_DELETE_ARRAY(article);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new named class array with argument constructor macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewClassArrayArgNamed()
{
    std::cout << "\n SimCompatibility 17: New Named Class Array with Argument Constructor Test ";

    // new (A[4] construction with arguments)
    A* article;
    TS_NEW_CLASS_ARRAY(article, 4, A, (7.0), "name");

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 4; i++) {
        CPPUNIT_ASSERT(&article[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, article[i].getX(), 0.0);
    }

    // delete
    TS_DELETE_ARRAY(article);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new class pointer array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewClassPointerArray()
{
    std::cout << "\n SimCompatibility 18: New Class Pointer Array Test ";

    // new (A*[5] base and derived)
    A** article;
    TS_NEW_CLASS_POINTER_ARRAY(article, 5, A);
    TS_NEW_CLASS_OBJECT(article[0], A, (0.2));
    TS_NEW_CLASS_OBJECT(article[1], B, (0.3));
    TS_NEW_CLASS_OBJECT(article[2], B, (0.4));
    TS_NEW_CLASS_OBJECT(article[3], B, (0.5));
    TS_NEW_CLASS_OBJECT(article[4], A, (0.6));

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 5; i++) {
        CPPUNIT_ASSERT(article[i]);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, article[0]->getX(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, article[1]->getX(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, article[2]->getX(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, article[3]->getX(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, article[4]->getX(), 0.0);

    // delete
    for (int i = 0; i < 5; i++) {
        TS_DELETE_OBJECT(article[i]);
    }
    TS_DELETE_ARRAY(article);

    std::cout << "                  ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new named class pointer array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewClassPointerArrayNamed()
{
    std::cout << "\n SimCompatibility 19: New Named Class Pointer Array Test ";

    // new (A*[5] base and derived)
    A** article;
    TS_NEW_CLASS_POINTER_ARRAY(article, 5, A, "name");
    TS_NEW_CLASS_OBJECT(article[0], A, (0.2), "name0");
    TS_NEW_CLASS_OBJECT(article[1], B, (0.3), "name1");
    TS_NEW_CLASS_OBJECT(article[2], B, (0.4), "name2");
    TS_NEW_CLASS_OBJECT(article[3], B, (0.5), "name3");
    TS_NEW_CLASS_OBJECT(article[4], A, (0.6), "name4");

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 5; i++) {
        CPPUNIT_ASSERT(article[i]);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, article[0]->getX(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, article[1]->getX(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, article[2]->getX(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, article[3]->getX(), 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6, article[4]->getX(), 0.0);

    // delete
    for (int i = 0; i < 5; i++) {
        TS_DELETE_OBJECT(article[i]);
    }
    TS_DELETE_ARRAY(article);

    std::cout << "                  ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new class multi-pointer array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewClassMultiPointerArray()
{
    std::cout << "\n SimCompatibility 20: New Class Multi-Pointer Array Test ";

    // new (A[2][2][2])
    A*** article;
    TS_NEW_CLASS_MULTI_POINTER_ARRAY(article, 2, A, **);
    for (int i = 0; i < 2; i++) {
        TS_NEW_CLASS_POINTER_ARRAY(article[i], 2, A);
        for (int j = 0; j < 2; j++) {
            TS_NEW_CLASS_ARRAY(article[i][j], 2, A, ());
        }
    }

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 2; i++) {
        CPPUNIT_ASSERT(article[i]);
        for (int j = 0; j < 2; j++) {
            CPPUNIT_ASSERT(article[i][j]);
            for (int k = 0; k < 2; k++) {
                CPPUNIT_ASSERT(&article[i][j][k]);
            }
        }
    }

    // delete
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            TS_DELETE_ARRAY(article[i][j]);
        }
        TS_DELETE_ARRAY(article[i]);
    }
    TS_DELETE_ARRAY(article);

    std::cout << "            ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for new named class multi-pointer array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testNewClassMultiPointerArrayNamed()
{
    std::cout << "\n SimCompatibility 21: New Named Class Multi-Pointer Array Test ";

    // new (A[2][2][2])
    A*** article;
    TS_NEW_CLASS_MULTI_POINTER_ARRAY(article, 2, A, **, "name");
    for (int i = 0; i < 2; i++) {
        TS_NEW_CLASS_POINTER_ARRAY(article[i], 2, A, "subname");
        for (int j = 0; j < 2; j++) {
            TS_NEW_CLASS_ARRAY(article[i][j], 2, A, (), "subsubname");
        }
    }

    // verify
    CPPUNIT_ASSERT(article);
    for (int i = 0; i < 2; i++) {
        CPPUNIT_ASSERT(article[i]);
        for (int j = 0; j < 2; j++) {
            CPPUNIT_ASSERT(article[i][j]);
            for (int k = 0; k < 2; k++) {
                CPPUNIT_ASSERT(&article[i][j][k]);
            }
        }
    }

    // delete
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            TS_DELETE_ARRAY(article[i][j]);
        }
        TS_DELETE_ARRAY(article[i]);
    }
    TS_DELETE_ARRAY(article);

    std::cout << "            ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for is allocated macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testIsAllocated()
{
    std::cout << "\n SimCompatibility 22: Is Allocated Test ";

    {
        /// @test allocation detection of class object pointer
        A* article = 0;
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        TS_NEW_CLASS_OBJECT(article, A, (0.0));
        CPPUNIT_ASSERT(TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(article);
        TS_DELETE_OBJECT(article);
    }

    std::cout << "                             ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for delete object macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testDeleteObject()
{
    std::cout << "\n SimCompatibility 23: Delete Object Test ";

    {
        /// @test deletion of primitive object
        double* article;
        TS_NEW_PRIM_OBJECT(article, double, (0.0));
        CPPUNIT_ASSERT(TS_IS_ALLOCATED(article));
        TS_DELETE_OBJECT(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    } {
        /// @test deletion of STL object
        std::string* article;
        TS_NEW_STL_OBJECT(article, string, ("article"));
        CPPUNIT_ASSERT(TS_IS_ALLOCATED(article));
        TS_DELETE_OBJECT(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    } {
        /// @test deletion of class object
        A* article;
        TS_NEW_CLASS_OBJECT(article, A, (0.0));
        CPPUNIT_ASSERT(TS_IS_ALLOCATED(article));
        TS_DELETE_OBJECT(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    } {
        /// @test deletion of unallocated primitive object
        double* article = 0;
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        TS_DELETE_OBJECT(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    } {
        /// @test deletion of unallocated STL object
        std::string* article = 0;
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        TS_DELETE_OBJECT(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    } {
        /// @test deletion of unallocated class object
        A* article = 0;
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        TS_DELETE_OBJECT(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    }

    std::cout << "                            ... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for delete array macro.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSimCompatibility::testDeleteArray()
{
    std::cout << "\n SimCompatibility 24: Delete Array Test ";

    {
        /// @test deletion of primitive array
        double*** article;
        TS_NEW_PRIM_MULTI_POINTER_ARRAY(article, 3, double, **);
        for (int i = 0; i < 3; i++) {
            TS_NEW_PRIM_POINTER_ARRAY(article[i], 3, double);
            for (int j = 0; j < 3; j++) {
                TS_NEW_PRIM_OBJECT(article[i][j], double, (1.0 * (i + j + 1)));
            }
        }
        CPPUNIT_ASSERT(article);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                TS_DELETE_OBJECT(article[i][j]);
            }
            TS_DELETE_ARRAY(article[i]);
        }
        TS_DELETE_ARRAY(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    } {
        /// @test deletion of string
        char* article;
        TS_STRDUP(article, "Foo");
        CPPUNIT_ASSERT(TS_IS_ALLOCATED(article));
        TS_DELETE_ARRAY(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    } {
        /// @test deletion of class array
        A*** article;
        TS_NEW_CLASS_MULTI_POINTER_ARRAY(article, 2, A, **);
        for (int i = 0; i < 2; i++) {
            TS_NEW_CLASS_POINTER_ARRAY(article[i], 2, A);
            for (int j = 0; j < 2; j++) {
                TS_NEW_CLASS_ARRAY(article[i][j], 2, A, ());
            }
        }
        CPPUNIT_ASSERT(TS_IS_ALLOCATED(article));
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                TS_DELETE_ARRAY(article[i][j]);
            }
            TS_DELETE_ARRAY(article[i]);
        }
        TS_DELETE_ARRAY(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    } {
        /// @test deletion of unallocated primitive array
        double*** article = 0;
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        TS_DELETE_ARRAY(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    } {
        /// @test deletion of unallocated string array
        char* article = 0;
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        TS_DELETE_ARRAY(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    } {
        /// @test deletion of unallocated class array
        A*** article = 0;
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        TS_DELETE_ARRAY(article);
        CPPUNIT_ASSERT(!TS_IS_ALLOCATED(article));
        CPPUNIT_ASSERT(!article);
    }

    std::cout << "                             ... Pass" << std::endl;
    std::cout << "--------------------------------------------------------------------------------";
}
