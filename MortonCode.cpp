/**
 * MortonCode.cpp
 * 
 * by Brent Williams
 */

#include "MortonCode.hpp"

uint32_t mortonCode(unsigned int x, unsigned int y, unsigned int z, unsigned int level)
{
   uint32_t answer = 0;
   unsigned int i;

   for (i = 0; i < level; ++i)
   {
      answer |= ((x & ((uint32_t)1 << i)) << 2*i) 
       | ((y & ((uint32_t)1 << i)) << (2*i + 1)) 
       | ((z & ((uint32_t)1 << i)) << (2*i + 2));
   }
   
   return answer;
}

void mortonCodeToXYZ(uint32_t mortonCode, unsigned int *x, unsigned int *y, unsigned int *z, unsigned int level)
{
   unsigned int i;
   *x = 0;
   *y = 0;
   *z = 0;

   for (i = 0; i < level; ++i)
   {
      *x |= (mortonCode & ((uint32_t)1 << 3*i)) >> (2*i);
      *y |= (mortonCode & ((uint32_t)1 << (3*i + 1))) >> (2*i+1);
      *z |= (mortonCode & ((uint32_t)1 << (3*i + 2))) >> (2*i+2);
   }
}

void printBinary(uint32_t value)
{
   unsigned int i;
   unsigned int set;
   uint32_t mask;
   
   for (i = 0; i < 32; i++)
   {
      mask = (uint32_t) 1 << i; 
      set = ((mask & value) > 0) ? 1 : 0;
      std::cout << set;
   }
   std::cout << "\n";
} 

void printBinary(uint32_t value, unsigned int level)
{
   unsigned int i;
   unsigned int set;
   uint32_t mask;
   
   for (i = 0; i < 3*level; i++)
   {
      mask = (uint32_t) 1 << i; 
      set = ((mask & value) > 0) ? 1 : 0;
      std::cout << set;
      if ((i+1) % 3 == 0)
         std::cout << " ";
   }
   std::cout << "\n";
} 

/*
int main()
{
   uint32_t value;
   unsigned int x,y,z;
   unsigned int level = 1;
   unsigned int dim = pow(2,level);
   uint32_t i;
   
   for (z = 0; z < dim; z++)
   {
      for (y = 0; y < dim; y++)
      {
         for (x = 0; x < dim; x++)
         {
            value = mortonCode(x,y,z,level);
            std::cout << value << " (" << x << ", " << y << ", " << z << "): \n\t";
            printBinary(value, level);
            std::cout << "\n";
         }
      }
   }
   
   std::cout << "mortonCodeToXYZ\n";
   for (i = 0; i < dim*dim*dim; i++)
   {
      mortonCodeToXYZ(1,&x,&y,&z,level);
      std::cout << i << " (" << x << ", " << y << ", " << z << "): \n\t";
      printBinary(i, level);
      std::cout << "\n";
   }
   
   return 0;
}*/
