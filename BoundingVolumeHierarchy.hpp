/**
 *  BoundingVolumeHierarchy.h
 *  by Brent Williams
 **/

#ifndef BOUNDING_VOLUME_HIERARCHY_HPP
#define BOUNDING_VOLUME_HIERARCHY_HPP

#include <algorithm>
#include <vector>
#include "Triangle.hpp"
#include "BVHBoundingBox.hpp"

using namespace std;
using namespace glm;

bool compareTrianglesX(Triangle *t1, Triangle *t2);
bool compareTrianglesY(Triangle *t1, Triangle *t2);
bool compareTrianglesZ(Triangle *t1, Triangle *t2);

class BVHNode {
   public:
      BVHNode();
      BVHNode(BVHBoundingBox boundingBox, Triangle *triangle, BVHNode *left, BVHNode *right);
      bool intersect(Ray ray, float *t, Triangle **trianglePtr);
      
      BVHBoundingBox boundingBox;
      Triangle *triangle;
      BVHNode *left;
      BVHNode *right;
};

class BoundingVolumeHierarchy 
{
   public:
      BVHNode *tree;
      
      BoundingVolumeHierarchy(vector<Triangle*> triangle);
      BVHNode* generateNode(vector<Triangle*> triangle, int axis);
      bool intersect(Ray ray, float *t, Triangle **trianglePtr);
      
};

#endif
