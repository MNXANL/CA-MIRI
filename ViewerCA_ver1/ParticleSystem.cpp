#include "ParticleSystem.h"
#include <random>
#include <iostream>

#define rand01() ((float)std::rand()/RAND_MAX)


ParticleSystem::ParticleSystem( )
{
    m_numParticles = 1;
    float X = 6.0;    //     Points       Normals
    floorPlane      = Plane( 0, -X, 0,    0, 1, 0 );

    leftWallPlane   = Plane(  X, 0, 0,   -1, 0, 0 );
    rightWallPlane  = Plane( -X, 0, 0,    1, 0, 0 );

    frontWallPlane  = Plane( 0, 0, -X,    0, 0, 1 );
    backWallPlane   = Plane( 0, 0,  X,    0, 0,-1 );

                      //     Points         Radius
    sph             = Sphere(0, -5, 0,      1.0f );

    t1 = glm::vec3(  5, -3,  1 );   // triangle points
    t2 = glm::vec3(  2,  1, -4 );
    t3 = glm::vec3(  0, -1, -2 );
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::setParticleSystem(int numParticles, ParticleSystemType systemType){
	m_particleSystem.resize(numParticles);
	m_numParticles = numParticles;

    iniParticleSystem( systemType );
}


Particle ParticleSystem::getParticle(int i){
	return m_particleSystem[i];
}


float fRand(float fMin, float fMax)
{
    float f = (float)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void ParticleSystem::iniParticleSystem(ParticleSystemType systemType){
	switch (systemType)
	{
		case ParticleSystemType::Waterfall:
		{							
        for (int i = 0; i < m_numParticles; i++)
           {
            float x = fRand(0.0, 2.0);
            m_particleSystem[i].setPosition(x, 9-x, x);
            m_particleSystem[i].setVelocity((rand01() - 0.5), 0, 0.5*rand01());

            m_particleSystem[i].setMass( fRand( 1.0, 10.0 ) );
            m_particleSystem[i].setBouncing( fRand( 0.5, 1.5 ) );
		   }
		}
		break;
		case ParticleSystemType::Fountain:
		{
           for (int i = 0; i < m_numParticles; i++)
           {
               float x = fRand(0.0, 2.0);
               m_particleSystem[i].setPosition(x, 9-x, x);
               m_particleSystem[i].setVelocity(2 * (rand01() - 0.5), 5, 2 * rand01());

               m_particleSystem[i].setMass( fRand( 1.0, 10.0 ) );
               m_particleSystem[i].setBouncing( fRand( 0.5, 1.5 ) );
		   }
		}
		break;
	}

}




void ParticleSystem::updateParticleSystem(const float& dt, Particle::UpdateMethod method){

    for (int i = 1; i < m_numParticles; i++)
	{
        if ( m_particleSystem[i].getLife() >= m_particleSystem[i].getLifetime() )
        {   // reset pos + life
            float x = fRand(0.0, 2.0);
            m_particleSystem[i].setPosition(x, 9-x, x);
            m_particleSystem[i].setVelocity(2 * (rand01() - 0.5), 5, 2 * rand01());

            m_particleSystem[i].setLife(0.0f);
        }
        else
        {
            m_particleSystem[i].setForce(0.0f, -9.8f, 0.0f);
            m_particleSystem[i].updateParticle(dt, method);

            //Check Floor collisions
            if ( m_particleSystem[i].collisionParticlePlane( floorPlane ) ){
                m_particleSystem[i].correctCollisionParticlePlane( floorPlane );
            }
            if ( m_particleSystem[i].collisionParticlePlane( leftWallPlane ) ){
                m_particleSystem[i].correctCollisionParticlePlane( leftWallPlane );
            }
            if ( m_particleSystem[i].collisionParticlePlane( rightWallPlane ) ){
                m_particleSystem[i].correctCollisionParticlePlane( rightWallPlane );
            }
            if ( m_particleSystem[i].collisionParticlePlane( frontWallPlane ) ){
                m_particleSystem[i].correctCollisionParticlePlane( frontWallPlane  );
            }
            if ( m_particleSystem[i].collisionParticlePlane( backWallPlane ) ){
                m_particleSystem[i].correctCollisionParticlePlane( backWallPlane );
            }

            //Check SPHERE collisions
            if ( m_particleSystem[i].collisionParticleSphere( sph ) ){
                m_particleSystem[i].correctCollisionParticleSphere( sph );
            }

            /*Check TRI collisions
            if ( m_particleSystem[i].collisionParticleTriangle( t1,t2,t3 ) ){
                m_particleSystem[i].correctCollisionParticleTriangle( t1,t2,t3 );
            }*/

        }
        m_particleSystem[i].addLife( dt );
	}
}

