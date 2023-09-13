#ifndef SPHERE_H
#define SPHERE_H


#pragma once
#ifdef WIN32
    #include <glm\glm.hpp>
#else
    #include <glm/glm.hpp>
#endif
class Sphere
{
public:
    Sphere();
    Sphere(const float& x,  const float& y,  const float& z, const float& radius );
    ~Sphere();

    glm::vec3 center;
    float radius;

    //setter + getters
    void setSphereCenter(const float& x, const float& y, const float& z);
    void setSphereRadius(const float& rad);

    glm::vec3 getSphereCenter( );
    float getSphereRadius( );

};



#endif // SPHERE_H
