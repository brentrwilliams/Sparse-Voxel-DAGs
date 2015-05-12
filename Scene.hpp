/**
 * Scene.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef SCENE_HPP
#define SCENE_HPP

#include <iostream>
#include <stdint.h>
#include <vector>
#include <glm/glm.hpp>
#include "Triangle.hpp"
#include "omp.h"
#include "Ray.hpp"
#include "PhongMaterial.hpp"
#include "BoundingVolumeHierarchy.hpp"

using namespace std;

class Scene
{
   public:
      vector<Triangle> triangles;
      vector<PhongMaterial> materials;
      BoundingVolumeHierarchy *bvh;

      Scene();
      Scene(vector<Triangle>& triangles, vector<PhongMaterial> materials);
      bool intersect(Ray ray, float& t, Triangle& triangle);
};

#endif
