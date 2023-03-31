#include "solarSystemGenerator.hpp"
#include <memory>
#include <Eigen/Core>
#include <random>

SolarSystemGenerator::SolarSystemGenerator()
{
}

std::vector<std::shared_ptr<Particle>> SolarSystemGenerator::generateInitialConditions()
{
    // All the particles are stored in a vector, in order of Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune
    std::vector<std::shared_ptr<Particle>> p_list;
    // vector of masses of planets, sun = 1
    std::vector<double> masses{1. / 6023600, 1. / 408524, 1. / 332946.038, 1. / 3098710, 1. / 1047.55, 1. / 3499, 1. / 22962, 1. / 19352};
    // vector of distances of planets, earth = 1
    std::vector<double> distances{0.4, 0.7, 1, 1.5, 5.2, 9.5, 19.2, 30.1};
    // vector of angles, randomly generated between 0 and 2pi with length 8 (the sun is set to be the origin and excluded).
    std::vector<double> angles;
    std::mt19937 rng_mt{std::random_device{}()};
    std::uniform_real_distribution<double> distribution{0, 2 * M_PI};
    for (int i = 0; i < 8; i++)
    {
        angles.push_back(distribution(rng_mt));
    }
    // create the sun and append to the vector
    p_list.push_back(std::make_shared<Particle>(1, Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0)));

    // create planets and append to the vector
    for (int i = 0; i < 8; i++)
    {
        p_list.push_back(
            std::make_shared<Particle>(
                masses[i],
                Eigen::Vector3d(distances[i] * sin(angles[i]), distances[i] * cos(angles[i]), 0),
                Eigen::Vector3d(-cos(angles[i]) / sqrt(distances[i]), sin(angles[i]) / sqrt(distances[i]), 0),
                Eigen::Vector3d(0, 0, 0)));
    }

    return p_list;
}