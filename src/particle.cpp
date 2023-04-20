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

Eigen::Vector3d& Particle::getPosition()
{
    return this->position;
}

Eigen::Vector3d& Particle::getVelocity()
{
    return this->velocity;
}

Eigen::Vector3d& Particle::getAcceleration()
{
    return this->acceleration;
}
const double& Particle::getMass()
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
    #pragma opm parallel for reduction(+:acceleration_sum) schedule(runtime)
    for (const auto& p : p_list) 
    {
        if (p.get() != this)
        {
            acceleration_sum += calcAcceleration(*this, *p, epsilon);
        }
    }
    this->acceleration = acceleration_sum;
}

double Particle::calKineticEnergy()
{
    double kinetic_energy = 0.5 * this->mass * this->velocity.squaredNorm();
    return kinetic_energy;
}

double Particle::calPotentialEnergy(const std::vector<std::shared_ptr<Particle>> &p_list)
{
    double potential_energy = 0;
    #pragma opm parallel for reduction(+:potential_energy) schedule(runtime)
    for (const auto& p : p_list)
    {
        if (p.get() != this)
        {
            double distance = (p->getPosition() - this->getPosition()).norm();
            potential_energy += -0.5 * this->mass * p->getMass() / distance;
        }
    }
    return potential_energy;
}