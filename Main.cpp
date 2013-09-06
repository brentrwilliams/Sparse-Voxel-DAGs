/**
 * Main.cpp
 * 
 * by Brent Williams
 */

#include "Main.hpp"

int main()
{
   std::string fileName("SphereDetailed.obj");
   OBJFile sphereFile(fileName);
   unsigned int numLevels = 3;
   
   Voxels voxels(numLevels, sphereFile.getBoundingBox(), sphereFile.getTriangles());
   unsigned int i;
   
   for (i = 0; i < 8; i++)
   {
      std::cout << "z = " << i << ": " << voxels[i] << "\n";
      printBinary(voxels[i]);
      std::cout << "\n";
   }
}
