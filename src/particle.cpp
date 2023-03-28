#include "particle.hpp"
#include "nbody.hpp"
#include <Eigen/Core>
#include <cmath>
#include <iostream>
#include <random>

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

void Particle::update(double dt)
{
    this->position += this->velocity * dt;
    this->velocity += this->acceleration * dt;
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

void Particle::updateAcceleration(const std::vector<std::shared_ptr<Particle>> &p_list, double epsilon)
{
    Eigen::Vector3d acceleration_sum{0, 0, 0};
    for (const auto& p : p_list) //why const auto& p? especially const? ans: change the variable in this only 
    {
        // check if p is the same particle as *this
        // when ==, why output is nan * 3?
        if (p->getPosition() != this->getPosition())
        {
            acceleration_sum += calcAcceleration(*this, *p, epsilon); // why *this?
        }
    }
    this->acceleration = acceleration_sum;
}
