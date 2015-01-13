/**
 * BoundingBox.hpp
 * 
 * by Brent Williams
 */

#ifndef BoundingBox_HPP
#define BoundingBox_HPP

#include <stdio.h>

#include "Vec3.hpp"

class BoundingBox 
{
   public:
      Vec3 mins, maxs;
      BoundingBox();
      BoundingBox(const Vec3& minsVal, const Vec3& maxsVal);
      void print() const;
      void square();
      Vec3 getCenter() const;
};

#endif
