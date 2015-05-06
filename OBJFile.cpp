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

// void OBJFile::parse()
// {
//    std::vector<Vec3> vertices;
//    std::vector<Face> faces;
//    int result, tempI1, tempI2, tempI3;
//    unsigned int i;
//    float tempF1, tempF2, tempF3;
//    char buffer[MAX_CHARS_PER_LINE];
   
//    std::ifstream objFile (fileName.c_str(), std::ifstream::in);
   
//    if (!(objFile.is_open() && objFile.good()))
//    {
//       std::cerr << "Unable to open file " << fileName << "\n";
//       exit(1);
//    }
   
//    //Read in all vertices and faces
//    while (objFile.good())
//    {
//       objFile.getline(buffer, MAX_CHARS_PER_LINE);
      
//       result = sscanf(buffer, " v %f %f %f ", &tempF1, &tempF2, &tempF3);
//       if (result == 3)
//       {
//          vertices.push_back(Vec3(tempF1, tempF2, tempF3));
         
//          //check for boundingBox.mins and boundingBox.maxs
//          if (tempF1 < boundingBox.mins.x)
//             boundingBox.mins.x = tempF1;
//          if (tempF2 < boundingBox.mins.y)
//             boundingBox.mins.y = tempF2;
//          if (tempF3 < boundingBox.mins.z)
//             boundingBox.mins.z = tempF3;
            
//          if (tempF1 > boundingBox.maxs.x)
//             boundingBox.maxs.x = tempF1;
//          if (tempF2 > boundingBox.maxs.y)
//             boundingBox.maxs.y = tempF2;
//          if (tempF3 > boundingBox.maxs.z)
//             boundingBox.maxs.z = tempF3;
//       }
      
//       result = sscanf(buffer, " f %d %d %d ", &tempI1, &tempI2, &tempI3);
//       if (result == 3)
//       {
//          //Faces are indexed by 1 and not 0 in the .obj file
//          faces.push_back(Face(tempI1-1, tempI2-1, tempI3-1));
//       }
//    }
   
//    //Convert the vertices and faces into triangles
//    for (i = 0; i < faces.size(); i++)
//    {
//       triangles.push_back(Triangle(vertices[faces[i].v1], vertices[faces[i].v2], 
//        vertices[faces[i].v3])); 
//    }


//    updateBoundingBox();
// }

void OBJFile::parse()
{
   Assimp::Importer importer;
   // And have it read the given file with some example postprocessing
   // Usually - if speed is not the most important aspect for you - you'll 
   // propably to request more postprocessing than we do in this example.
   const aiScene* scene = importer.ReadFile( fileName, 
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
  
   // If the import failed, report it
   if( !scene)
   {
      cerr << "ERROR: Failed to import mesh!!!" << endl;
   }

   for (int i = 0; i < scene->mNumMeshes; i++)
   {
      const struct aiMesh* mesh = scene->mMeshes[i];
      unsigned int materialIndex = mesh->mMaterialIndex;
      cout << "materialIndex = " << materialIndex << endl;

      for (int f = 0; f < mesh->mNumFaces; f++) 
      {
         const struct aiFace* face = &mesh->mFaces[f];
         if (face->mNumIndices != 3)
         {
            cerr << "ERROR: Found non-triangle geometry!" << endl;
            return;
         }
         int vertexIndex0 = face->mIndices[0];
         int vertexIndex1 = face->mIndices[1];
         int vertexIndex2 = face->mIndices[2];

         Vec3 v0(mesh->mVertices[vertexIndex0].x, mesh->mVertices[vertexIndex0].y, mesh->mVertices[vertexIndex0].z);
         Vec3 v1(mesh->mVertices[vertexIndex1].x, mesh->mVertices[vertexIndex1].y, mesh->mVertices[vertexIndex1].z);
         Vec3 v2(mesh->mVertices[vertexIndex2].x, mesh->mVertices[vertexIndex2].y, mesh->mVertices[vertexIndex2].z);

         Triangle triangle(v0,v1,v2,materialIndex);
         triangles.push_back(triangle);
      }
   }

   for (int i = 0; i < scene->mNumMaterials; i++)
   {
      const struct aiMaterial* mtl = scene->mMaterials[i];
      int ret1, ret2;
      unsigned int max;
      float shininess, strength;

      aiColor4D kdAi;
      aiColor4D ksAi;
      aiColor4D kaAi;

      glm::vec3 ka(0.0f,0.0f,0.0f);
      glm::vec3 kd(0.5f,0.5f,0.5f);
      glm::vec3 ks(0.5f,0.5f,0.5f);
      float ns = 1.0f;

      if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &kaAi))
      {
         ka = glm::vec3(kaAi.r, kaAi.g, kaAi.b);
      }

      if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &kdAi))
      {
         kd = glm::vec3(kdAi.r, kdAi.g, kdAi.b);
      }

      if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &ksAi))
      {
         ks = glm::vec3(ksAi.r, ksAi.g, ksAi.b);
      }

      max = 1;
      ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
      max = 1;
      ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);

      if((ret1 == AI_SUCCESS))
      {
         ns = shininess / 4.0f;
      }

      PhongMaterial material(ka,kd,ks,ns);
      materials.push_back(material);

      cout << endl;
      cout << "material " << i << ":" << endl;
      cout << "\tambient: <" << ka.x << ", " << ka.y << ", " << ka.z << ">" << endl;
      cout << "\tdiffuse: <" << kd.x << ", " << kd.y << ", " << kd.z << ">" << endl;
      cout << "\tspecular: <" << ks.x << ", " << ks.y << ", " << ks.z << ">" << endl;
      cout << "\tshininess: " << ns << endl;
      cout << endl;
   }

   updateBoundingBox();
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

