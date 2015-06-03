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

void BoundingBox::print() const
{
   fprintf(stderr, "mins: <%f, %f, %f>, maxs: <%f, %f, %f>\n", mins.x, mins.y, mins.z, maxs.x, maxs.y, maxs.z);  
}

void BoundingBox::square()
{
   float xLen = maxs.x - mins.x;
   float yLen = maxs.y - mins.y;
   float zLen = maxs.z - mins.z;
   float largest = xLen;
   
   if (yLen > largest)
      largest = yLen;
   if (zLen > largest)
      largest = zLen;

   float halfLength = xLen * 0.5f;

   float centerX = (mins.x + maxs.x) * 0.5f;
   mins.x = centerX - halfLength;
   maxs.x = centerX + halfLength;

   float centerY = (mins.y + maxs.y) * 0.5f;
   mins.y = centerY - halfLength;
   maxs.y = centerY + halfLength;

   float centerZ = (mins.z + maxs.z) * 0.5f;
   mins.z = centerZ - halfLength;
   maxs.z = centerZ + halfLength;
}

Vec3 BoundingBox::getCenter() const
{
   return (maxs + mins) * 0.5;
}
