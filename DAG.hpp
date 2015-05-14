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
#include "Traceable.hpp"
#include "Ray.hpp"
#include "AABB.hpp"
#include "MortonCode.hpp"
#include "PhongMaterial.hpp"
#include <algorithm>
#include <unordered_map>
#include "tbb/concurrent_unordered_map.h"

#include <vector>
#include <stdint.h>
#include <cfloat>
#include <string>

#define SET_8_BITS 255

class DAG : public Traceable
{
   public:
      DAG(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles, std::string meshFilePath, std::vector<PhongMaterial> materialsVal);
      ~DAG();
      void build(const std::vector<Triangle> triangles, std::string meshFilePath);
      void buildMoxelTable(const std::vector<Triangle> triangles);
      bool isSet(unsigned int x, unsigned int y, unsigned int z);
      void* getChildPointer(void* node, unsigned int index, unsigned int level);
      bool isLeafSet(uint64_t* node, unsigned int i);
      bool isChildSet(void* node, unsigned int i);
      void writeImages();
      void printLevels();
      unsigned int getNumChildren(void* node);
      uint64_t getNumFilledVoxels();
      void printMask(void* node);
      void printSVOMask(SVONode* node);
      bool isSetSVO(unsigned int x, unsigned int y, unsigned int z);
      bool isSVOChildSet(SVONode *node, unsigned int i);
      void writeSVOImages();
      void printSVOLevels();
      uint64_t getNumEmptyLeafNodes(uint64_t leafNode);
      uint64_t getEmptyCount(void* node, unsigned int index);
      uint64_t getLeafNodeEmptyCount(uint64_t leafNode, unsigned int index);
      uint64_t getLevelIndexSum(unsigned int level, unsigned int index);
      void getNormalFromMoxelTable(uint32_t index, glm::vec3& normal, unsigned int& materialIndex);
      bool intersect(const Ray& ray, float& t, glm::vec3& normal, uint64_t& moxelIndex);
      bool intersect(const Ray& ray, float& t, void* node, unsigned int level, AABB aabb, glm::vec3& normal, uint64_t& moxelIndex);
      void getEmptyCount(void* node, uint64_t* expected);
      string getMemorySize(unsigned int size);

      BoundingBox boundingBox;
      unsigned int numLevels;
      unsigned long size; // Total number of voxels if the SVO was full
      unsigned int dimension; // Number of voxels for one side of the cube
      float voxelWidth; // The length of one voxel in world space
      uint64_t numFilledVoxels;
      //SVONode* root;
      void* root;
      void* svoRoot;
      void** levels;
      void** newLevels; //SVO levels
      unsigned int * sizeAtLevel; // Number nodes at a level
      void* moxelTable;
      tbb::concurrent_unordered_map<unsigned int, unsigned int>* voxelTriangleIndexMap;
      std::vector<PhongMaterial> materials;
      
};

#endif
