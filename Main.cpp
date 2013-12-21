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
   unsigned int numLevels = 5;
   
   Voxels voxels(numLevels, sphereFile.getBoundingBox(), sphereFile.getTriangles());
   voxels.printBinary();
   
   std::cout << "Num set bits: " << voxels.countSetVoxels() << "\n";
}
