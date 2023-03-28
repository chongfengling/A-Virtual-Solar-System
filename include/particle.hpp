#include <Eigen/Core>
#include <memory>

class Particle
{
public:
    Particle(double in_mass);
    Particle(double in_mass, Eigen::Vector3d position, Eigen::Vector3d velocity, Eigen::Vector3d acceleration);
    Eigen::Vector3d getPosition();
    Eigen::Vector3d getVelocity();
    Eigen::Vector3d getAcceleration();
    const double getMass();
    void update(double dt);
    void setAcceleration(Eigen::Vector3d acceleration);
    void setVelocity(Eigen::Vector3d velocity);
    void setPosition(Eigen::Vector3d position);
    void updateAcceleration(const std::vector<std::shared_ptr<Particle>>& p_list, double epsilon);

private:
    const double mass;
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    Eigen::Vector3d acceleration;
};