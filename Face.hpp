/**
 * Face.hpp
 * 
 * by Brent Williams
 */


#ifndef FACE_HPP
#define FACE_HPP

#include <stdio.h>

class Face 
{
   public:
      int v1;
      int v2;
      int v3;
      
      Face(int v1Val, int v2Val, int v3Val);
      void print() const;
};

#endif
