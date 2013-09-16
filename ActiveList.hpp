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

class ActiveList
{
   public:
      unsigned int currentLevel;
      unsigned int levels;
      uint64_t *data;
      unsigned int dataSize;
      unsigned int dimension; // Number of voxels for one side of the cube
      
      ActiveList(const Voxels& voxels);
      ~ActiveList();
      unsigned int calculateDataSize(unsigned int levels);
      uint64_t& operator[](unsigned int i);
      void set(unsigned int x, unsigned int y, unsigned int z);
      unsigned int countSetVoxels();
      unsigned int nextLevel();
};

#endif
