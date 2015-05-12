/**
 * Scene.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "Scene.hpp"

Scene::Scene()
{
}

Scene::Scene(vector<Triangle>& triangles, vector<PhongMaterial> materials)
{
   this->triangles = triangles;
   this->materials = materials;

   std::vector<Triangle*> triPtrs;

   for (int i = 0; i < triangles.size(); ++i)
   {
      triPtrs.push_back(&(triangles[i]));
   }

   cerr << endl << "Building BoundingVolumeHierarchy..." << endl;
   bvh = new BoundingVolumeHierarchy(triPtrs);
   cerr << "Finished Building BoundingVolumeHierarchy" << endl;
}


// bool Scene::intersect(Ray ray, float& t, Triangle& triangle)
// {
//    bool hit = false;
//    t = 10000000.0f;

//    #pragma omp parallel for
//    for (int i = 0; i < triangles.size(); i++)
//    {
//       int numThreads = omp_get_num_threads();

//       float tempT;
//       if (triangles[i].intersect(ray, tempT))
//       {
//          #pragma omp critical
//          {
//             if (tempT < t && tempT > 0.0f)
//             {
//                t = tempT;
//                triangle = triangles[i];
//                hit = true;
//             }
//          }
//       }
//    }

//    return hit;
// }

bool Scene::intersect(Ray ray, float& t, Triangle& triangle)
{
   t = 10000000.0f;

   return bvh->intersect(ray, &t, triangle);
}