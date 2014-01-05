/**
 * MortonCode.hpp
 * 
 * by Brent Williams
 */

#include <stdint.h>
#include <limits.h>
#include <iostream>
#include <math.h>

uint32_t mortonCode(unsigned int x, unsigned int y, unsigned int z);
void mortonCodeToXYZ(uint32_t value, unsigned int *x, unsigned int *y, unsigned int *z, unsigned int level);
void printBinary(uint32_t value);
