/**
 * Face.cpp
 * 
 * by Brent Williams
 */

#include "Face.hpp"

Face::Face(int v1Val, int v2Val, int v3Val)
 : v1(v1Val),
   v2(v2Val),
   v3(v3Val)
{}

void Face::print() const
{
   printf("[%d, %d, %d]\n", v1, v2, v3);
}
