#pragma once
#include "Particle.h"
#include <vector>
#include "Plane.h"
#include "Sphere.h"
//#include "Triangle.h"

class ParticleSystem
{
public:
    enum class ParticleSystemType : std::int8_t { Fountain, Waterfall };
	ParticleSystem();
	~ParticleSystem();
    void setParticleSystem(int numParticles, ParticleSystemType systemType = ParticleSystemType::Fountain);
	Particle getParticle(int i);
    glm::vec3 getSpringForce( int i );

    void iniParticleSystem( );
    void updateParticleSystem(const float& dt, Particle::UpdateMethod method = Particle::UpdateMethod::EulerOrig);

    void setSpringDamping( float val );
    void setSpringElasticity( float val );
    void setSpringLength( float val );

private:
	int m_numParticles;
	std::vector<Particle> m_particleSystem;

    // box walls
    Plane floorPlane;
    Plane leftWallPlane;
    Plane rightWallPlane;
    Plane frontWallPlane;
    Plane backWallPlane;

    // extra objects to collide with
    Sphere sph;

    //Triangle tri DEPRECATERINO
    glm::vec3 t1, t2, t3;

    // global spring information (read: all springs have equal stats)
    float k_e;  //elasticity
    float k_d;  //dampening
    float Long; //longitude (between 2 particles)

    float GF;

};

