/**
 * OBJFile.hpp
 * 
 * by Brent Williams
 *
 * Notes:
 * - Currently only supports triangles
 */

#ifndef OBJ_PARSER_HPP
#define OBJ_PARSER_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

#include "Vec3.hpp"
#include "Triangle.hpp"
#include "PhongMaterial.hpp"
#include "Face.hpp"
#include "BoundingBox.hpp"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#define MAX_CHARS_PER_LINE 256
#define MIN_FLOAT -1000000000
#define MAX_FLOAT 1000000000

using namespace std;

class OBJFile
{
   private:
      std::string fileName;
      BoundingBox boundingBox;
      std::vector<Triangle> triangles;
      std::vector<PhongMaterial> materials;
      void parse();
   
   public:
      OBJFile(std::string fileNameVal);
      const BoundingBox& getBoundingBox();
      const std::vector<Triangle>& getTriangles();
      unsigned int getNumTriangles();
      void centerMesh();
      void updateBoundingBox();
};

#endif
