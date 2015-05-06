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
   // float halfWidth = float(imageWidth) / 2.0f;
   // float halfHeight = float(imageHeight) / 2.0f;
   // float width = 1.0f;
   // float height = float(imageHeight) / float(imageWidth);

   // float left = 0.0f - (width / 2.0f); // x value of the left hand side of the image plane
   // float bottom = 0.0f - (height / 2.0f); // y value of the bottom of the image plane
   // float pixelOffset = width / imageWidth; // space between pixels
   // float halfPixelOffset = pixelOffset / 2.0f;

   // cout << "left: " << left << endl;
   // cout << "bottom: " << bottom << endl;
   // cout << "pixelOffset: " << pixelOffset << endl << endl;


   glm::vec3 ka(0,0.2,0);
   glm::vec3 kd(0,0.4,0);
   glm::vec3 ks(0.5,0.5,0.5);
   float ns = 20.0;
   PhongMaterial phongMat(ka,kd,ks,ns);

   // glm::vec3 cameraPosition(0.0f,0.0f,0.25f);
   // glm::vec3 cameraRight(1.0f,0.0f,0.0f);
   // glm::vec3 cameraUp(0.0f,1.0f,0.0f);

   glm::vec3 cameraPosition(0.25f,0.0f,0.25f);
   glm::vec3 cameraRight = glm::normalize(glm::vec3(1.0f,0.0f,-1.0f));
   glm::vec3 cameraUp = glm::normalize(glm::vec3(0.0f,1.0f,0.0f));
   Camera camera(cameraPosition, cameraRight, cameraUp, imageWidth, imageHeight);

   float numPixels = imageWidth * imageHeight;

   for (unsigned int y = 0; y < imageHeight; y++)
   {
      for (unsigned int x = 0; x < imageWidth; x++)
      {
         float t = 0.0f;
         // float zPos = 0.0f;
         
         // // Perspective Projection
         // glm::vec3 position(0.0f, 0.0f, zPos + 4.0f);
         // glm::vec3 imagePlanePosition(left + halfPixelOffset + (float(x)  * pixelOffset), bottom + halfPixelOffset + (float(y) * pixelOffset), zPos);
               
         // glm::vec3 direction = glm::normalize(imagePlanePosition - position);
         //Ray ray(position, direction);

         Ray ray = camera.getRay(x,y);

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
            // color = phongMat.calculateSurfaceColor(ray, hitPosition, moxelNormal);
            //color = normal;
            //color = moxelNormal;
            // color = phongMat.calculateSurfaceColor(ray, hitPosition, normal);

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
            //cout << "(" << x << ", " << y << ") => Empty" << endl;
         }
         image.addColor(y,x, color);
      }
      
      float percentageDone = ((y * imageWidth) / numPixels) * 100.0f;
      cerr << percentageDone << "%" << endl;  
   }
   cerr << "100%" << endl;
}

void Raytracer::writeImage(const char* imageName)
{
   image.writeTGA(imageName);
}





































