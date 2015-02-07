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
   unsigned int numChildren = numLeafs;

   if (copyLeafVoxels == NULL)
   {
      cerr << "Unable to allocate copyLeafVoxels\n";
   }
   cout << endl;

   int parentLevelNum = numLevels-3;
   SVONode* parentLevel = (SVONode*) svo.levels[parentLevelNum];
   unsigned int numParents = numLeafs / 8;

   std::cout << "Starting leaf level (parentLevelNum: " << parentLevelNum << ")" << endl;
   std::cout << "numParents: " << numParents << endl;
   std::cout << "numChildren: " << numChildren << endl << endl;

   cout << "\tSorting leaf nodes..." << endl;
   memcpy(copyLeafVoxels, leafVoxels, sizeOfLeafs);
   std::sort(copyLeafVoxels, copyLeafVoxels + numLeafs);
   cout << "\tFinished sorting leaf nodes." << endl << endl;
   // for (unsigned int i = 0; i < numLeafs; ++i)
   // {
   //    std::cout << copyLeafVoxels[i] << " ";
   // }
   // std::cout << endl;

   // Calculate the number of unique leafs
   cout << "\tReducing leaf nodes..." << endl;
   unsigned int numUniqueLeafs = 1;
   for (unsigned int i = 0; i < numLeafs-1; ++i)
   {
      if (copyLeafVoxels[i] != copyLeafVoxels[i+1])
      {
         numUniqueLeafs++;
      }
   }
   std::cout << "\t\tnumUniqueLeafs: " << numUniqueLeafs << endl;
   
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

   // std::cout << "uniqueLeafIndex: " << uniqueLeafIndex << endl;

   // cout << "uniqueLeafs: " << endl;
   // for (unsigned int i = 0; i < numUniqueLeafs; ++i)
   // {
   //    std::cout << uniqueLeafs[i] << " ";
   // }
   // std::cout << endl << endl; 

   // Free the copy of the leafs
   delete [] copyLeafVoxels;
   cout << "\tFinished reducing leaf nodes." << endl << endl;

   // Adjust the pointers of the level above in the svo
   cout << "\tAdjusting parent node's pointer's to unique children..." << endl;

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
   cout << "\tFinished adjusting parent node's pointer's to unique children." << endl << endl;
   std::cout << "Finished leaf level (parentLevelNum: " << parentLevelNum << ")" << endl << endl << endl;

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

      std::cout << "Starting parentLevelNum: " << parentLevelNum << endl;
      std::cout << "numParents: " << numParents << endl;
      std::cout << "numChildren: " << numChildren << endl << endl;

      // Sort the child level based on the pointers of each child node
      cout << "\tSorting child nodes..." << endl;
      SVONode* copyChildNodes = new SVONode[numChildren];
      memcpy(copyChildNodes, childLevel, sizeofChildren);
      std::sort(copyChildNodes, copyChildNodes + numChildren);
      cout << "\tFinished sorting child nodes." << endl << endl;

      //Reducing child nodes
      cout << "\tReducing child nodes..." << endl;
      unsigned int numUniqueChildren = 1;
      for (unsigned int i = 0; i < numChildren-1; ++i)
      {
         if (copyChildNodes[i] != copyChildNodes[i+1])
         {
            numUniqueChildren++;
         }
      }
      cout << "\t\tnumUniqueChildren: " << numUniqueChildren << endl;
      cout << "\tFinished reducing child nodes." << endl << endl;

      // Reducing child nodes
      cout << "\tAdjusting parent node's pointer's to unique children..." << endl;
      cout << "\tFinished djusting parent node's pointer's to unique children." << endl << endl;
      delete [] copyChildNodes;



      // for (unsigned int i = 0; i < numParents; i++)
      // {
      //    for (unsigned int j = 0; j < 8; j++)
      //    {
      //       if (parentLevel[i].childPointers[j] != NULL)
      //       {
      //          SVONode** childValue = *((uint64_t*) parentLevel[i].childPointers[j]);

      //          // Search for the corresponding child value in the uniqueLeafs and replace the 
      //          // childpointer with a pointer to the unique leaf
      //          for (unsigned int k = 0; k < numUniqueLeafs; k++)
      //          {
      //             if (uniqueLeafs[k] == childValue)
      //             {
      //                parentLevel[i].childPointers[j] = &(uniqueLeafs[k]);
      //             }
      //          }

      //       }
      //    }
      // }

      std::cout << "Finished parentLevelNum: " << parentLevelNum << endl << endl << endl;
      parentLevelNum--;
   }

}














































