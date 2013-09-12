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
   std::cerr << "data: " << data << std::endl; 
   
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
   unsigned int voxelNumber = x + dimension*y + dimension*dimension*z;
   unsigned int dataIndex = voxelNumber / 64;
   unsigned int bitIndex =  voxelNumber % 64;
   uint64_t toOr = (1L << bitIndex);
   
   data[dataIndex] |= toOr; // sets the bitIndex bit 
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
 * Print the binary of an unsigned 64-bit int in sets of 8 bits
 *
 * Tested: 9-3-2013 
 */
void printBinary(uint64_t data)
{
   unsigned int i;
   char bits[64];
   
   for (i = 0; i < 64; i++)
   {
      if (data & 1)
         bits[i] = '1';
      else
         bits[i] = '0';
      
      data >>= 1;
   }
   
   for (i = 0; i < 64; i++)
   {
      std::cout << bits[i]; 
      
      if ((i+1) % 8 == 0)
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

