/**
 * SVONode.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef SVO_NODE_HPP
#define SVO_NODE_HPP

#include <iostream>
#include <stdint.h>

class SVONode
{
   public:
      uint64_t childMask;
      void* childPointers[8];
};

#endif
