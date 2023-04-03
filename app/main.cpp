#include <iostream>
#include <Eigen/Core>
#include "CLI11.hpp"
#include "particle.hpp"
#include "nbody.hpp"
#include "solarSystemGenerator.hpp"
#include "randomSystemGenerator.hpp"

int main(int argc, char **argv)
{
    CLI::App app("Solar System");
    // version flag
    std::string version = "0.1.0";
    app.set_version_flag("--version", version);
    // delta time required
    double dt(-1);
    app.add_option("--dt, --delta_time", dt, "Time step for simulation")->check(CLI::PositiveNumber);
    // use either total time or number of steps
    double year_time(-1);
    app.add_option("--yt, --year_time", year_time, "Total time for simulation in year")->check(CLI::PositiveNumber);
    int n_steps(-1);
    app.add_option("--ns, --n_steps", n_steps, "The number of simulation steps")->check(CLI::PositiveNumber);
    double epsilon(0);
    app.add_option("--ep, --epsilon", epsilon, "parameter epsilon for simulation")->check(CLI::PositiveNumber);
    std::string task("None");
    app.add_option("--task", task, "task to run");
    int n_particles;
    app.add_option("--np, --n_particles", n_particles, "The number of particles in the system")->check(CLI::PositiveNumber);
    int seed(2023);
    app.add_option("--sd, --seed", seed, "random seed for random initialized system")->check(CLI::PositiveNumber);

    CLI11_PARSE(app, argc, argv);

    if (argc == 1)
    {
        std::cout << app.help() << std::endl;
        return 0;
    }

    if (dt > 0)
    {
        if (year_time > 0 & n_steps <= 0)
        {
            n_steps = year_time * 2 * M_PI / dt;
        }
        else if (year_time <= 0 & n_steps > 0)
        {
            year_time = n_steps * dt;
        }
        else
        {
            // std::count << std::string(dt);
            printf("%f\n", dt);
            printf("%f\n", year_time);
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
    if (task == "SS") // The Solar system
    {
        std::cout << "task: Solar System" << std::endl;
        run_Solar_System(dt, year_time, n_steps, epsilon);
        return 0;
    }
    else if (task == "RS") // The random initialized system
    {
        if (n_particles > 0)
        {
            std::cout << "Task: Random System" << std::endl;
            std::shared_ptr<RandomSystemGenerator> generator = std::make_shared<RandomSystemGenerator>(n_particles, seed, epsilon);
            std::vector<std::shared_ptr<Particle>> SS_initial = generator->generateInitialConditions();
            double total_energy_initial = calTotalEnergy(SS_initial);

            // ! SS_initial will be changed in the update_Solar_System function
            std::vector<std::shared_ptr<Particle>> SS_updated = update_Solar_System(SS_initial, dt, year_time, n_steps, epsilon);
            double total_energy_updated = calTotalEnergy(SS_updated);

            std::cout << "total energy of the solar system at the beginning is "
                      << total_energy_initial
                      << std::endl;
            std::cout << "total energy of the solar system after "
                      << year_time
                      << " years with dt = "
                      << dt
                      << " is "
                      << total_energy_updated
                      << std::endl;
            std::cout << "total energy increased during this period is "
                      << total_energy_updated - total_energy_initial
                      << std::endl;
            return 0;
        }
        else
        {
            std::cerr << "Error: n_particles is required, please refer to the help information '-h'." << std::endl;
            return 1;
        }
        return 0;
    }
    else
    {
        std::cerr << "Error: task is required, please refer to the help information '-h'." << std::endl;
        return 1;
    }

    return 0;
}
