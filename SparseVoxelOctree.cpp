/**
 * SparseVoxelOctree.cpp
 * 
 * by Brent Williams
 */

#include "SparseVoxelOctree.hpp"

/**
 * Instantiates and initializes the SVO.
 */
SparseVoxelOctree::SparseVoxelOctree(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles)
 : boundingBox(boundingBoxVal),
   levels(levelsVal),
   size(pow(8, levelsVal)), 
   dimension(pow(2,levelsVal)),
   voxelWidth(0)
{
   if (levels <= 2)
   {
      std::string err("\nNumber of levels too small\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }
   
   boundingBox.square();
   voxelWidth = (boundingBox.maxs.x - boundingBox.mins.x) / dimension;
   build(triangles);
}

/**
 * Free any allocated data and clean up
 */
SparseVoxelOctree::~SparseVoxelOctree()
{
   
}

/**
 * Build the SVO.
 */
void SparseVoxelOctree::build(const std::vector<Triangle> triangles)
{
   Voxels* leafVoxels = new Voxels(levels, boundingBox, triangles);
   uint64_t* leafVoxelData = leafVoxels->data;
   unsigned int dataSize = leafVoxels->dataSize;
   
   //Make the SVO leaf nodes and their parents
   unsigned int currentLevel = levels-2;
   unsigned int currentLevelNodeCount = pow(8,currentLevel);
   SVONode* leafParentNodes = new SVONode[currentLevelNodeCount];
   unsigned int i;
   bool childMask[8];
   unsigned int currentParent = 0;
   unsigned int currentChild = 0;
      
   for (i = 0; i < dataSize; i++)
   {
      uint64_t* newLeafData = new uint64_t;
      *newLeafData = leafVoxelData[i];
      leafParentNodes[currentParent].childPointers[currentChild] = (void*) newLeafData;
      
      childMask[currentChild] = leafVoxelData[i] > 0;
      currentChild = (currentChild+1) % 8;
      
      if (currentChild == 0)
      {
         setChildMask(childMask, &(leafParentNodes[currentParent].childMask));
         currentParent++;
      }
   }
   currentLevel--;
   
   //Build remaining levels
   SVONode* childNodes = leafParentNodes;
   SVONode* newParentNodes;
   
   for (; currentLevel > 0; currentLevel--)
   {
      buildLevel(currentLevel, childNodes, &newParentNodes);
      childNodes = newParentNodes;
   }
   root = newParentNodes;
}


void SparseVoxelOctree::buildLevel(int currentLevel, SVONode* childNodes, SVONode** newParentNodes)
{
   unsigned int currentLevelNodeCount = pow(8,currentLevel);
   unsigned int previousLevelNodeCount = pow(8,currentLevel+1);
   unsigned int i;
   bool childMask[8];
   unsigned int currentParent = 0;
   unsigned int currentChild = 0;
   
   SVONode* parentNodes = new SVONode[currentLevelNodeCount];
   for (i = 0; i < previousLevelNodeCount; i++)
   {
      childMask[currentChild] = childNodes[i].childMask > 0;
      currentChild = (currentChild+1) % 8;
      
      if (currentChild == 0)
      {
         setChildMask(childMask, &(parentNodes[currentParent].childMask));
         currentParent++;
      }
   }
   
   *newParentNodes = parentNodes;
}


void SparseVoxelOctree::setChildMask(bool childMaskBools[8], uint64_t* childMask)
{
   unsigned int i;
   *childMask = 0;
   for (i = 0; i < 8; i++)
   {
      if (childMaskBools[i])
      {
         *childMask |= (1L << i);
      }
      
   }
}
































