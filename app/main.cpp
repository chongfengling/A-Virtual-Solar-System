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
    double dt(-1);
    app.add_option("--dt, --delta_time", dt, "Time step for simulation")->check(CLI::PositiveNumber);
    // use either total time or number of steps
    double year_time(-1);
    app.add_option("--yt, --year_time", year_time, "Total time (in year) for simulation")->check(CLI::PositiveNumber);
    int n_steps(-1);
    app.add_option("--ns, --n_steps", n_steps, "Number of steps for simulation")->check(CLI::PositiveNumber);

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

    std::vector<std::shared_ptr<Particle>> SS_initial = initialize_Solar_System();
    double total_energy_initial = calTotalEnergy(SS_initial);

    // ! SS_initial will be changed in the update_Solar_System function
    std::vector<std::shared_ptr<Particle>> SS_updated = update_Solar_System(SS_initial, dt, year_time, n_steps);
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

    // std::cout << "Hello World!\n"
    //           << std::endl;

    // run_Solar_System_in_one_year();
    return 0;
}
