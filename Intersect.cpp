/**
 * Intersect.cpp
 * 
 * by Brent Williams
 */


#include "Intersect.hpp"


/**
 * Tests whether the triangle intersects the given axis aligned bounding box (represented by a 
 * minimum corner p and a maximum corner p + deltaP).
 */
bool triangleAABBIntersect(const Triangle& triangle, Vec3& p, Vec3& deltaP)
{
   // Two requirements for intersection
   // 1) The triangle's plane overlaps the AABB
   // 2) For each of the coordinate planes (xy, xz, yz), the 2D projections of the triangle and 
   //    AABB into this plane overlap
   int i;
   Vec3 c(0,0,0); 
   Vec3 n(triangle.getNormal()); 
   float d1;
   float d2;
   Vec3 v[3];
   v[0] = triangle.v0;
   v[1] = triangle.v1;
   v[2] = triangle.v2;
   float nInnerProductP = n.innerProduct(p);
   
   // 1) The triangle's plane overlaps the AABB
   
   if(n.x > 0)
      c.x = deltaP.x;
   if(n.y > 0)
      c.y = deltaP.y;
   if(n.z > 0)
      c.z = deltaP.z;
   
   d1 = n.innerProduct(c - v[0]);
   d2 = n.innerProduct((deltaP - c) - v[0]);
   
   if ((nInnerProductP + d1) * (nInnerProductP + d2) > 0)
      return false;
   
   
   // 2) For each of the coordinate planes (xy, xz, yz), the 2D projections of the triangle and 
   //    AABB into this plane overlap
   
   //Check for XY
   Vec2 pXY;
   Vec2 vXY[3];
   Vec2 nXY[3];
   Vec3 e[3];
   float dXY[3];
   
   pXY.x = p.x;
   pXY.y = p.y;
   
   //printf("pXY: ");
   //pXY.print();
   
   for (i = 0; i <= 2; i++)
   {
      //printf("\n\ni: ", i);
      e[i] = v[(i+1) % 3] - v[i];
      
      //printf("e[%d]: ", i);
      //e[i].print();
      
      nXY[i].x = -1.0 * e[i].y;
      nXY[i].y = e[i].x;
      
      //printf("nXY[%d]: ", i);
      //nXY[i].print();
      
      //printf("n.z: %f\n", n.z);
      if (n.z < 0)
         nXY[i] *= -1.0;
      
      vXY[i].x = v[i].x;
      vXY[i].y = v[i].y;
      
      //printf("vXY[%d]: ", i);
      //vXY[i].print();
      
      dXY[i] = (-1.0 * nXY[i].innerProduct(vXY[i])) + std::max(0.0f, deltaP.x*nXY[i].x) + std::max(0.0f, deltaP.y*nXY[i].y); 
      //printf("dXY[%d]: %f\n", i, dXY[i]);
      
      if (nXY[i].innerProduct(pXY) + dXY[i] < 0)
      {
         //printf("Failed on XY\n");
         return false;
      }
   }
   
   //Check for XZ
   Vec2 pXZ;
   Vec2 vXZ[3];
   Vec2 nXZ[3];
   float dXZ[3];
   
   pXZ.x = p.x;
   pXZ.y = p.z;
   
   //printf("pXZ: ");
   //pXZ.print();
   
   for (i = 0; i <= 2; i++)
   {
      //printf("\n\ni: ");
      e[i] = v[(i+1) % 3] - v[i];
      //printf("e[%d]: ", i);
      //e[i].print();
      
      nXZ[i].x = -1.0 * e[i].z;
      nXZ[i].y = e[i].x;
      //printf("nXZ[%d]: ", i);
      //nXZ[i].print();
      
      //printf("n.y: %f\n", n.y);
      if (n.y >= 0)
         nXZ[i] *= -1.0;
      
      vXZ[i].x = v[i].x;
      vXZ[i].y = v[i].z;
      
      //printf("vXZ[%d]: ", i);
      //vXZ[i].print();
      
      dXZ[i] = (-1.0 * nXZ[i].innerProduct(vXZ[i])) + std::max(0.0f, deltaP.x*nXZ[i].x) + std::max(0.0f, deltaP.z*nXZ[i].y); 
      //printf("dXZ[%d]: %f\n", i, dXZ[i]);
      
      if (nXZ[i].innerProduct(pXZ) + dXZ[i] < 0)
      {
         //printf("Failed on XZ\n");
         return false;
      }
   }
   
   
   //Check YZ
   Vec2 pYZ;
   Vec2 vYZ[3];
   Vec2 nYZ[3];
   float dYZ[3];
   
   pYZ.x = p.y;
   pYZ.y = p.z;
   //printf("pYZ: ");
   //pYZ.print();
   
   for (i = 0; i <= 2; i++)
   {
      //printf("\n\ni: ");
      e[i] = v[(i+1) % 3] - v[i];
      //printf("e[%d]: ", i);
      //e[i].print();
      
      nYZ[i].x = -1.0 * e[i].z;
      nYZ[i].y = e[i].y;
      //printf("nYZ[%d]: ", i);
      //nYZ[i].print();
      
      //printf("n.x: %f\n", n.x);
      if (n.x < 0)
         nYZ[i] *= -1.0;
      
      vYZ[i].x = v[i].y;
      vYZ[i].y = v[i].z;
      //printf("vYZ[%d]: ", i);
      //vYZ[i].print();
      
      dYZ[i] = (-1.0 * nYZ[i].innerProduct(vYZ[i])) + std::max(0.0f, deltaP.y*nYZ[i].x) + std::max(0.0f, deltaP.z*nYZ[i].y); 
      //printf("dYZ[%d]: %f\n", i, dYZ[i]);
      
      if (nYZ[i].innerProduct(pYZ) + dYZ[i] < 0)
      {
         //printf("Failed on YZ\n");
         return false;
      }
   }
   
   
   return true;
}

