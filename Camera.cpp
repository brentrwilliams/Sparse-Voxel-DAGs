/**
 * Camera.cpp
 * 
 * by Brent Williams
 */

#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 right, unsigned int width, unsigned int height)
{
   this->position = position;
   this->up = up;
   this->right = right;

   this->width = width;
   this->height = height;

   // Calculate the bounds of the image plane
   r = glm::length(right) / 2.0f;
   l = -r;
   t = glm::length(up) / 2.0f;
   b = - t;

   // Create an orthonormal basis for the camera
   u = right / glm::length(right);
   v = up / glm::length(up);
   w = glm::cross(u, v);
   w = w / glm::length(w);
}


Ray Camera::getRay(unsigned int x, unsigned int y, float xFraction, float yFraction)
{
   float u_s;
   float v_s;
   float w_s;

   u_s = l + (r-l) * ( (x + 0.5f + xFraction) / width );
   v_s = b + (t-b) * ( (y + 0.5f + yFraction) / height );
   w_s = 1.0f;

   glm::vec3 s = position + (u_s * u) + (v_s * v) + (w_s * w);
   glm::vec3 direction = glm::normalize(s-position); 
   Ray ray(position,direction);
   return ray;
 }