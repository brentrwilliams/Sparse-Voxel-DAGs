/**
 * AABB.cpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "AABB.hpp"

AABB::AABB(const glm::vec3& mins, const glm::vec3& maxs)
{
   this->mins = mins;
   this->maxs = maxs;
}

bool AABB::contains(const glm::vec3& val)
{
   for (int i = 0; i < 3; i++)
   {
      if (val[i] < mins[i] || val[i] > maxs[i])
         return false;
   }
   return true;
}

bool AABB::inRange(float val, float min, float max)
{
   return val >= min && val <= max;
}

bool AABB::intersect(const Ray& ray, float& t, glm::vec3& normal, uint64_t moxelIndex)
{
   float normalDirection[3] = {1.0f,1.0f,1.0f};
   normal = glm::vec3(0.0f,0.0f,0.0f);
   //cout << "AABB: ";
   //print();
   //cout << "Ray: ";
   //ray.print();
   // If the ray starts inside the box, or ends inside
   if (contains(ray.position))
   {
      return false; 
   }

   // the algorithm says, find 3 t's,
   glm::vec3 tVals;

   // LARGEST t is the only one we need to test if it's on the face.
   for (int i = 0 ; i < 3 ; i++)
   {
      if (ray.direction[i] == 0)
      {
         tVals[i] = -FLT_MAX;
      }
      else if(ray.direction[i] > 0) // CULL BACK FACE
      {
         tVals[i] = ( mins[i] - ray.position[i] ) / ray.direction[i];
         
         // If the ray direction is positive, it will hit the more negative AABB side which makes 
         // the normal point in the negative direction
         normalDirection[i] = -1.0f; 
      }
      else
      {
         tVals[i] = ( maxs[i] - ray.position[i] ) / ray.direction[i];

         // If the ray direction is positive, it will hit the more positive AABB side which makes 
         // the normal point in the positive direction
         normalDirection[i] = 1.0f; 
      }
   }

   // cout << "tVals: ";
   // for (int i = 0; i < 3; ++i)
   // {
   //    cout << "(" << i << ": " << tVals[i] << ")  ";
   // }
   // cout << endl;

   // Get the max index
   float maxValue = tVals[0];
   int maxIndex = 0;
   if (tVals[1] > maxValue)
   {
      maxValue = tVals[1];
      maxIndex = 1;
   }
   
   if (tVals[2] > maxValue)
   {
      maxValue = tVals[2];
      maxIndex = 2;
   }

   t = maxValue;
   normal[maxIndex] = normalDirection[maxIndex];

   if (tVals[maxIndex] > 0.0f)
   {
      glm::vec3 pt = ray.at(tVals[maxIndex]);

      // check it's in the box in other 2 dimensions
      int o1 = ( maxIndex + 1 ) % 3; // i=0: o1=1, o2=2, i=1: o1=2,o2=0 etc.
      int o2 = ( maxIndex + 2 ) % 3;

      return inRange(pt[o1], mins[o1], maxs[o1]) && inRange(pt[o2], mins[o2], maxs[o2]) ;
   }

   return false ; // the ray did not hit the box.
}


void AABB::print() const
{
   cout << "mins <" << mins.x << ", " << mins.y << ", " << mins.z << ">  ";
   cout << "maxs <" << maxs.x << ", " << maxs.y << ", " << maxs.z << ">" << endl; 
}
