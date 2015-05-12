/**
 * TriangleRaytracer.cpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "TriangleRaytracer.hpp"

TriangleRaytracer::TriangleRaytracer(unsigned int imageWidth, unsigned int imageHeight, Scene& scene) 
   : image(imageWidth, imageHeight)
{
   this->imageWidth = imageWidth;
   this->imageHeight = imageHeight;
   fillColor = glm::vec3(0,0,0);
   this->scene = scene;
}

void TriangleRaytracer::trace()
{
   glm::vec3 ka(0,0.2,0);
   glm::vec3 kd(0,0.4,0);
   glm::vec3 ks(0.5,0.5,0.5);
   float ns = 20.0;
   PhongMaterial phongMat(ka,kd,ks,ns);

   // bigBunny Camera
   glm::vec3 cameraPosition(10.25f,0.0f,10.25f);
   glm::vec3 cameraRight = glm::normalize(glm::vec3(1.0f,0.0f,-1.0f));
   glm::vec3 cameraUp = glm::normalize(glm::vec3(0.0f,1.0f,0.0f));
   Camera camera(cameraPosition, cameraRight, cameraUp, imageWidth, imageHeight);

   // // toyStore Camera
   // glm::vec3 cameraPosition(25.25f,0.0f,25.25f);
   // glm::vec3 cameraRight = glm::normalize(glm::vec3(1.0f,0.0f,-1.0f));
   // glm::vec3 cameraUp = glm::normalize(glm::vec3(0.0f,1.0f,0.0f));
   // Camera camera(cameraPosition, cameraRight, cameraUp, imageWidth, imageHeight);

   float numPixels = imageWidth * imageHeight;

   unsigned int stepSize = (imageWidth * imageHeight) / 10000;
   unsigned int progress = 0;
   

   for (unsigned int y = 0; y < imageHeight; y++)
   {
      for (unsigned int x = 0; x < imageWidth; x++)
      {
         glm::vec3 colorSum = glm::vec3(0.0f,0.0f,0.0f);
         glm::vec2 offsets[5];
         offsets[0] = glm::vec2(0.0f,0.0f);
         offsets[1] = glm::vec2(-0.25f,-0.25f);
         offsets[2] = glm::vec2(0.25f,-0.25f);
         offsets[3] = glm::vec2(-0.25f,0.25f);
         offsets[4] = glm::vec2(0.25f,0.25f);


         // Super Sampled Anti-Aliasing
         for (int i = 0; i < 5; i++)
         {
            float t = 0.0f;

            Ray ray = camera.getRay(x, y, offsets[i].x, offsets[i].y);

            glm::vec3 color;
            glm::vec3 normal;
            Triangle triangle;

            if (scene.intersect(ray, t, triangle))
            {
               cout << "HIT!" << endl;
               glm::vec3 hitPosition = ray.position + (t * ray.direction);
               normal = triangle.getGLMNormal();
               color = scene.materials[triangle.materialIndex].calculateSurfaceColor(ray, hitPosition, normal);
            }
            else
            {
               color = fillColor;
            }
            colorSum += color;
         }
         colorSum /= 5.0f;

         image.addColor(y,x, colorSum);
      }  
      float percentDone = ((float) y) / ((float)imageHeight);
      cerr << (percentDone * 100.0f) << "%" << endl;
   }
   cerr << "100%" << endl;
}

void TriangleRaytracer::writeImage(const char* imageName)
{
   image.writeTGA(imageName);
}

































