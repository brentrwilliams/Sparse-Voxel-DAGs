/**
 * Main.cpp
 * 
 * by Brent Williams
 */

#include "Main.hpp"

int main(int argc, char const *argv[])
{
   if (argc < 3)
   {
      perror("Missing arguments.\n Exitting\n");
      exit(1);
   }
   
   std::string fileName(argv[1]);
   OBJFile objFile(fileName);
   unsigned int numLevels = atoi(argv[2]);
   objFile.centerMesh();

   DAG dag(numLevels, objFile.getBoundingBox(), objFile.getTriangles());
   if (argc == 3)
   {
      dag.writeImages();
   }

   Raytracer raytracer(500,500, &dag);
   raytracer.trace();
   raytracer.writeImage("images/raytraced/image.tga");

   return 0;
}