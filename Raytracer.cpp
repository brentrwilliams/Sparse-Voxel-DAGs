/**
 * Raytracer.cpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "Raytracer.hpp"

Raytracer::Raytracer(unsigned int imageWidth, unsigned int imageHeight, Traceable* traceable) 
   : image(imageWidth, imageHeight)
{
   this->imageWidth = imageWidth;
   this->imageHeight = imageHeight;
   fillColor = glm::vec3(0,0,0);
   this->traceable = traceable;
}

void Raytracer::trace()
{
   float halfWidth = float(imageWidth) / 2.0f;
   float halfHeight = float(imageHeight) / 2.0f;

   glm::vec3 ka(0,0.1,0);
   glm::vec3 kd(0,0.4,0);
   glm::vec3 ks(0.5,0.5,0.5);
   float ns = 20.0;
   PhongMaterial phongMat(ka,kd,ks,ns);

   for (unsigned int x = 0; x < imageWidth; x++)
   {
      for (unsigned int y = 0; y < imageHeight; y++)
      {
         float t = 0.0f;

         // Orthographic Projection
         vec3 position(float(x) - halfWidth, float(y) - halfHeight, 1000.0f);
         vec3 direction(0, 0, -1);
         Ray ray(position, direction);

         if (traceable->intersect(ray, t))
         {
            glm::vec3 hitPosition = ray.position + (t * ray.direction);
            //glm::vec3 normal = sphere.getNormal(hitPosition);
            glm::vec3 normal = glm::vec3(0.0f,0.0f,1.0f);

            //vec3 color = phongMat.calculateSurfaceColor(ray, hitPosition, normal);
            vec3 color(1,1,1);
            image.addColor(x,y, color);
         }
         else
         {
            image.addColor(x,y, fillColor);
         }
      }
   }
}

void Raytracer::writeImage(const char* imageName)
{
   image.writeTGA(imageName);
}





































