/**
 *  Voxels.cpp
 * 
 * by Brent Williams
 */

#include "Voxels.hpp"


/**
 * Instantiates and initializes the voxel data to be all empty (i.e. 0).
 */
Voxels::Voxels(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles)
 : data(0), 
   boundingBox(boundingBoxVal),
   levels(levelsVal),
   size(pow(8, levelsVal)), 
   dimension(pow(2,levelsVal)),
   voxelWidth(0),
   dataSize(0)
{
   dataSize = calculateDataSize(levels);
   data = new uint64_t[dataSize]();
   
   if (data == NULL)
   {
      std::string err("\nAllocation failed for data\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }
   
   boundingBox.square();
   voxelWidth = (boundingBox.maxs.x - boundingBox.mins.x) / dimension;
   
   std::cout << "Number of 64bit ints allocated: " << dataSize << "\n";
   std::cout << "sizeof(uint64_t) " << sizeof(uint64_t) << "\n";
   
   build(triangles);
}

/**
 * Typical destructor releasing the data.
 */
Voxels::~Voxels()
{
   delete [] data;
}

/**
 * Returns the number of 64-bit unsigned ints needed with a set of voxels with levels
 * number of levels.
 *
 * Tested: 9-3-2013 
 */
unsigned int Voxels::calculateDataSize(unsigned int levels)
{
   if (levels < 2)
   {
      std::string err("\nInvalid number of levels!\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }
   
   return pow(8, levels-2);
}

/**
 * Return the set of 64 voxels (i.e. an 64 bit int or int64_t) that are the voxels 
 * at the index i.
 *
 * Tested: 9-3-2013 
 */
uint64_t& Voxels::operator[](unsigned int i)
{
   if (i >= dataSize)
   {
      std::string err = "\nIndex out of range in Voxels::operator[] with an index of ";
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
void Voxels::set(unsigned int x, unsigned int y, unsigned int z)
{
   //If each individual voxel had an index, the voxeNumber is that index
   unsigned int voxelNumber = x + dimension*y + dimension*dimension*z;
   
   //dataIndex is the index into the uint64 array of the current voxel
   unsigned int dataIndex = voxelNumber / 64;
   
   //bitIndex is the current voxel (represented by a bit) to set
   unsigned int bitIndex =  voxelNumber % 64;
   
   //The mask used to set the voxel
   uint64_t toOr = (1L << bitIndex);
   
   data[dataIndex] |= toOr; // sets the bitIndex bit 
}

/**
 * Checks is the voxel at the given x, y and z values as set 
 *
 * Tested: 12-22-2013
 */
bool Voxels::isSet(unsigned int x, unsigned int y, unsigned int z)
{
   //If each individual voxel had an index, the voxeNumber is that index
   unsigned int voxelNumber = x + dimension*y + dimension*dimension*z;
   
   //dataIndex is the index into the uint64 array of the current voxel
   unsigned int dataIndex = voxelNumber / 64;
   
   //bitIndex is the current voxel (represented by a bit) to set
   unsigned int bitIndex =  voxelNumber % 64;
   
   //The mask used check if the voxel is set
   uint64_t toAnd = (1L << bitIndex);
   
   return (data[dataIndex] & toAnd) > 0;
}

/**
 * Returns the number of voxels 
 *
 * Tested: 9-3-2013 
 */
unsigned long Voxels::getSize() const
{
   return size;
}

/**
 * Builds the volume of voxels 
 */
void Voxels::build(const std::vector<Triangle> triangles)
{
   unsigned int i;
   for (i = 0; i < triangles.size(); i++)
      voxelizeTriangle(triangles[i]);
}

/**
 * Voxelizes the given triangle into the volume 
 */
void Voxels::voxelizeTriangle(const Triangle& triangle)
{
   Vec3 triMins(triangle.getMins());
   Vec3 triMaxs(triangle.getMaxs());
   
   //Calculate the indexes into the voxel
   unsigned int minX = (triMins.x - boundingBox.mins.x) / voxelWidth;
   unsigned int maxX = (triMaxs.x - boundingBox.mins.x + 0.5) / voxelWidth;
   unsigned int minY = (triMins.y - boundingBox.mins.y) / voxelWidth;
   unsigned int maxY = (triMaxs.y - boundingBox.mins.y + 0.5) / voxelWidth;
   unsigned int minZ = (triMins.z - boundingBox.mins.z) / voxelWidth;
   unsigned int maxZ = (triMaxs.z - boundingBox.mins.z + 0.5) / voxelWidth;
   
   //Deal with floating point error
   maxX = (maxX >= dimension) ? (dimension-1) : maxX;
   maxY = (maxY >= dimension) ? (dimension-1) : maxY; 
   maxZ = (maxZ >= dimension) ? (dimension-1) : maxZ;
   
   unsigned int x, y, z;
   Vec3 deltaP(voxelWidth, voxelWidth, voxelWidth); //The (maxs-mins) of the 
                                                    //voxel's bounding box
   
   for (x = minX; x <= maxX; x++)
   {
      for (y = minY; y <= maxY; y++)
      {
         for (z = minZ; z <= maxZ; z++)
         {
            Vec3 p(boundingBox.mins.x + (x*voxelWidth),  //The mins of the 
             boundingBox.mins.y + (y*voxelWidth),        //voxel's bounding box
             boundingBox.mins.z + (z*voxelWidth) );
             
            if (triangleAABBIntersect(triangle, p, deltaP))
               set(x,y,z);
         }
      }
   }   
   
   
}

/**
 * Returns the number of voxels that are set
 *
 * Tested: 9-12-2013 
 */
unsigned int Voxels::countSetVoxels()
{
   unsigned int i, count = 0;
   
   for (i = 0; i < dataSize; i++)
      count += countSetBits(data[i]);
   
   return count;
}

/**
 * Returns the string representation of the binary of an unsigned 64-bit int 
 *
 * Tested: 12-21-2013 
 */
void binaryToString(uint64_t data, char* str)
{
   unsigned int i;
   
   for (i = 0; i < 64; i++)
   {
      if (data & 1)
         str[i] = '1';
      else
         str[i] = '0';
      
      data >>= 1;
   }
}

/**
 * Print the binary representation of the voxels
 *
 * Tested: 12-21-2013 
 */
void Voxels::printBinary()
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
 * Returns the number of set bits in an unsigned 64-bit int
 *
 * Written based on the algorithm published by Peter Wagner in CACM 3 (1960)
 *
 * Tested: 9-12-2013 
 */
unsigned int countSetBits(uint64_t data)
{
   unsigned int count; 
   
   for (count = 0; data; count++) 
      data &= data - 1;
   
   return count;
}

