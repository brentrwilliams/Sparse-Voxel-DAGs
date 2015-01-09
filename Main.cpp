/**
 * Main.cpp
 * 
 * by Brent Williams
 */

#include "Main.hpp"

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
   
   SparseVoxelOctree svo(numLevels, objFile.getBoundingBox(), objFile.getTriangles());
   objFile.getBoundingBox().print();
   
   if (argc == 3)
   {
      svo.writeImages();
   }

   std::cout << "Number of triangles: " << objFile.getNumTriangles() << std::endl;
   
    
   return 0;
}