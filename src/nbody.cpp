#include "nbody.hpp"
#include "particle.hpp"
#include "solarSystemGenerator.hpp"
#include <Eigen/Core>
#include <cmath>
#include <iostream>
#include <random>
#include <chrono>

// return the acceleration of p1 due to p2
Eigen::Vector3d calcAcceleration(Particle &p1, Particle &p2, double epsilon) // no default value for epsilon.
// ! Why not shared_ptr? why shared_ptr in other functions?
{
    Eigen::Vector3d r_vec = p2.getPosition() - p1.getPosition();
    double r_norm = r_vec.norm();
    Eigen::Vector3d acceleration = p2.getMass() * r_vec / pow(pow(r_norm, 2) + pow(epsilon, 2), 1.5);
    return acceleration;
}

std::vector<std::shared_ptr<Particle>> update_Solar_System(std::vector<std::shared_ptr<Particle>> Solar_System, double dt, double total_time, int n_steps, double epsilon)
{
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int n = 0; n < n_steps; n++)
    {
        // update the gravitational acceleration of each body
        for (int i = 0; i < Solar_System.size(); i++)
        {
            // ? What is epsilon, 0 or 1? 0 works for test case 'a simple Solar System (the Sun and the Earth only)'
            Solar_System[i]->updateAcceleration(Solar_System, epsilon);
        }
        // update the position and velocity of each body
        for (int j = 0; j < Solar_System.size(); j++)
        {
            Solar_System[j]->update(dt);
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    double elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count(); // unit: ms
    double time_per_step = elapsed_time / n_steps;
    std::cout << "dt = "
              << dt
              << " (year/2Pi). Total time = "
              << total_time
              << " year, step = " //in run_one_year will * 2pi
              << n_steps
              << std::endl
              << "Running time =  "
              << elapsed_time
              << " ms. "
              << "time per step is "
              << time_per_step
              << " ms"
              << std::endl;

    return Solar_System;
}

void run_Solar_System(double dt, double total_time, int n_steps, double epsilon)
{
    // Simulation of the real solar system for one year
    // initialize the solar system
    std::shared_ptr<SolarSystemGenerator> generator = std::make_shared<SolarSystemGenerator>();
    std::vector<std::shared_ptr<Particle>> SS_initial = generator->generateInitialConditions();
    // print the initial position of planets in the solar system
    for (int i = 0; i < SS_initial.size(); i++)
    {
        std::cout << "initial position of the "
                  << i
                  << "th planet: "
                  << std::endl
                  << SS_initial[i]->getPosition()
                  << std::endl;
    }
    #ifdef DEBUG
    // calculate the total energy of the solar system at the beginning
    double total_energy_initial = calTotalEnergy(SS_initial);
    std::cout << "total energy of the solar system at the beginning is "
              << total_energy_initial
              << std::endl;
    #endif
    std::vector<std::shared_ptr<Particle>> SS_updated = update_Solar_System(SS_initial, dt, total_time, n_steps, epsilon);
    // print the final position of planets in the solar system
    for (int i = 0; i < SS_updated.size(); i++)
    {
        std::cout << "final position of "
                  << i
                  << "th planet after one year and dt = "
                  << dt
                  << std::endl
                  << SS_updated[i]->getPosition()
                  << std::endl;
    }
    #ifdef DEBUG
    // calculate the total energy of the solar system
    double total_energy_updated = calTotalEnergy(SS_updated);
    std::cout << "total energy of the solar system after one year and dt = "
              << dt
              << " is "
              << total_energy_updated
              << std::endl;
    std::cout << "the difference between the initial and final total energy is "
              << total_energy_updated - total_energy_initial
              << std::endl;
    #endif
}

double calTotalEnergy(const std::vector<std::shared_ptr<Particle>> &Solar_System)
{
    double total_energy(0);
    for (int i = 0; i < Solar_System.size(); i++)
    {
        total_energy += (Solar_System[i]->calKineticEnergy() + Solar_System[i]->calPotentialEnergy(Solar_System));
    }
    return total_energy;
}