/**
 * Ray.cpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "Ray.hpp"

Ray::Ray(const glm::vec3& position, const glm::vec3& direction)
{
   this->position = position;
   this->direction = glm::normalize(direction);
}

Ray::~Ray()
{
   
}

glm::vec3 Ray::at(float t) const
{
   return position + (direction * t);
}

void Ray::print() const
{
   std::cout << "position <" << position.x << ", " << position.y << ", " << position.z << ">  ";
   std::cout << "direction <" << direction.x << ", " << direction.y << ", " << direction.z << ">" << std::endl;
}