/**
 * DAG.cpp
 * 
 * by Brent Williams
 */

#include "DAG.hpp"

DAG::DAG(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles, std::string meshFilePath)
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
   
   levels = new void*[numLevels-1]; // has the -1 because the last two levels are uint64's 
   sizeAtLevel = new unsigned int[numLevels-1](); // has the -1 because the last two levels are uint64's 
   newLevels = new void*[numLevels-1]; // has the -1 because the last two levels are uint64's 
   boundingBox.square();
   voxelWidth = (boundingBox.maxs.x - boundingBox.mins.x) / dimension;
   build(triangles, meshFilePath);
}

DAG::~DAG()
{
}

/**
 * Builds the DAG from a SVO
 * Tested: 
 */

void DAG::build(const std::vector<Triangle> triangles, std::string meshFilePath)
{
   SparseVoxelOctree* svoPtr = new SparseVoxelOctree(numLevels, boundingBox, triangles, meshFilePath);
   SparseVoxelOctree svo = *svoPtr;
   svoRoot = svo.root;
   unsigned int* newLevelSizes = new unsigned int[numLevels-1]();
   unsigned int newLevelIndex = numLevels-2;

   cerr << "Levels: " << endl;
   for (int i = 0; i < 2; ++i)
   {
      cerr << "\t" << i << ": " << svo.countAtLevel(i) << endl;
   }

   unsigned int numLeafs = size / 64;
   unsigned int sizeOfLeafs = numLeafs * sizeof(uint64_t);
   uint64_t* leafVoxels = (uint64_t*) svo.levels[numLevels-2]; // numLevels -2 b/c last 2 levels are together
   uint64_t* copyLeafVoxels = new uint64_t[numLeafs];
   unsigned int numChildren = numLeafs;
   unsigned int updateCount = 0;

   if (copyLeafVoxels == NULL)
   {
      cerr << "Unable to allocate copyLeafVoxels\n";
   }
   cerr << endl;

   int parentLevelNum = numLevels-3;
   SVONode* parentLevel = (SVONode*) svo.levels[parentLevelNum];
   unsigned int numParents = numLeafs / 8;

   std::cerr << "Starting leaf level (parentLevelNum: " << parentLevelNum << ")" << endl;
   std::cerr << "numParents: " << numParents << endl;
   std::cerr << "numChildren: " << numChildren << endl << endl;

   cerr << "\tSorting leaf nodes..." << endl;
   memcpy(copyLeafVoxels, leafVoxels, sizeOfLeafs);
   std::sort(copyLeafVoxels, copyLeafVoxels + numLeafs);
   cerr << "\tFinished sorting leaf nodes." << endl << endl;

   // Calculate the number of unique leafs
   cerr << "\tReducing leaf nodes..." << endl;
   unsigned int numUniqueLeafs = 1;
   for (unsigned int i = 0; i < numLeafs-1; ++i)
   {
      if (copyLeafVoxels[i] != copyLeafVoxels[i+1])
      {
         numUniqueLeafs++;
      }
   }
   std::cerr << "\t\tnumUniqueLeafs: " << numUniqueLeafs << endl;
   
   // Allocate the number of unique leafs and make copies of them
   uint64_t* uniqueLeafs = new uint64_t[numUniqueLeafs];
   unsigned int uniqueLeafIndex = 1;
   uniqueLeafs[0] = copyLeafVoxels[0];
   newLevels[newLevelIndex] = uniqueLeafs;
   newLevelSizes[newLevelIndex] = numUniqueLeafs;
   cout << "newLevels at leafs (newLevelIndex = " << newLevelIndex << ")" << endl;
   newLevelIndex--;

   for (unsigned int i = 0; i < numLeafs-1; ++i)
   {
      if (copyLeafVoxels[i] != copyLeafVoxels[i+1])
      {
         uniqueLeafs[uniqueLeafIndex] = copyLeafVoxels[i+1];
         uniqueLeafIndex++;
      }
   }

   cout << "\nuniqueLeafs: " << endl;
   for (unsigned int i = 0; i < numUniqueLeafs; ++i)
   {
      std::cout << uniqueLeafs[i] << " => " << &(uniqueLeafs[i]) << "\n";
   }
   std::cout << endl << endl; 

   // Free the copy of the leafs
   delete [] copyLeafVoxels;
   cerr << "\tFinished reducing leaf nodes." << endl << endl;

   // Adjust the pointers of the level above in the svo
   cerr << "\tAdjusting parent node's pointer's to unique children..." << endl; 

   updateCount = 0;
   for (unsigned int i = 0; i < numParents; i++)
   {
      for (unsigned int j = 0; j < 8; j++)
      {
         if (parentLevel[i].childPointers[j] != NULL)
         {
            uint64_t childValue = *((uint64_t*)parentLevel[i].childPointers[j]);
            bool foundUpdate = false;
            // Search for the corresponding child value in the uniqueLeafs and replace the 
            // childpointer with a pointer to the unique leaf
            for (unsigned int k = 0; k < numUniqueLeafs && childValue != 0; k++)
            {
               if (uniqueLeafs[k] == childValue)
               {
                  parentLevel[i].childPointers[j] = &(uniqueLeafs[k]);
                  updateCount++;
                  cerr << "\t\tUpdate: " << updateCount << "\t=> "  << childValue << " &=> " << &(uniqueLeafs[k]) << endl;
                  foundUpdate = true;
               }
            }

            if (!foundUpdate)
            {
               cout << "!!!!!!!!! Did not find update for " << childValue << endl;
            }
         }
      }
   }
   cerr << "\tFinished adjusting parent node's pointer's to unique children." << endl << endl;
   std::cerr << "Finished leaf level (parentLevelNum: " << parentLevelNum << ")" << endl << endl << endl;

   parentLevelNum--;

////////////////////////////////////////////////////////////////////////////////////////////////
   //Adjust the pointers of the rest of the levels in the svo
   SVONode* childLevel;

   while (parentLevelNum >= 0)
   {
      childLevel = parentLevel;
      parentLevel = (SVONode*) svo.levels[parentLevelNum];
      numChildren = numParents;
      numParents /= 8;
      unsigned int sizeofChildren = sizeof(SVONode) * numChildren;

      std::cerr << "Starting parentLevelNum: " << parentLevelNum << endl;
      std::cerr << "numParents: " << numParents << endl;
      std::cerr << "numChildren: " << numChildren << endl << endl;

      // Sort the child level based on the pointers of each child node
      cerr << "\tSorting child nodes..." << endl;
      SVONode* copyChildNodes = new SVONode[numChildren];
      memcpy(copyChildNodes, childLevel, sizeofChildren);
      std::sort(copyChildNodes, copyChildNodes + numChildren);

      cout << "copyChildNodes (at parentLevel " << parentLevelNum << "): " << endl;
      for (unsigned int i = 0; i < numChildren; i++)
      {
         cout << &copyChildNodes[i] << ": ";
         copyChildNodes[i].printOneLine();
      }
      cout << endl;

      cerr << "\tFinished sorting child nodes." << endl << endl;

      //Reducing child nodes
      cerr << "\tReducing child nodes..." << endl;
      unsigned int numUniqueChildren = 1;
      for (unsigned int i = 0; i < numChildren-1; ++i)
      {
         if (copyChildNodes[i] != copyChildNodes[i+1])
         {
            numUniqueChildren++;
         }
      }
      cerr << "\t\tnumUniqueChildren: " << numUniqueChildren << endl;

      SVONode* uniqueChildren = new SVONode[numUniqueChildren];
      unsigned int uniqueChildIndex = 0;
      memcpy(&uniqueChildren[uniqueChildIndex], &copyChildNodes[0], sizeof(SVONode));
      uniqueChildIndex++;
      newLevels[newLevelIndex] = uniqueChildren;
      newLevelSizes[newLevelIndex] = numUniqueChildren;
      cout << "newLevels at newLevelIndex = " << newLevelIndex << endl;
      newLevelIndex--;
      for (unsigned int i = 0; i < numChildren-1; ++i)
      {
         if (copyChildNodes[i] != copyChildNodes[i+1])
         {
            memcpy(&uniqueChildren[uniqueChildIndex], &copyChildNodes[i+1], sizeof(SVONode));
            uniqueChildIndex++;
         }
      }
      delete [] copyChildNodes;

      cerr << "\tFinished reducing child nodes." << endl << endl;

      // Reducing child nodes
      cerr << "\tAdjusting parent node's pointer's to unique children..." << endl;
      updateCount = 0;
      bool foundUpdate = false;
      for (unsigned int i = 0; i < numParents; i++)
      {
         for (unsigned int j = 0; j < 8; j++)
         {
            if (parentLevel[i].childPointers[j] != NULL)
            {
               // Get a pointer to the address of the child value so you can replace it with the 
               // new unique child pointer
               SVONode childValue = *((SVONode*) parentLevel[i].childPointers[j]);

               // Search for the corresponding child value in the uniqueChildren and replace the 
               // childpointer with a pointer to the unique leaf
               for (unsigned int k = 0; k < numUniqueChildren; k++)
               {
                  if (uniqueChildren[k] == childValue)
                  {
                     parentLevel[i].childPointers[j] = &(uniqueChildren[k]);
                     updateCount++;
                     cerr << "\t\tUpdate: " << updateCount << " &=> " << &(uniqueChildren[k]) << endl;
                     foundUpdate = true;
                  }
               }
               if (!foundUpdate)
               {
                  cout << "!!!!!!!!! Did not find update for ";
                  childValue.printOneLine();
               }
            }
         }
      }

      cerr << "\tFinished adjusting parent node's pointer's to unique children." << endl << endl;

      std::cerr << "Finished parentLevelNum: " << parentLevelNum << endl << endl << endl;
      parentLevelNum--;
   }

   // Update the newLevels to include the root
   SVONode* newRoot = new SVONode;
   memcpy(newRoot, svo.levels[0], sizeof(SVONode));
   newLevels[0] = newRoot;
   svoRoot = newRoot;
   newLevelSizes[0] = 1;

   cerr << "After updated SVO root:" << endl; 
   for (int i = 0; i < 8; ++i)
   {
      cerr << "\t" << newRoot->childPointers[i] << endl;
   }
   cerr << endl;

   cout << "Num unique nodes at level:" << endl;
   for (unsigned int i = 0; i < numLevels-1; ++i)
   {
      cout << "\t" << i << ": " << newLevelSizes[i] << endl;
   }

   // Go through each level and count the number of nonvoid pointers
   unsigned int levelIndex;
   for (levelIndex = 0; levelIndex < numLevels-2; levelIndex++)
   {
      unsigned int pointerCount = 0;
      for (unsigned int i = 0; i < newLevelSizes[levelIndex]; i++)
      {
         for (int j = 0; j < 8; j++)
         {
            if ( ((SVONode*) newLevels[levelIndex])[i].childPointers[j] != NULL)
            {
               pointerCount++;
            }
         }
      }

      // A level is made up of the pointers in the nodes and the masks of the nodes
      levels[levelIndex] = (void*)malloc((pointerCount * sizeof(void*)) + (newLevelSizes[levelIndex] * sizeof(uint64_t)));
      cerr << levelIndex << ": " << ((pointerCount * sizeof(void*)) + (newLevelSizes[levelIndex] * sizeof(uint64_t))) / 8 << " uint64_t's or void*'s" << endl;
      sizeAtLevel[levelIndex] = newLevelSizes[levelIndex];//((pointerCount * sizeof(void*)) + (newLevelSizes[levelIndex] * sizeof(uint64_t))) / 8;
   }
   cerr << levelIndex << " (leafs): " << numUniqueLeafs << " uint64_t's or void*'s" << endl << endl;
   sizeAtLevel[levelIndex] = numUniqueLeafs;

   // Just use the already allocated leafs of the compacted SVO instead of allocating a new one
   levels[numLevels-2] = newLevels[numLevels-2];

   // Set the masks and pointers of the level above the leafs
   unsigned int currentLevelIndex = numLevels-3;
   cout << "Working at level above the leafs: " << currentLevelIndex << endl;
   uint64_t* maskPtr;
   uint64_t** currPtr = (uint64_t**) levels[currentLevelIndex];
   unordered_map<void*, void*>* leafParentMapping = new unordered_map<void*, void*>;
   for (unsigned int i = 0; i < newLevelSizes[currentLevelIndex]; i++)
   {
      uint64_t mask = 0;
      maskPtr = (uint64_t*) currPtr;
      leafParentMapping->insert( std::make_pair<void*,void*>( (void*) &((SVONode*) newLevels[currentLevelIndex])[i], (void*)maskPtr ) );
      cout << "Adding to Map: " << "( " <<  (void*) &((SVONode*) newLevels[currentLevelIndex])[i] << ", " << (void*)maskPtr << " )" << endl;
      currPtr++;
      for (int j = 0; j < 8; j++)
      {
         if ( ((SVONode*) newLevels[currentLevelIndex])[i].childPointers[j] != NULL)
         {
            uint64_t toOr = 1 << j;
            // It is the same pointer as in the SVONodes because we are using the same leaf nodes
            *currPtr = (uint64_t*) ((SVONode*) newLevels[currentLevelIndex])[i].childPointers[j];
            mask |= toOr;
            currPtr++;
         }
      }
      *maskPtr = mask;
   }

   unordered_map<void*, void*>* currLevelsMap = leafParentMapping;
   unordered_map<void*, void*>* prevLevelsMap = NULL;
   
   // For each levels nodes: set the mask 
   for (int levelIndex = numLevels-4; levelIndex >= 0; levelIndex--)
   {
      cout << "Working at level: " << levelIndex << endl;
      cerr << "Level " << levelIndex << endl;
      // Create a new map to 
      delete prevLevelsMap;
      prevLevelsMap = currLevelsMap;
      currLevelsMap = new unordered_map<void*, void*>;

      currPtr = (uint64_t**) levels[levelIndex];

      for (unsigned int i = 0; i < newLevelSizes[levelIndex]; i++)
      {
         uint64_t mask = 0;
         maskPtr = (uint64_t*) currPtr;
         currLevelsMap->insert( std::make_pair<void*,void*>( (void*) &((SVONode*) newLevels[levelIndex])[i], (void*)maskPtr ) );
         cout << "\tAdding to the map: ( " << &((SVONode*) newLevels[levelIndex])[i] << ", " << (void*)maskPtr << " )" << endl;
         currPtr++;
         
         cerr << "\t";
         for (int j = 0; j < 8; j++)
         {
            if ( ((SVONode*) newLevels[levelIndex])[i].childPointers[j] != NULL)
            {
               uint64_t toOr = 1 << j;
               cout << "Searching for[" << j << "]: " << ((SVONode*) newLevels[levelIndex])[i].childPointers[j] << endl;
               *currPtr = (uint64_t*) prevLevelsMap->at( ((SVONode*) newLevels[levelIndex])[i].childPointers[j] );
               cout << "\tReturned value: " << *currPtr << endl;
               mask |= toOr;
               cerr << "1";
               currPtr++;
            }
            else 
            {
               cerr << "0";
            }
         }
         cerr << endl;
         cerr << "\tmask(" << maskPtr << "): " << mask << endl << endl;
         *maskPtr = mask;
      }
      cout << endl;
   }
   root=levels[0];

   cout << "\nsizeAtLevel: " << endl;
   for (unsigned int i = 0; i < numLevels-1; ++i)
   {
      cout << i << ": " << sizeAtLevel[i] << endl;
   }
   cout << endl;
}


