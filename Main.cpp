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

   unsigned int imageWidth = 500;
   unsigned int imageHeight = 500;
   
   std::string filePath(argv[1]);
   OBJFile objFile(filePath);
   unsigned int numLevels = atoi(argv[2]);
   objFile.centerMesh();

   cout << "************************************************************************" << endl;
   cout << "************************************************************************" << endl;
   cout << endl;
   cout << argv[1] << endl << endl;
   cout << "Levels: " << numLevels << endl;

   DAG dag(numLevels, objFile.getBoundingBox(), objFile.getTriangles(), filePath, objFile.materials);
   if (argc == 3)
   {
      //dag.writeImages();
   }

   auto start = chrono::steady_clock::now();
   Raytracer raytracer(imageWidth, imageHeight, &dag);
   raytracer.trace();
   raytracer.writeImage("images/raytraced/image.tga");
   auto end = chrono::steady_clock::now();
   auto diff = end - start;
   cout << "\t\tTime Raytracing: " << chrono::duration <double, milli> (diff).count() << " ms" << endl;

   cout << endl;
   cout << "************************************************************************" << endl;
   cout << "************************************************************************" << endl;
   cout << endl << endl << endl;

   return 0;
}