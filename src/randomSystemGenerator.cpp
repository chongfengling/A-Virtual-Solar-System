#include <randomSystemGenerator.hpp>
#include <memory>
#include <Eigen/Core>
#include <random>

RandomSystemGenerator::RandomSystemGenerator(int num_planets, int seed, double epsilon) : seed(seed), epsilon(epsilon), num_planets(num_planets)
{
    // ?! should initialize p_list here? How to make p_list private?
    // this->p_list = std::vector<std::shared_ptr<Particle>>();
}

std::vector<std::shared_ptr<Particle>> RandomSystemGenerator::generateInitialConditions()
{
    // add the Sun to the list
    this->p_list.push_back(std::make_shared<Particle>(1.0, Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0)));
    // add planets to the list
    std::mt19937 rng_mt{static_cast<std::mt19937::result_type>(this->seed)};
    std::uniform_real_distribution<double> dist_mass{1. / 6000000, 1. / 1000};
    std::uniform_real_distribution<double> dist_distance{0.4, 30};
    std::uniform_real_distribution<double> dist_angle{0, 2 * M_PI};
    for (int i = 0; i < this->num_planets; i++) // The Sun is not a planet
    {
        double mass = dist_mass(rng_mt);
        double distance = dist_distance(rng_mt);
        double angle = dist_angle(rng_mt);
        this->p_list.push_back(
            std::make_shared<Particle>(
                mass,
                Eigen::Vector3d(distance * sin(angle), distance * cos(angle), 0),
                Eigen::Vector3d(-cos(angle) / sqrt(distance), sin(angle) / sqrt(distance), 0),
                Eigen::Vector3d(0, 0, 0)));
    }
    return this->p_list;
}