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
#include "Face.hpp"
#include "BoundingBox.hpp"

#define MAX_CHARS_PER_LINE 256
#define MIN_FLOAT -1000000000
#define MAX_FLOAT 1000000000


class OBJFile
{
   private:
      std::string fileName;
      BoundingBox boundingBox;
      std::vector<Triangle> triangles;
      void parse();
   
   public:
      OBJFile(std::string fileNameVal);
      const BoundingBox& getBoundingBox();
      const std::vector<Triangle>& getTriangles();
};

#endif
