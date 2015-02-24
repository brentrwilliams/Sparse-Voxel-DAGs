/**
 * Ray.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef RAY_HPP
#define RAY_HPP

#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 

using namespace std;

class Ray
{
   public:
      glm::vec3 position;
      glm::vec3 direction;
      Ray(const glm::vec3& position, const glm::vec3& direction);
      ~Ray();
      glm::vec3 at(float t) const;
      void print() const;

};


#endif