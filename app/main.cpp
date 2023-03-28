#include <iostream>
#include "/usr/include/eigen3/Eigen/Core"
#include "CLI11.hpp"
#include "particle.hpp"
#include "nbody.hpp"

int main(int argc, char **argv)
{
    CLI::App app("Solar System");
    // version flag
    std::string version = "0.1.0";
    app.set_version_flag("--version", version);
    // delta time required
    double dt;
    app.add_option("--dt, --delta_time", dt, "Time step for simulation")->check(CLI::PositiveNumber);
    // use either total time or number of steps
    double total_time;
    app.add_option("--tt, --total_time", total_time, "Total time for simulation")->check(CLI::PositiveNumber);
    int n_steps;
    app.add_option("--ns, --n_steps", n_steps, "Number of steps for simulation")->check(CLI::PositiveNumber);

    CLI11_PARSE(app, argc, argv);

    if (argc == 1)
    {
        std::cout << app.help() << std::endl;
        return 0;
    }

    if (dt > 0)
    {
        if (total_time > 0 & n_steps <= 0)
        {
            n_steps = total_time / dt;
        }
        else if (total_time <= 0 & n_steps > 0)
        {
            total_time = n_steps * dt;
        }
        else
        {
            // std::count << std::string(dt);
            printf("%f\n", dt);
            printf("%f\n", total_time);
            printf("%d\n", n_steps);
            // std::count << total_time;
            // std::count << n_steps;
            std::cerr << "Error: arguments are unacceptable, please refer to the help information '-h'." << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "Error: ht is required, please refer to the help information '-h'." << std::endl;
        return 1;
    }

    std::vector<Particle> initial_solar_system = initialize_Solar_System();

    std::vector<Particle> updated_solar_system = update_Solar_System(initial_solar_system, dt, total_time, n_steps);

    std::cout << "Hello World!\n"
              << std::endl;

    // run_Solar_System_in_one_year();
    return 0;
}
