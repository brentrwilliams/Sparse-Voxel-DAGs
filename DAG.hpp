/**
 * DAG.hpp
 * 
 * by Brent Williams
 */

#ifndef DAG_HPP
#define DAG_HPP

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

class DAG 
{
   public:
      DAG(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles);
      ~DAG();
      void build(const std::vector<Triangle> triangles);

      BoundingBox boundingBox;
      unsigned int levels;
      unsigned long size; // Total number of voxels if the SVO was full
      unsigned int dimension; // Number of voxels for one side of the cube
      float voxelWidth; // The length of one voxel in world space
      
};

#endif
