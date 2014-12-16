/**
 * OBJFile.cpp
 * 
 * by Brent Williams
 *
 * Notes:
 * - Currently only supports triangles
 */

#include "OBJFile.hpp"

OBJFile::OBJFile(std::string fileNameVal)
 : fileName(fileNameVal),
   boundingBox(),
   triangles() 
{
   parse();
}

void OBJFile::parse()
{
   std::vector<Vec3> vertices;
   std::vector<Face> faces;
   int result, tempI1, tempI2, tempI3;
   unsigned int i;
   float tempF1, tempF2, tempF3;
   char buffer[MAX_CHARS_PER_LINE];
   
   std::ifstream objFile (fileName.c_str(), std::ifstream::in);
   
   if (!(objFile.is_open() && objFile.good()))
   {
      std::cerr << "Unable to open file " << fileName << "\n";
      exit(1);
   }
   
   //Read in all vertices and faces
   while (objFile.good())
   {
      objFile.getline(buffer, MAX_CHARS_PER_LINE);
      
      result = sscanf(buffer, " v %f %f %f ", &tempF1, &tempF2, &tempF3);
      if (result == 3)
      {
         vertices.push_back(Vec3(tempF1, tempF2, tempF3));
         
         //check for boundingBox.mins and boundingBox.maxs
         if (tempF1 < boundingBox.mins.x)
            boundingBox.mins.x = tempF1;
         if (tempF2 < boundingBox.mins.y)
            boundingBox.mins.y = tempF2;
         if (tempF3 < boundingBox.mins.z)
            boundingBox.mins.z = tempF3;
            
         if (tempF1 > boundingBox.maxs.x)
            boundingBox.maxs.x = tempF1;
         if (tempF2 > boundingBox.maxs.y)
            boundingBox.maxs.y = tempF2;
         if (tempF3 > boundingBox.maxs.z)
            boundingBox.maxs.z = tempF3;
      }
      
      result = sscanf(buffer, " f %d %d %d ", &tempI1, &tempI2, &tempI3);
      if (result == 3)
      {
         //Faces are indexed by 1 and not 0 in the .obj file
         faces.push_back(Face(tempI1-1, tempI2-1, tempI3-1));
      }
   }
   
   //Convert the vertices and faces into triangles
   for (i = 0; i < faces.size(); i++)
   {
      triangles.push_back(Triangle(vertices[faces[i].v1], vertices[faces[i].v2], 
       vertices[faces[i].v3])); 
   }

}

/**
 * Returns the bounding box for the geometry in the obj file. 
 *
 * Note: Not guaranteed to be square
 */
const BoundingBox& OBJFile::getBoundingBox()
{
   return boundingBox;
}

/**
 * Returns a vector of all the triangles in the obj file.
 */
const std::vector<Triangle>& OBJFile::getTriangles()
{
   return triangles;
}

/**
 * Returns the number of triangles it has.
 *
 */
unsigned int OBJFile::getNumTriangles()
{
   return triangles.size();
}
