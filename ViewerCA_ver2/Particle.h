#pragma once
#ifdef WIN32
	#include <glm\glm.hpp>
#else
	#include <glm/glm.hpp>
#endif
#include "Plane.h"
#include "Sphere.h"

class Particle
{
public:
	enum class UpdateMethod : std::int8_t { EulerOrig, EulerSemi, Verlet };

	Particle();
	Particle(const float& x, const float& y, const float& z);
//	Particle(glm::vec3 pos, glm::vec3 vel, float bouncing = 1.0f, bool fixed = false, int lifetime = -1, glm::vec3 force = glm::vec3(0, 0, 0));
	~Particle();
	//setters
	void setPosition(const float& x, const float& y, const float& z);
	void setPosition(glm::vec3 pos);
	void setPreviousPosition(const float& x, const float& y, const float& z);
	void setPreviousPosition(glm::vec3 pos);
	void setVelocity(const float& x, const float& y, const float& z);
	void setVelocity(glm::vec3 vel);
	void setForce(const float& x, const float& y, const float& z);
	void setForce(glm::vec3 force);
    void setMass(float mass);
    void setBouncing(float bouncing);
    void setLifetime(float lifetime);
    void setLife(float life);
    void addLife(float add);
	void setFixed(bool fixed);


	//getters
	glm::vec3 getCurrentPosition();
	glm::vec3 getPreviousPosition();
	glm::vec3 getForce();
    glm::vec3 getVelocity();
    float getMass();
    float getBouncing();
    float getLifetime();
    float getLife();
	bool isFixed();

	//other
	void addForce(glm::vec3 force);
	void addForce(const float& x, const float& y, const float& z);
	void updateParticle(const float& dt, UpdateMethod method = UpdateMethod::EulerOrig);
    bool collisionParticlePlane(Plane p);
    void correctCollisionParticlePlane(Plane p);

    bool collisionParticleSphere( Sphere sph );
    void correctCollisionParticleSphere( Sphere sph );

    bool collisionParticleTriangle( glm::vec3 p1, glm::vec3 p2, glm::vec3 p3 );
    void correctCollisionParticleTriangle( glm::vec3 p1, glm::vec3 p2, glm::vec3 p3 );


private:
	glm::vec3 m_currentPosition;
	glm::vec3 m_previousPosition;
	glm::vec3 m_force;
	glm::vec3 m_velocity;

    float m_bouncing;
    float m_lifetime;
    float m_life;
    float m_k;
    float m_mass;
	bool  m_fixed;

};

