/**
 * TriMain.cpp
 * 
 * by Brent Williams
 */

#include "TriMain.hpp"

int main(int argc, char const *argv[])
{
   if (argc < 3)
   {
      perror("Missing arguments.\n Exitting\n");
      exit(1);
   }

   unsigned int imageWidth = 500;
   unsigned int imageHeight = 500;
   
   std::string filePath(argv[1]);
   OBJFile objFile(filePath);
   unsigned int numLevels = atoi(argv[2]);
   objFile.centerMesh();

   Scene scene(objFile.getTriangles(), objFile.materials);
   TriangleRaytracer triRaytracer(imageWidth, imageHeight, scene);
   triRaytracer.trace();
   triRaytracer.writeImage("images/raytraced/image_tri.tga");

   return 0;
}