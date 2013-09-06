/**
 * Vec3.cpp
 * 
 * by Brent Williams
 */

#include "Vec3.hpp"

Vec3::Vec3()
 : x(0),
   y(0),
   z(0)
{}

Vec3::Vec3(const float val)
 : x(val),
   y(val),
   z(val)
{}

Vec3::Vec3(const float xVal, const float yVal, const float zVal)
 : x(xVal),
   y(yVal),
   z(zVal)
{}

void Vec3::print() const
{
   printf("<%f, %f, %f>\n", x, y, z);  
}

Vec3& Vec3::operator=(const Vec3 &rhs)
{
   x = rhs.x;
   y = rhs.y;
   z = rhs.z;
   
   return *this;
}

Vec3& Vec3::operator+=(const Vec3 &rhs)
{
   x += rhs.x;
   y += rhs.y;
   z += rhs.z;
   
   return *this;
}

Vec3& Vec3::operator-=(const Vec3 &rhs)
{
   x -= rhs.x;
   y -= rhs.y;
   z -= rhs.z;
   
   return *this;
}

Vec3& Vec3::operator*=(const float rhs)
{
   x *= rhs;
   y *= rhs;
   z *= rhs;
   
   return *this;
}

const Vec3 Vec3::operator+(const Vec3 &other) const 
{
   Vec3 result(*this);
   result += other;
   return result;
}

const Vec3 Vec3::operator-(const Vec3 &other) const 
{
   Vec3 result(*this);
   result -= other;
   return result;
}

const Vec3 Vec3::operator*(const float other) const 
{
   Vec3 result(*this);
   result *= other;
   return result;
}

float Vec3::dot(const Vec3 &rhs) const
{
   return x*rhs.x + y*rhs.y +z*rhs.z;
}

Vec3 Vec3::cross(const Vec3 &rhs) const
{
   return Vec3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
}

float Vec3::innerProduct(const Vec3 &rhs) const
{
   return dot(rhs);
}

float Vec3::length() const
{
   return sqrt(x*x + y*y + z*z);
}

const Vec3& Vec3::normalize() 
{
   float len = length();
   
   x /= len;
   y /= len;
   z /= len;
   
   return *this;
}
