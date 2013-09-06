/**
 * Triangle.cpp
 * 
 * by Brent Williams
 */

#include "Triangle.hpp"

Triangle::Triangle()
 : v0(0),
   v1(0),
   v2(0)
{
}

Triangle::Triangle(const Vec3& v0Val, const Vec3& v1Val, const Vec3& v2Val)
 : v0(v0Val),
   v1(v1Val),
   v2(v2Val)
{
}

void Triangle::print() const
{
   printf("<%f, %f, %f>, <%f, %f, %f>, <%f, %f, %f>\n", v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);  
}

const Vec3 Triangle::getMins() const 
{
   Vec3 mins(v0);
   
   if (v1.x < mins.x)
      mins.x = v1.x;
   if (v1.y < mins.y)
      mins.y = v1.y;
   if (v1.z < mins.z)
      mins.z = v1.z;
   
   if (v2.x < mins.x)
      mins.x = v2.x;
   if (v2.y < mins.y)
      mins.y = v2.y;
   if (v2.z < mins.z)
      mins.z = v2.z;
   
   return mins;
}

const Vec3 Triangle::getMaxs() const
{
   Vec3 maxs(v0);
   
   if (v1.x > maxs.x)
      maxs.x = v1.x;
   if (v1.y > maxs.y)
      maxs.y = v1.y;
   if (v1.z > maxs.z)
      maxs.z = v1.z;
   
   if (v2.x > maxs.x)
      maxs.x = v2.x;
   if (v2.y > maxs.y)
      maxs.y = v2.y;
   if (v2.z > maxs.z)
      maxs.z = v2.z;
   
   return maxs;
}

const Vec3 Triangle::getNormal() const
{
   Vec3 b0(v1 - v0);
   Vec3 b1(v2 - v0);
   
   return b0.cross(b1).normalize();
}

