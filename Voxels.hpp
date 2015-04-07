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

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

// OpenMP
#include <omp.h>

#include "BoundingBox.hpp"
#include "Triangle.hpp"
#include "Intersect.hpp"
#include "MortonCode.hpp"
#include "Image.hpp"


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
      bool isSet(unsigned int x, unsigned int y, unsigned int z);
      void build(const std::vector<Triangle> triangles);
      void build(std::string meshFilePath);
      void voxelizeTriangle(const Triangle& triangle);
      unsigned int countSetVoxels();
      void printBinary();
      void writeImages();
      bool cacheExists(std::string fileName);
      void writeVoxelCache(std::string fileName);
      std::string getFileNameFromPath(std::string fileName);
      
   //Will be
   //public:
      Voxels(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles, std::string meshFilePath);
      ~Voxels();
      uint64_t& operator[](unsigned int i);
      
      unsigned long getSize() const;
      
};

void binaryToString(uint64_t data, char* str);
unsigned int countSetBits(uint64_t data);

#endif

