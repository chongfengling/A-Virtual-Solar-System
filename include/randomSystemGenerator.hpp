#ifndef RANDOMSYSTEMGENERATOR_HPP
#define RANDOMSYSTEMGENERATOR_HPP

#include <generator.hpp>

// generate a random system with num_planets planets and a random sun
class RandomSystemGenerator : public InitialConditionGenerator
{
public:
    // constructor, default seed is 2023
    RandomSystemGenerator(int num_planets, int seed = 2023, double epsilon = 0.001);
    // generate the initial conditions
    std::vector<std::shared_ptr<Particle>> generateInitialConditions();
private:
    int seed;
    int num_planets;
    double epsilon;
};

#endif /* RANDOMSYSTEMGENERATOR_HPP */