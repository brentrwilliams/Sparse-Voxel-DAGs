/**
 * Camera.hpp
 * 
 * by Brent Williams
 */


#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <stdio.h>
#include <glm/glm.hpp>

#include "Ray.hpp"

class Camera 
{
   public:
      glm::vec3 position;
      glm::vec3 up;
      glm::vec3 right;
      unsigned int width; // width of image in pixels
      unsigned int height; // height of image in pixels
      float l; // left
      float r; // right
      float b; // bottom
      float t; // top
      glm::vec3 u;
      glm::vec3 v;
      glm::vec3 w;
      
      Camera(glm::vec3 position, glm::vec3 up, glm::vec3 right, unsigned int width, unsigned int height);
      Ray getRay(unsigned int x, unsigned int y, float xFraction, float yFraction);
};

#endif
