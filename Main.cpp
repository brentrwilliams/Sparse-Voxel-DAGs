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

int main(int argc, char const *argv[])
{
   if (argc < 3)
   {
      perror("Missing arguments.\n Exitting\n");
      exit(1);
   }
   
   std::string fileName(argv[1]);
   OBJFile objFile(fileName);
   unsigned int numLevels = atoi(argv[2]);
   
   Voxels voxels(numLevels, objFile.getBoundingBox(), objFile.getTriangles());
   
   if (argc == 3)
   {
      voxels.writeImages();
   }

   std::cout << "Number of triangles: " << objFile.getNumTriangles() << std::endl;
   
    
   return 0;
}