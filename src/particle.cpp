#include "particle.hpp"
#include <Eigen/Core>
#include <cmath>

Particle::Particle(double in_mass) : mass{in_mass}
{
}

Particle::Particle(double in_mass, Eigen::Vector3d position, Eigen::Vector3d velocity, Eigen::Vector3d acceleration) : mass{in_mass}, position{position}, velocity{velocity}, acceleration{acceleration}
{
}

Eigen::Vector3d Particle::getPosition()
{
    return this->position;
}

Eigen::Vector3d Particle::getVelocity()
{
    return this->velocity;
}

Eigen::Vector3d Particle::getAcceleration()
{
    return this->acceleration;
}
const double Particle::getMass()
{
    return this->mass;
}

void Particle::setAcceleration(Eigen::Vector3d acceleration)
{
    this->acceleration = acceleration;
}

void Particle::setVelocity(Eigen::Vector3d velocity)
{
    this->velocity = velocity;
}

void Particle::setPosition(Eigen::Vector3d position)
{
    this->position = position;
}

double Particle::getMass() const {
    return mass;
}