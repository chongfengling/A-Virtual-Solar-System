#ifndef SOLARSYSTEMGENERATOR_HPP
#define SOLARSYSTEMGENERATOR_HPP

#include <generator.hpp>
// generate the solar system with the sun and eight planets
class SolarSystemGenerator : public InitialConditionGenerator
{
public:
    // constructor
    SolarSystemGenerator();
    // generate the initial conditions
    std::vector<std::shared_ptr<Particle>> generateInitialConditions();
};

#endif /* SOLARSYSTEMGENERATOR_HPP */


