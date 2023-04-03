#ifndef NBODY_HPP
#define NBODY_HPP

#include <Eigen/Core>
#include <memory>

class Particle;

Eigen::Vector3d calcAcceleration(Particle &p1, Particle &p2, double epsilon = 0);
std::vector<std::shared_ptr<Particle>> update_Solar_System(std::vector<std::shared_ptr<Particle>> Solar_System, double dt, double total_time, int n_steps, double epsilon = 0);
void run_Solar_System(double dt, double total_time, int n_steps, double epsilon = 0);
double calTotalEnergy(const std::vector<std::shared_ptr<Particle>>& Solar_System);

# endif // NBODY_HPP