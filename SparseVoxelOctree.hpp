/**
 * SparseVoxelOctree.hpp
 * 
 * by Brent Williams
 */

#ifndef SPARSE_VOXEL_OCTREE_HPP
#define SPARSE_VOXEL_OCTREE_HPP

#include "Triangle.hpp"
#include "Intersect.hpp"
#include "Vec3.hpp"
#include "Voxels.hpp"
#include "BoundingBox.hpp"
#include "SparseVoxelOctree.hpp"
#include "SVONode.hpp"
#include "Image.hpp"

#include <vector>
#include <stdint.h>

#define SET_8_BITS 255

class SparseVoxelOctree
{
   private:
      //Fix later
   public:
      SparseVoxelOctree(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles);
      ~SparseVoxelOctree();
      void build(const std::vector<Triangle> triangles);
      void setVoxel(unsigned int x, unsigned int y, unsigned int z, uint64_t* activeNodes, uint64_t* nodes);
      void voxelizeTriangle(const Triangle& triangle, uint64_t* activeNodes, uint64_t* nodes);
      void setChildMask(bool childMaskBools[8], uint64_t* childMask);
      bool isSet(unsigned int x, unsigned int y, unsigned int z);
      bool isMaskNotEmpty(SVONode* node);
      bool isMaskSet(SVONode* node, unsigned int i);
      bool isLeafSet(uint64_t* node, unsigned int i);
      void printBinary();
      void writeImages();
      
      BoundingBox boundingBox;
      unsigned int levels;
      unsigned long size; // Total number of voxels if the SVO was full
      unsigned int dimension; // Number of voxels for one side of the cube
      float voxelWidth; // The length of one voxel in world space
      SVONode* root;
};


#endif
