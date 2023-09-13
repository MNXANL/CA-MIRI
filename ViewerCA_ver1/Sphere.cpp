#include "Sphere.h"

Sphere::Sphere(){

    center = glm::vec3(-100, -100, -100);
    radius = 0;
}

Sphere::Sphere(const float& x,  const float& y,  const float& z,
               const float& rad )
{
    center = glm::vec3(x, y, z);
    radius = rad;
}

Sphere::~Sphere() {}

//setter
void Sphere::setSphereCenter(const float& x, const float& y, const float& z)
{
    center = glm::vec3(x, y, z);
}

void Sphere::setSphereRadius(const float& rad)
{
    radius = rad;
}


glm::vec3 Sphere::getSphereCenter( )
{
    return center;
}

float Sphere::getSphereRadius( )
{
    return radius;
}

