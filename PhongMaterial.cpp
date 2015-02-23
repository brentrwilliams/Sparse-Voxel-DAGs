/**
 * PhongMaterial.cpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(const glm::vec3& ka, const glm::vec3& kd, const glm::vec3& ks, float ns)
{
   this->ka = ka;
   this->kd = kd;
   this->ks = ks;
   this->ns = ns;
}

PhongMaterial::~PhongMaterial()
{
   
}

glm::vec3 PhongMaterial::calculateSurfaceColor(Ray ray, glm::vec3 hitPosition, glm::vec3 n)
{  
   glm::vec3 lightPosition(500, 500, -500);
   glm::vec3 lc(1.0,1.0,1.0); // Light color
   glm::vec3 l = glm::normalize(lightPosition - hitPosition);
   glm::vec3 v = ray.direction;

   // Ambient
   glm::vec3 ambientComponent = ka * lc;

   // Diffuse
   float nDotL = max(glm::dot(n, l), 0.0f);
   glm::vec3 diffuseComponent = kd * nDotL * lc;

   // Specular
   glm::vec3 r = glm::reflect(l, n);
   float vDotR = max(glm::dot(v,r), 0.0f);
   glm::vec3 specularComponent = ks * pow(vDotR, ns) * lc;

   return ambientComponent + diffuseComponent + specularComponent;
}