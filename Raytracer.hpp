/**
 * Raytracer.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP

#include <iostream>
#include <stdint.h>
#include <glm/glm.hpp>
#include "Image.hpp"
#include "PhongMaterial.hpp"
#include "Traceable.hpp"
#include "Camera.hpp"
#include "DAG.hpp"

class Raytracer
{
   public:
      unsigned int imageWidth;
      unsigned int imageHeight;
      Image image;
      glm::vec3 fillColor;
      DAG* dag;

      Raytracer(unsigned int imageWidth, unsigned int imageHeight, DAG* dag);
      void trace();
      void writeImage(const char* imageName);
};

#endif
