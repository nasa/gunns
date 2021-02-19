/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
// basic file operations
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int main () {
   for(int ii = 0; ii < 101; ii++)
   {
      ofstream myfile;
      const char* fileName;
       ostringstream convert;
       convert << ii;

       string fileN = "file" + convert.str() + ".xml";
       myfile.open (fileN.c_str());
       myfile.close();
   }
  return 0;
}
