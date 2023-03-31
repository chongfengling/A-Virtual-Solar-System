#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <memory>
#include <particle.hpp>

class InitialConditionGenerator
{
public:
    virtual std::vector<std::shared_ptr<Particle>> generateInitialConditions() = 0;
};

#endif /* generator_hpp */