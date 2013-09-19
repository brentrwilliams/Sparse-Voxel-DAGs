/**
 *  ActiveList.hpp
 * 
 * by Brent Williams
 */

#ifndef ACTIVE_LIST_HPP
#define ACTIVE_LIST_HPP

#include <stdint.h>
#include <string.h>

#include "Voxels.hpp"

class ActiveList : public Voxels
{
   public:
      unsigned int currentLevel;
      
      ActiveList(const unsigned int levelsVal, const BoundingBox& boundingBoxVal, const std::vector<Triangle> triangles);
      unsigned int nextLevel();
};

#endif
