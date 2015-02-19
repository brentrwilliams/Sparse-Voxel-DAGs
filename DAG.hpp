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
#include <algorithm>
#include <unordered_map>

#include <vector>
#include <stdint.h>

#define SET_8_BITS 255

class DAG 
{
   public:
      DAG(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles);
      ~DAG();
      void build(const std::vector<Triangle> triangles);
      bool isSet(unsigned int x, unsigned int y, unsigned int z);
      void* getChildPointer(void* node, unsigned int index);
      bool isLeafSet(uint64_t* node, unsigned int i);
      bool isChildSet(void* node, unsigned int i);
      void writeImages();
      void printLevels();
      unsigned int getNumChildren(void* node);
      void printMask(void* node);
      bool isSetSVO(unsigned int x, unsigned int y, unsigned int z);
      bool isSVOChildSet(SVONode *node, unsigned int i);
      void writeSVOImages();
      void printSVOLevels();

      BoundingBox boundingBox;
      unsigned int numLevels;
      unsigned long size; // Total number of voxels if the SVO was full
      unsigned int dimension; // Number of voxels for one side of the cube
      float voxelWidth; // The length of one voxel in world space
      //SVONode* root;
      void* root;
      void* svoRoot;
      void** levels;
      void** newLevels; //SVO levels
      unsigned int * sizeAtLevel; // Number nodes at a level
      
};

#endif
