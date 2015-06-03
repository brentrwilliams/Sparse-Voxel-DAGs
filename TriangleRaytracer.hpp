/**
 * TriangleRaytracer.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef TRIANGLE_RAYTRACER_HPP
#define TRIANGLE_RAYTRACER_HPP

#include <iostream>
#include <stdint.h>
#include <vector>
#include <iomanip>
#include <glm/glm.hpp>
#include "Image.hpp"
#include "PhongMaterial.hpp"
#include "Traceable.hpp"
#include "Camera.hpp"
#include "Triangle.hpp"
#include "Scene.hpp"

class TriangleRaytracer
{
   public:
      unsigned int imageWidth;
      unsigned int imageHeight;
      Image image;
      glm::vec3 fillColor;
      Scene scene;
      

      TriangleRaytracer(unsigned int imageWidth, unsigned int imageHeight, Scene& scene);
      void trace();
      void writeImage(const char* imageName);
};

#endif
