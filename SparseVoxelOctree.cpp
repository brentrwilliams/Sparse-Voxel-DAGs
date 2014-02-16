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
/*
void SparseVoxelOctree::build(const std::vector<Triangle> triangles)
{
   Voxels* leafVoxels = new Voxels(levels, boundingBox, triangles);
   uint64_t* leafVoxelData = leafVoxels->data;
   unsigned int dataSize = leafVoxels->dataSize;
   
   //Make the SVO leaf nodes and their parents
   int currentLevel = levels-2;
   unsigned int currentLevelNodeCount = pow(8,currentLevel);
   SVONode* leafParentNodes = new SVONode[currentLevelNodeCount];
   unsigned int i;
   bool childMask[8];
   unsigned int currentParent = 0;
   unsigned int currentChild = 0;
   uint64_t* newLeafData = new uint64_t[dataSize];
   
   for (i = 0; i < dataSize; i++)
   {
      newLeafData[i] = leafVoxelData[i];
      leafParentNodes[currentParent].childPointers[currentChild] = (void*) &newLeafData[i];
      
      childMask[currentChild] = newLeafData[i] > 0;
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
   
   for (; currentLevel >= 0; currentLevel--)
   {
      buildLevel(currentLevel, childNodes, &newParentNodes);
      childNodes = newParentNodes;
   }
   root = newParentNodes;
}*/
/*
void SparseVoxelOctree::build(const std::vector<Triangle> triangles)
{
   root = new SVONode;
   bool childMaskBools[8] = {true, false, false, false, false, false, false, false};
   setChildMask(childMaskBools, &(root->childMask));
   uint64_t* leafNode = new uint64_t;
   *leafNode = 18446744073709551615L;
   root->childPointers[0] = leafNode;
   
   int i;
   std::cout << "\nMask bits: ";
   for (i = 0; i < 8; i++)
   {
      if (isMaskSet(root, i))
         std::cout << 1;
      else
         std::cout << 0;
   }
   std::cout << "\n\n";
}*/

void SparseVoxelOctree::build(const std::vector<Triangle> triangles)
{
   Voxels* leafVoxels = new Voxels(levels, boundingBox, triangles);
   uint64_t* leafVoxelData = leafVoxels->data;
   unsigned int dataSize = leafVoxels->dataSize;
   
   std::cout << "Number of leaf nodes: " << dataSize << "\n";
   
   bool childMask[8];
   root = new SVONode;
   for (int i = 0; i < dataSize; i++)
   {
      childMask[i] = leafVoxelData[i] > 0;
      if (childMask[i])
      {
         root->childPointers[i] = (void *)  &(leafVoxelData[i]);
      }
      else
      {
         root->childPointers[i] = NULL;
      }
   }
   setChildMask(childMask, &(root->childMask));
   
   
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
      parentNodes[currentParent].childPointers[currentChild] = (void*) &childNodes[i];
      currentChild = (currentChild+1) % 8;
      
      if (currentChild == 0)
      {
         setChildMask(childMask, &(parentNodes[currentParent].childMask));
         currentParent++;
      }
   }
   
   *newParentNodes = parentNodes;
}

/**
 * Sets the given child mask according to the array of 8 bools.
 *
 * Tested: 1-27-2014 
 */
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

/**
 * Returns a boolean indicating whether the node's childmask at the ith bit is 
 * set.
 *
 * Tested: 1-27-2014 
 */
bool SparseVoxelOctree::isMaskSet(SVONode* node, unsigned int i)
{
   uint64_t toAnd = (1L << i);
   return (node->childMask & toAnd) > 0; 
}


/**
 * Returns a boolean indicating whether the node's lead at the ith bit is set.
 *
 * Tested: 1-27-2014 
 */
bool SparseVoxelOctree::isLeafSet(uint64_t* node, unsigned int i)
{
   uint64_t leaf = (uint64_t) *node;
   uint64_t toAnd = (1L << i);
   return (leaf & toAnd) > 0; 
}

bool SparseVoxelOctree::isSet(unsigned int x, unsigned int y, unsigned int z)
{
   void* currentNode = root;
   int currentLevel = levels;
   
   unsigned int octreeIndex = mortonCode(x,y,z,currentLevel);
   unsigned int childIndex;
   unsigned int leafIndex;
   
   childIndex = octreeIndex / pow(8, currentLevel-1);
   if (!isMaskSet((SVONode*)currentNode, childIndex)) 
   {
      return false;
   }
   
   /*while (currentLevel > 3)
   {
      childIndex = octreeIndex / pow(8, currentLevel-1);
      if (!isMaskSet((SVONode*)currentNode, childIndex)) 
      {
         return false;
      }
      
      currentNode = (void*) ((SVONode*) currentNode)->childPointers[childIndex];
      currentLevel--;
   }*/
   
   uint64_t* leafPtr = (uint64_t*) ((SVONode*) currentNode)->childPointers[childIndex];
   leafIndex = octreeIndex - (childIndex * pow(8, currentLevel)); /// ***???***
   return isLeafSet(leafPtr, leafIndex);
}


void SparseVoxelOctree::printBinary()
{
   unsigned int i, j, k;
   
   for (k = 0; k < dimension; k++)
   {  
      std::cout << "z = " << k << "\n";
      for (j = 0; j < dimension; j++)
      {
         for (i = 0; i < dimension; i++)
         {
            if (isSet(i,j,k))
               std::cout << "1 ";
            else
               std::cout << "0 ";
         }
         std::cout << "\n";
      }
      std::cout << "\n";
   }
}




























