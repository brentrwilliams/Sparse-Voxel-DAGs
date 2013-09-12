/**
 * Voxels.hpp
 * 
 * Represents a set of sparse voxels. 
 *
 * Note:
 * Only supports voxels 
 *
 * by Brent Williams
 */

#ifndef VOXELS_HPP
#define VOXELS_HPP

#include <stdint.h>
#include <math.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

#include "BoundingBox.hpp"
#include "Triangle.hpp"
#include "Intersect.hpp"


class Voxels
{
   public:
   //Will be after testing
   //private:
      uint64_t *data;
      BoundingBox boundingBox;
      unsigned int levels;
      unsigned long size; // Total number of voxels 
      unsigned int dimension; // Number of voxels for one side of the cube
      float voxelWidth; // The length of one voxel in world space
      unsigned int dataSize; // The number of uint64_t allocated to data
      
      unsigned int calculateDataSize(unsigned int levels);
      void set(unsigned int x, unsigned int y, unsigned int z);
      void build(const std::vector<Triangle> triangles);
      void voxelizeTriangle(const Triangle& triangle);
      
   //Will be
   //public:
      Voxels(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles);
      ~Voxels();
      uint64_t& operator[](unsigned int i);
      
      unsigned long getSize() const;
      
};

void printBinary(const uint64_t data);
unsigned int countSetBits(uint64_t data);

#endif

