/**
 *  BVHBoundingBox.h
 *  by Brent Williams
 **/

#ifndef BVH_BOUNDING_BOX_H
#define BVH_BOUNDING_BOX_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include "Ray.hpp"

#define MAX_CHARS_PER_LINE 512
#define MAXIMUM_FLOAT 1000000000
#define MINIMUM_FLOAT -1000000000

using namespace std;
using namespace glm;


class BVHBoundingBox
{
   public:
      vec3 mins;
      vec3 maxs;
      vec3 corner[8];
      vec3 center;
      
      BVHBoundingBox();
      BVHBoundingBox(vec3 mins, vec3 maxs);
      BVHBoundingBox(BVHBoundingBox bb1, BVHBoundingBox bb2);
      void calculateCorners();
      bool intersect(Ray ray);
      void transform(mat4 trans);
      void calculateCenter();
};

#endif
