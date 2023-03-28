#include <Eigen/Core>
class Particle;

Eigen::Vector3d calcAcceleration(Particle p1, Particle p2, double epsilon);

std::vector<Particle> initialize_Solar_System();
std::vector<Particle> update_Solar_System(std::vector<Particle> Solar_System, double dt, double total_time, int n_steps);
void run_Solar_System_in_one_year();