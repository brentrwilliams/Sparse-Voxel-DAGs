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
 * Build the SVO that works for >= 4 levels.
 *
 * Tested: 2-16-2014 
 */
void SparseVoxelOctree::build(const std::vector<Triangle> triangles)
{
   Voxels* leafVoxels = new Voxels(levels, boundingBox, triangles);
   uint64_t* leafVoxelData = leafVoxels->data;
   unsigned int numLeafs = leafVoxels->dataSize;
   
   std::cout << "levels: " << levels << "\n";
   std::cout << "Number of leaf nodes: " << numLeafs << "\n";
   
   if (levels <= 3)
   {
      std::cerr << "CANNOT build SVO with levels <= 3.\nExitting...\n";
      exit(EXIT_FAILURE);
   }
   
   int numPrevLevelNodes = numLeafs; //512
   int numCurrLevelNodes = numPrevLevelNodes / 8; //64
   SVONode* prevLevelNodes;
   SVONode* currLevelNodes = new SVONode[numCurrLevelNodes];
   std::cout << "numCurrLevelNodes: " << numCurrLevelNodes << "\n";
   
   // Set the level above the leaf nodes
   for (int i = 0; i < numPrevLevelNodes; i++)
   {
      if (leafVoxelData[i] > 0)
      {
         currLevelNodes[i/8].childPointers[i%8] = (void *)  &(leafVoxelData[i]);
      }
   }
   
   // Set the rest of the non leaf nodes
   while(numCurrLevelNodes >= 64)
   {
      numPrevLevelNodes = numCurrLevelNodes; //64
      numCurrLevelNodes /= 8; // 8
      prevLevelNodes = currLevelNodes;
      currLevelNodes = new SVONode[numCurrLevelNodes];
      std::cout << "numCurrLevelNodes: " << numCurrLevelNodes << "\n";
      
      // For each of the previous level's nodes we set the child pointers
      for (int i = 0; i < numPrevLevelNodes; i++)
      {
         if (isNodeNotEmpty(&prevLevelNodes[i]))
         {
            currLevelNodes[i/8].childPointers[i%8] = (void *)  &(prevLevelNodes[i]);
         }
      }
   }
   
   // Set the root node
   prevLevelNodes = currLevelNodes;
   root = new SVONode;
   for (int i = 0; i < 8; i++)
   {
      if (isNodeNotEmpty(&prevLevelNodes[i]))
      {
         root->childPointers[i] = (void *)  &(prevLevelNodes[i]);
      }
   }
}


/**
 * Returns a boolean indicating whether all the node's children are not empty.
 *
 * Tested: 1-16-2015
 */
bool SparseVoxelOctree::isNodeNotEmpty(SVONode *node)
{
   for (int i = 0; i < 8; i++)
   {
      if (node->childPointers[i] != NULL)
      {
         return true;
      }
   }

   return false;
}


/**
 * Returns a boolean indicating whether the node's leaf at the ith bit is set.
 *
 * Tested: 1-27-2014 
 */
bool SparseVoxelOctree::isLeafSet(uint64_t* node, unsigned int i)
{
   uint64_t leaf = (uint64_t) *node;
   uint64_t toAnd = (1L << i);
   return (leaf & toAnd) > 0; 
}

/**
 * Returns a boolean indicating whether the node's child is set.
 *
 * Tested: 1-16-2015 
 */
bool SparseVoxelOctree::isChildSet(SVONode *node, unsigned int i)
{
   return node->childPointers[i] != NULL;
}


/**
 * Returns a boolean indicating whether the voxel at the given coordinate is 
 * set.
 *
 * Tested: 2-16-2014 
 */
bool SparseVoxelOctree::isSet(unsigned int x, unsigned int y, unsigned int z)
{
   void* currentNode = root;
   int currentLevel = levels;
   unsigned int mortonIndex = mortonCode(x,y,z,currentLevel);
   
   int divBy = pow(8, currentLevel-1);
   int modBy = divBy;
   int index = mortonIndex / divBy;
   
   while (divBy >= 64)
   {
      if (!isChildSet((SVONode*)currentNode, index)) 
      {
         return false;
      }
      currentNode = (void*) ((SVONode*)currentNode)->childPointers[index];
      modBy = divBy;
      divBy /= 8;
      index = (mortonIndex % modBy) / divBy;
   }
   index = mortonIndex % modBy;
   return isLeafSet((uint64_t*)currentNode, index);
}


/**
 * Returns a boolean indicating whether the voxel at the given coordinate is 
 * set.
 *
 * Tested: 2-16-2014 
 */
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


/**
 * Writes the voxel data to tga files where the file name is the z axis voxel number.
 *
 * Tested: 12-15-2014
 */
void SparseVoxelOctree::writeImages()
{
   unsigned int i, j, k;
   
   for (k = 0; k < dimension; k++)
   {  
      cout << "Witing image " << k << endl;
      Image image(dimension,dimension);
      for (j = 0; j < dimension; j++)
      {
         for (i = 0; i < dimension; i++)
         {
            if (isSet(i,j,k))
            {
               image.setColor(i,j, 1.0f,1.0f,1.0f);
            }
         }
      }
      char fileName[30];
      sprintf(fileName, "./images/%04d.tga",k);
      image.writeTGA((fileName));
   }
}



























