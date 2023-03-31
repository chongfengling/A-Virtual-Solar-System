#ifndef NBODY_HPP
#define NBODY_HPP

#include <Eigen/Core>
#include <memory>

class Particle;

Eigen::Vector3d calcAcceleration(Particle &p1, Particle &p2, double epsilon);
std::vector<std::shared_ptr<Particle>> update_Solar_System(std::vector<std::shared_ptr<Particle>> Solar_System, double dt, double total_time, int n_steps);
void run_Solar_System_in_one_year();
double calTotalEnergy(const std::vector<std::shared_ptr<Particle>>& Solar_System);

# endif // NBODY_HPP