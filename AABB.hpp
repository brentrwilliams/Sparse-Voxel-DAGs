/**
 * AABB.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef AABB_HPP
#define AABB_HPP

#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <math.h>

#include "Ray.hpp"

using namespace std;

#define EPSILON 1.0e-6

class AABB
{
   public:
      glm::vec3 mins;
      glm::vec3 maxs;

      AABB(const glm::vec3& mins, const glm::vec3& maxs);
      bool intersect(const Ray& ray, float& t, glm::vec3& normal);
      bool contains(const glm::vec3& val);
      bool inRange(float val, float min, float max);
      void print() const;
};


#endif