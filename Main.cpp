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
   unsigned int numLevels = 3;
   
   SparseVoxelOctree svo(numLevels, sphereFile.getBoundingBox(), sphereFile.getTriangles());
   svo.printBinary();
   
   /*void* current = svo.root;
   uint64_t* leafPtr = (uint64_t*) ((SVONode*) current)->childPointers[0];
   int j;
   for (j = 0; j < 64; j++)
   {
      if (svo.isLeafSet(leafPtr, j))
         std::cout << "1 ";
      else
         std::cout << "0 ";
      
      if ((j+1) % 8 == 0)
         std::cout << "\n";
   }
   std::cout << "\n";
   */
   /*
   SVONode rootData;
   SVONode* root = &rootData;
   bool childMaskBools[8] = {true, false, false, false, false, false, false, false};
   svo.setChildMask(childMaskBools, &root->childMask);
   
   int i;
   std::cout << "\nMask bits: ";
   for (i = 0; i < 8; i++)
   {
      if (svo.isMaskSet(root, i))
         std::cout << 1;
      else
         std::cout << 0;
   }
   std::cout << "\n\n";
   
   uint64_t leafNode0 = 1;
   std::cout << "Leaf bits: \n ";
   for (i = 0; i < 64; i++)
   {
      if (svo.isLeafSet(&leafNode0, i))
         std::cout << 1;
      else
         std::cout << 0;
      
      if ((i+1) % 8 == 0)
         std::cout << "\n ";
   }
   std::cout << "\n";
   */
   return 0;
}
