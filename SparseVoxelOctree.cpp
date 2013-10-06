/**
 * SparseVoxelOctree.cpp
 * 
 * by Brent Williams
 */

#include "SparseVoxelOctree.hpp"

SparseVoxelOctree::SparseVoxelOctree(unsigned int levelsVal, std::vector<Triangle>& triangles, BoundingBox &boundingBox)
 : numLevels(levelsVal)
{
   build(triangles, boundingBox);
}

void SparseVoxelOctree::build(std::vector<Triangle>& triangles, BoundingBox& bb)
{
   BoundingBox boundingBox(bb);
   boundingBox.square();
   unsigned int svoLevels = numLevels-2;
   ActiveList actNodes(svoLevels, boundingBox, triangles);
   unsigned int numNodes = actNodes.countSetVoxels(); // Number of 64 bit ints to allocate for the bottom level
   uint64_t *leafNodes;
   std::vector<void *> levels;
   unsigned int i;
   
   
   levels.push_back(new uint64_t[numNodes]);
   
   if (levels[0] == NULL)
   {
      std::string err("\nAllocation failed for newData in SparseVoxelOctree::build\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }
   
   
   for (i = 1; i < svoLevels; i++)
   {
      actNodes.nextLevel();
      numNodes = actNodes.countSetVoxels();
      levels.push_back(new uint64_t[numNodes]);
      
      if (levels[i] == NULL)
      {
         std::string err("\nAllocation failed for newData in SparseVoxelOctree::build\n");
         std::cerr << err;
         throw std::out_of_range(err);
      }
   }
   
   
   
}
