/**
 * Vec3.hpp
 * 
 * by Brent Williams
 */

#ifndef VEC3_HPP
#define VEC3_HPP

#include <stdio.h>
#include <math.h>

class Vec3 
{
   public:
      float x;
      float y;
      float z;
      
      Vec3();
      Vec3(const float val);
      Vec3(const float xVal, const float yVal, const float zVal);
      void print() const;
      Vec3& operator=(const Vec3 &rhs);
      Vec3& operator+=(const Vec3 &rhs);
      Vec3& operator-=(const Vec3 &rhs);
      Vec3& operator*=(const float rhs);
      const Vec3 operator+(const Vec3 &other) const;
      const Vec3 operator-(const Vec3 &other) const;
      const Vec3 operator*(const float other) const;
      float dot(const Vec3 &rhs) const;
      Vec3 cross(const Vec3 &rhs) const;
      float innerProduct(const Vec3 &rhs) const;
      float length() const;
      const Vec3& normalize();
};

#endif
