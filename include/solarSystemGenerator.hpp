#ifndef SOLARSYSTEMGENERATOR_HPP
#define SOLARSYSTEMGENERATOR_HPP

#include <generator.hpp>

class SolarSystemGenerator : public InitialConditionGenerator
{
public:
    SolarSystemGenerator();
    std::vector<std::shared_ptr<Particle>> generateInitialConditions();
};

#endif /* SOLARSYSTEMGENERATOR_HPP */


