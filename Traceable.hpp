/**
 * Traceable.hpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#ifndef TRACEABLE_HPP
#define TRACEABLE_HPP

#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <math.h>

#include "Ray.hpp"

using namespace std;

#define EPSILON 1.0e-6

class Traceable
{
   public:
      virtual bool intersect(const Ray& ray, float& t) = 0;
};


#endif