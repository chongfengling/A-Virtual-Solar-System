#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <Eigen/Core>
#include <memory>

// the planet or the sun in the solar system
class Particle
{
public:
    // constructor by mass
    Particle(double in_mass);
    // constructor by mass, position, velocity and acceleration
    Particle(double in_mass, Eigen::Vector3d position, Eigen::Vector3d velocity, Eigen::Vector3d acceleration);
    // get the position of the particle
    Eigen::Vector3d &getPosition();
    // get the velocity of the particle
    Eigen::Vector3d &getVelocity();
    // get the acceleration of the particle
    Eigen::Vector3d &getAcceleration();
    // get the mass of the particle
    const double &getMass();
    // update the position and velocity of the particle
    void update(double dt);
    // set the acceleration of the particle
    void setAcceleration(Eigen::Vector3d acceleration);
    // set the velocity of the particle
    void setVelocity(Eigen::Vector3d velocity);
    // set the position of the particle
    void setPosition(Eigen::Vector3d position);
    // update the acceleration of the particle by other particles in the list
    void updateAcceleration(const std::vector<std::shared_ptr<Particle>> &p_list, double epsilon);
    // calculate the kinetic energy of the particle
    double calKineticEnergy();
    // calculate the potential energy of the particle with other particles in the list
    double calPotentialEnergy(const std::vector<std::shared_ptr<Particle>> &p_list);

private:
    const double mass;
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    Eigen::Vector3d acceleration;
};
#endif // PARTICLE_HPP