/**
 * BoundingBox.cpp
 * 
 * by Brent Williams
 */

#include "BoundingBox.hpp"

BoundingBox::BoundingBox()
 : mins(0),
   maxs(0)
{
}

BoundingBox::BoundingBox(const Vec3& minsVal, const Vec3& maxsVal)
 : mins(minsVal),
   maxs(maxsVal)
{
}

void BoundingBox::print()
{
   printf("mins: <%f, %f, %f>, maxs: <%f, %f, %f>\n", mins.x, mins.y, mins.z, maxs.x, maxs.y, maxs.z);  
}

void BoundingBox::square()
{
   float xLen = mins.x - maxs.x;
   float yLen = mins.y - maxs.y;
   float zLen = mins.z - maxs.z;
   float largest = xLen;
   
   if (yLen > largest)
      largest = yLen;
   if (zLen > largest)
      largest = zLen;
   
   float xDif = 0.5 * (largest - xLen);
   mins.x -= xDif;
   maxs.x += xDif;
   
   float yDif = 0.5 * (largest - yLen);
   mins.y -= yDif;
   maxs.y += yDif;
   
   float zDif = 0.5 * (largest - zLen);
   mins.z -= zDif;
   maxs.z += zDif;
}
