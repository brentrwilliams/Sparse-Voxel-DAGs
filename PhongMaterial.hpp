/**
 * PhongMaterial.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef PhongMaterial_HPP
#define PhongMaterial_HPP

#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <math.h>

#include "Ray.hpp"

using namespace std;

class PhongMaterial
{
   public:
      glm::vec3 ka; // Ambient color
      glm::vec3 kd; // Diffuse color
      glm::vec3 ks; // Specular color
      float ns; // Specular coefficient
      PhongMaterial(const glm::vec3& ka, const glm::vec3& kd, const glm::vec3& ks, float ns);
      ~PhongMaterial();
      glm::vec3 calculateSurfaceColor(Ray ray, glm::vec3 hitPosition, glm::vec3 n);
};


#endif