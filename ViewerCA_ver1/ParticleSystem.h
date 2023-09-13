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
    void iniParticleSystem(ParticleSystemType systemType = ParticleSystemType::Fountain);
    void updateParticleSystem(const float& dt, Particle::UpdateMethod method = Particle::UpdateMethod::EulerOrig);


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
    //Triangle tri;
    glm::vec3 t1, t2, t3;

};

