// #include "particle.hpp"
#include "/workspaces/PHAS0100/a-virtual-solar-system-chongfengling/include/particle.hpp"
#include "/usr/include/eigen3/Eigen/Core"
#include <cmath>
#include <iostream>

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

void Particle::updateAcceleration(std::vector<Particle> p_list, double epsilon)
{
    Eigen::Vector3d acceleration_sum{0, 0, 0};
    for (Particle p : p_list)
    {
        // check if p is the same particle as *this
        // when ==, why output is nan * 3?
        if (p.getPosition() != this->getPosition())
        {
            acceleration_sum += calcAcceleration(*this, p, epsilon);
        }
    }
    this->acceleration = acceleration_sum;
}

// return the acceleration of p1 due to p2
Eigen::Vector3d calcAcceleration(Particle p1, Particle p2, double epsilon = 0)
{
    Eigen::Vector3d r_vec = p2.getPosition() - p1.getPosition();
    double r_norm = r_vec.norm();
    Eigen::Vector3d acceleration = p2.getMass() * r_vec / pow(pow(r_norm, 2) + pow(epsilon, 2), 1.5);
    return acceleration;
}

// // main function
// int main(int argc, char* argv[])
// {
//     std::cout << "Hello World!\n";
//     return 0;
// }
