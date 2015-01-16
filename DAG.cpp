/**
 * DAG.cpp
 * 
 * by Brent Williams
 */

#include "DAG.hpp"

DAG::DAG(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles)
: boundingBox(boundingBoxVal),
   levels(levelsVal),
   size(pow(8, levelsVal)), 
   dimension(pow(2,levelsVal)),
   voxelWidth(0)
{
   if (levels <= 2)
   {
      std::string err("\nNumber of levels too small\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }
   
   boundingBox.square();
   voxelWidth = (boundingBox.maxs.x - boundingBox.mins.x) / dimension;
   build(triangles);
}

DAG::~DAG()
{
}

void DAG::build(const std::vector<Triangle> triangles)
{
}