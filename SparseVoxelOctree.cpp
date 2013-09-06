/**
 * SparseVoxelOctree.cpp
 * 
 * by Brent Williams
 */

#include "SparseVoxelOctree.hpp"

SparseVoxelOctree::SparseVoxelOctree(std::vector<Triangle>& triangles, BoundingBox &boundingBox)
{
   build(triangles, boundingBox);
}

void SparseVoxelOctree::build(std::vector<Triangle>& triangles, BoundingBox& bb)
{
   BoundingBox boundingBox(bb);
   boundingBox.square();
   
   
}

void SparseVoxelOctree::voxelizeTriangle(Triangle triangle, Voxels& voxels)
{
   
}