void OBJFile::centerMesh()
{
   // Center the mesh around (0, 0, 0)
   Vec3 translate = boundingBox.getCenter();

   for (unsigned long i = 0; i < triangles.size(); i++)
   {
      Triangle& triangle = triangles.at(i);
      triangle.v0 -= translate;
      triangle.v1 -= translate;
      triangle.v2 -= translate;
   }

   boundingBox.mins -= translate;
   boundingBox.maxs -= translate;
   updateBoundingBox();
}

void OBJFile::updateBoundingBox()
{
   std::cerr << "triangles.size() = " << triangles.size() << std::endl;
   boundingBox.mins = triangles[0].v0;
   boundingBox.maxs = triangles[0].v1;
   for (unsigned long i = 0; i < triangles.size(); i++)
   {
      Vec3 v = triangles[i].v0;
      if (v.x < boundingBox.mins.x)
         boundingBox.mins.x = v.x;
      if (v.y < boundingBox.mins.y)
         boundingBox.mins.y = v.y;
      if (v.z < boundingBox.mins.z)
         boundingBox.mins.z = v.z;
            
      if (v.x > boundingBox.maxs.x)
         boundingBox.maxs.x = v.x;
      if (v.y > boundingBox.maxs.y)
         boundingBox.maxs.y = v.y;
      if (v.z > boundingBox.maxs.z)
         boundingBox.maxs.z = v.z;

      v = triangles[i].v1;
      if (v.x < boundingBox.mins.x)
         boundingBox.mins.x = v.x;
      if (v.y < boundingBox.mins.y)
         boundingBox.mins.y = v.y;
      if (v.z < boundingBox.mins.z)
         boundingBox.mins.z = v.z;
            
      if (v.x > boundingBox.maxs.x)
         boundingBox.maxs.x = v.x;
      if (v.y > boundingBox.maxs.y)
         boundingBox.maxs.y = v.y;
      if (v.z > boundingBox.maxs.z)
         boundingBox.maxs.z = v.z;

      v = triangles[i].v2;
      if (v.x < boundingBox.mins.x)
         boundingBox.mins.x = v.x;
      if (v.y < boundingBox.mins.y)
         boundingBox.mins.y = v.y;
      if (v.z < boundingBox.mins.z)
         boundingBox.mins.z = v.z;
            
      if (v.x > boundingBox.maxs.x)
         boundingBox.maxs.x = v.x;
      if (v.y > boundingBox.maxs.y)
         boundingBox.maxs.y = v.y;
      if (v.z > boundingBox.maxs.z)
         boundingBox.maxs.z = v.z;
   }
}
