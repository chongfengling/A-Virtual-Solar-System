#ifndef NBODY_HPP
#define NBODY_HPP

#include <Eigen/Core>
#include <memory>

class Particle;
// calculate the acceleration of p1 due to p2
Eigen::Vector3d calcAcceleration(Particle &p1, Particle &p2, double epsilon = 0);
// update the position and velocity of each body
std::vector<std::shared_ptr<Particle>> update_Solar_System(std::vector<std::shared_ptr<Particle>> Solar_System, double dt, double total_time, int n_steps, double epsilon = 0);
// simulate the solar system with time step dt and total time total_time
void run_Solar_System(double dt, double total_time, int n_steps, double epsilon = 0);
// calculate the total energy of the solar system
double calTotalEnergy(const std::vector<std::shared_ptr<Particle>>& Solar_System);

# endif // NBODY_HPP