/**
 * Returns a boolean indicating whether the voxel, in the reduced SVO, at the given coordinate is 
 * set.
 *
 * Tested: 
 */
bool DAG::isSetSVO(unsigned int x, unsigned int y, unsigned int z)
{
   void* currentNode = svoRoot;
   int currentLevel = numLevels;
   unsigned int mortonIndex = mortonCode(x,y,z,currentLevel);
   
   int divBy = pow(8, currentLevel-1);
   int modBy = divBy;
   int index = mortonIndex / divBy;
   
   while (divBy >= 64)
   {
      if (!isSVOChildSet((SVONode*)currentNode, index)) 
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
 * Returns a boolean indicating whether the node's child is set.
 *
 * Tested: 1-16-2015 
 */
bool DAG::isSVOChildSet(SVONode *node, unsigned int i)
{
   return node->childPointers[i] != NULL;
}

bool DAG::isSet(unsigned int x, unsigned int y, unsigned int z)
{
   void* currentNode = levels[0];
   int currentLevel = numLevels;
   unsigned int mortonIndex = mortonCode(x,y,z,currentLevel);
   
   int divBy = pow(8, currentLevel-1);
   int modBy = divBy;
   int index = mortonIndex / divBy;
   
   while (divBy >= 64)
   {
      if (!isChildSet(currentNode, index)) 
      {
         return false;
      }
      currentNode = (void*) getChildPointer(currentNode, index);
      modBy = divBy;
      divBy /= 8;
      index = (mortonIndex % modBy) / divBy;
   }
   index = mortonIndex % modBy;
   return isLeafSet((uint64_t*)currentNode, index);
}


void* DAG::getChildPointer(void* node, unsigned int index)
{
   void** pointer = (void**)node;
   pointer++;
   for (unsigned int i = 0; i < index; i++)
   {
      if (isChildSet(node, i))
      {
         pointer++;
      }
   }

   //cout << "Getting childpointer " << index << " for node " << node << " = " << *pointer << endl;
   return *pointer;  
}


void DAG::printLevels() 
{
   void* node;
   void** pointer;

   // levels above the leafs
   cout << "DAG:" << endl;
   unsigned int levelIndex;
   for (levelIndex = 0; levelIndex < numLevels-2; levelIndex++)
   {
      node = levels[levelIndex];
      pointer = (void**)node;
      cout << "Level " << levelIndex << ":" << endl;

      for (unsigned int i = 0; i < sizeAtLevel[levelIndex]; ++i)
      {
         cout << i << ": " << node << " (";
         printMask(node);
         cout << "): ";
         for (unsigned int i = 0; i < 8; i++)
         {
            if (isChildSet(node, i))
            {
               pointer++;
               cout << *pointer << " ";
            }
         }
         cout << endl;
         pointer++;
         node = (void*)pointer;
      }
      cout << endl;
   }

   // Print the leafs
   node = levels[levelIndex];
   pointer = (void**)node;
   cout << "Level " << levelIndex << ":" << endl;

   for (unsigned int i = 0; i < sizeAtLevel[levelIndex]; ++i)
   {
      cout << i << ": " << node << " = " << *((uint64_t*)pointer);
      cout << endl;
      pointer++;
      node = (void*)pointer;
   }
   cout << endl << endl;
}

void DAG::printSVOLevels()
{
   unsigned int levelIndex;
   SVONode* nodes;
   cout << "Compacted SVO:" << endl << endl;

   for (levelIndex = 0; levelIndex < numLevels-2; levelIndex++)
   {
      cout << "Level " << levelIndex << ":" << endl;
      nodes = (SVONode*) newLevels[levelIndex];
      for (unsigned int i = 0; i < sizeAtLevel[levelIndex]; i++)
      {
         cout << i << ": " << &nodes[i] << " (";
         printSVOMask(&nodes[i]);
         cout << "): ";
         for (unsigned int j = 0; j < 8; j++)
         {
            if (isSVOChildSet(&nodes[i], j))
            {
               cout << nodes[i].childPointers[j] << " ";
            }
         }
         cout << endl;
      }
      cout << endl;
   }

   cout << "Level " << levelIndex << ":" << endl;
   uint64_t* leafs = (uint64_t*)newLevels[levelIndex];
   for (unsigned int i = 0; i < sizeAtLevel[levelIndex]; i++)
   {
      cout << i << ": " << &leafs[i] << " = " << leafs[i] << endl;
   }
}



void DAG::printMask(void* node)
{
   uint64_t mask = *((uint64_t*)node);
   for (unsigned int i = 0; i < 8; i++)
   {
      if (isChildSet(node,i))
      {
         cout << "1";
      }
      else
      {
         cout << "0";
      }
   }

   cout << " " << mask;
}


void DAG::printSVOMask(SVONode* node)
{
   for (unsigned int i = 0; i < 8; i++)
   {
      if (isSVOChildSet(node,i))
      {
         cout << "1";
      }
      else
      {
         cout << "0";
      }
   }
}


/**
 * Returns a boolean indicating whether the node's leaf at the ith bit is set.
 *
 * Tested: 
 */
bool DAG::isLeafSet(uint64_t* node, unsigned int i)
{
   uint64_t leaf = (uint64_t) *node;
   uint64_t toAnd = (1L << i);
   return (leaf & toAnd) > 0; 
}


/**
 * Returns a boolean indicating whether the node's child is set.
 *
 * Tested: 
 */
bool DAG::isChildSet(void* node, unsigned int i)
{
   uint64_t toAnd = 1L << i;
   uint64_t mask = *((uint64_t*) node);
   //cout << "\t\t(" << mask <<  " & " << toAnd << ") = " << (mask | toAnd) << endl;
   return (mask & toAnd) != 0;
}



/**
 * Writes the voxel data to tga files where the file name is the z axis voxel number.
 *
 * Tested: 
 */
void DAG::writeImages()
{
   unsigned int i, j, k;
   
   for (k = 0; k < dimension; k++)
   {  
      cerr << "Witing image " << k << endl;
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
 * Writes the voxel data to tga files where the file name is the z axis voxel number.
 *
 * Tested: 2/20/2015
 */
void DAG::writeSVOImages()
{
   unsigned int i, j, k;
   
   for (k = 0; k < dimension; k++)
   {  
      cerr << "Witing image " << k << endl;
      Image image(dimension,dimension);
      for (j = 0; j < dimension; j++)
      {
         for (i = 0; i < dimension; i++)
         {
            if (isSetSVO(i,j,k))
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


unsigned int DAG::getNumChildren(void* node)
{
   uint64_t mask = *((uint64_t*)node);
   unsigned int count = 0;
   for (int i = 0; i < 8; ++i)
   {
      uint64_t toOr = 1 << i;
      if (toOr | mask)
         count++;
   }
   return count;
}


/**
 * Returns whether the ray provided intersects the DAG and t the distance along the ray
 *
 * Tested: 
 */
bool DAG::intersect(const Ray& ray, float& t, glm::vec3& normal)
{
   glm::vec3 mins(boundingBox.mins.x, boundingBox.mins.y, boundingBox.mins.z);
   glm::vec3 maxs(boundingBox.maxs.x, boundingBox.maxs.y, boundingBox.maxs.z);
   AABB aabb(mins, maxs);
   return intersect(ray, t, root, 0, aabb, normal);
}

/**
 * Resursive intersection method that returns whether the ray provided intersects the DAG and t the
 * distance along the ray.
 *
 * Tested: 
 */
bool DAG::intersect(const Ray& ray, float& t, void* node, unsigned int level, AABB aabb, glm::vec3& normal)
{
   // Child values by index based on morton encoding
   glm::vec3 childOffsets[8] = { 
      glm::vec3(0, 0, 0),
      glm::vec3(1, 0, 0),
      glm::vec3(0, 1, 0),
      glm::vec3(1, 1, 0),
      glm::vec3(0, 0, 1),
      glm::vec3(1, 0, 1),
      glm::vec3(0, 1, 1),
      glm::vec3(1, 1, 1) };


   glm::vec3 mins = aabb.mins;
   glm::vec3 maxs = aabb.maxs;
   glm::vec3 uselessNormal;

   //cout << "\tTraversing level " << level << endl;


   // node is not a leaf node
   if (level < numLevels-2)
   {
      // If the parent node is hit
      if (aabb.intersect(ray,t,uselessNormal))
      {
         //cout << "\tNode hit." << endl;
         float newDim = (maxs.x - mins.x) / 2.0f;
         bool isHit = false;
         t = FLT_MAX;
         
         for (int i = 0; i < 8; i++)
         {
            if (isChildSet(node, i))
            {
               //cout << "\tChild " << i << " is set." << endl;
               glm::vec3 newMins(mins + (childOffsets[i] * newDim));
               glm::vec3 newMaxs(newMins.x + newDim, newMins.y + newDim, newMins.z + newDim);
               AABB newAABB(newMins, newMaxs);
               
               //cout << "\tNew AABB: ";
               //newAABB.print();
               //cout <<  "\tIntersecting with child..." << endl << endl;
               
               float newT;
               bool newHit = intersect(ray, newT, getChildPointer(node,i), level+1, newAABB, normal);
               //cout << "\n\tChild " << i << " hit: " << newHit << endl;
               
               if (newHit && newT < t)
                  t = newT;
               isHit = isHit || newHit;
            }  
         }

         return isHit;
      }
      // If the parent node is not hit
      else
      {
         return false;
      }
   }
   // else
   // {
   //    bool retVal = aabb.intersect(ray,t);
   //    cout << "\tAt leaf level" << endl;
   //    cout << "\tHit leaf:" << retVal << endl << endl;
   //    return retVal;
   // }
   // node is a leaf node
   else
   {
      float newDim = (maxs.x - mins.x) / 4.0f;
      t = FLT_MAX;
      bool isHit = false;

      // Go through each of the 64 child nodes stored in the given leaf
      for (unsigned int i = 0; i < 64; i++)
      {
         // If the leaf is not empty
         if (isLeafSet((uint64_t*)node, i))
         {
            unsigned int x, y, z;
            mortonCodeToXYZ((uint32_t)i, &x, &y, &z, 2);
            glm::vec3 offset((float)x,(float)y,(float)z);
            glm::vec3 newMins(mins + (offset * newDim));
            glm::vec3 newMaxs(newMins.x + newDim, newMins.y + newDim, newMins.z + newDim);
            AABB newAABB(newMins, newMaxs);
            float newT;
            glm::vec3 tempNormal;
            bool newHit = newAABB.intersect(ray,newT,tempNormal);

            if (newHit && newT < t)
            {
               t = newT;
               normal = tempNormal;
            }
            isHit = isHit || newHit;
         }
      }
      return isHit;
   }
}




































