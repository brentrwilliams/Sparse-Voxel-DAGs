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
   uint64_t* leafVoxelData = leafVoxels2Voxels->data;
   unsigned int dataSize = leafVoxels2Voxels->dataSize;
   
   //Make the SVO leaf nodes
   unsigned int i;
   for (i = 0; i < dataSize; i++)
   {
      
   }
}


































