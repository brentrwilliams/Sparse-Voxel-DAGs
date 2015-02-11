/**
 * DAG.cpp
 * 
 * by Brent Williams
 */

#include "DAG.hpp"

DAG::DAG(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles)
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
   build(triangles);
}

DAG::~DAG()
{
}

/**
 * Builds the DAG from a SVO
 * Tested: 
 */

void DAG::build(const std::vector<Triangle> triangles)
{
   SparseVoxelOctree svo(numLevels, boundingBox, triangles);
   root = svo.root;

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
   // cerr << "Parent Level of LeafNodes = " << parentLevelNum << endl;
   // cerr << "levels[leafParentLevel] in DAG = " << (void*)parentLevel << endl;
   // SVONode temp = parentLevel[30];
   // temp.print();
   // cerr << "*(parentLevel[30].childPointers[0]) = " << *((uint64_t*)parentLevel[30].childPointers[0]) << endl;

   unsigned int numParents = numLeafs / 8;

   std::cerr << "Starting leaf level (parentLevelNum: " << parentLevelNum << ")" << endl;
   std::cerr << "numParents: " << numParents << endl;
   std::cerr << "numChildren: " << numChildren << endl << endl;

   cerr << "\tSorting leaf nodes..." << endl;
   memcpy(copyLeafVoxels, leafVoxels, sizeOfLeafs);
   std::sort(copyLeafVoxels, copyLeafVoxels + numLeafs);
   cerr << "\tFinished sorting leaf nodes." << endl << endl;
   
   cout << "\nLeafs:" << endl;
   for (unsigned int i = 0; i < numLeafs; ++i)
   {
      std::cout << copyLeafVoxels[i] << "\n";
   }
   std::cout << endl << endl;

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
   for (unsigned int i = 0; i < numLeafs-1; ++i)
   {
      if (copyLeafVoxels[i] != copyLeafVoxels[i+1])
      {
         uniqueLeafs[uniqueLeafIndex] = copyLeafVoxels[i+1];
         uniqueLeafIndex++;
      }
   }

   //std::cerr << "uniqueLeafIndex: " << uniqueLeafIndex << endl;

   cout << "\nuniqueLeafs: " << endl;
   for (unsigned int i = 0; i < numUniqueLeafs; ++i)
   {
      std::cout << uniqueLeafs[i] << "\n";
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

            // Search for the corresponding child value in the uniqueLeafs and replace the 
            // childpointer with a pointer to the unique leaf
            for (unsigned int k = 0; k < numUniqueLeafs && childValue != 0; k++)
            {
               if (uniqueLeafs[k] == childValue)
               {
                  parentLevel[i].childPointers[j] = &(uniqueLeafs[k]);
                  updateCount++;
                  cerr << "\t\tUpdate: " << updateCount << "\t=> "  << childValue << " &=> " << &(uniqueLeafs[k]) << endl;
               }
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
      updateCount = 0;
      for (unsigned int i = 0; i < numChildren-1; ++i)
      {
         if (copyChildNodes[i] != copyChildNodes[i+1])
         {
            memcpy(&uniqueChildren[uniqueChildIndex], &copyChildNodes[i], sizeof(SVONode));
            uniqueChildIndex++;
            updateCount++;
            cerr << "\t\tUpdate: " << updateCount << endl;
         }
      }
      delete [] copyChildNodes;

      cerr << "\tFinished reducing child nodes." << endl << endl;

      // Reducing child nodes
      cerr << "\tAdjusting parent node's pointer's to unique children..." << endl;

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
                  }
               }

            }
         }
      }

      cerr << "\tFinished adjusting parent node's pointer's to unique children." << endl << endl;

      std::cerr << "Finished parentLevelNum: " << parentLevelNum << endl << endl << endl;
      parentLevelNum--;
   }

}


/**
 * Returns a boolean indicating whether the voxel at the given coordinate is 
 * set.
 *
 * Tested: 
 */
bool DAG::isSet(unsigned int x, unsigned int y, unsigned int z)
{
   void* currentNode = root;
   int currentLevel = numLevels;
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
bool DAG::isChildSet(SVONode *node, unsigned int i)
{
   return node->childPointers[i] != NULL;
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


































