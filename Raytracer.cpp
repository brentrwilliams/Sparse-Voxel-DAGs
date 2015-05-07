/**
 * Raytracer.cpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "Raytracer.hpp"

Raytracer::Raytracer(unsigned int imageWidth, unsigned int imageHeight, DAG* dag) 
   : image(imageWidth, imageHeight)
{
   this->imageWidth = imageWidth;
   this->imageHeight = imageHeight;
   fillColor = glm::vec3(0,0,0);
   this->dag = dag;
}

void Raytracer::trace()
{
   glm::vec3 ka(0,0.2,0);
   glm::vec3 kd(0,0.4,0);
   glm::vec3 ks(0.5,0.5,0.5);
   float ns = 20.0;
   PhongMaterial phongMat(ka,kd,ks,ns);

   // // bigBunny Camera
   // glm::vec3 cameraPosition(10.25f,0.0f,10.25f);
   // glm::vec3 cameraRight = glm::normalize(glm::vec3(1.0f,0.0f,-1.0f));
   // glm::vec3 cameraUp = glm::normalize(glm::vec3(0.0f,1.0f,0.0f));
   // Camera camera(cameraPosition, cameraRight, cameraUp, imageWidth, imageHeight);

   // toyStore Camera
   glm::vec3 cameraPosition(25.25f,0.0f,25.25f);
   glm::vec3 cameraRight = glm::normalize(glm::vec3(1.0f,0.0f,-1.0f));
   glm::vec3 cameraUp = glm::normalize(glm::vec3(0.0f,1.0f,0.0f));
   Camera camera(cameraPosition, cameraRight, cameraUp, imageWidth, imageHeight);

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
            uint64_t moxelIndex = 0;

            if (dag->intersect(ray, t, normal, moxelIndex))
            {
               glm::vec3 hitPosition = ray.position + (t * ray.direction);
               
               glm::vec3 moxelNormal;
               unsigned int moxelMaterialIndex;
               dag->getNormalFromMoxelTable(moxelIndex, moxelNormal, moxelMaterialIndex);
               PhongMaterial moxelMaterial = dag->materials[moxelMaterialIndex];
               
               color = moxelMaterial.calculateSurfaceColor(ray, hitPosition, moxelNormal);

               //cout << "(" << x << ", " << y << ") => Full -> (" << color.x << ", " << color.y << ", " << color.z << ")" << " Normal -> (" << normal.x << ", " << normal.y << ", " << normal.z << ")  t = " << t << endl;
               if (moxelIndex >= dag->numFilledVoxels)
               {
                  cout << "@ERROR: moxelIndex > numFilledVoxels (" << dag->numFilledVoxels << ")\n\t(" << x << ", " << y << ") => moxelIndex = " << moxelIndex << endl;
               }
               else 
               {
                  cout << "(" << x << ", " << y << ") => moxelIndex = " << moxelIndex << endl;
                  cout << "\tmoxelNormal Normal: <" << moxelNormal.x << ", " << moxelNormal.y << ", " << moxelNormal.z << ">" << endl; 
                  cout << "\tTriangle Normal: <" << normal.x << ", " << normal.y << ", " << normal.z << ">" << endl; 
               }
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

void Raytracer::writeImage(const char* imageName)
{
   image.writeTGA(imageName);
}





































