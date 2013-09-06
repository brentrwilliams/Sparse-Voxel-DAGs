/**
 * Vec2.hpp
 * 
 * by Brent Williams
 */

#ifndef VEC2_HPP
#define VEC2_HPP

#include <stdio.h>

class Vec2 
{
   public:
      float x;
      float y;
      
      Vec2();
      Vec2(const float val);
      Vec2(const float xVal,const  float yVal);
      void print() const;
      Vec2& operator=(const Vec2 &rhs);
      Vec2& operator+=(const Vec2 &rhs);
      Vec2& operator-=(const Vec2 &rhs);
      Vec2& operator*=(const float rhs);
      const Vec2 operator+(const Vec2 &other) const;
      const Vec2 operator-(const Vec2 &other) const;
      const Vec2 operator*(const float other) const;
      float innerProduct(const Vec2 &rhs) const;
};

#endif
