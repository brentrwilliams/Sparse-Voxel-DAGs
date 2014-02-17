/**
 * Main.cpp
 * 
 * by Brent Williams
 */

#include "Main.hpp"

/*
//Old main
int main()
{
   std::string fileName("SphereDetailed.obj");
   OBJFile sphereFile(fileName);
   unsigned int numLevels = 3;
   
   Voxels voxels(numLevels, sphereFile.getBoundingBox(), sphereFile.getTriangles());
   voxels.printBinary();
   
   std::cout << "Num set bits: " << voxels.countSetVoxels() << "\n";
}
*/

int main()
{
   std::string fileName("SphereDetailed.obj");
   OBJFile sphereFile(fileName);
   unsigned int numLevels = 5;
   
   SparseVoxelOctree svo(numLevels, sphereFile.getBoundingBox(), sphereFile.getTriangles());
   svo.printBinary();
   
   return 0;
}
