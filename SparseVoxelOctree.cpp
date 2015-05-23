/**
 * SparseVoxelOctree.cpp
 * 
 * by Brent Williams
 */

#include "SparseVoxelOctree.hpp"

/**
 * Instantiates and initializes the SVO.
 */
SparseVoxelOctree::SparseVoxelOctree(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles, std::string meshFilePath)
 : boundingBox(boundingBoxVal),
   numLevels(levelsVal),
   size(pow(8, levelsVal)), 
   dimension(pow(2,levelsVal)),
   voxelWidth(0)
{
   if (numLevels <= 2)
   {
      std::string err("\nNumber of levels too small\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }
   
   boundingBox.square();
   voxelWidth = (boundingBox.maxs.x - boundingBox.mins.x) / dimension;
   levels = new void*[numLevels-1]; // has the -1 because the last two levels are uint64's 
   levelSizes = new unsigned int[numLevels-1]();
   build(triangles,meshFilePath);
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
void SparseVoxelOctree::build(const std::vector<Triangle> triangles, std::string meshFilePath)
{
   Voxels* leafVoxels = new Voxels(numLevels, boundingBox, triangles, meshFilePath);
   uint64_t* leafVoxelData = leafVoxels->data;
   unsigned int numLeafs = leafVoxels->dataSize;

   voxelTriangleIndexMap = leafVoxels->voxelTriangleIndexMap;
   
   // std::cout << "levels: " << numLevels << "\n";
   // std::cout << "Number of leaf nodes: " << numLeafs << "\n";

   levelSizes[numLevels-1] = numLeafs * sizeof(uint64_t);

   // Save the pointer to the leaf nodes
   unsigned int currentLevel = numLevels-2;
   levels[currentLevel] = (void*)leafVoxelData; 
   cerr << "Leaf Level = " << currentLevel << endl;
   
   
   if (numLevels <= 3)
   {
      std::cerr << "CANNOT build SVO with levels <= 3.\nExitting...\n";
      exit(EXIT_FAILURE);
   }
   
   int numPrevLevelNodes = numLeafs; //512
   int numCurrLevelNodes = numPrevLevelNodes / 8; //64
   SVONode* prevLevelNodes;
   SVONode* currLevelNodes; 
   currLevelNodes = new SVONode[numCurrLevelNodes](); 
   levelSizes[currentLevel] = 0;
   
   // Set the level above the leaf nodes
   for (int i = 0; i < numPrevLevelNodes; i++)
   {
      if (leafVoxelData[i] > 0)
      {
         currLevelNodes[i/8].childPointers[i%8] = (void *)  &(leafVoxelData[i]);
         levelSizes[currentLevel]++;
      }
   }

   currentLevel--;
   levels[currentLevel] = (void*)currLevelNodes;
   currentLevel--;
   
   // Set the rest of the non leaf nodes
   while(numCurrLevelNodes >= 64)
   {
      numPrevLevelNodes = numCurrLevelNodes; //64
      numCurrLevelNodes /= 8; // 8
      prevLevelNodes = currLevelNodes;
      currLevelNodes = new SVONode[numCurrLevelNodes];
      levelSizes[currentLevel] = 0;
      
      // For each of the previous level's nodes we set the child pointers
      for (int i = 0; i < numPrevLevelNodes; i++)
      {
         if (isNodeNotEmpty(&prevLevelNodes[i]))
         {
            currLevelNodes[i/8].childPointers[i%8] = (void *)  &(prevLevelNodes[i]);
            levelSizes[currentLevel+1]++;
         }
      }

      // Save the pointer for each level's nodes in the level's array
      levels[currentLevel] = currLevelNodes;
      currentLevel--;
   }
   
   // Set the root node
   prevLevelNodes = currLevelNodes;
   root = new SVONode;
   levelSizes[currentLevel] = 0;
   for (int i = 0; i < 8; i++)
   {
      if (isNodeNotEmpty(&prevLevelNodes[i]))
      {
         root->childPointers[i] = (void *)  &(prevLevelNodes[i]);
         levelSizes[currentLevel+1]++;
      }
   }

   // Save the pointer for root node in the levels array
   levels[currentLevel] = root;
   levelSizes[currentLevel] = 1;

   unsigned int memorySizeWithoutLeafs = 0;
   //cout << "SVO Size: " << endl;
   for (int i = 0; i < numLevels-2; ++i)
   {
      // cout << "[" << i << "]: " << levelSizes[i] * sizeof(SVONode) << endl;
      memorySizeWithoutLeafs += levelSizes[i] * sizeof(SVONode);
   }
   // cout << "[" << numLevels-2 << "]: " << levelSizes[numLevels-2] * sizeof(uint64_t) << endl;
   // cout << "Filled Voxels: (from SVO perspective): " << levelSizes[numLevels-2] << endl;

   sizeWithoutMaterials = memorySizeWithoutLeafs + ( levelSizes[numLevels-2] * sizeof(uint64_t) );

   //                                                normal          index into material table
   unsigned int sizeOfSVOLeafWithMaterials = ( 3 * sizeof(float) ) + (1 * sizeof(unsigned int) );
   uint64_t sizeWithMaterials = memorySizeWithoutLeafs + ( levelSizes[numLevels-2] * sizeOfSVOLeafWithMaterials );
   
   cout << "\nSVO (No Materials) Memory Size: " << sizeWithoutMaterials << " (" << getMemorySize(sizeWithoutMaterials) << ")" << endl;

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
   int currentLevel = numLevels;
   unsigned int mortonIndex = mortonCode(x,y,z,currentLevel);
   
   int divBy = pow(8.0f, (float)currentLevel-1);
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

/**
 * Given a level as input, it will return a number of active nodes in that level
 * Tested:
 */
unsigned int SparseVoxelOctree::countAtLevel(unsigned int level)
{
   unsigned int count = 0;
   if (level == 0)
      return 1;
   else if (level == 1)
   {
      SVONode curr = *root;
      for (int i = 0; i < 8; i++)
      {
         if (curr.childPointers[i] != NULL)
         {
            count++;
         } 
      }
   }
   return count;
}

string SparseVoxelOctree::getMemorySize(unsigned int size)
{
   string b = " B";
   string kb = " KB";
   string mb = " MB";
   string gb = " GB";
   string units[] = {b,kb,mb,gb};
   float currentSize = (float)size;
   float lastSize = (float)size;
   int i;

   for (i = 0; i < 4 && currentSize > 1.0f; ++i)
   {
      lastSize = currentSize;
      currentSize /= 1024.0f;
   }

   return to_string(lastSize) + units[i-1];
}






















