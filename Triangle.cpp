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

Triangle::Triangle(const Vec3& v0Val, const Vec3& v1Val, const Vec3& v2Val, unsigned int materialIndexVal)
 : v0(v0Val),
   v1(v1Val),
   v2(v2Val),
   materialIndex(materialIndexVal)
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

/*
const Vec3 Triangle::getNormal() const
{
   Vec3 b0(v1 - v0);
   Vec3 b1(v2 - v0);
   
   return b0.cross(b1).normalize();
}*/
   
const Vec3 Triangle::getNormal() const
{
   // Calculate b0 and b1
   float b0x = v1.x - v0.x;
   float b0y = v1.y - v0.y;
   float b0z = v1.z - v0.z;
   
   float b1x = v2.x - v1.x;
   float b1y = v2.y - v1.y;
   float b1z = v2.z - v1.z;
   
   // Cross 
   //Vec3(b0.y*b1.z - b0.z*b1.y, b0.z*b1.x - b0.x*b1.z, b0.x*b1.y - b0.y*b1.x);
   float crossX = b0y*b1z - b0z*b1y;
   float crossY = b0z*b1x - b0x*b1z;
   float crossZ = b0x*b1y - b0y*b1x;
   
   //Normalize
   float len = sqrtf(crossX*crossX + crossY*crossY + crossZ*crossZ); 
   crossX /= len;
   crossY /= len;
   crossZ /= len;
   
   
   return Vec3(crossX, crossY, crossZ);
}

