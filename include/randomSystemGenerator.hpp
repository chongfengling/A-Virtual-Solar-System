#ifndef RANDOMSYSTEMGENERATOR_HPP
#define RANDOMSYSTEMGENERATOR_HPP

#include <generator.hpp>

class RandomSystemGenerator : public InitialConditionGenerator
{
public:
    RandomSystemGenerator(int num_planets, int seed = 2023, double epsilon = 0.001);
    std::vector<std::shared_ptr<Particle>> generateInitialConditions();
private:
    int seed;
    int num_planets;
    double epsilon;
};

#endif /* RANDOMSYSTEMGENERATOR_HPP */