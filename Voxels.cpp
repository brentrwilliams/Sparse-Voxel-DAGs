/**
 *  Voxels.cpp
 * 
 * by Brent Williams
 */

#include "Voxels.hpp"


/**
 * Instantiates and initializes the voxel data to be all empty (i.e. 0).
 */
Voxels::Voxels(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles, std::string meshFilePath)
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
   std::cout << "Voxels constructor boundingBox: " << endl;
   boundingBox.print();
   std::cout << endl;
   voxelWidth = (boundingBox.maxs.x - boundingBox.mins.x) / dimension;
   
   std::cout << "Number of 64bit ints allocated: " << dataSize << "\n";
   std::cout << "sizeof(uint64_t) " << sizeof(uint64_t) << "\n";

   string fileName = getFileNameFromPath(meshFilePath);
   std::cout << "FILENAME: " << fileName << endl;

   voxelTriangleIndexMap = new tbb::concurrent_unordered_map<unsigned int, unsigned int>();

   // if (!cacheExists(fileName))
   // {
   //    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  NO VOXEL CACHE... VOXELIZING NOW  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
      build(triangles);
   //    writeVoxelCache(fileName);
   // }
   // else
   // {
   //    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  FOUND VOXEL CACHE... READING IN VOXELIZATION  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
   //    build(fileName);
   // }
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
   unsigned int voxelNumber = mortonCode(x,y,z,levels);
   //unsigned int voxelNumber = x + dimension*y + dimension*dimension*z;
   
   //dataIndex is the index into the uint64 array of the current voxel
   unsigned int dataIndex = voxelNumber / 64;
   
   //bitIndex is the current voxel (represented by a bit) to set
   unsigned int bitIndex =  voxelNumber % 64;
   
   //The mask used to set the voxel
   uint64_t toOr = (1L << bitIndex);
   
   #pragma omp atomic
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
   unsigned int voxelNumber = mortonCode(x,y,z,levels);
   //unsigned int voxelNumber = x + dimension*y + dimension*dimension*z;
   
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
 * Builds the volume of voxels from triangles
 */
void Voxels::build(const std::vector<Triangle> triangles)
{
   unsigned int i;
   unsigned int stepSize = triangles.size() / 100;
   unsigned int progress = 0;
   #pragma omp parallel for
   for (i = 0; i < triangles.size(); i++)
   {
      voxelizeTriangle(triangles[i], i);

      #pragma omp atomic
      progress += 1;

      if (progress % (stepSize-1))
      {
         #pragma omp critical
         fprintf(stderr, "%.2f\n", (((float)progress)/triangles.size()) * 100.0f);
      }
      
   }
}


/**
 * Builds the volume of voxels from cache
 */
void Voxels::build(std::string fileName)
{
   unsigned int i, j, k;

   for (k = 0; k < dimension; k++)
   {  
      //cout << "Witing image " << k << endl;
      char filePath[1000];
      sprintf(filePath, "./voxelCache/%s/%d/%04d.tga",fileName.c_str(),levels,k);
      Image image(dimension,dimension);
      image.readTGA(filePath);
      for (j = 0; j < dimension; j++)
      {
         for (i = 0; i < dimension; i++)
         {
            glm::vec3 color = image.getColor(i,j);
            if (color.x > 0)
            {
               set(i,j,k);
            }
            // if (isSet(i,j,k))
            // {
            //    image.setColor(i,j, 1.0f,1.0f,1.0f);
            // }
         }
      }
   }
}


/**
 * Voxelizes the given triangle into the volume 
 */
void Voxels::voxelizeTriangle(const Triangle& triangle, unsigned int i)
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
            {
               unsigned int mortonIndex = mortonCode(x, y, z, levels);
               glm::vec3 v0(triangle.v0.x,triangle.v0.y,triangle.v0.z);
               glm::vec3 v1(triangle.v1.x,triangle.v1.y,triangle.v1.z);
               glm::vec3 v2(triangle.v2.x,triangle.v2.y,triangle.v2.z);

               // Calculate the normal of the triangle/plane
               // glm::vec3 normal = glm::normalize( glm::cross(v1-v0, v2-v0) );
               // cout << "Voxelization: (" << x << ", " << y << ", " << z << ") => mortonIndex: " << mortonIndex << endl;
               // cout << "\tTriangle (" << i << ") " << endl;
               // cout << "\tTriangle Normal: <" << normal.x << ", " << normal.y << ", " << normal.z << ">" << endl; 
               // cout << endl;

               voxelTriangleIndexMap->insert( std::make_pair<unsigned int,unsigned int>( mortonIndex, i ) );

               set(x,y,z);
            }
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

/**
 * Writes the voxel data to tga files where the file name is the z axis voxel number.
 *
 *
 * Tested: 12-15-2014
 */
void Voxels::writeImages()
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

bool Voxels::cacheExists(std::string fileName)
{
   struct stat sb;
   std::string parentFolderPath = "./voxelCache/";
   parentFolderPath += fileName + "/";
   std::ostringstream strm;
   strm << levels;
   std::string levelFolderName = strm.str();
   std::string fullPath = parentFolderPath + levelFolderName;
   
   return stat(fullPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
}


/**
 * Writes the voxel cache to tga files where the file name is the z axis voxel number.
 *
 *
 * Tested: 3-22-2015
 */
void Voxels::writeVoxelCache(std::string fileName)
{
   unsigned int i, j, k;

   char meshFolderPath[1000];
   sprintf(meshFolderPath, "./voxelCache/%s",fileName.c_str());
   struct stat st1;  

   // If the folder doesn not already exist, create it
   if (stat(meshFolderPath, &st1) == -1) {
      mkdir(meshFolderPath, 0777);
   } 

   char parentFolderPath[1000];
   sprintf(parentFolderPath, "./voxelCache/%s/%d",fileName.c_str(),levels);
   struct stat st2;

   // If the folder doesn not already exist, create it
   if (stat(parentFolderPath, &st2) == -1) {
      mkdir(parentFolderPath, 0777);
   }

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

      char filePath[1000];
      sprintf(filePath, "./voxelCache/%s/%d/%04d.tga",fileName.c_str(),levels,k);
      image.writeTGA((filePath));
   }
}



std::string Voxels::getFileNameFromPath(string filePath)
{
   string filename = filePath;
   // Remove directory if present.
   // Do this before extension removal incase directory has a period character.
   const size_t last_slash_idx = filename.find_last_of("\\/");
   if (std::string::npos != last_slash_idx)
   {
       filename.erase(0, last_slash_idx + 1);
   }

   // Remove extension if present.
   const size_t period_idx = filename.rfind('.');
   if (std::string::npos != period_idx)
   {
       filename.erase(period_idx);
   }
   return filename;
}






















