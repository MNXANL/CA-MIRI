#include "Particle.h"

//Particle::Particle()
//{
//}


float fRando(float fMin, float fMax)
{
    float f = (float)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}



Particle::Particle() :
m_currentPosition(0,0,0), m_previousPosition(0, 0, 0), m_velocity(0, 0, 0), m_force(0, 0, 0), m_bouncing(0), m_life(0), /*m_lifetime(50),*/ m_fixed(false), m_mass(1.0)
{
    m_lifetime = fRando(10, 20);
}

Particle::Particle(const float& x, const float& y, const float& z) :
m_previousPosition(0, 0, 0), m_velocity(0, 0, 0), m_force(0, 0, 0), m_bouncing(0), /*m_lifetime(50),*/ m_fixed(false), m_mass(1.0)
{
	m_currentPosition.x = x;
	m_currentPosition.y = y;
	m_currentPosition.z = z;
    m_lifetime = fRando(10, 20);
}

/*
Particle::Particle(glm::vec3 pos, glm::vec3 vel, float bouncing, bool fixed, int lifetime, glm::vec3 force) :
m_currentPosition(pos), m_previousPosition(pos), m_force(force), m_velocity(vel), m_bouncing(bouncing), m_lifetime(lifetime), m_fixed(fixed)
{
}
*/

Particle::~Particle()
{
}

//setters
void Particle::setPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos;
	pos.x = x; pos.y = y; pos.z = z;
	m_currentPosition = pos;
}
void Particle::setPosition(glm::vec3 pos)
{
	m_currentPosition = pos;
}

void Particle::setPreviousPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos;
	pos.x = x; pos.y = y; pos.z = z;
	m_previousPosition = pos;
}

void Particle::setPreviousPosition(glm::vec3 pos)
{
	m_previousPosition = pos;
}

void Particle::setForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force;
	force.x = x; force.y = y; force.z = z;
	m_force = force;
}

void Particle::setForce(glm::vec3 force)
{
    m_force = force;
}
void Particle::setMass(float mass)
{
    m_mass = mass;
}

void Particle::addForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force;
	force.x += x; force.y += y; force.z += z;
	m_force = force;
}

void Particle::addForce(glm::vec3 force)
{
	m_force += force;
}

void Particle::setVelocity(const float& x, const float& y, const float& z)
{
	glm::vec3 vel;
	vel.x = x; vel.y = y; vel.z = z;
	m_velocity = vel;
}

void Particle::setVelocity(glm::vec3 vel)
{
	m_velocity = vel;
}

void Particle::setBouncing(float bouncing)
{
	m_bouncing = bouncing;
}

void Particle::setLifetime(float lifetime)
{
    m_lifetime = lifetime;
}

void Particle::setLife(float life)
{
    m_life = life;
}

void Particle::addLife(float add)
{
    m_life += add;
}

void Particle::setFixed(bool fixed)
{
	m_fixed = fixed;
}

//getters
glm::vec3 Particle::getCurrentPosition()
{
	return m_currentPosition;
}

glm::vec3 Particle::getPreviousPosition()
{
	return m_previousPosition;
}

glm::vec3 Particle::getForce()
{
	return m_force;
}

glm::vec3 Particle::getVelocity()
{
	return m_velocity;
}

float Particle::getBouncing()
{
    return m_bouncing;
}

float Particle::getMass()
{
    return m_mass;
}

float Particle::getLifetime()
{
    return m_lifetime;
}

float Particle::getLife()
{
    return m_life;
}

bool Particle::isFixed()
{
	return m_fixed;
}

void Particle::updateParticle(const float& dt, UpdateMethod method)
{
	if (!m_fixed)
	{
		switch (method)
		{
            case UpdateMethod::EulerOrig:
            {
                m_previousPosition = m_currentPosition;
                m_currentPosition += m_velocity*dt;
                m_velocity += m_force*dt;
            }
                break;
            case UpdateMethod::EulerSemi:
            {
                m_previousPosition = m_currentPosition;
                m_velocity += m_force*dt;
                m_currentPosition += m_velocity*dt;
            }
                break;
            case UpdateMethod::Verlet:
            {
                m_velocity = (m_currentPosition  - m_previousPosition) / dt;
                m_previousPosition = m_currentPosition;
                m_currentPosition +=  0.99f*( m_velocity*dt ) + ( m_force*(dt*dt) / m_mass );

            }
            break;
		}
	}
	return;
}

bool Particle::collisionParticlePlane(Plane p){
    float sign = glm::dot(m_currentPosition,  p.normal) + p.d;
    sign      *= glm::dot(m_previousPosition, p.normal) + p.d;

    return ( sign <= 0.0f );
}

