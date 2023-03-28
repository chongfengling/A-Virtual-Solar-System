#include "nbody.hpp"
#include "particle.hpp"
#include <Eigen/Core>
#include <cmath>
#include <iostream>
#include <random>

// return the acceleration of p1 due to p2
Eigen::Vector3d calcAcceleration(Particle &p1, Particle &p2, double epsilon) // no default value for epsilon. 
// ! Why not shared_ptr? why shared_ptr in other functions?
{
    Eigen::Vector3d r_vec = p2.getPosition() - p1.getPosition();
    double r_norm = r_vec.norm();
    Eigen::Vector3d acceleration = p2.getMass() * r_vec / pow(pow(r_norm, 2) + pow(epsilon, 2), 1.5);
    return acceleration;
}

std::vector<std::shared_ptr<Particle>> initialize_Solar_System()
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
                Eigen::Vector3d(-cos(angles[i]) / sqrt(distances[0]), sin(angles[i]) / sqrt(distances[i]), 0),
                Eigen::Vector3d(0, 0, 0)));
    }

    return p_list;
}

std::vector<std::shared_ptr<Particle>> update_Solar_System(std::vector<std::shared_ptr<Particle>> Solar_System, double dt, double total_time, int n_steps)
{
    for (int n = 0; n < n_steps; n++)
    {
        // update the gravitational acceleration of each body
        for (int i = 0; i < Solar_System.size(); i++)
        {
            // ? What is epsilon, 0 or 1? 0 works for test case 'a simple Solar System (the Sun and the Earth only)'
            Solar_System[i]->updateAcceleration(Solar_System, 0);
        }
        // update the position and velocity of each body
        for (int j = 0; j < Solar_System.size(); j++)
        {
            Solar_System[j]->update(dt);
        }
    }
    return Solar_System;
}

void run_Solar_System_in_one_year()
{
    // Simulation of the real solar system for one year
    // initialize the solar system
    std::vector<std::shared_ptr<Particle>> initial_solar_system = initialize_Solar_System();   
    // print the initial position of planets in the solar system
    for (int i = 1; i < initial_solar_system.size(); i++)
    {
        std::cout << "initial position of the "
                  << i
                  << "th planet: "
                  << std::endl
                  << initial_solar_system[i]->getPosition()
                  << std::endl;
    }

    // define the time step and total time
    double dt(0.0001);
    double total_time(2 * M_PI);
    int n_steps(total_time / dt);
    // update the solar system
    std::vector<std::shared_ptr<Particle>> updated_solar_system = update_Solar_System(initial_solar_system, dt, total_time, n_steps);
    // print the final position of planets in the solar system
    for (int i = 1; i < updated_solar_system.size(); i++)
    {
        std::cout << "final position of "
                  << i
                  << "th planet after one year and dt = "
                  << dt
                  << std::endl
                  << updated_solar_system[i]->getPosition()
                  << std::endl;
    }
}