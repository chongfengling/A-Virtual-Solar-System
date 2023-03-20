#include <iostream>
#include "/usr/include/eigen3/Eigen/Core"
#include "CLI11.hpp"
#include "/workspaces/PHAS0100/a-virtual-solar-system-chongfengling/include/particle.hpp"

int main(int argc, char **argv)
{
    CLI::App app("Solar System");
    CLI11_PARSE(app, argc, argv);
    Eigen::Vector3d v{1, 0, 0};

    std::cout << "Hello World!\n"
              << v << std::endl;

    return 0;
}
