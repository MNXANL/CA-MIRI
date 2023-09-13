#include "ParticleSystem.h"
#include <random>
#include <iostream>

#define rand01() ((float)std::rand()/RAND_MAX)


ParticleSystem::ParticleSystem( )
{
    m_numParticles = 1;
    float X = 6.0;
    float r = 0.0f; //radius of a particle

                    //     Points       Normals
    floorPlane      = Plane( 0, -X+r, 0,    0, 1, 0 );

    leftWallPlane   = Plane(  X-r, 0, 0,   -1, 0, 0 );
    rightWallPlane  = Plane( -X+r, 0, 0,    1, 0, 0 );

    frontWallPlane  = Plane( 0, 0, -X+r,    0, 0, 1 );
    backWallPlane   = Plane( 0, 0,  X-r,    0, 0,-1 );

                      //     Points         Radius
    float sr = 1.55f;
    sph             = Sphere(0, -6.0+sr, 0,    sr );


    k_d  = 13.0f;
    k_e  = -200.0f;
    Long = 0.30f;

    // debug
    GF = 0.1f;
}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::setParticleSystem(int numParticles, ParticleSystemType systemType){
    m_particleSystem.resize(numParticles);
    m_numParticles = numParticles;

    iniParticleSystem( );
}


void ParticleSystem::setSpringDamping( float val ){
    k_d = val;
}

void ParticleSystem::setSpringElasticity( float val ){
    k_e = val;
}

void ParticleSystem::setSpringLength( float val ){
    Long = val;
}


Particle ParticleSystem::getParticle(int i){
	return m_particleSystem[i];
}


float fRand(float fMin, float fMax)
{
    float f = (float)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void ParticleSystem::iniParticleSystem( ){
    bool partHori = 1;
    if (partHori)
    {
        // no initialization type! :)
        float Xp = -4.0;
        float Yp = 0.0;
        float Zp = 0.0;
        m_particleSystem[ 0 ].setFixed( true );
        m_particleSystem[ 0 ].setPosition(Xp, Yp, Zp);

        for (int i = 1; i < m_numParticles; i++)
        {
            m_particleSystem[ i ].setFixed( false );

            m_particleSystem[ i ].setPosition(Xp + 1.1*Long*float(i), Yp, Zp);
            m_particleSystem[ i ].setVelocity(0.0, 0.0, 0.0);
            m_particleSystem[ i ].setForce(0, -9.81f*GF, 0);

            m_particleSystem[ i ].setMass( 0.1 );
            m_particleSystem[ i ].setBouncing( 1.0 ); //1.3

        }
        //m_particleSystem[ m_numParticles-1 ].setFixed( true );
    }
    else {
        float Xp = 1.0;
        float Yp = 2.0;
        float Zp = 0.0;
        m_particleSystem[ 0 ].setFixed( true );
        m_particleSystem[ 0 ].setPosition(Xp, Yp, Zp);

        for (int i = 1; i < m_numParticles; i++)
        {
            m_particleSystem[ i ].setFixed( false );

            m_particleSystem[ i ].setPosition(Xp , Yp - 1.1*Long*float(i), Zp);
            m_particleSystem[ i ].setVelocity(0.0, 0.0, 0.0);
            m_particleSystem[ i ].setForce(0, -9.81f*GF, 0);

            m_particleSystem[ i ].setMass( 0.1 );
            m_particleSystem[ i ].setBouncing( 1.0 );

        }
        //m_particleSystem[ m_numParticles-1 ].setFixed( true );
    }
}

glm::vec3 ParticleSystem::getSpringForce( int i )
{
    Particle P1 = m_particleSystem[ i   ];
    Particle P2 = m_particleSystem[ i+1 ];

    glm::vec3 dPos = P1.getCurrentPosition() - P2.getCurrentPosition();
    float     dist = glm::length( dPos );

    glm::vec3 nDir = dPos / dist;
    glm::vec3 dVec = P2.getVelocity() - P1.getVelocity();

    if ( dist > 0 ) // that equation
    {
        return ( k_e*(dist - Long) + k_d*( glm::dot( dVec, nDir ) ) ) * nDir;
    }

    return glm::vec3( 0.0f );
}



void ParticleSystem::updateParticleSystem(const float& dt, Particle::UpdateMethod method){
    glm::vec3 F_spring;     //force of the current spring
    for (int i = 0; i < m_numParticles; i++)
    {
        m_particleSystem[i].setForce(0.0f, -9.81f, 0.0f);

        if (i==0) {
            m_particleSystem[i].setFixed(true);
            F_spring = getSpringForce( i );
        }
        else if (i == m_numParticles - 1) {
            m_particleSystem[i].addForce( -F_spring ); // force up
        }
        else {
            m_particleSystem[i].addForce( -F_spring ); // force up
            F_spring = getSpringForce( i ); // next spring
            m_particleSystem[i].addForce(  F_spring ); // force down
        }
        m_particleSystem[i].setForce( m_particleSystem[i].getForce().x*GF, m_particleSystem[i].getForce().y*GF, m_particleSystem[i].getForce().z*GF );

        //std::cout << "FS(" << i << ") = {" << F_spring.x << ", " << F_spring.y << ", " << F_spring.z << "}\n" ;
    }
    //std::cout << std::endl;


    for (int i = 0; i < m_numParticles; i++)
    {

        /* ******** LIFE SYSTEM IS "DEAD" ******** */
        if ( m_particleSystem[i].getLife() >= m_particleSystem[i].getLifetime() )
        {   // reset speed + life
            m_particleSystem[i].setLife(0.0f);
        }
        else {
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
                std::cout << "SPHERE[" << i << "]\n";
                m_particleSystem[i].correctCollisionParticleSphere( sph );
            }



            m_particleSystem[i].addLife( dt );
        }
    }
}

