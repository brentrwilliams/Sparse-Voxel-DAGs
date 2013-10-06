/**
 *  ActiveList.cpp
 * 
 * by Brent Williams
 */

#include "ActiveList.hpp"


/**
 * Instantiates and initializes the active list.
 */
ActiveList::ActiveList(const unsigned int levelsVal, 
 const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles)
 : Voxels(levelsVal, boundingBoxVal, triangles)
{
   currentLevel = levelsVal;
}

/**
 * Updates the active list to be the next level returning the number of levels 
 * remaining in the active list.
 *
 * Logically makes sense but untested
 */
unsigned int ActiveList::nextLevel()
{
   uint64_t *newData;
   uint64_t mask8Bits = 255;//A mask with 8 1's in binary
   uint64_t dataToMask; 
   unsigned int oldI, newI = -1, shiftI;
   unsigned int newDataSize = calculateDataSize(levels-1);
   newData = new uint64_t[newDataSize];
   
   if (newData == NULL)
   {
      std::string err("\nAllocation failed for newData in ActiveList::nextLevel\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }

   for (oldI = 0; oldI < dataSize; oldI++)
   {
      dataToMask = data[oldI];
      
      for (shiftI = 0; shiftI < 8; shiftI++)
      {   
         if (oldI % 8 == 0)
            newI++; 
         
         if (dataToMask & mask8Bits)   // If 1 or more of the 8 bits is set (ie 
         {                             // the parent node is filled)
            newData[newI] |= (1L << shiftI); //set the bit at shiftI
         }
         
         dataToMask >>= 8; 
      }
   }
   
   // Free the old data and swap in the new data
   delete [] data;
   data = newData;

   currentLevel--;
   return currentLevel;
}


