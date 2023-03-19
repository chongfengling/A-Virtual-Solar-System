#include <Eigen/Core>

class Particle
{
public:
    Particle(double in_mass);
    Particle(double in_mass, Eigen::Vector3d position, Eigen::Vector3d velocity, Eigen::Vector3d acceleration);
    Eigen::Vector3d getPosition();
    Eigen::Vector3d getVelocity();
    Eigen::Vector3d getAcceleration();
    const double getMass();

private:
    const double mass;
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    Eigen::Vector3d acceleration;
};