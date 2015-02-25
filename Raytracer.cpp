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
   // float halfWidth = float(imageWidth) / 2.0f;
   // float halfHeight = float(imageHeight) / 2.0f;
   float scalingFactor = 0.25f;
   float width = 1.0f * scalingFactor;
   float height = scalingFactor * float(imageHeight) / float(imageWidth);

   float left = 0.0f - (width / 2.0f); // x value of the left hand side of the image plane
   float bottom = 0.0f - (height / 2.0f); // y value of the bottom of the image plane
   float offset = scalingFactor / imageWidth; // space between pixels
   float halfOffset = offset / 2.0f;

   cout << "left: " << left << endl;
   cout << "bottom: " << bottom << endl;
   cout << "offset: " << offset << endl << endl;


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
         //vec3 position(float(x) - halfWidth, float(y) - halfHeight, 1000.0f);
         vec3 position(left + halfOffset + (float(x)  * offset), bottom + halfOffset + (float(y) * offset), 1000.0f);
         cout << "(" << x << ", " << y << "): <" << left + halfOffset + (float(x)  * offset) << ", " << bottom + halfOffset + (float(y) * offset) << ">" << endl;
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
         cout << endl;
      }
   }
}

void Raytracer::writeImage(const char* imageName)
{
   image.writeTGA(imageName);
}





