void Particle::correctCollisionParticlePlane(Plane p){
    m_currentPosition = m_currentPosition - (1 + m_bouncing)*(glm::dot(m_currentPosition, p.normal) + p.d)*p.normal;
    m_velocity        = m_velocity        - (1 + m_bouncing)*(glm::dot(m_velocity,        p.normal) /*+ p.d*/)*p.normal;
}







bool Particle::collisionParticleSphere(Sphere sph)
{
    float distPrev = sqrt( pow((m_previousPosition.x - sph.center.x), 2) + pow((m_previousPosition.y - sph.center.y), 2) + pow((m_previousPosition.z - sph.center.z), 2) );
    float distNow  = sqrt( pow((m_currentPosition.x  - sph.center.x), 2) + pow((m_currentPosition.y  - sph.center.y), 2) + pow((m_currentPosition.z  - sph.center.z), 2) );
    return distNow <= (sph.radius * sph.radius) && distPrev > (sph.radius * sph.radius);
}



void Particle::correctCollisionParticleSphere(Sphere sph)
{
    /*Plane tanPlaneToSphere2(
             m_currentPosition[0], m_currentPosition[1], m_currentPosition[2],   //P
             m_currentPosition[0] - sph.center[0],                               //N.x
             m_currentPosition[1] - sph.center[1],                               //N.y
             m_currentPosition[2] - sph.center[2] );                             //N.z
    */

    //https://math.stackexchange.com/questions/831109/closest-point-on-a-sphere-to-another-point
    float xq, yq, zq, dist;
    dist = sqrt( pow((m_currentPosition.x  - sph.center.x), 2) + pow((m_currentPosition.y  - sph.center.y), 2) + pow((m_currentPosition.z  - sph.center.z), 2) );
    xq = sph.center[0] + (sph.radius*( m_previousPosition[0] - sph.center[0] ) / dist);
    yq = sph.center[1] + (sph.radius*( m_previousPosition[1] - sph.center[1] ) / dist);
    zq = sph.center[2] + (sph.radius*( m_previousPosition[2] - sph.center[2] ) / dist);

/*
    glm::vec3 V = m_currentPosition - m_previousPosition;
    float A = glm::dot( V, V );
    float B = glm::dot( V, m_previousPosition-sph.center ) * 2;
    float C = glm::dot( sph.center, sph.center )           + glm::dot( m_previousPosition, m_previousPosition ) ;
            - glm::dot( m_previousPosition, sph.center )*2 - sph.radius*sph.radius;

    float D = B*B - 4*A*C;

    float l1 = (-B + sqrt(D)) / (2*A);
    float l2 = (-B + sqrt(D)) / (2*A);
    glm::vec3 ps;
    if (0 < l1 && l2 < 1)
        ps = m_previousPosition + l1*V;
    else
        ps = m_previousPosition + l2*V;

    xq = ps.x;  yq = ps.y;  zq = ps.z;
*/
    Plane tanPlaneToSphere(
             xq, yq, zq,            //P
             xq - sph.center[0],    // N.x
             yq - sph.center[1],    // N.y
             zq - sph.center[2] );  // N.z


   float auxBounc = m_bouncing;
   m_bouncing = -0.9f; // no bouncing
   correctCollisionParticlePlane( tanPlaneToSphere );
   m_bouncing = auxBounc;
}










bool SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b)
{
    glm::vec3 c1 = glm::cross(b-a, p1-a);
    glm::vec3 c2 = glm::cross(b-a, p2-a);
    return (glm::dot(c1, c2)) >= 0;
}

// maybe review, seems sketchy
bool Particle::collisionParticleTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    if( SameSide(m_currentPosition, p1,  p2, p3) and  SameSide(m_currentPosition, p2,  p1, p3) and  SameSide(m_currentPosition, p3,  p1, p2) )
    {
        glm::vec3 vc1 = glm::cross(p1-p2, p2-p3);
        return glm::abs(glm::dot( (p1-m_currentPosition), vc1 )) >= 0.01;
    }
}


void Particle::correctCollisionParticleTriangle( glm::vec3 p1, glm::vec3 p2, glm::vec3 p3 )
{
    glm::vec3 p12 = p2+p1;
    glm::vec3 p13 = p3+p1;
    //glm::vec3 norm = glm::cross(p12, p13);

    float a1 = p2.x - p1.x;
    float b1 = p2.y - p1.y;
    float c1 = p2.z - p1.z;
    float a2 = p3.x - p1.x;
    float b2 = p3.y - p1.y;
    float c2 = p3.z - p1.z;
    float na = b1 * c2 - b2 * c1;
    float nb = a2 * c1 - a1 * c2;
    float nc = a1 * b2 - b1 * a2;
    Plane tanPlaneToSphere(
            p1[0],    p1[1],   p1[2],  // P
            na,       nb,      nc);    // N

     correctCollisionParticlePlane( tanPlaneToSphere );

}
