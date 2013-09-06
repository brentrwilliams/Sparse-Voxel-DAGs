/**
 * Vec2.cpp
 * 
 * by Brent Williams
 */

#include "Vec2.hpp"

Vec2::Vec2()
 : x(0),
   y(0)
{}

Vec2::Vec2(const float val)
 : x(val),
   y(val)
{}

Vec2::Vec2(const float xVal, const float yVal)
 : x(xVal),
   y(yVal)
{}


void Vec2::print() const
{
   printf("<%f, %f>\n", x, y);  
}

Vec2& Vec2::operator=(const Vec2 &rhs)
{
   x = rhs.x;
   y = rhs.y;

   return *this;
}

Vec2& Vec2::operator+=(const Vec2 &rhs)
{
   x += rhs.x;
   y += rhs.y;
   
   return *this;
}

Vec2& Vec2::operator-=(const Vec2 &rhs)
{
   x -= rhs.x;
   y -= rhs.y;
   
   return *this;
}

Vec2& Vec2::operator*=(const float rhs)
{
   x *= rhs;
   y *= rhs;
   
   return *this;
}

const Vec2 Vec2::operator+(const Vec2 &other) const 
{
   Vec2 result(*this);
   result += other;
   return result;
}

const Vec2 Vec2::operator-(const Vec2 &other) const 
{
   Vec2 result(*this);
   result -= other;
   return result;
}

const Vec2 Vec2::operator*(const float other) const 
{
   Vec2 result(*this);
   result *= other;
   return result;
}

float Vec2::innerProduct(const Vec2 &rhs) const
{
   return x*rhs.x + y*rhs.y;
}
