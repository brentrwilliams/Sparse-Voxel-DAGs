/**
 * Triangle.hpp
 * 
 * by Brent Williams
 */

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <stdio.h>

#include "Vec3.hpp"

class Triangle 
{
   public:
      Vec3 v0, v1, v2;
      unsigned int materialIndex;
      
      Triangle();
      Triangle(const Vec3& v0Val, const Vec3& v1Val, const Vec3& v2Val);
      Triangle(const Vec3& v0Val, const Vec3& v1Val, const Vec3& v2Val, unsigned int materialIndexVal);
      void print() const;
      const Vec3 getMins() const;
      const Vec3 getMaxs() const;
      const Vec3 getNormal() const;
};

#endif
