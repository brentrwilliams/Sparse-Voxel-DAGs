/**
 * Intersect.hpp
 * 
 * by Brent Williams
 */

#ifndef INTERSECT_HPP
#define INTERSECT_HPP

#include "Triangle.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include <algorithm>

bool triangleAABBIntersect(const Triangle& triangle, Vec3& p, Vec3& deltaP);

#endif

