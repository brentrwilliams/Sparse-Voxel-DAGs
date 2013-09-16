/**
 *  ActiveList.cpp
 * 
 * by Brent Williams
 */

#include "ActiveList.hpp"


/**
 * Instantiates and initializes the active list.
 */
ActiveList::ActiveList(const Voxels& voxels)
 : currentLevel(voxels.levels),
   levels(voxels.levels),
   data(0),
   dataSize(calculateDataSize(voxels.levels)),
   dimension(pow(2,voxels.levels))
{
   data = new uint64_t[dataSize];
   
   if (data == NULL)
   {
      std::string err("\nAllocation failed for data\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }
   
   memcpy(data, voxels.data, dataSize*sizeof(uint64_t));
}

/**
 * Typical destructor releasing the data.
 */
ActiveList::~ActiveList()
{
   delete [] data;
}

/**
 * Returns the number of 64-bit unsigned ints needed with a set of voxels with levels
 * number of levels.
 *
 * Tested: 9-3-2013 
 */
unsigned int ActiveList::calculateDataSize(unsigned int numLevels)
{
   if (numLevels < 2)
   {
      std::string err("\nInvalid number of levels in ActiveList::calculateDataSize!\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }
   
   return pow(8, numLevels-2);
}

/**
 * Return the set of 64 voxels (i.e. an 64 bit int or int64_t) that are the voxels 
 * at the index i.
 *
 * Tested: 9-3-2013 
 */
uint64_t& ActiveList::operator[](unsigned int i)
{
   if (i >= dataSize)
   {
      std::string err = "\nIndex out of range in ActiveList::operator[] with an index of ";
      std::ostringstream convert;
      convert << i;
      err += convert.str();
      std::cerr << err;
      throw std::out_of_range(err);
   }
   return data[i];
}

/**
 * Sets the voxel at the given x, y and z values as filled 
 *
 * Tested: 9-3-2013 
 */
void ActiveList::set(unsigned int x, unsigned int y, unsigned int z)
{
   unsigned int voxelNumber = x + dimension*y + dimension*dimension*z;
   unsigned int dataIndex = voxelNumber / 64;
   unsigned int bitIndex =  voxelNumber % 64;
   uint64_t toOr = (1L << bitIndex);
   
   data[dataIndex] |= toOr; // sets the bitIndex bit 
}

/**
 * Returns the number of voxels that are set.
 *
 * Tested: 9-12-2013 
 */
unsigned int ActiveList::countSetVoxels()
{
   unsigned int i, count = 0;
   
   for (i = 0; i < dataSize; i++)
      count += countSetBits(data[i]);
   
   return count;
}
/**
 * Updates the active list to be the next level returning the number of levels 
 * remaining in the active list.
 *
 * Tested: 9-12-2013 
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
            data[newI] |= (1L << shiftI); //set the bit at shiftI
         }
         
         dataToMask >>= 8; 
      }
   }

   currentLevel--;
   return currentLevel;
}

