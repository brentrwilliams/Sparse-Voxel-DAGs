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

void DAG::build(const std::vector<Triangle> triangles)
{
   SparseVoxelOctree svo(numLevels, boundingBox, triangles);

   unsigned int numLeafs = size / 64;
   unsigned int sizeOfLeafs = numLeafs * sizeof(uint64_t);
   uint64_t* leafVoxels = (uint64_t*) svo.levels[numLevels-2]; // numLevels -2 b/c last 2 levels are together
   uint64_t* copyLeafVoxels = new uint64_t[numLeafs];

   if (copyLeafVoxels == NULL)
   {
      cerr << "Unable to allocate copyLeafVoxels\n";
   }

   memcpy(copyLeafVoxels, leafVoxels, sizeOfLeafs);
   std::sort(copyLeafVoxels, copyLeafVoxels + numLeafs);
   for (unsigned int i = 0; i < numLeafs; ++i)
   {
      std::cout << copyLeafVoxels[i] << " ";
   }
   std::cout << endl;

   // Calculate the number of unique leafs
   unsigned int numUniqueLeafs = 1;
   for (unsigned int i = 0; i < numLeafs-1; ++i)
   {
      if (copyLeafVoxels[i] != copyLeafVoxels[i+1])
      {
         numUniqueLeafs++;
      }
   }
   std::cout << "numUniqueLeafs: " << numUniqueLeafs << endl;
   
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

   std::cout << "uniqueLeafIndex: " << uniqueLeafIndex << endl;

   for (unsigned int i = 0; i < numUniqueLeafs; ++i)
   {
      std::cout << uniqueLeafs[i] << " ";
   }
   std::cout << endl; 

   // Free the copy of the leafs
   delete [] copyLeafVoxels;

   // Adjust the pointers of the level above in the svo
   unsigned int parentLevelNum = numLevels-3;
   SVONode* parentLevel = (SVONode*) svo.levels[parentLevelNum];
   unsigned int numParents = numLeafs / 8;

   std::cout << numParents << endl;

   for (unsigned int i = 0; i < numParents; i++)
   {
      for (unsigned int j = 0; j < 8; j++)
      {
         if (parentLevel[i].childPointers[j] != NULL)
         {
            uint64_t childValue = *((uint64_t*) parentLevel[i].childPointers[j]);

            // Search for the corresponding child value in the uniqueLeafs and replace the 
            // childpointer with a pointer to the unique leaf
            for (unsigned int k = 0; k < numUniqueLeafs; k++)
            {
               if (uniqueLeafs[k] == childValue)
               {
                  parentLevel[i].childPointers[j] = &(uniqueLeafs[k]);
               }
            }

         }
      }
   }

}














































