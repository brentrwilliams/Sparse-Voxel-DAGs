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
#include "ActiveList.hpp"

#include <vector>

class SparseVoxelOctree
{
   private:
      unsigned int numLevels;
      void build(std::vector<Triangle>& triangles, BoundingBox& bb);
      void voxelizeTriangle(Triangle triangle, Voxels& voxels);
   
   public:
      SparseVoxelOctree(unsigned int levelsVal, std::vector<Triangle>& triangles, BoundingBox &boundingBox);
};

class SVONode1
{
   public:
      uint64_t mask;
      void *childPointer1;
};

class SVONode2
{
   public:
      uint64_t mask;
      void *childPointer1;
      void *childPointer2;
};

class SVONode3
{
   public:
      uint64_t mask;
      void *childPointer1;
      void *childPointer2;
      void *childPointer3;
};

class SVONode4
{
   public:
      uint64_t mask;
      void *childPointer1;
      void *childPointer2;
      void *childPointer3;
      void *childPointer4;
};

class SVONode5
{
   public:
      uint64_t mask;
      void *childPointer1;
      void *childPointer2;
      void *childPointer3;
      void *childPointer4;
      void *childPointer5;
};

class SVONode6
{
   public:
      uint64_t mask;
      void *childPointer1;
      void *childPointer2;
      void *childPointer3;
      void *childPointer4;
      void *childPointer5;
      void *childPointer6;
};

class SVONode7
{
   public:
      uint64_t mask;
      void *childPointer1;
      void *childPointer2;
      void *childPointer3;
      void *childPointer4;
      void *childPointer5;
      void *childPointer6;
      void *childPointer7;
};

class SVONode8
{
   public:
      uint64_t mask;
      void *childPointer1;
      void *childPointer2;
      void *childPointer3;
      void *childPointer4;
      void *childPointer5;
      void *childPointer6;
      void *childPointer7;
      void *childPointer8;
};

class SVOLeafNode
{
   public:
      uint64_t data[8];
};


#endif
